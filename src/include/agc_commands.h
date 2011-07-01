
#define TIMEOUT 0
#define ACK 1
#define STATUS 2
#define BAD_ADDRESS 3
#define BAD_COMMAND 4
#define OUT_OF_SERVICE 5
#define GARBLED 6

#define ON 1
#define OFF 0
#define OPEN 0
#define CLOSED 1
#define GOOD 1
#define BAD 0
#define IN 1
#define OUT 0
#define LOCAL 0
#define REMOTE 1

#define STX 0x55

#define READ_STATUS 1
#define RESET_RELAY 2
#define TRIP_RELAY 3
#define CLOSE_LOOP 4
#define OPEN_LOOP 5
#define CLOSE_CAP1 6
#define OPEN_CAP1 7
#define CLOSE_CAP2 8
#define OPEN_CAP2 9
#define RESTART 10
#define PING 11
#define ENABLE_AUTO 12
#define DISABLE_AUTO 13

//valid command numbers and expected response
//no	command                 target  response
//1	read status             agc             11 status bytes
//2	reset relay             agc             ack
//3	trip relay              agc             ack
//4	close loop              agc             ack
//5	open loop               agc             ack
//6	close cap1              agc             ack
//7	open cap1               agc             ack
//8	close cap2              agc             ack
//9	open cap2               agc             ack
//10    restart                 agc             ack
//11    ping                    agc             ack
//12	enable auto     agc             ack
//13	disable auto    agc             ack
//14	read status             basbox  17 status bytes
//20	read status     pmatrix 20 status bytes

//valid status returns:
//0:    timeout
//1:    ack
//2:    status bytes to follow in results array field
//3:    bad_address, logical address of send message not in range 1 to MAX_UCONT
//4:    bad_command, invalid command number in send message
//5:    out of service, requested microcontroller out of service
//6:    garbled response from microcontroller


struct agc_state{
                int dev;                //logical ucont address
                int relay;              //relay open/closed
                int loop;               //loop open/closed
                int cap1;               //cap 1 in/out
                int cap2;               //cap 2 in/out
                int autor;              //auto fault reset on/off
                };

struct agc_results{
                int alg_5v;             //a/d reading for   5v
                int alg_15v;    //                                      15v             
                int alg_500v;   //                                      500v
                int alg_neg15v; //                                      -15v
                int alg_50v;    //                                      50v
                int alg_temp;   //                                      temperature sensor
                int alg_fwd;    //                                      forward power
                int alg_ref;    //                                      reflected power
                int relay;              //relay state
                int inhibit;    //inhibit state
                int power;              //power state
                int cap1;               //cap 1 state
                int cap2;               //cap 2 state
                int loop;               //loop state
                int dc;                 //duty cycle state
                int vswr;               //vswr state
                int dig_5v;             //5v state
                int dig_15v;    //15v state
                int dig_500v;   //500v state
                int dig_neg15v; //-15v state
                int dig_50v;    //50v state
                };

//basbox status results structure
struct basbox_results{
                int psu1;                       //temp of 50v psu heatsink  1
                int psu2;                       //                                                      2
                int psu3;                       //                                                      3
                int psu4;                       //                                                      4
                int box_temp;           //temp of basbox
                int moni_5v;            //basbox 5v supply monitor
                long freq;                      //PTS-IF ie transmit frequency  (in kHz)
                int beam;                       //beam number
                int ant;                        //antenna number
                int attn;                       //attenuator setting
                int local;                      //local/remote
                int test;                       //test mode
                int tx;                         //stretched tx pulse
                int txrx;                       //stretched txrx gate pulse
                double dcycle;          //duty cycle (in %)
                };

//phasing matrix monitor status results
struct card_results{                            //one of these for each card
        int faila;                                              //tree A failed
        int failb;                                              //tree B failed
        int beam;                                               //beam no card thinks it is getting
        int testa;                                              //test mode for tree A
        int testb;                                              //test mode for tree B
        };

struct pm_results{
        int alg_5v;                                             //card's +5v reading
        int alg_neg5v;                                  //card's -5v  reading
        int box_temp;                                   //temp sensor on microcontroller
        int alg_12v;                                    //12v supply to tx/rx interface
        int alg_neg12v;                                 //-12v supply to tx/rx interface
        struct card_results card[10];   //each phasing card returns same sort of data
        int beam;                                               //beam no being sent to the cards
        int invbeam;                                    //negative logic version of above
        int test_mode;                                  //test mode on or off to cards
        int invtest_mode;                               //negative logic version of above
        };

