#include <stdio.h>
#include <string.h>

#define INFINITY 999
#define NODE_ID 3

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
const int defaultcosts3[4] = { INFINITY, INFINITY, INFINITY, INFINITY };
const int connectcosts3[4] = { 7, INFINITY, 2, 0 };

struct distance_table 
{
  int costs[4][4];
} dt3;

void printdt3(struct distance_table *);

/* students to write the following two routines, and maybe some others */

void rtinit3() 
{
  printf("rtinit3 called at time %f\n", clocktime);

  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      dt3.costs[i][j] = INFINITY;
    }
  }

  // set initial costs
  for(int i = 0; i < 4; i++) {
    dt3.costs[i][NODE_ID] = connectcosts3[i];
    dt3.costs[i][i] = connectcosts3[i];
  }

  /* Update neighbor nodes with initial distance vector */
  struct rtpkt updatepkt;

  for (int y = 0; y < 4; ++y)
  {
    if (y != NODE_ID && connectcosts3[y] != INFINITY)
    {
      updatepkt.sourceid = NODE_ID;
      updatepkt.destid = y;
      for(int i = 0; i < 4; i++) {
        updatepkt.mincost[i] = dt3.costs[i][NODE_ID];
      }
      printf("calling tolayer2 in rtinit3 with NODE_ID=%d, packet source id = %d and packet dest id = %d\n", NODE_ID, updatepkt.sourceid, updatepkt.destid=y);
      tolayer2(updatepkt);
    }
  }
}



void rtupdate3(rcvdpkt)
  struct rtpkt *rcvdpkt;
  
{
    printf("rtupdate3\n");
    int self = 3;
    struct distance_table *selfCostTable = &dt3;
    const int* connectCosts = &connectcosts3[0];

    int sid = rcvdpkt->sourceid;
    int did = rcvdpkt->destid;
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
        int costFromSelfToSid = selfCostTable->costs[sid][self];
        int currentCostFromSelfToI = selfCostTable->costs[i][self];
        int costFromSelfToIViaSid = MIN(costFromSelfToSid+costFromSidToI, INFINITY);
        printf("incoming packet says cost from %d to %d is %d\n", sid, i, costFromSidToI);
        printf("we already know that the cost from %d to %d is %d\n", self, sid, costFromSelfToSid);
        printf("the current cost from %d to %d is %d\n", self, i, currentCostFromSelfToI);
        printf("given new information, we know that the cost from %d to %d via %d is now %d\n", self, i, sid, costFromSelfToIViaSid);
        selfCostTable->costs[i][sid] = costFromSelfToIViaSid;
        if(costFromSelfToIViaSid < currentCostFromSelfToI) {

            selfCostTable->costs[i][self] = costFromSelfToIViaSid;
            printf("shorter distance found: distance from %d to %d is now %d by going through %d\n", self, i, costFromSelfToIViaSid, sid);


            for(int i = 0; i < 4; i++) {
                if(connectCosts[i] != INFINITY && i != self) {
                    struct rtpkt packet;
                    packet.sourceid = self;
                    packet.destid = i;
                    for(int j = 0; j < 4; j++) {
                        packet.mincost[j] = selfCostTable->costs[j][self];
                    }
                    tolayer2(packet);
                }
            }



        }
    }

    printdt3(selfCostTable);

}


void printdt3(dtptr)
  struct distance_table *dtptr;
  
{
  printf("             via     \n");
  printf("   D3 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 1|  %3d   %3d\n",dtptr->costs[1][0], dtptr->costs[1][2]);
  printf("     2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);

}







