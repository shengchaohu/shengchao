#include "board.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef unsigned char player_t;

typedef struct board_t{
	unsigned int h;
	unsigned int w;
	unsigned int r;
	player_t * xo;
} board_t;

typedef board_t * board_tp;

static bool check_one_winner(const board_t * b, int now, player_t * player);
int generate(board_t *board, char place, int start, int max);
bool board_check(board_t *board,int a);

bool board_create (board_t ** b, unsigned int height,
		unsigned int width, unsigned int run,
        const player_t * initial){
	if(height && width && run && b){
		player_t * temp;
		board_tp btp;
		btp=(board_tp)malloc(sizeof(board_t));
		btp->h=height;btp->w=width;btp->r=run;
		if(initial){			
			temp=(player_t *)malloc((height*width+1)*sizeof(unsigned char));
			memcpy(temp,initial,height*width);
			*(temp+height*width)='\0';
			btp->xo=temp;	
		}
		else{
			temp=(player_t *)malloc((height*width+1)*sizeof(unsigned char));
			for(int i=0;i<(btp->h)*(btp->w);i++){
				*(temp+i)=0;
			}
			btp->xo=temp;	
		}
		assert(b);
		*b=btp;
		return true;
	}
	else{return false;}
}

bool board_destroy (board_t * b){
	if(b){
		free(b->xo);free(b);return true;
	}
	else{return false;}
}

bool board_play (board_t * b, unsigned int column, player_t player){
	if(b){
		for(int i=0;i<b->h;i++){
			if(*(b->xo+column+i*(b->w))==0){
				*(b->xo+column+i*(b->w))=player;
				return true;
			}
		}
		return false;
	}
	return false;
}

bool board_can_play (const board_t * b, player_t p){
	if(b){
		for(int j=0;j<b->w;j++){
			if(*(b->xo+j+(b->h-1)*(b->w))==0){
				return true;
			}
		}
		return false;
	}
	return false;
}

bool board_can_play_move (const board_t * b, player_t p, unsigned int
      column){
	if(b){
		for(int i=0;i<b->h;i++){
			if(*(b->xo+column+i*(b->w))==0){
				return true;
			}
		}
		return false;
	}
	return false;
}

bool board_unplay (board_t * b, unsigned int column){
	if(b){
		for(int i=0;i<b->h;i++){
			if(*(b->xo+column+i*(b->w))==0){
				if(i){
					*(b->xo+column+(i-1)*(b->w))=0;
					return true;
				}
				else{return false;}
			}
		}
		return false;
	}
	return false;
}

bool board_duplicate (board_t ** newboard, const board_t * old){
	if(newboard){
		assert(old);assert(old->xo);
		board_tp btp;
		btp=(board_tp)malloc(sizeof(board_t));
		btp->xo=(player_t *)malloc(((old->h)*(old->w)+1)*sizeof(unsigned char));
		memcpy(btp->xo,old->xo,(old->h)*(old->w));
		*(btp->xo+(old->h)*(old->w))='\0';
		btp->h=old->h;btp->w=old->w;btp->r=old->r;
		*newboard=btp;
		return true;
	}
	else{return false;}
}

static bool check_one_winner(const board_t * b, int now, player_t * player){
//for each node, we don't need to "look back"
	unsigned char color=b->xo[now];
	int count;
	assert(player);
	//check horizonal
	if(b->w-now%(b->w) >= b->r){
		count=0;
		for(int i=now; i<b->r+now;i++){
			if(b->xo[i]!=color){
				break;
			}
			else{count++;}
		}
		if(count==b->r){
			*player=color;
			return true;
		}
	}
	//check vertical
	if(b->h-now/(b->w) >= b->r){
		count=0;
		for(int i=0; i<b->r;i++){
			if(b->xo[i*(b->w)+now]!=color){
				break;
			}
			else{count++;}
		}
		if(count==b->r){
			*player=color;
			return true;
		}
	}
	//check -45
	if(b->h-now/(b->w) >= b->r && now%(b->w)+1>=b->r){
		count=0;
		for(int i=0; i<b->r;i++){
			if(b->xo[i*(b->w)+now-i]!=color){
				break;
			}
			else{count++;}
		}
		if(count==b->r){
			*player=color;
			return true;
		}
	}
	//check +45
	if(b->h-now/(b->w)>=b->r && b->w-now%(b->w) >= b->r){
		count=0;
		for(int i=0; i<b->r;i++){
			if(b->xo[i*(b->w)+now+i]!=color){
				break;
			}
			else{count++;}
		}
		if(count==b->r){
			*player=color;
			return true;
		}
	}
	return false;
}

int board_has_winner (const board_t * b, player_t * player){
	assert(b);
	for(int i=0;i<(b->h)*(b->w);i++){
		if(b->xo[i] != 0 && check_one_winner(b,i,player)==true){
			return 1;
		}
	}
	if(board_can_play(b,0)==false){return -1;}
	return 0;	
}

bool board_get (const board_t * b, unsigned int row, unsigned int column,
      player_t * piece){
	assert(b);assert(piece);
	if(row<(b->h) && column<(b->w)){
		*piece=b->xo[row*(b->w)+column];
		return true;
	}
	return false;
}

bool board_print (const board_t * b, FILE * f){
	if(f){
		player_t temp=3;
		for(unsigned int i=0;i<b->h;i++){
			if(i>0){putc('\n',f);}
			for(unsigned int j=0;j<b->w;j++){
				board_get(b,i,j,&temp);
				putc(temp+'0',f);
			}
		}
		putc('\n',f);
		return true;
	}
	return false;
}

bool board_clear (board_t * b){
	for(int i=0;i<(b->h)*(b->w);i++){
		b->xo[i]=0;
	}
	return true;
}

unsigned int board_get_height (const board_t * b){
	if(b){
		return b->h;
	}
	else{return 0;}
}

unsigned int board_get_width (const board_t * b){
	if(b){
		return b->w;
	}
	else{return 0;}
}

unsigned int board_get_run (const board_t * b){
	if(b){
		return b->r;
	}
	else{return 0;}
}

bool board_check(board_t *board,int a){
   unsigned int w=board->w;
   player_t *x=board->xo;
   if(a/w){
       for(int j=0;j<a/w;j++){
           if(*(x+a%w+j*w)==PLAYER_EMPTY){
              return false;
           }
      }
        return true;
   }
   else{return true;}
}


int generate(board_t *board, char place, int start, int max) 
{
   int a,temp;
   player_t p=0;
   if(place==PLAYER_BLUE && board_has_winner(board,&p)==0){//opponent playing
       a=-100;
      for (unsigned int i=start; i<max; ++i)
      {
         char c=board->xo[i];
         if (c!=PLAYER_EMPTY || board_check(board,i)==false)
            // can't place anything here, also cannot place here for rule
            continue;
         board->xo[i]=place;
         board_print(board,stdout);
         printf("%d\n",a);
         printf("\n");
         if(board_has_winner(board,&p)==1 && p==PLAYER_BLUE 
                  && (a==100 || a==-100)){//
            a=5;
         }
         else if(board_has_winner(board,&p)==1 && p==PLAYER_YELLOW 
                  && (a==100 || a==-100)){
            a=-5;
         }
         else if(board_has_winner(board,&p)==-1 
                  && (a==100 || a==-100)){
            a=0;
         }
         temp=generate(board, (place ==PLAYER_BLUE ? PLAYER_YELLOW:PLAYER_BLUE),
               start, max);
           board->xo[i]=PLAYER_EMPTY;
          a=(a>temp?a:temp);
      }
   }
   if(place==PLAYER_YELLOW && board_has_winner(board,&p)==0){
        a=100;
        for (unsigned int i=start; i<max; ++i)
      {
         char c=board->xo[i];
         if (c!=PLAYER_EMPTY || board_check(board,i)==false)
            // can't place anything here
            continue;
         board->xo[i]=place;
         board_print(board,stdout);
         printf("%d\n",a);
         printf("\n");
         if(board_has_winner(board,&p)==1 && p==PLAYER_BLUE 
                  && (a==100 || a==-100)){//
            a=5;
         }
         else if(board_has_winner(board,&p)==1 && p==PLAYER_YELLOW 
                  && (a==100 || a==-100)){
            a=-5;
         }
         else if(board_has_winner(board,&p)==-1 
                  && (a==100 || a==-100)){
            a=0;
         }
         temp=generate(board, (place ==PLAYER_BLUE ? PLAYER_YELLOW:PLAYER_BLUE),
               start, max);
         board->xo[i]=PLAYER_EMPTY;          
         a=(a<temp?a:temp);
      }
   }
   // board_print(board,stdout);
   // printf("\n");
   return a;
}


