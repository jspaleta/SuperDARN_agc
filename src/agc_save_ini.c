#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "iniparser.h"
#include "agc_commands.h"

void agc_save_results(dictionary *agc_ini,int tx,struct agc_results *res,int tx_disabled[],int tx_lockout[]){
	char entry[120];
	char value[120];
        char filename[120];
        char *time_string;
        int length;
	struct timeval t0;
        gettimeofday(&t0,NULL);
        sprintf(filename,"%s","/tmp/agc.status");
        sprintf(entry,"tx_%d:time",tx);
        time_string=ctime(&t0.tv_sec);
        length=strlen(time_string);
        strncpy(value,time_string,length-1);
        value[length-1]='\0';
        iniparser_set(agc_ini,entry,value,NULL);
        sprintf(entry,"tx_%d:disabled",tx);
        sprintf(value,"%d",tx_disabled[tx]);
        iniparser_set(agc_ini,entry,value,NULL);
        sprintf(entry,"tx_%d:lockout",tx);
        sprintf(value,"%d",tx_lockout[tx]);
        iniparser_set(agc_ini,entry,value,NULL);

        sprintf(entry,"tx_%d:status_relay",tx);
        switch(res->relay) {
	  case CLOSED:
            iniparser_set(agc_ini,entry,"CLOSED",NULL);
            break; 
	  case OPEN:
            iniparser_set(agc_ini,entry,"OPEN",NULL);
            break; 
	  default:
            printf("Relay: Unknown condition\n");
            break; 
        }
        sprintf(entry,"tx_%d:status_loop",tx);
        switch(res->loop) {
	  case CLOSED:
            iniparser_set(agc_ini,entry,"CLOSED",NULL);
            break; 
	  case OPEN:
            iniparser_set(agc_ini,entry,"OPEN",NULL);
            break; 
	  default:
            printf("Loop: Unknown condition\n");
            break; 
        }
        sprintf(entry,"tx_%d:status_cap1",tx);
        switch(res->cap1) {
	  case IN:
            iniparser_set(agc_ini,entry,"IN",NULL);
            break; 
	  case OUT:
            iniparser_set(agc_ini,entry,"OUT",NULL);
            break; 
	  default:
            printf("Cap1: Unknown condition\n");
            break; 
        }
        sprintf(entry,"tx_%d:status_cap2",tx);
        switch(res->cap2) {
	  case IN:
            iniparser_set(agc_ini,entry,"IN",NULL);
            break; 
	  case OUT:
            iniparser_set(agc_ini,entry,"OUT",NULL);
            break; 
	  default:
            printf("Cap2: Unknown condition\n");
            break; 
        }
        sprintf(entry,"tx_%d:status_inhibit",tx);
        switch(res->inhibit) {
	  case OFF:
            iniparser_set(agc_ini,entry,"OFF",NULL);
            break; 
	  case ON:
            iniparser_set(agc_ini,entry,"ON",NULL);
            break; 
	  default:
            printf("Inhibit: Unknown condition\n");
            break; 
        }
        sprintf(entry,"tx_%d:status_power",tx);
        switch(res->power) {
	  case GOOD:
            iniparser_set(agc_ini,entry,"GOOD",NULL);
            break; 
	  case BAD:
            iniparser_set(agc_ini,entry,"BAD",NULL);
            break; 
	  default:
            printf("Power: Unknown condition\n");
            break; 
        }
        sprintf(entry,"tx_%d:status_dc",tx);
        switch(res->dc) {
	  case GOOD:
            iniparser_set(agc_ini,entry,"GOOD",NULL);
            break; 
	  case BAD:
            iniparser_set(agc_ini,entry,"BAD",NULL);
            break; 
	  default:
            printf("DC: Unknown condition\n");
            break; 
        }
        sprintf(entry,"tx_%d:status_vswr",tx);
        switch(res->vswr) {
	  case GOOD:
            iniparser_set(agc_ini,entry,"GOOD",NULL);
            break; 
	  case BAD:
            iniparser_set(agc_ini,entry,"BAD",NULL);
            break; 
	  default:
            printf("VSWR: Unknown condition\n");
            break; 
        }
        sprintf(entry,"tx_%d:status_50v",tx);
        switch(res->dig_50v) {
	  case GOOD:
            iniparser_set(agc_ini,entry,"GOOD",NULL);
            break; 
	  case BAD:
            iniparser_set(agc_ini,entry,"BAD",NULL);
            break; 
	  default:
            printf("50v: Unknown condition\n");
            break; 
        }
        sprintf(entry,"tx_%d:status_neg15v",tx);
        switch(res->dig_neg15v) {
	  case GOOD:
            iniparser_set(agc_ini,entry,"GOOD",NULL);
            break; 
	  case BAD:
            iniparser_set(agc_ini,entry,"BAD",NULL);
            break; 
	  default:
            printf("neg 15v: Unknown condition\n");
            break; 
        }
        sprintf(entry,"tx_%d:status_15v",tx);
        switch(res->dig_15v) {
	  case GOOD:
            iniparser_set(agc_ini,entry,"GOOD",NULL);
            break; 
	  case BAD:
            iniparser_set(agc_ini,entry,"BAD",NULL);
            break; 
	  default:
            printf("neg 15v: Unknown condition\n");
            break; 
        }
        sprintf(entry,"tx_%d:status_500v",tx);
        switch(res->dig_500v) {
	  case GOOD:
            iniparser_set(agc_ini,entry,"GOOD",NULL);
            break; 
	  case BAD:
            iniparser_set(agc_ini,entry,"BAD",NULL);
            break; 
	  default:
            printf("500v: Unknown condition\n");
            break; 
        }
        sprintf(entry,"tx_%d:status_5v",tx);
        switch(res->dig_5v) {
	  case GOOD:
            iniparser_set(agc_ini,entry,"GOOD",NULL);
            break; 
	  case BAD:
            iniparser_set(agc_ini,entry,"BAD",NULL);
            break; 
	  default:
            printf("500v: Unknown condition\n");
            break; 
        }
/*
        printf("5v: %d\n",res->alg_5v);
        printf("50v: %d\n",res->alg_50v);
        printf("500v: %d\n",res->alg_500v);
        printf("neg 15v: %d\n",res->alg_15v);
        printf("pos 15v: %d\n",res->alg_neg15v);
        printf("temp: %d\n",res->alg_temp);
        printf("fwd: %d\n",res->alg_fwd);
        printf("ref: %d\n",res->alg_ref);
*/
}
