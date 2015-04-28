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
extern void tolayer2();

/* Internal declarations */
const int defaultcosts1[4] = { INFINITY, INFINITY, INFINITY, INFINITY };
const int connectcosts1[4] = { 1,  0,  1, INFINITY };

struct distance_table 
{
  int costs[4][4];
} dt1;

/* students to write the following two routines, and maybe some others */

void rtinit1()
{
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
      tolayer2(updatepkt);
    }
  }
}


void rtupdate1(rcvdpkt)
  struct rtpkt *rcvdpkt;
  
{

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


