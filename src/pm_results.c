#include "agc_commands.h"

void pm_results(char results[],struct pm_results *res){
	int i,beam,temp;

//sort out the a/d results
//channel	reading
//0			-5v		local
//1			-12v	from pm power supply via front connector
//2			+5v		local
//3			+12v	from pm power supply via front connector
//4			unused
//5			temperature on microcontroller
//6			unused
//7			unused

	res->alg_5v=(int)(results[2]&0xff);
	res->alg_neg5v=(int)(results[0]&0xff);
	res->alg_12v=(int)(results[3]&0xff);
	res->alg_neg12v=(int)(results[1]&0xff);
//	res->box_temp=(int)(results[5]&0xff);
	res->box_temp=(int)(((((((double)(results[5]&0xff)-0))*5.0)/256.0)*100)-273);

//now the bits that are inputs from the basbox directly to this card
	
//beam number being sent to the phasing cards
//the ls 3 beam bits need inverting because they are presented
//upsidedown by the hardware.
	temp=(int)(results[18]&0xff);	//contains beam and !beam
	beam=(temp)&0x01;					//beam in bit positions 6,4,2,0
	beam=beam+(((temp)&0x04)/2);
	beam=beam+(((temp)&0x10)/4);
	beam=beam+((temp&0x40)/8);
	res->beam=beam;

//inverse logic beam number being sent to the phasing cards
	beam=((temp)&0x02)/2;				//!beam in bit positions 7,5,3,1
	beam=beam+(((temp)&0x08)/4);
	beam=beam+(((temp)&0x20)/8);
	beam=beam+((temp&0x80)/16);
	res->invbeam=(beam^0x0f)&0x0f;

//test mode signals to cards
	temp=(int)(results[19]&0x3);
	if((temp&0x01)==1)
		res->test_mode=ON;
	else
		res->test_mode=OFF;

	if((temp&0x02)==2)
		res->invtest_mode=OFF;
	else
		res->invtest_mode=ON;

//now run round each phasing card in turn
	for(i=0;i!=10;i++){
		temp=(int)(results[i+8]&0xff);
		if((temp&0x01)==1)
			res->card[i].failb=BAD;
		else
			res->card[i].failb=GOOD;
	
		if((temp&0x02)==2)
			res->card[i].faila=BAD;
		else
			res->card[i].faila=GOOD;
	
		res->card[i].beam=((temp&0x3c)/4)^0x0f; // bits inverted
		if((temp&0x40)==0x40)
			res->card[i].testa=ON;
		else
			res->card[i].testa=OFF;

		if((temp&0x80)==0x80)
			res->card[i].testb=ON;
		else
			res->card[i].testb=OFF;

		}	//all cards done
	}

