#include <stdio.h>
#include <string.h>

#define INFINITY 999
#define NODE_ID 1



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
const int defaultcosts1[4] = { INFINITY, INFINITY, INFINITY, INFINITY };
const int connectcosts1[4] = { 1,  0,  1, INFINITY };

struct distance_table 
{
  int costs[4][4];
} dt1;

void printdt1(struct distance_table *);

/* students to write the following two routines, and maybe some others */

void rtinit1()
{
  printf("rtinit1 called at time %f\n", clocktime);

  /* Initialize all costs to infinity */
  for (int x = 0; x < 4; ++x)
  {
    memcpy(&dt1.costs[x], &defaultcosts1, sizeof(defaultcosts1));
  }

  /* Initialize link costs from node to neighbors */
  memcpy(&dt1.costs[NODE_ID], &connectcosts1, sizeof(connectcosts1));

  /* Update neighbor nodes with initial distance vector */
  struct rtpkt updatepkt;

  for (int y = 0; y < 4; ++y)
  {
    if (y != NODE_ID && connectcosts1[y] != INFINITY)
    {
      updatepkt.sourceid = NODE_ID;
      updatepkt.destid = y;
      memcpy(&updatepkt.mincost, &dt1.costs[NODE_ID], sizeof(updatepkt.mincost));
      printf("calling tolayer2 in rtinit1 with NODE_ID=%d, packet source id = %d and packet dest id = %d\n", NODE_ID, updatepkt.sourceid, updatepkt.destid=y);
      tolayer2(updatepkt);
    }
  }
}


void rtupdate1(rcvdpkt)
  struct rtpkt *rcvdpkt;
  
{
    printf("rtupdate1\n");
    int self = 1;
    struct distance_table *selfCostTable = &dt1;
    const int* connectCosts = &connectcosts1[0];

    int sid = rcvdpkt->sourceid;
    int did = rcvdpkt->destid;
    if(did != self) {
        return;
    }

    for(int i = 0; i < 4; i++) {
        selfCostTable->costs[sid][i] = rcvdpkt->mincost[i];
    }

    // for all i and j, check if we can reach i faster by going through j
    int somethingChanged = 0;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++) {
            if(i == self || j == self) {
                continue;
            }
            // can already reach node i in shorter time that node j
            int distance = selfCostTable->costs[self][j]+selfCostTable->costs[j][i];
            int currentCost = selfCostTable->costs[self][i];
            
            if(distance < currentCost) {
                printf("detected change: node %d now knows it can reach %d with distance %d\n", self, i, distance);
                selfCostTable->costs[self][i] = distance;
                somethingChanged = 1;
            }
        }
    }
    if(somethingChanged > 0) {
        for(int i = 0; i < 4; i++) {
            // skip sending it to yourself or nodes not directly connected
            if(i == self || connectCosts[i] == INFINITY) {
                continue;
            }
            struct rtpkt packet;
            packet.sourceid = self;
            packet.destid = i;
            for(int j = 0; j < 4; j++) {
                packet.mincost[j] = selfCostTable->costs[self][j];
            }
            printf("rtupdate1: distance change discovered for %d: sending packet from %d to %d\n", self, self, i);
            tolayer2(packet);
        }
    } else {
        printf("rtupdate1: nothing changed in distance tables\n");
    }
    printdt1(selfCostTable);
}


void printdt1(dtptr)
  struct distance_table *dtptr;
  
{
  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d\n",dtptr->costs[3][0], dtptr->costs[3][2]);

}



void linkhandler1(linkid, newcost)
int linkid, newcost;   
/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
}


