#include "agc_commands.h"

void agc_results(char results[],struct agc_results *res){

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

	}

