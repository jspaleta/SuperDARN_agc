#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h> /* POSIX terminal control definitions */
#include <getopt.h>

#include "iniparser.h"
#include "agc_commands.h"

int debug=0;
int verbose=0;
int tx=0;
int phasing=0;
int bas=0;
int save=0;
FILE *fp=NULL;
dictionary *agc_ini;
struct timeval t_now,t_then;
int timeout=3600;
void usage() {
	printf ("Usage:\n" );
	printf ("Transmitter Ucontroller manager\n" );
	printf ("-h / --help : show this message\n");
	printf ("-c PATH / --configfile PATH : \n configfile");
	printf ("-o PATH / --output PATH : \n outputfile");
	printf ("-v / --verbose : increase verbosity by 1\n");
	printf ("-r / --reset : reset relays\n");
	printf ("-t / --trip : trip relays\n");
	printf ("-s / --status : get status\n");
	printf ("-p / --phasing : talk to phasing matrix\n");
	printf ("-b / --bas : talk to basbox\n");
	printf ("-S / --save : save status\n");
	printf ("-a / --auto : automatically run every hour\n");
	printf ("-x NUM / --transmitter NUM : \n transmitter postion\n");
	printf ("If no transmitter selected, loops over all 16 transmitters\n");
}
int main(int argc, char **argv){

	static char transmitter_port[80]="/dev/ttyMP0";
	//static char basbox_port[80]="/dev/ttyS1";
	//static char phasing_port[80]="/dev/ttyS1";

	static int tx_positions[19]={
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18
	};
	static int tx_addresses[19]={
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,40,0
	};
	static int tx_disabled[19];
	static int tx_lockout[19];
	int	auto_loop=0,flag=1,status,i,transmitter_fd;
        unsigned char command=0;
	char	results[25];
	char	entry[120];
	char	configfile[120]="/data/ros/agc.ini";
	char	outputfile[120];
	struct termios options;
	struct agc_results agc_res;
	struct pm_results pm_res;
	struct basbox_results bas_res;
        int arg;                //command line option parsing
	static struct option long_options[] =
        {
		/* These options don't set a flag.
		* We distinguish them by their indices. */
		{"verbose",      no_argument,            0,      'v'},
		{"reset_relays", no_argument,      0,      'r'},
		{"trip_relays",  no_argument,      0,      't'},
		{"status",  no_argument,      0,      's'},
		{"phasing",  no_argument,      0,      'p'},
		{"bas",  no_argument,      0,      'b'},
		{"save",  no_argument,      0,      'S'},
		{"atuo",  no_argument,      0,      'a'},
		{"transmitter",  required_argument,      0,      'x'},
		{"configfile",  required_argument,      0,      'c'},
		{"outputfile",  required_argument,      0,      'o'},
		{"help",         required_argument,      0,      'h'},
		{0, 0, 0, 0}
        };
        /* Process arguments */
        while (1)
        {
           /* getopt_long stores the option index here. */
           int option_index = 0;
     
           arg = getopt_long (argc, argv, "abpvrsStx:c:o:h",
                            long_options, &option_index);
     
           /* Detect the end of the options. */
           if (arg == -1)
             break;

	switch (arg)
	{
		case 'a':
                  auto_loop=1;
		break;
		case 't':
                  command=TRIP_RELAY;
		break;
		case 'r':
                  command=RESET_RELAY;
		break;
		case 's':
                  command=READ_STATUS;
		break;
		case 'x':
		tx=atoi(optarg); 
                printf("TX: %d\n",tx);
		break;
		case 'c':
		sprintf(configfile,"%s",optarg); 
                printf("configfile: %s\n",configfile);
		break;
		case 'o':
		sprintf(outputfile,"%s",optarg); 
                printf("outputfile: %s\n",outputfile);
		break;
		case 'v':
		verbose++;      
		debug=1;
		break;
		case 'p':
		phasing=1;
                tx=17;
		break;
		case 'b':
		bas=1;
                tx=18;
		break;
		case 'S':
		save=1;      
		break;
		case 'h':  
		case '?':
                usage();
		exit(0);  
		break;
     
		default:
		abort();
	}
        }
        if(command==0) {
          printf("Invalid command, read --help option\n");
          exit(0);
        }
	printf("Open Serial Port: %s\n",iniparser_getstring(agc_ini,"serial_port:dev",transmitter_port));
        if(phasing) {
          command=20;
          if(tx!=17) {
            printf("Invalid phasing address. Use 17\n");
            exit(0);
	  }
        } else if(bas) {
          command=14;
          if(tx!=18) {
            printf("Invalid bas address. Use 18\n");
            exit(0);
	  }
        } else {
          if((tx==0)|| (tx > 16) ) {
            printf("Invalid TX position. Use 1-16  or use -1 to loop\n");
            exit(0);
          }
        }
        gettimeofday(&t_now,NULL);
        t_then.tv_sec=t_now.tv_sec-timeout-10;
        agc_ini=iniparser_load(configfile);
        //iniparser_dump_ini(agc_ini,stdout);
        for(i=1;i<=16;i++){
          sprintf(entry,"tx_%d:address",i);
          tx_addresses[i]=iniparser_getint(agc_ini,entry,0);
          sprintf(entry,"tx_%d:disabled",i);
          tx_disabled[i]=iniparser_getboolean(agc_ini,entry,0);
          sprintf(entry,"tx_%d:lockout",i);
          tx_lockout[i]=iniparser_getint(agc_ini,entry,0);
        }

	while(flag) {
          if(auto_loop==1) {
            gettimeofday(&t_now,NULL);
            if (abs(t_now.tv_sec-t_then.tv_sec) < timeout) {
              sleep(1);
              continue;
            } 
          } else {
          }        
          if(tx>0) {
            transmitter_fd=tx_port_open(agc_ini,transmitter_port);
	    printf("Transmitter FD: %d\n",transmitter_fd);
	    printf("Transmitter:: Position %d : Address %d: Command: %d\n",tx,tx_addresses[tx],command);
            if(((tx_disabled[tx])==0 && (tx_lockout[tx] < 3 )) || (command==READ_STATUS)) { 
	      status=send_command(transmitter_fd,tx_addresses[tx],command,results,debug);
              printf("Command Status,%d\n",status);
              if(status==TIMEOUT) {
                printf("Command Timeout, retrying one more time after 5 second wait\n");
                sleep(5);
	        status=send_command(transmitter_fd,tx_addresses[tx],command,results,debug);
                if(status==TIMEOUT) {
                  printf("Retry Timeout\n");
		}
              }
	      if(command==20) {
                if(phasing) {
                  pm_results(results,&pm_res);
                  if(verbose > 0 ) pm_display(&pm_res);
                }  
                if(phasing) {
                  basbox_results(results,&bas_res);
                  if(verbose > 0 ) basbox_display(&bas_res);
                }  
              }
	      if(command==READ_STATUS) {
                sprintf(entry,"tx_%d:status",i);
                switch(status){
		  case GARBLED:
                    iniparser_set(agc_ini,entry,"GARBLED",NULL);
                    break;
		  case TIMEOUT:
                    iniparser_set(agc_ini,entry,"TIMEOUT",NULL);
		    break;
                  case STATUS:  
                    agc_results(results,&agc_res);
                    agc_lockout_test(tx,&agc_res,tx_disabled,tx_lockout);
                    if(verbose > 0 ) agc_display_results(&agc_res);
                    agc_save_results(agc_ini,tx,&agc_res,tx_disabled,tx_lockout);
                    break;
		  default:
                    iniparser_set(agc_ini,entry,"UNKNOWN",NULL);
		    break;
                } 
              } 
            } else {
              printf("  Transmitter command ignored:: Disabled: %d  Lockout: %d\n",tx_disabled[tx],tx_lockout[tx]);
            }
          }
          if(tx<0) {
            printf("Looping over TX positions 1-16\n");
	    for(i=1;i<=16;i++) {
              transmitter_fd=tx_port_open(agc_ini,transmitter_port);
	      printf("Transmitter FD: %d\n",transmitter_fd);
	      tx=i;
	      printf("Transmitter:: Position %d : Address %d: Command: %d\n",tx,tx_addresses[tx],command);
              if(((tx_disabled[tx])==0 && (tx_lockout[tx] < 3 )) || (command==READ_STATUS)) { 
                sleep(1);
	        status=send_command(transmitter_fd,tx_addresses[tx],command,results,debug);
                printf("Command Status,%d\n",status);
                if(status==TIMEOUT) {
                  printf("Command Timeout, retrying one more time after 5 second wait\n");
                  sleep(5);
	          status=send_command(transmitter_fd,tx_addresses[tx],command,results,debug);
                  if(status==TIMEOUT) {
                    printf("Retry Timeout\n");
		  }
                }
	        if(command==READ_STATUS) {
                  sprintf(entry,"tx_%d:status",i);
                  switch(status){
		    case GARBLED:
                      iniparser_set(agc_ini,entry,"GARBLED",NULL);
                      break;
		    case TIMEOUT:
                      iniparser_set(agc_ini,entry,"TIMEOUT",NULL);
		      break;
                    case STATUS:  
                      iniparser_set(agc_ini,entry,"STATUS",NULL);
                      agc_results(results,&agc_res);
                      agc_lockout_test(tx,&agc_res,tx_disabled,tx_lockout);
                      if (verbose > 0 ) agc_display_results(&agc_res);
                      agc_save_results(agc_ini,tx,&agc_res,tx_disabled,tx_lockout);
                      if(agc_res.relay==OPEN) {
                        if((tx_disabled[tx]==0) && (tx_lockout[tx] < 3 )) { 
                          printf("TX %d has open relay: Attempting reset: %d %d\n",tx, tx_disabled[tx],tx_lockout[tx]);
	                  status=send_command(transmitter_fd,tx_addresses[tx],RESET_RELAY,results,debug);
                        } else {
                          printf("TX %d has open relay: and is disabled/lockedout: %d %d\n",tx, tx_disabled[tx],tx_lockout[tx]);
                        }
                      }
                      if(agc_res.relay==CLOSED) {
                        if(tx_disabled[tx]==1) { 
                          printf("TX %d is disabled with closed relay: Attempting to trip: %d %d\n",tx, tx_disabled[tx],tx_lockout[tx]);
	                  status=send_command(transmitter_fd,tx_addresses[tx],TRIP_RELAY,results,debug);
                        } else {
                        }
                      }
                      break;
		    default:
                      iniparser_set(agc_ini,entry,"UNKNOWN",NULL);
		      break;
                  }
                } 
              } else {
                printf("  Transmitter command ignored:: Disabled: %d  Lockout: %d\n",tx_disabled[tx],tx_lockout[tx]);
              }
	      close(transmitter_fd);
	    }
          }  
	  if(save) {
            fp=NULL;
            if(save) fp=fopen(outputfile,"w+");
	    if(fp!=NULL) {
	      iniparser_dump_ini(agc_ini,fp);
	      fclose(fp); 
	    } else {
	      if (verbose > 0 ) iniparser_dump_ini(agc_ini,stdout);
   	    }
          }
	  if(auto_loop==1) {
            t_then.tv_sec=t_now.tv_sec;
            printf("Waiting %d seconds\n",timeout); 
	  } else {
            flag=0;
	  }
          transmitter_fd=-1; 
	} // end the while loop
  return 0;
}
