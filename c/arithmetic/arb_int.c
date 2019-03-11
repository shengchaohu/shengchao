#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "arb_int.h"
#include <limits.h>
#include <errno.h>



struct arb_int
{
   int ndigits;
   char* d;
   char pn;
};

void arb_free (arb_int_t * i)
{
   free((*i)->d);
   free(*i);
}

int arb_duplicate (arb_int_t * new, const arb_int_t original)
{
   arb_int_t q=(arb_int_t)malloc(sizeof(arb_int));//è¦ååå§åï¼ç»æéåéåå­
   *new=q;
   int a=(*original).ndigits;
   char* s1=(*original).d;
   char* s2=(char*) malloc ((a+1)* sizeof(char));
   if (s2 !=NULL)
   {        
      (*new)->d=strcpy(s2,s1);;
      (*new)->ndigits=a;
      (*new)->pn=original->pn;
      return 0;
   }
   else {return 1;}
}

//check if a string contains only +,-,0-9; return 1 if yes, 0 else
int isnumstring(char* s)
{
   if (isdigit(*s) || *s=='+' || *s=='-')
   {
      s++;
      while (*s)
      {
         if (isdigit(*s)==0)
            return 0;
         else {s++;}
      }
      return 1;
   }
   else {return 0;}
}

//reverse copy from an string to arb_int_t->d, includes '\0'
char* reversecopy(char* s)//è¿åchar*ä¼æ´å¥½ï¼ä¸æ¹é¢é¿åº¦è¿åä¸äºï¼å¦ä¸æ¹é¢éç¨
{
   // arb_int_t tt, *t;//ä¸è½åªå®ä¹ä¸ä¸ªstruct pointerï¼ä¸æåä¸è¥¿çæ¶åå®æ¯æ²¡ææä¹ç
   // t=&tt;
   int a=strlen((const char*)s);//length of s (without \0)
   char* s2=(char*)malloc((a+1) * sizeof(char));
   if (s2 !=NULL)//if there is enough memory for copy
      //also need to be freed
   {
      //free(x);//return the memory immediately after check
      //allocate one more empty block in the end of char array of arb_int
      //which will not be counted in arb_int.ndigits
      //Use: for assign of '\0' at the end of char array 
      //s2=(char*) malloc ((a+1)* sizeof(char));
      for(char* p=s2+a;p>s2-1;--p,++s)//ææ¸æ¥å¾ªç¯ä»åªéå¼å§ï¼å°åªéç»æ..
      {
         if (p==s2+a)
         {
            *p='\0';
            --s;
         }
         else {*p=*s;}
      }
      //t->d=s2;//è¦åå¤å¶åå»ç
      return s2;//è¿ä¸ªå¯ä»¥è¿åï¼å ä¸ºt points toçä¸è¥¿æ¯mallocåéåå­çãèå¦ä¸ä¸ªå°±æ²¡äºã
   }
   else //return pointer pointing to NULL
   {
      s2=NULL;
      return s2;
   }
}

int arb_from_string (arb_int_t * i, const char * s)
{
   if(s){
      arb_int_t p=(arb_int_t)malloc(sizeof(arb_int));
      *i=p;
      char* r=(char*)s;
      char *temp;
      int len=strlen(s);
      if (isdigit(*r) && *r !='0'){
         for(temp=r+1;temp<r+len;++temp){
            if(isdigit(*temp)==0){return 1;}
         }
         (*i)->d=reversecopy(r);
         (*i)->ndigits=strlen(r);
         (*i)->pn='+';
         return 0;
      }
      if (*r=='0' && len==1){
         (*i)->d=reversecopy(r);
         (*i)->ndigits=1;
         (*i)->pn='+';
         return 0;
      }
      if (*r=='-'){
         if (isdigit(*(r+1)) && *(r+1) !='0'){
            for(temp=r+2;temp<r+len;++temp){
               if(isdigit(*temp)==0){return 1;}
            }
            (*i)->d=reversecopy(r+1);
            (*i)->ndigits=strlen(r+1);
            (*i)->pn='-';
            return 0;
         }
         return 1;
      }
      //printf("%s\n",i->d);//è¿ä¸ªè¿ånullï¼å ä¸ºå¨ä¹åå°±æåå­freeäºã
      if (*r=='+'){
         if (isdigit(*(r+1)) && *(r+1) !='0'){
            for(temp=r+2;temp<r+len;++temp){
               if(isdigit(*temp)==0){return 1;}
            }
            (*i)->d=reversecopy(r+1);//starts from the char after +
            (*i)->ndigits=strlen(r+1);
            (*i)->pn='+';     
            return 0;
         }
         return 1;
      }
      return 1;
   }
   else {return 1;}
}

int arb_from_int (arb_int_t * i, signed long long int source)
{
   //find the "string" length of source
   int d;
   if (source>0){
      signed long long int a=llabs(source);
      double b=log10(a);
      d=(int)b+1;//room for \0
   }
   if (source<0){
      if (source==LONG_MIN){d=21;}//room for \0 and '-'
      else{
         signed long long int a=llabs(source);
         double b=log10(a);
         d=(int)b+2;//room for \0 and '-'
      }  
   }
   if (source==0){d=1;}//room for \0
   //printf("%d\n",d);
   char *str=(char*)malloc((d+1)*sizeof(char));
   sprintf(str, "%lli", source);
   return arb_from_string(i, str);
}

int arb_to_string (const arb_int_t i, char * buf, size_t max)
{
   char* s=i->d;
   int a=strlen(i->d);
   char b=i->pn;
   if (strcmp(i->d,"0")==0){
      *buf='0';*(buf+1)='\0';
      return 0;
   }
   if (b=='+'){
      if (a<max){
         for(char *p=buf+a;p>buf-1;--p,++s){
            if(p==buf+a){*p='\0';--s;}
            else{*p=*s;}
         }
         return 0;
      }
      else{return 1;}
   }
   if (b=='-'){
      if (a<max){
         for(char *p=buf+a+1;p>buf-1;--p,++s){
            if(p==buf+a+1){*p='\0';--s;}
            else{
               if(p==buf){*p='-';}
               else{*p=*s;}
            }
         }
         return 0;
      }
      else{return 1;}
   }
   else {return 1;}
}

int arb_to_int (const arb_int_t i, long long int * out)
{
   char* s=i->d;
   char* r;
   signed long long int a;
   if (strcmp(s,"8085774586302733229")==0){a=LONG_MIN;}
   else{
      a=strtoll(reversecopy(s),&r,10);
      if(errno != ERANGE){
         if (i->pn=='-'){
            a=-a;
         }
      }
      else{return 1;}
      
   }
   if (a<=LONG_MAX && a>=LONG_MIN)
   {
      *out=a;
      return 0;
   }
   else {return 1;}
}


int arb_assign (arb_int_t x, const arb_int_t y)
{
   int len=strlen(y->d);
   char* s=y->d;
   char q=y->pn;
   char* s2=(char*)malloc((len+1)* sizeof(char));
   //since we allocated one more empty when arb_assign is converted
   //from string or int, we can now assign '\0' to its end.
   if (s2!=NULL)
   {  
      x->d=strcpy(s2,s);
      x->ndigits=len;
      x->pn=q;
      return 0;
   }
   else {return 1;}
}

//model version for add on two string, all positive, with x bigger than y.
//return 0 if success.
int arb_add_model(arb_int_t u, arb_int_t v)
{
   char* x=u->d; char* y=v->d;
   int a=strlen(x);int b=strlen(y);int residual=0;
   for(char* i=x;i<x+b;++i,++y){
      if (*i+*y-'0'+residual>=':'){
         *i=*i+*y-':'+residual;
         residual=1;
      }
      else{
         *i=*i+*y-'0'+residual;
         residual=0;
      }
   }
   if (residual){
      for(char*i=x+b;i<x+a;++i){
         if (*i=='9'){
            residual=1;
            *i='0';
         }
         else {
            *i=*i+1;
            residual=0;
            break;
         }
      }
   }
   if (residual){
      char* s2=(char*)malloc((a+2)* sizeof(char));
      if (s2){
         s2=strcpy(s2,x);
         *(s2+a)='1';
         *(s2+a+1)='\0';
         u->d=s2;
         u->ndigits=a+1;
         return 0;
      }
      else {return 1;}
   }
   return 0;
}

int arb_add (arb_int_t x, const arb_int_t y)//x=x+y
{
   arb_int_t temp;
   int k;
   char xpn=x->pn; char ypn=y->pn;
   if (strcmp(x->d,"0")==0){
      k=arb_assign(x,y);
      return k;
   }
   if (strcmp(y->d,"0")==0){
      return 0;
   }
   if (xpn=='-' && ypn=='+'){
      if (arb_compare(x,y)==-1 ||arb_compare(x,y)==0){
         arb_duplicate(&temp,y);
         x->pn='+';
         arb_subtract(temp,x);
         k=arb_assign(x,temp);
         return k;
      }
      arb_duplicate(&temp,y);
      temp->pn='-';
      return arb_subtract(x,temp);
   }
   if (xpn=='+' && ypn=='-'){
      if (arb_compare(x,y)==1 ||arb_compare(x,y)==0){
         arb_duplicate(&temp,y);
         temp->pn='+';
         return arb_subtract(x,temp);
      }
      else{
         arb_duplicate(&temp,y);
         temp->pn='+';
         return arb_subtract(x,temp);
      }
   }
   if (xpn=='+' && ypn=='+'){
      if (arb_compare(x,y)==1 ||arb_compare(x,y)==0){
         return arb_add_model(x,y);
      }
      else{
         arb_duplicate(&temp,y);//make a copy of y
         arb_add_model(temp,x);
         return arb_assign(x,temp);
      }
   }
   if (xpn=='-' && ypn=='-'){
      if (arb_compare(x,y)==-1 ||arb_compare(x,y)==0){
         return arb_add_model(x,y);
      }
      else{
         arb_duplicate(&temp,y);//make a copy of y
         arb_add_model(temp,x);
         return arb_assign(x,temp);
      }
   }
   return 0;
}


//model version for add on two string, all positive, with x bigger than y.
//return 0 if success.
int arb_substract_model(arb_int_t u, arb_int_t v){
   char* x=u->d; char* y=v->d;
   int a=strlen(x);int b=strlen(y);int residual=0;
   for(char* i=x;i<x+b;++i,++y){
      if (*i-*y-residual>=0){
         *i=*i-*y-residual+'0';
         residual=0;
      }
      else{
         *i=*i+10-*y-residual+'0';
         residual=1;
      }
   }
   if (residual){
      for(char*i=x+b;i<x+a;++i){
         if (i==x+a-1 && *i=='1'){
            *i='\0';
            u->ndigits=a-1;
            break;
         }
         if (*i=='0'){
            residual=1;
            *i='9';
         }
         else {
            *i=*i-1;
            residual=0;
            break;
         }
      }
   }
   arb_zerocheck(u);
   return 0;
}

int arb_subtract (arb_int_t x, const arb_int_t y){
   char xpn=x->pn; char ypn=y->pn;
   arb_int_t temp;
   int k;
   if (strcmp(x->d,"0")==0){
      k=arb_assign(x,y);
      if(y->pn=='+'){y->pn='-';}
      if(y->pn=='-'){y->pn='+';}
      return k;
   }
   if (strcmp(y->d,"0")==0){
      return 0;
   }
   if (xpn=='-' && ypn=='+'){
      arb_duplicate(&temp,y);
      temp->pn='-';
      return arb_add(x,temp);
   }
   if (xpn=='+' && ypn=='-'){
      arb_duplicate(&temp,y);
      temp->pn='+';
      return arb_add(x,temp);
   }
   if (xpn=='+' && ypn=='+'){
      if (arb_compare(x,y)==1 ||arb_compare(x,y)==0){
         return arb_substract_model(x,y);
      }
      else{
         arb_duplicate(&temp,y);//make a copy of y
         arb_substract_model(temp,x);
         k=arb_assign(x,temp);
         x->pn='-';
         return k;
      }
   }
   if (xpn=='-' && ypn=='-'){
      if (arb_compare(x,y)==-1 ||arb_compare(x,y)==0){
         arb_duplicate(&temp,y);
         temp->pn='+';
         x->pn='+';
         k=arb_substract_model(x,temp);
         x->pn='-';
         return k;
      }
      else{
         arb_int_t temp;
         arb_duplicate(&temp,y);//make a copy of y
         arb_substract_model(temp,x);
         k=arb_assign(x,temp);
         x->pn='+';
         return k;
      }
   }
   return 0;
}

//v has only one char
arb_int_t arb_multiply_one(arb_int_t x, char v){
   char* u=x->d;
   int residual=0;
   int a=strlen(u);
   arb_int_t q=(arb_int_t)malloc(sizeof(arb_int));
   char *h=(char*)malloc((a+1)*sizeof(char));
   q->d=h;
   for(char* i=u, *j=h;i<u+a;++i,++j){
      char temp=(*i-'0')*(v-'0')+residual;
      residual=(int)temp /10;
      *j=temp+'0'-residual*10;
   }
   *(h+a)='\0';
   q->ndigits=a;q->pn=x->pn;
   if (residual){
      char* s2=(char*)malloc((a+2)* sizeof(char));
      if (s2){
         s2=strcpy(s2,h);
         *(s2+a)='0'+residual;
         *(s2+a+1)='\0';
         q->d=s2;q->ndigits=a+1;
         return q;
      }
      else {s2=NULL;q->d=s2;return q;}
   }
   return q;
}

arb_int_t arb_multiply_byten(arb_int_t x,int times){
   char* u=x->d;
   arb_int_t q=(arb_int_t)malloc(sizeof(arb_int));
   q->ndigits=strlen(u);q->pn=x->pn;
   char* s2=(char*)malloc((strlen(u)+times+1)* sizeof(char));
   if (s2){
      strcpy(s2+times,u);
      for(char*i=s2;i<s2+times;++i){
         *i='0';
      }
      *(s2+strlen(u)+times)='\0';
      q->d=s2;q->ndigits=strlen(u)+times;
      arb_free(&x);
      return q;
   }
   else {s2=NULL;q->d=s2;arb_free(&x);return q;}
}

//all positive, and u bigger than v
int arb_multiply_model(arb_int_t u, arb_int_t v){
   char* y=v->d;char *x=u->d;
   int b=strlen(y);int a=strlen(x);
   arb_int_t temp=(arb_int_t)malloc(sizeof(arb_int));
   char *o=(char*)malloc(a*b*sizeof(char));*o='0';*(o+1)='\0';
   temp->ndigits=1;temp->d=o;temp->pn='+';
   int count=0;
   for (char *i=y;i<y+b;++i,++count){
      arb_add(temp,arb_multiply_byten(arb_multiply_one(u,*i),count));
   }
   arb_zerocheck(temp);
   u->d=temp->d;
   return 0;
}

void arb_zerocheck(arb_int_t u){
   char* x=u->d;int a=strlen(x);int count=0;
   for(char*i=x+a-1;i>x-1;--i,++count){
      if(*i !='0')
         break;
   }
   u->ndigits=a-count;
   if(a==count){*(x+1)='\0';}
   else{*(x+a-count)='\0';}
}

int arb_multiply (arb_int_t x, const arb_int_t y)
{
   arb_int_t temp;
   char xpn=x->pn; char ypn=y->pn;
   int k;
   if (strcmp(x->d,"0")==0){
      return 0;
   }
   if (strcmp(y->d,"0")==0){
      k=arb_assign(x,y);
      return 0;
   }
   if (xpn=='-' && ypn=='+'){
      x->pn='+';
      if (arb_compare(x,y)==1 ||arb_compare(x,y)==0){
         k=arb_multiply_model(x,y);
         x->pn='-';
         return k;
      }
      else{
         arb_duplicate(&temp,y);
         arb_multiply_model(temp,x);
         k=arb_assign(x,temp);
         x->pn='-';
         arb_free(&temp);
         return k;
      }
   }
   if (xpn=='+' && ypn=='-'){
      x->pn='-';
      if (arb_compare(x,y)==-1 ||arb_compare(x,y)==0){
         k=arb_multiply_model(x,y);
         return k;
      }
      else{
         arb_duplicate(&temp,y);
         arb_multiply_model(temp,x);
         temp->pn='-';
         k=arb_assign(x,temp);
         arb_free(&temp);
         return k;
      }
   }
   if (xpn=='+' && ypn=='+'){
      if (arb_compare(x,y)==1 ||arb_compare(x,y)==0){
         return arb_multiply_model(x,y);
      }
      else{
         arb_duplicate(&temp,y);//make a copy of y
         arb_multiply_model(temp,x);
         k=arb_assign(x,temp);
         arb_free(&temp);
         return k;
      }
   }
   if (xpn=='-' && ypn=='-'){
      if (arb_compare(x,y)==-1 ||arb_compare(x,y)==0){
         k=arb_multiply_model(x,y);
         x->pn='+';
         return k;
      }
      else{
         arb_duplicate(&temp,y);//make a copy of y
         arb_multiply_model(temp,x);
         temp->pn='+';
         k=arb_assign(x,temp);
         arb_free(&temp);
         return k;
      }
   }
   return 0;
}



int arb_compare (const arb_int_t x, const arb_int_t y)
{
   char xpn=x->pn; char ypn=y->pn;
   int xd=x->ndigits; int yd=y->ndigits;
   char* s1=x->d; char* s2=y->d;
   if (xpn=='-' && ypn=='+'){
      return -1;
   }
   if (xpn=='+' && ypn=='-'){
      return 1;
   }
   if (xpn=='+' && ypn=='+'){
      if (xd==yd){
         for(char* i=s1+xd-1, *j=s2+xd-1;i>s1-1;--i,--j){
            if(*i>*j){
               return 1;
            }
            if(*i<*j){
               return -1;
            }
         }
         return 0;
      }
      if (xd<yd){return -1;}
      else {return 1;}
   }
   if (xpn=='-' && ypn=='-'){
      if (xd==yd){
         for(char* i=s1+xd-1, *j=s2+xd-1;i>s1-1;--i,--j){
            if(*i>*j){
               return -1;
            }
            if(*i<*j){
               return 1;
            }
         }
         return 0;
      }
      if (xd<yd){return 1;}
      else {return -1;}
   }
   return 0;
}

int arb_digit_count (const arb_int_t x){
   if (x->pn=='+'){return x->ndigits;}
   else{return x->ndigits+1;}
}













