#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h> /* POSIX terminal control definitions */

#include "agc_commands.h"

int debug=1;

int main(){

	static char transmitter_port[80]="/dev/ttyS0";
	static char basbox_port[80]="/dev/ttyS1";
	static char phasing_port[80]="/dev/ttyS1";

	static int transmitter_addresses[18]={
		1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};
	static int basbox_address=20;
	static int phasing_address=40;
	int	status,i,transmitter_fd;
        unsigned char command;
	char	results[25];
	struct termios options;
	struct agc_results agc_res;

	printf("Open Serial Ports\n");
    /* open the transmitter port */
    transmitter_fd = open(transmitter_port, O_RDWR | O_NOCTTY | O_NDELAY);
    fcntl(transmitter_fd, F_SETFL, 0);
    /* get the current options */
    tcgetattr(transmitter_fd, &options);
    /* set raw input, 5 second timeout */
    options.c_cflag     |= (CLOCAL | CREAD);
    options.c_lflag     &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag     &= ~OPOST;
    options.c_cc[VMIN]  = 0;
    options.c_cc[VTIME] = 50;

    /* set the options */
    tcsetattr(transmitter_fd, TCSANOW, &options);

	printf("Transmitter FD: %d\n",transmitter_fd);
	if(transmitter_fd <=0 ) { 
		fprintf(stderr,"Couldn't open the transmitter port\n");
		printf("Close ports\n");
		close(transmitter_fd);
		exit(-1);
	}
	while(1) {
          command=READ_STATUS;
	  for(i=0;i<16;i++) {
	    status=send_command(transmitter_fd,transmitter_addresses[i],command,results,debug);
            if(status==STATUS) { 
              agc_results(results,&agc_res);
            }
	  }
	  sleep(60);
 	}
  return 0;
}
