#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include "iniparser.h"
#include "agc_commands.h"

void agc_lockout_test(int tx,struct agc_results *res,int *tx_disabled,int *tx_lockout){
        int flag=0;
        switch(res->relay) {
	  case CLOSED:
            break; 
	  case OPEN:
            break; 
	  default:
            break; 
        }
        switch(res->loop) {
	  case CLOSED:
            break; 
	  case OPEN:
            break; 
	  default:
            break; 
        }
        switch(res->power) {
	  case GOOD:
            break; 
	  case BAD:
            break; 
	  default:
            break; 
        }
        switch(res->dc) {
	  case GOOD:
            break; 
	  case BAD:
            flag=1;
            break; 
	  default:
            break; 
        }
        switch(res->vswr) {
	  case GOOD:
            break; 
	  case BAD:
            flag=1;
            break; 
	  default:
            break; 
        }
        switch(res->dig_50v) {
	  case GOOD:
            break; 
	  case BAD:
            flag=1;
            break; 
	  default:
            break; 
        }
        switch(res->dig_neg15v) {
	  case GOOD:
            break; 
	  case BAD:
            flag=1;
            break; 
	  default:
            break; 
        }
        switch(res->dig_15v) {
	  case GOOD:
            break; 
	  case BAD:
            flag=1;
            break; 
	  default:
            break; 
        }
        switch(res->dig_500v) {
	  case GOOD:
            break; 
	  case BAD:
            flag=1;
            break; 
	  default:
            break; 
        }
        switch(res->dig_5v) {
	  case GOOD:
            break; 
	  case BAD:
            flag=1;
            break; 
	  default:
            break; 
        }
        if(flag){
          if(tx_lockout[tx]<3) tx_lockout[tx]++;
        } else {
          if(tx_lockout[tx]>0) tx_lockout[tx]=0;
        }
        if(tx_lockout[tx]>=3) {
          tx_disabled[tx]=1;
        } 
}
