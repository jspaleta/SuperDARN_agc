#include "agc_commands.h"
void basbox_results(char results[],struct basbox_results *res){

	unsigned long a,b,c;

	res->psu1=(int)(((((((double)(results[0]&0xff)-2))*5.0)/256.0)*100)-273);
	res->psu2=(int)(((((((double)(results[1]&0xff)-2))*5.0)/256.0)*100)-273);
	res->psu3=(int)(((((((double)(results[2]&0xff)-2))*5.0)/256.0)*100)-273);
	res->psu4=(int)(((((((double)(results[3]&0xff)-2))*5.0)/256.0)*100)-273);
	res->box_temp=(int)(((((((double)(results[5]&0xff)-2))*5.0)/256.0)*100)-273);
	res->moni_5v=results[7]&0xff;

	a=(unsigned long)((results[8]&0x0f)+(((results[8]&0xf0)/16)*10)); //1&10kHz
	b=(unsigned long)(((results[9]&0x0f)*100)+(((results[9]&0xf0)/16)*1000)); //100kHz & 1MHz
	c=((unsigned long)(results[10]&0x0f))*10000; //10MHz
	res->freq=(a+b+c)-40625;	//subtract if

	res->beam=results[11]&0xff;
	res->ant=results[12]&0xff;
	results[13]=results[13]&0x07;				//attn is only ls 3 bits
	res->attn=results[13]*10;
	if((results[14]&0x01)==0)
		res->local=LOCAL;
	else
		res->local=REMOTE;
	if((results[14]&0x02)==0)
		res->test=OFF;
	else
		res->test=ON;
	if((results[14]&0x04)==0)
		res->tx=OFF;
	else
		res->tx=ON;
	if((results[14]&0x08)==0)
		res->txrx=OFF;
	else
		res->txrx=ON;
	

	res->dcycle=((double)(results[15]&0xff))+((((double)(results[16]&0xff))/256.0));

	}

