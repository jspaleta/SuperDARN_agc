#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "agc_commands.h"
int send_command(int fd,unsigned char address,unsigned char command,char results[],int debug) {
        unsigned char tx_string[10];
        unsigned char rx_string[80];
        unsigned char bcc;
        int i,ack,rlen;
	ssize_t status,count;
        size_t  bytes_to_read; 
//need to form the transmit block which consists of the following bytes:
//
// Byte 0               1               2               3               4
//              STX             addr    len             cmd             bcc
//
//where STX = 0x55
//              addr= microcontroller's physical address
//              len = length of the data field, always 1 for command packets
//              cmd = command number (this is the data field)
//              bcc = checksum. This is the least significant 8 bits of the sum of bytes
//                        1-3, ie all bytes excluding STX and bcc
//
//

        tx_string[0]=STX;
        tx_string[2]=1;
        tx_string[1]=(unsigned char)address;
        tx_string[3]=(unsigned char)command;
        bcc=0;
        for(i=1;i!=4;i++){
                bcc=bcc+tx_string[i];
        }
        tx_string[4]=bcc;
        if(debug==ON){
                fprintf(stderr,"\n-----------\n");
                fprintf(stderr,"xtx string: %x %x %x %x %x\n",(int)tx_string[0],(int)tx_string[1],(int)tx_string[2],
                                (int)tx_string[3],(int)tx_string[4]);
        }

        status=write(fd,tx_string,5);
        if(status!=5&&status!=(-1)){
                fprintf(stderr,"Odd write status...exiting\n");
                exit(-1);
        }
        if(status==(-1)){
                fprintf(stderr,"write error...exiting\n");
                exit(-1);
        }

        ack=OFF;
        switch(command){
                case 1:
                        rlen=16; //status from agc
                        break;
                case 14:
                        rlen=24; //status from basbox
                        break;
                case 20:
                        rlen=27; //status from phase shifter
                        break;
                default:
                        rlen=2;  //ack
                        ack=ON;
                        break;
	}
//receive packets are of the following format:
//ack packet: STX addr   where addr is the address of the replying microcontroller
//status packet:
//Byte  0               1               2               3.......        len+3
//              STX             0               len             data            bcc
//
//where len is the length of the data field
//              first element of the data is the address of the replying microcontroller
//              bcc is the checksum formed by the least significant 8 bits of the sum
//              of the byte excluding byte 0 (STX) and bcc
//
//
//get the reply
//
        bytes_to_read=rlen;
//Probably want to set some timeouts here
        count=read(fd,rx_string,bytes_to_read);
	if(count==0){ //timed out
		if(debug==ON)fprintf(stderr,"Timed out\n");
                return(TIMEOUT);
	}
        if(debug==ON){
                fprintf(stderr,"%d characters received\n",(int)count);
                fprintf(stderr,"Received string (hex):\n");
                for(i=0;i<rlen;i++){
                        fprintf(stderr,"%x ",(int)rx_string[i]);
                }
                fprintf(stderr,"\n");
        }
        if(count!=rlen){ //wrong length packet
                if(debug==ON)fprintf(stderr,"Bad rx length %d, expecting %d\n",(int)count,rlen);
                return(GARBLED);
        }

        if(rx_string[0]!=STX){  //STX not first character
                if(debug==ON)fprintf(stderr,"STX not first character\n");
                return(GARBLED);
        }

        if(ack==ON){
                if(rx_string[1]==tx_string[1]){ //check addresses match
                        if(debug==ON)fprintf(stderr,"Vaild ack\n");
                        return(ACK); 
                }else{
                        if(debug==ON)fprintf(stderr,"Bad ack address, got %d, expected %d\n",(int)rx_string[1],(int)tx_string[1]);
                        return(GARBLED);
                }
        }else{
                if(debug==ON)fprintf(stderr,"Processing status packet\n");
                if(rx_string[1]!=0){ //must always be 0
                        if(debug==ON)fprintf(stderr,"target address not 0, got %d\n",(int)rx_string[1]);
                        return(GARBLED);
                }
                if(rx_string[3]!=tx_string[1]){ //check addresses match
                        if(debug==ON)fprintf(stderr,"xBad address, got %d, expected %d\n",(int)rx_string[3],(int)tx_string[1]);
                        return(GARBLED);
                }
                if(command==14)rlen=rlen-2; //get 2 extra strange chars from basbox!!
                if(command==20)rlen=rlen-2; //get 2 extra strange chars from pm!!
                bcc=0;
                for(i=1;i!=(rlen-1);i++){
                        bcc=bcc+rx_string[i];
                }
                if(bcc!=rx_string[rlen-1]){ //bad bcc
                        if(debug==ON)fprintf(stderr,"Bad bcc, calculated %d, got %d\n",(int)bcc,(int)rx_string[rlen-1]);
                        return(GARBLED);
                }else{
//good status block so copy the results into results array
                        for(i=4;i!=(rlen-1);i++){
                                results[i-4]=rx_string[i];
                        }
                        return(STATUS); //status block ok
                        if(debug==ON)fprintf(stderr,"Good status block\n");
                }
        }
	return(GARBLED);
}
