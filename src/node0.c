#include <stdio.h>
#include <string.h>

#define INFINITY 999
#define NODE_ID 0

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
const int defaultcosts0[4] = { INFINITY, INFINITY, INFINITY, INFINITY };
const int connectcosts0[4] = { 0, 1, 3, 7 };

struct distance_table 
{
  int costs[4][4];
} dt0;

void printdt0(struct distance_table *);

/* students to write the following two routines, and maybe some others */

void rtinit0() 
{
  printf("rtinit0 called at time %f\n", clocktime);

  /* Initialize all costs to infinity */
  for (int x = 0; x < 4; ++x)
  {
    memcpy(&dt0.costs[x], &defaultcosts0, sizeof(defaultcosts0));
  }

  /* Initialize link costs from node to neighbors */
  memcpy(&dt0.costs[NODE_ID], &connectcosts0, sizeof(connectcosts0));

  /* Update neighbor nodes with initial distance vector */
  struct rtpkt updatepkt;

  for (int y = 0; y < 4; ++y)
  {
    if (y != NODE_ID && connectcosts0[y] != INFINITY)
    {
      updatepkt.sourceid = NODE_ID;
      updatepkt.destid = y;
      memcpy(&updatepkt.mincost, &dt0.costs[NODE_ID], sizeof(updatepkt.mincost));
      printf("calling tolayer2 in rtinit0 with NODE_ID=%d, packet source id = %d and packet dest id = %d\n", NODE_ID, updatepkt.sourceid, updatepkt.destid=y);
      tolayer2(updatepkt);
    }
  }
}


void rtupdate0(rcvdpkt)
  struct rtpkt *rcvdpkt;
{

    printf("rtupdate0\n");
    int self = 0;
    struct distance_table *selfCostTable = &dt0;
    const int* connectCosts = &connectcosts0[0];

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
            printf("rtupdate0: distance change discovered for %d: sending packet from %d to %d\n", self, self, i);
            tolayer2(packet);
        }
    } else {
        printf("rtupdate0: nothing changed in distance tables\n");
    }
    printdt0(selfCostTable);
}


void printdt0(dtptr)
  struct distance_table *dtptr;
  
{
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n",dtptr->costs[1][1],
	 dtptr->costs[1][2],dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n",dtptr->costs[2][1],
	 dtptr->costs[2][2],dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][1],
	 dtptr->costs[3][2],dtptr->costs[3][3]);
}

void linkhandler0(linkid, newcost)
  int linkid, newcost;

/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
}

