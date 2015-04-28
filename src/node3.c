#include <stdio.h>
#include <string.h>

#define INFINITY 999
#define NODE_ID 3

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
const int defaultcosts3[4] = { INFINITY, INFINITY, INFINITY, INFINITY };
const int connectcosts3[4] = { 7, INFINITY, 2, 0 };

struct distance_table 
{
  int costs[4][4];
} dt3;

/* students to write the following two routines, and maybe some others */

void rtinit3() 
{
  /* Initialize all costs to infinity */
  for (int x = 0; x < 4; ++x)
  {
    memcpy(&dt3.costs[x], &defaultcosts3, sizeof(defaultcosts3));
  }

  /* Initialize link costs from node to neighbors */
  memcpy(&dt3.costs[NODE_ID], &connectcosts3, sizeof(connectcosts3));

  /* Update neighbor nodes with initial distance vector */
  struct rtpkt updatepkt;

  for (int y = 0; y < 4; ++y)
  {
    if (y != NODE_ID && connectcosts3[y] != INFINITY)
    {
      updatepkt.sourceid = NODE_ID;
      updatepkt.destid = y;
      memcpy(&updatepkt.mincost, &dt3.costs[NODE_ID], sizeof(updatepkt.mincost));
      tolayer2(updatepkt);
    }
  }
}


void rtupdate3(rcvdpkt)
  struct rtpkt *rcvdpkt;
  
{

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







