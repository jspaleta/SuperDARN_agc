#include "agc_commands.h"

void pm_display(struct pm_results *res){
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

        printf("5v: %d\n",res->alg_5v);
        printf("-5v: %d\n",res->alg_neg5v);
        printf("12v: %d\n",res->alg_12v);
        printf("-12v: %d\n",res->alg_neg12v);
        printf("Box temp: %d\n",res->box_temp);
        printf("Beam: %d\n",res->beam);
        printf("invBeam: %d\n",res->invbeam);
        printf("Test Mode: %d\n",res->test_mode);
        printf("invTest Mode: %d\n",res->invtest_mode);


//now run round each phasing card in turn
        printf("A: ");
	for(i=0;i!=10;i++){
          if(res->card[i].faila==BAD) {
            printf("X"); 
          } else {
            printf("."); 
          }
	}
        printf("\n");
        printf("B: ");
	for(i=0;i!=10;i++){
          if(res->card[i].failb==BAD) {
            printf("X"); 
          } else {
            printf("."); 
          }
	}
        printf("\n");
}
