#include "computer_agent.h"
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
const char * computer_agent_describe (agent_t * gthis)
{
	char *a="R";
    return a;
}


static
int computer_agent_play (agent_t * gthis, const board_t * b,
        player_t color, unsigned int * move)
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

int generate(char * b,char place, int start, int max) 
{
   // find empty spot
   int a;
	//check if there is a winner each time
   for (unsigned int i=start; i<max; ++i)
   {
      char c=b[i];
      if (c!=' ')
         // can't place anything here
         continue;

      b[i]=place;
      // recurse
      puts(b);
      if(place=='X'){
         a=-100;
         a=MAX(a,generate(b, (place =='X' ? 'O':'X'),start, max));
      }
      if(place=='O'){
         a=100;
         a=MIN(a,generate(b, (place =='X' ? 'O':'X'),start, max));
      }
      generate(b, (place =='X' ? 'O':'X'),start, max);//关键是这个只能往后走,因为i+1

      // Remove token and try next pos
      b[i]=' ';
   }
   return a
}

static
bool computer_agent_destroy (agent_t * this)
{
   return true;
}

agent_t * computer_agent_create ()
{
    return NULL;
}



