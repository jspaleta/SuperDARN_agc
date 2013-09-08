#include <stdio.h>
#include "agc_commands.h"

void agc_display_results(struct agc_results *res){

        switch(res->relay) {
	  case CLOSED:
            printf("Relay: Closed\n");
            break; 
	  case OPEN:
            printf("Relay: Open\n");
            break; 
	  default:
            printf("Relay: Unknown condition\n");
            break; 
        }
        switch(res->loop) {
	  case CLOSED:
            printf("Loop: Closed\n");
            break; 
	  case OPEN:
            printf("Loop: Open\n");
            break; 
	  default:
            printf("Loop: Unknown condition\n");
            break; 
        }
        switch(res->cap1) {
	  case IN:
            printf("Cap1: In\n");
            break; 
	  case OUT:
            printf("Cap1: Out\n");
            break; 
	  default:
            printf("Cap1: Unknown condition\n");
            break; 
        }
        switch(res->cap2) {
	  case IN:
            printf("Cap2: In\n");
            break; 
	  case OUT:
            printf("Cap2: Out\n");
            break; 
	  default:
            printf("Cap2: Unknown condition\n");
            break; 
        }
        switch(res->inhibit) {
	  case OFF:
            printf("Inhibit: Off\n");
            break; 
	  case ON:
            printf("Inhibit: On\n");
            break; 
	  default:
            printf("Inhibit: Unknown condition\n");
            break; 
        }
        switch(res->power) {
	  case GOOD:
            printf("Power: Good\n");
            break; 
	  case BAD:
            printf("Power: Bad\n");
            break; 
	  default:
            printf("Power: Unknown condition\n");
            break; 
        }
        switch(res->dc) {
	  case GOOD:
            printf("DC: Good\n");
            break; 
	  case BAD:
            printf("DC: Bad\n");
            break; 
	  default:
            printf("DC: Unknown condition\n");
            break; 
        }
        switch(res->vswr) {
	  case GOOD:
            printf("VSWR: Good\n");
            break; 
	  case BAD:
            printf("VSWR: Bad\n");
            break; 
	  default:
            printf("VSWR: Unknown condition\n");
            break; 
        }
        switch(res->dig_50v) {
	  case GOOD:
            printf("50v: Good\n");
            break; 
	  case BAD:
            printf("50v: Bad\n");
            break; 
	  default:
            printf("50v: Unknown condition\n");
            break; 
        }
        switch(res->dig_neg15v) {
	  case GOOD:
            printf("neg 15v: Good\n");
            break; 
	  case BAD:
            printf("neg 15v: Bad\n");
            break; 
	  default:
            printf("neg 15v: Unknown condition\n");
            break; 
        }
        switch(res->dig_15v) {
	  case GOOD:
            printf("pos 15v: Good\n");
            break; 
	  case BAD:
            printf("pos 15v: Bad\n");
            break; 
	  default:
            printf("neg 15v: Unknown condition\n");
            break; 
        }
        switch(res->dig_500v) {
	  case GOOD:
            printf("500v: Good\n");
            break; 
	  case BAD:
            printf("500v: Bad\n");
            break; 
	  default:
            printf("500v: Unknown condition\n");
            break; 
        }
        switch(res->dig_5v) {
	  case GOOD:
            printf("5v: Good\n");
            break; 
	  case BAD:
            printf("5v: Bad\n");
            break; 
	  default:
            printf("500v: Unknown condition\n");
            break; 
        }
        printf("5v: %d\n",res->alg_5v);
        printf("50v: %d\n",res->alg_50v);
        printf("500v: %d\n",res->alg_500v);
        printf("neg 15v: %d\n",res->alg_15v);
        printf("pos 15v: %d\n",res->alg_neg15v);
        printf("temp: %d\n",res->alg_temp);
        printf("fwd: %d\n",res->alg_fwd);
        printf("ref: %d\n",res->alg_ref);
/*
	res->alg_5v=results[0]&0x00ff;
	res->alg_15v=results[1]&0x00ff;
	res->alg_500v=results[2]&0x00ff;
	res->alg_neg15v=results[3]&0x00ff;
	res->alg_50v=results[4]&0x00ff;
	res->alg_temp=results[5]&0x00ff;
	res->alg_fwd=results[6]&0x00ff;
	res->alg_ref=results[7]&0x00ff;
	if((results[8]&0x01)!=0)
		res->relay=CLOSED;
	else
		res->relay=OPEN;

	if((results[8]&0x02)!=0)
		res->inhibit=ON;
	else
		res->inhibit=OFF;

	if((results[8]&0x04)!=0)
		res->power=GOOD;
	else
		res->power=BAD;

	if((results[9]&0x04)!=0)
		res->cap1=IN;
	else
		res->cap1=OUT;

	if((results[9]&0x08)!=0)
		res->cap2=IN;
	else
		res->cap2=OUT;

	if((results[9]&0x10)!=0)
		res->loop=CLOSED;
	else
		res->loop=OPEN;

	if((results[10]&0x01)!=0)
		res->dc=GOOD;
	else
		res->dc=BAD;

	if((results[10]&0x02)!=0)
		res->vswr=GOOD;
	else
		res->vswr=BAD;

	if((results[10]&0x04)!=0)
		res->dig_5v=GOOD;
	else
		res->dig_5v=BAD;

	if((results[10]&0x08)!=0)
		res->dig_15v=GOOD;
	else
		res->dig_15v=BAD;

	if((results[10]&0x10)!=0)
		res->dig_500v=GOOD;
	else
		res->dig_500v=BAD;

	if((results[10]&0x20)!=0)
		res->dig_neg15v=GOOD;
	else
		res->dig_neg15v=BAD;

	if((results[10]&0x40)!=0)
		res->dig_50v=GOOD;
	else
		res->dig_50v=BAD;
*/
}
