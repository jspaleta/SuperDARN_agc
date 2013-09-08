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

int tx_port_open(dictionary *agc_ini,char *transmitter_port) {
    int transmitter_fd=-1; 
    struct termios options;

    /* open the transmitter port */
    transmitter_fd = open(
      iniparser_getstring(agc_ini,"serial_port:dev",transmitter_port),
      O_RDWR | O_NOCTTY | O_NDELAY);
    fcntl(transmitter_fd, F_SETFL, 0);
    /* get the current options */
    tcgetattr(transmitter_fd, &options);
    /* set raw input, 5 second timeout */
    cfsetispeed(&options, B9600);  //9600 baud
    options.c_cflag     |= ( CLOCAL | CREAD);  //7 data bits 1 stop
    options.c_lflag     &= ~(ICANON | ECHO | ECHOE | ISIG); //raw input
    options.c_oflag     &= ~OPOST; // raw output
    options.c_cc[VMIN]  = 0;
    options.c_cc[VTIME] = 10;

    /* set the options */
    tcsetattr(transmitter_fd, TCSANOW, &options);

        if(transmitter_fd <=0 ) {
                fprintf(stderr,"Couldn't open the transmitter port\n");
                printf("Close ports\n");
                close(transmitter_fd);
                exit(-1);
        }
   return transmitter_fd;
}
