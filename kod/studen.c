/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

#include "studen.h"
#include <string.h>
#include <stdlib.h>
#define D_LEN 20
#define NOACK -1
#define NOSEQ -1
#define NOCHK -1
#define A_SIDE 0
#define B_SIDE 1
#define WAIT_T 8

int A_state;
int B_state;
struct pkt *save_pk;
int make_checksum(struct pkt packet){
    int checksum = 0, i;
    char tmp;
    for(i = 0; i < D_LEN; i++){
        tmp = packet.payload[i];
        checksum+= (int)tmp;
    }
    checksum += packet.seqnum;
    checksum += packet.acknum;
    return checksum;
}
int check_checksum(struct pkt packet){
    int new_checksum = make_checksum(packet);
    if(new_checksum == packet.checksum) return 1;
    else    return 0;
}

int is_corrupt(struct pkt packet){
    if(check_checksum(packet) ) return 0;
    else return 1;
}

int is_ACK(struct pkt packet, int seqnum){
    // returns if 1 if the packets acknum matches seqnum
    // returns 0 if no match or packet is not ACK
    if (packet.acknum == seqnum){
        return 1;
    } else {
        return 0;
    }
}
int is_data(struct pkt packet, int seqnum){
    // returns 1 if the packets seqnum matches seqnum
    // returns 0 if packet doesnt match
    if (packet.seqnum == seqnum){
        return 1;
    } else {
        return 0;
    }
}
struct pkt make_pkt(struct msg message, int acknum, int checksum, int seq){
    struct pkt *packet = malloc(sizeof(struct pkt));
    packet->seqnum = seq;
    packet->acknum = acknum;
    strcpy(packet->payload, message.data);
    packet->checksum = checksum;

    return *packet;
}
void copy_pkt(struct pkt *source, struct pkt *dest){
    dest->seqnum = source->seqnum;
    dest->acknum = source->acknum;
    strncpy(dest->payload, source->payload, D_LEN);
    dest->checksum = source->checksum;

    return ;
}
struct pkt make_ACK(int acknum){
    struct pkt *packet = malloc(sizeof(struct pkt));
    packet->acknum = acknum;
    packet->seqnum = NOSEQ;
    packet->checksum = make_checksum(packet);
    return *packet;
}
void free_pkt(struct pkt *packet){
    free(packet->payload);
    free(packet);
    return;
}

/* called from layer 5, passed the data to be sent to other side */
void A_output( struct msg message){
    int checksum;
    struct pkt packet;
    printf("A:got message! %s\n", message.data);
    switch(A_state){
        case 0:
            checksum = make_checksum(packet);
            packet = make_pkt(message, NOACK, checksum, 0);
            copy_pkt(&packet, save_pk);
            tolayer3(A_SIDE, packet);
            starttimer(A_SIDE, WAIT_T);
            A_state = 1;
            printf("A: Sending packet seq: %d, ack %d, pay: %s\n", packet.seqnum, packet.acknum, packet.payload);
            break;
        case 1:
            // awaiting ACK for latest package, do nothing
            break;
        case 2:
            checksum = make_checksum(packet);
            packet = make_pkt(message, NOACK, checksum, 1);
            copy_pkt(&packet, save_pk);
            tolayer3(A_SIDE, packet);
            starttimer(A_SIDE, WAIT_T);
            A_state = 3;
            printf("A: Sending packet seq: %d, ack %d, pay: %s\n", packet.seqnum, packet.acknum, packet.payload);
            break;
        case 3:
            // awaiting ACK for latest package, do nothing
            break;
    }
}

void B_output(struct msg message)  /* need be completed only for extra credit */{}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet){
    printf("A: Got packet! seq: %d, ack %d, pay: %s chk: %d\n", packet.seqnum, packet.acknum, packet.payload, packet.checksum);
    switch(A_state){
        case 0:
            //not awaiting any ACK, do nothing
            break;
        case 1:
            if (!(is_corrupt(packet)) && is_ACK(packet, 0) ){
                puts("Received ACK for 0");
                // ACK received
                stoptimer(A_SIDE);
                free_pkt(save_pk);
                A_state = 2;
            } else {
                puts("NOT");
                printf("res are %d and %d\n", is_corrupt(packet), is_ACK(packet, 0));
            }
            break;
        case 2:
            //not awaiting any ACK, do nothing
            break;
        case 3:
            if (!(is_corrupt(packet)) && is_ACK(packet, 1) ){
                puts("Received ACK for 1");
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
            printf("A: Resending packet seq: %d, ack %d, pay: %s\n", save_pk->seqnum, save_pk->acknum, save_pk->payload);
            tolayer3(A_SIDE, *save_pk);
            starttimer(A_SIDE, WAIT_T);
            break;
        case 2:
            break;
        case 3:
            printf("A: Resending packet seq: %d, ack %d, pay: %s\n", save_pk->seqnum, save_pk->acknum, save_pk->payload);
            tolayer3(A_SIDE, *save_pk);
            starttimer(A_SIDE, WAIT_T);
            break;
    }
}

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init(){
    save_pk = malloc(sizeof(struct pkt));
    A_state = 0;
}


/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet){
    struct pkt send_pkt;
    printf("B: Got packet! seq: %d, ack %d, pay: %s\n", packet.seqnum, packet.acknum, packet.payload);
    switch(B_state){
        case 0:
            if (is_corrupt(packet) || is_data(packet, 1)){
               // do nothin
                puts("B: CORRUPT OR WRONG ORDER");
            } else if (!(is_corrupt(packet)) && is_data(packet, 0)){
                send_pkt = make_ACK(0);
                puts("B: sending ACK 0");
                tolayer3(B_SIDE, send_pkt);
            }
            B_state = 1;
            break;
        case 1:
            if (is_corrupt(packet) || is_data(packet, 0)){
                puts("B: CORRUPT OR WRONG ORDER");
               // do nothin
            } else if (!(is_corrupt(packet)) && is_data(packet, 1)){
                puts("B: sending ACK 1");
                send_pkt=make_ACK(1);
                tolayer3(B_SIDE, send_pkt);
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
