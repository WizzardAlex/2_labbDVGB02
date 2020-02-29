/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

#include "Stud.h"
#include <string.h>
#include <stdlib.h>
#define D_LEN 20
#define NOACK -1
#define NOSEQ -1
#define NOCHK -1
#define A_SIDE 0
#define B_SIDE 1
#define WAIT_T 8
extern int A_state;
extern int B_state;
extern struct pkt *save_pk;

int is_ACK(struct pkt *packet, int seqnum){
    // returns if 1 if the packets acknum matches seqnum
    // returns 0 if no match or packet is not ACK
    if (packet->acknum == seqnum){ 
        return 1;
    } else {
        return 0;
    }
}
int is_datapkt(struct pkt *packet, int seqnum){
    // returns 1 if the packets seqnum matches seqnum
    // returns 0 if packet doesnt match
    if (packet->seqnum == seqnum){ 
        return 1;
    } else {
        return 0;
    }
}
struct pkt *make_pkt(struct msg message, int acknum, int checksum, int seq){
    struct pkt *packet = malloc(sizeof(struct pkt));
    packet->seqnum = seq;
    packet->acknum = acknum;
    char *payload = malloc(sizeof(char)*D_LEN);
    strcpy(packet->payload, message.data);
    packet->checksum = checksum;

    return packet;
}
struct pkt *make_ACK(int acknum){
    struct pkt *packet = malloc(sizeof(struct pkt));
    packet->acknum = acknum;
    packet->seqnum = NOSEQ;
    packet->checksum = NOCHK;
    return packet;
}
void freepkt(struct pkt *packet){
    free(packet->payload);
    free(packet);
    return;
}

int is_corrupt(struct pkt packet);

/* called from layer 5, passed the data to be sent to other side */
void A_output( struct msg message){
    int checksum;
    struct pkt *packet;
    switch(A_state){
        case 0:
            checksum = mck_checksum(message);
            packet = make_pkt(message, NOACK, checksum, 0);
            tolayer3(B_SIDE, *packet);
            save_pk = packet;
            starttimer(A_SIDE, WAIT_T);
            A_state = 1;
            break;
        case 1:
            // awaiting ACK for latest package, do nothing
            break;
        case 2:
            checksum = mck_checksum(message);
            packet = make_pkt(message, NOACK, checksum, 1);
            tolayer3(B_SIDE, *packet);
            save_pk = packet;
            starttimer(A_SIDE, WAIT_T);
            A_state = 3;
            break;
        case 3:
            // awaiting ACK for latest package, do nothing
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
            if (!(is_corrupt(packet)) && is_ACK(&packet, 0) ){
                // ACK received
                stoptimer(A_SIDE);
                free_pkt(save_pk);
                A_state = 2;
            }
            break;
        case 2:
            //not awaiting any ACK, do nothing
            break;
        case 3:
            if (!(is_corrupt(packet)) && is_ACK(&packet, 1) ){
                // ACK received
                stoptimer(A_SIDE);
                free_pkt(save_pk);
                A_state = 0;
            }
            break;
    }
}


/* called when A's timer goes off */
void A_timerinterrupt(){
    switch(A_state){
        case 0:
            break;
        case 1:
            tolayer3(B_SIDE, *save_pk);
            starttimer(A_SIDE, WAIT_T);
            break;
        case 2:
            break;
        case 3:
            tolayer3(B_SIDE, *save_pk);
            starttimer(A_SIDE, WAIT_T);
            break;
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
            if (is_corrupt(packet) || is_datapkt(&packet, 1)){
               // do nothin 
            } else if (!(is_corrupt(packet)) && is_datapkt(&packet, 0)){
                make_ACK(0);
                tolayer3(B_SIDE, packet);
            }
            B_state = 1;
            break;
        case 1:
            if (is_corrupt(packet) || is_datapkt(&packet, 0)){
               // do nothin 
            } else if (!(is_corrupt(packet)) && is_datapkt(&packet, 1)){
                make_ACK(1);
                tolayer3(B_SIDE, packet);
            }
            B_state = 0;
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
    }
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init(){
    B_state = 0;
}
