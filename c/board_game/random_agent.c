#include "random_agent.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <errno.h>
#include <assert.h>
#include <stdbool.h>


static
const char * random_agent_describe (agent_t * gthis)
{
	char *a="R";
    return a;
}


static
int random_agent_play (agent_t * gthis, const board_t * b,
        player_t color, unsigned int * move)//color告诉我自己所执的颜色，也知道了先后
//先黄后蓝
{
   const unsigned int width = board_get_width(b);

   while (true)
   {
      *move = rand()%width;

      if (!board_can_play_move(b, color, *move))
      {
         printf ("Invalid move! I will Try again.\n");
         continue;
      }

      return 0;
   }
}

static
bool random_agent_destroy (agent_t * this)
{
   return true;
}

agent_t * random_agent_create ()
{
    // Allocate vtable structure
    agent_t * n = malloc (sizeof(agent_t));

    n->destroy = random_agent_destroy;
    n->play = random_agent_play;
    n->describe = random_agent_describe;
    n->data=NULL;

    return n;
}



