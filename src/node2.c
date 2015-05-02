/*
 * CS-521-A
 * Professor Rahman
 *
 * 4/30/15
 * Authors: Nicholas Bevacqua (nbevacqu) & Neal Trischitta (ntrischi) & Michael Peleshenko (mpeleshe)
 *
 * I pledge my honor that I have abided by the Stevens Honor System.
 *
 * Distance Vector Routing Simulator
 */

#include <stdio.h>
#include <string.h>

#define INFINITY 999
#define NODE_ID 2

#define MIN(a, b) ((a < b)? a : b)

extern struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
} rtpkt;

/* External declarations */
extern int TRACE;
extern int YES;
extern int NO;
extern float clocktime;
extern void tolayer2();

/* Internal declarations */
const int defaultcosts2[4] = { INFINITY, INFINITY, INFINITY, INFINITY };
const int connectcosts2[4] = { 3, 1, 0, 2 };

struct distance_table 
{
  int costs[4][4];
} dt2;

void printdt2(struct distance_table *);

/* students to write the following two routines, and maybe some others */

void rtinit2() 
{
    printf("[t=%f] [rtinit2] CALL - called\n", clocktime);

    int self = NODE_ID;
    struct distance_table *selfCostTable = &dt2;
    const int* connectCosts = &connectcosts2[0];

    /* Set initial costs to INFINITY */
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            selfCostTable->costs[i][j] = INFINITY;
        }
    }

    /* Set initial costs for self to neighbors */
    for(int i = 0; i < 4; i++) {
        selfCostTable->costs[i][self] = connectCosts[i];
        selfCostTable->costs[i][i] = connectCosts[i];
    }

    /* Update neighbor nodes with initial distance vector */
    struct rtpkt updatepkt;

    for (int y = 0; y < 4; ++y)
    {
        if (y != self && connectCosts[y] != INFINITY)
        {
            updatepkt.sourceid = self;
            updatepkt.destid = y;
            for(int i = 0; i < 4; i++) {
                updatepkt.mincost[i] = selfCostTable->costs[i][self];
            }
            printf("[t=%f] [rtinit2] SENDMSG - tolayer2 called in rtinit2 with NODE_ID=%d, packet source id = %d and packet dest id = %d\n", clocktime, self, updatepkt.sourceid, updatepkt.destid=y);
            tolayer2(updatepkt);
        }
    }
}



void rtupdate2(struct rtpkt *rcvdpkt)
{
    printf("[t=%f] [rtupdate2] CALL - called with packet from %d\n", clocktime, rcvdpkt->sourceid);

    int self = NODE_ID;
    short dv_changed = 0;
    struct distance_table *selfCostTable = &dt2;
    const int* connectCosts = &connectcosts2[0];

    /* Source and destination from received packet */
    int sid = rcvdpkt->sourceid;
    int did = rcvdpkt->destid;

    /* Check if destid is self */
    if(did != self) {
        return;
    }

    // how much does it cost to reach i from our current value compared to going through node sid
    for(int i = 0; i < 4; i++) {
        /*
        if(i == sid) {
            printf("no need to compute distance from %d to %d via %d\n", self, i, sid);
            continue;
        } else {
            printf("calculating distance from %d to %d via %d\n", self, i, sid);
        }
        */
        // selfCostTable[n1][n2] means cost to reach n1 via n2
        int costFromSidToI = rcvdpkt->mincost[i];
        int currentCostFromSelfToI = selfCostTable->costs[i][self];
        int costFromSelfToSid = selfCostTable->costs[sid][self];
        int costFromSelfToIViaSid = MIN(costFromSelfToSid+costFromSidToI, INFINITY);

        printf("[t=%f] [rtupdate2] DEBUG - current known cost from %d to %d is %d\n", clocktime, self, sid, costFromSelfToSid);
        printf("[t=%f] [rtupdate2] DEBUG - current known cost from %d to %d is %d\n", clocktime, self, i, currentCostFromSelfToI);
        printf("[t=%f] [rtupdate2] DEBUG - the cost from %d to %d via %d is now %d\n", clocktime, self, i, sid, costFromSelfToIViaSid);

        /* Update selfCostTable with the cost to I via Sid */
        selfCostTable->costs[i][sid] = costFromSelfToIViaSid;

        /* Update own DV if new calculated cost is lower than current */
        if(costFromSelfToIViaSid < currentCostFromSelfToI) {

            selfCostTable->costs[i][self] = costFromSelfToIViaSid;
            dv_changed = 1;
            printf("[t=%f] [rtupdate2] DVCHANGED - distance from %d to %d is now %d by going through %d\n", clocktime, self, i, costFromSelfToIViaSid, sid);

            /* Update neighbors */
            for (int i = 0; i < 4; i++) {
                if (connectCosts[i] != INFINITY && i != self) {
                    struct rtpkt packet;
                    packet.sourceid = self;
                    packet.destid = i;
                    for (int j = 0; j < 4; j++) {
                        packet.mincost[j] = selfCostTable->costs[j][self];
                    }
                    printf("[t=%f] [rtupdate2] SENDMSG - tolayer2 called in rtupdate2 with NODE_ID=%d, packet source id = %d and packet dest id = %d\n", clocktime, NODE_ID, packet.sourceid, packet.destid);
                    tolayer2(packet);
                }
            }
        }
    }

    /* Log if DV did not change */
    if (dv_changed == 0)
    {
        printf("[t=%f] [rtupdate2] DVNOTCHANGED - no shorter paths found\n", clocktime);
    }

    printdt2(selfCostTable);
}


void printdt2(dtptr)
  struct distance_table *dtptr;
  
{
  printf("                via     \n");
  printf("   D2 |    0     1    3 \n");
  printf("  ----|-----------------\n");
  printf("     0|  %3d   %3d   %3d\n",dtptr->costs[0][0],
	 dtptr->costs[0][1],dtptr->costs[0][3]);
  printf("dest 1|  %3d   %3d   %3d\n",dtptr->costs[1][0],
	 dtptr->costs[1][1],dtptr->costs[1][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][0],
	 dtptr->costs[3][1],dtptr->costs[3][3]);
}







