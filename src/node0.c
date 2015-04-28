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
      tolayer2(updatepkt);
    }
  }
}


void rtupdate0(rcvdpkt)
  struct rtpkt *rcvdpkt;
{

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

