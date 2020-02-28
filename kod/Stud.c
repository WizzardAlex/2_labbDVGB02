/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

#include "Stud.h"
#include <string.h>
#include <stdlib.h>
#define D_LEN 20
#define NOACK -1
#define A_SIDE 0
#define B_SIDE 1
#define WAIT_T 8
extern int A_state;
extern int B_state;
extern int base_seq;

struct pkt *make_pkt(struct msg message, int acknum, int checksum){
    struct pkt *packet = malloc(sizeof(pkt));
    packet.seqnum = base_seq;
    base_seq++;
    packet.acknum = acknum
    char *payload = malloc(sizeof(char)*D_LEN);
    strcpy(payload, message.data);
    packet.payload = payload;
    packet.checksum = checksum;

    return packet;
}

/* called from layer 5, passed the data to be sent to other side */
void A_output( struct msg message){
    switch(A_state){
        case 0:
            int checksum = mck_checksum(message);
            struct pkt *packet = make_pkt(message, NOACK, checksum);
            tolayer3(B_SIDE, packet);
            starttimer(A_SIDE, WAIT_T)
            A_state = 1;
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
    }
}

void B_output(struct msg message)  /* need be completed only for extra credit */{}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet){
    switch(A_state){
        case 0:
            //not awaiting any ACK, do nothing
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
    }
}


/* called when A's timer goes off */
void A_timerinterrupt(){
    switch(A_state){
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
    }
}

}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init(){
    A_state = 0;
}


/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet){
    switch(B_state){
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
    }
}

/* called when B's timer goes off */
void B_timerinterrupt(){
    switch(B_state){
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
    }
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init(){
    B_state = 0;
}
