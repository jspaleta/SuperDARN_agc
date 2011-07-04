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

int debug=1;
int verbose=0;
int tx=-1;
dictionary *agc_ini;
int main(int argc, char **argv){

	static char transmitter_port[80]="/dev/ttyS2";
	static char basbox_port[80]="/dev/ttyS1";
	static char phasing_port[80]="/dev/ttyS1";

	static int tx_positions[19]={
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18
	};
	static int tx_addresses[19]={
		0,1,10,16,17,5,4,13,18,14,8,11,12,2,9,15,6,3,7
	};
	static int basbox_address=20;
	static int phasing_address=40;
	int	status,i,transmitter_fd;
        unsigned char command;
	char	results[25];
	char	entry[120];
	char	configfile[120];
	struct termios options;
	struct agc_results agc_res;
        int arg;                //command line option parsing
	static struct option long_options[] =
        {
		/* These options don't set a flag.
		* We distinguish them by their indices. */
		{"verbose",      no_argument,            0,      'v'},
		{"reset_relays", no_argument,      0,      'r'},
		{"trip_relays",  no_argument,      0,      't'},
		{"status",  no_argument,      0,      's'},
		{"transmitter",  required_argument,      0,      'x'},
		{"configfile",  required_argument,      0,      'c'},
		{"help",         required_argument,      0,      'h'},
		{0, 0, 0, 0}
        };
        /* Process arguments */
        while (1)
        {
           /* getopt_long stores the option index here. */
           int option_index = 0;
     
           arg = getopt_long (argc, argv, "vrstx:c:h",
                            long_options, &option_index);
     
           /* Detect the end of the options. */
           if (arg == -1)
             break;

	switch (arg)
	{
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
		case 'v':
		verbose++;      
		break;
		case 'h':  
		case '?':
		printf ("Usage:\n" );
		printf ("Transmitter Ucontroller manager\n" );
		printf ("-h / --help : show this message\n");
		printf ("-c PATH / --configfile PATH : \n configfile");
		printf ("-v / --verbose : increase verbosity by 1\n");
		printf ("-r / --reset : reset relays\n");
		printf ("-t / --trip : trip relays\n");
		printf ("-x NUM / --transmitter NUM : \n transmitter postion\n");
		printf ("If no transmitter selected, loops over all 16 transmitters\n");
		exit(0);  
		break;
     
		default:
		abort();
	}
        }
        agc_ini=iniparser_load("agc.ini");
        iniparser_dump_ini(agc_ini,stdout);
        for(i=1;i<=16;i++){
          sprintf(entry,"tx_%d:address",i);
          tx_addresses[i]=iniparser_getint(agc_ini,entry,0);
        }
	printf("Open Serial Ports\n");
    /* open the transmitter port */
    transmitter_fd = open(transmitter_port, O_RDWR | O_NOCTTY | O_NDELAY);
    fcntl(transmitter_fd, F_SETFL, 0);
    /* get the current options */
    tcgetattr(transmitter_fd, &options);
    /* set raw input, 5 second timeout */
    cfsetispeed(&options, B9600);  //9600 baud
    options.c_cflag     |= (CS7 | CLOCAL | CREAD);  //7 data bits 1 stop
    options.c_lflag     &= ~(ICANON | ECHO | ECHOE | ISIG); //raw input
    options.c_oflag     &= ~OPOST; // raw output
    options.c_cc[VMIN]  = 0;
    options.c_cc[VTIME] = 50;

    /* set the options */
    tcsetattr(transmitter_fd, TCSANOW, &options);

	if(transmitter_fd <=0 ) { 
		fprintf(stderr,"Couldn't open the transmitter port\n");
		printf("Close ports\n");
		close(transmitter_fd);
		exit(-1);
	}
	printf("Transmitter FD: %d\n",transmitter_fd);
        if((tx==0)|| (tx > 16) ) {
          printf("Valid TX position is 1-16\n");
          exit(0);
        }
        if(tx>0) {
	    printf("Transmitter:: Position %d : Address %d: Command: %d\n",tx,tx_addresses[tx],command);
	    status=send_command(transmitter_fd,tx_addresses[tx],command,results,debug);
	    if(command==READ_STATUS) {
              if(status==STATUS) { 
                agc_results(results,&agc_res);
                agc_display(&agc_res);
              }
            } 
        }
        if(tx<0) {
          printf("Looping over all TX 1-16\n");
	  for(i=1;i<=16;i++) {
	  tx=i;
          sleep(1);
	    printf("Transmitter:: Position %d : Address %d: Command: %d\n",tx,tx_addresses[tx],command);
	    status=send_command(transmitter_fd,tx_addresses[tx],command,results,debug);
	    if(command==READ_STATUS) {
              if(status==STATUS) { 
                agc_results(results,&agc_res);
                agc_display(&agc_res);
              }
            } 
          sleep(1);
	  }
        }  
/*
	while(1) {
          command=READ_STATUS;
	  for(i=0;i<16;i++) {
            if(tx_addresses[i]==tx) {
	      printf("Transmitter %d : Get Status\n",i);
	      status=send_command(transmitter_fd,tx_addresses[i],command,results,debug);
              if(status==STATUS) { 
                agc_results(results,&agc_res);
                agc_display(&agc_res);
              }
	    }
          }
          exit(0);
	  sleep(60);
 	}
*/
  return 0;
}
