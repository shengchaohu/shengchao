#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "trie.h"

struct trie_data_t;
typedef struct trie_data_t * trie_t;

struct trie_node_t;
typedef struct trie_node_t * trie_pos_t;

typedef struct trie_data_t{
	trie_pos_t first;//points to root node, which has no key
}trie_data_t;

typedef struct trie_node_t{
	trie_pos_t parent;
	trie_pos_t *child;//储存结构指针的数组
	int length;//length of array
	char key;// store part of key
	char *fullkey;//store the full key
	void *value;
	int count; //if count!=0, it's a leaf node
}trie_node_t;

static void DFS(trie_t trie, trie_walk_t walkfunc, void * priv, trie_pos_t t, int *k);
static void mDFS(trie_t trie, trie_pos_t t, int *k);
static void mmDFS(trie_t trie, trie_pos_t t, trie_free_t freefunc);
static trie_pos_t NewTrieNode();
static trie_pos_t searchnode(trie_pos_t node, const char pkey);


static trie_pos_t NewTrieNode(){//create a new trie node
	trie_pos_t node= (trie_pos_t)malloc(sizeof(trie_node_t));
	node->count=0;
	node->length=0;
	node->key='0';
	node->fullkey=NULL;
	node->child=NULL;
	node->parent=NULL;
	node->value=NULL;
	return node;
}

trie_t trie_new(){//root node is created
	trie_t new=(trie_t)malloc(sizeof(trie_data_t));
	trie_pos_t fst=NewTrieNode();
	new->first=fst;
	return new;
}

void trie_destroy(trie_t trie, trie_free_t freefunc){
	if(trie->first != NULL){
		trie_pos_t t=trie->first;
		mmDFS(trie, t, freefunc);
		free(trie);
	}
	else{return;}
}

// mmDGS is similar to DGS, but is used to walk through trie and free each node
static void mmDFS(trie_t trie, trie_pos_t t, trie_free_t freefunc){
	int i;
	for(i=0;i<t->length;i++){
		mmDFS(trie, *(t->child+i), freefunc);
	}
	if(t != trie->first && t->value !=NULL && freefunc !=NULL){
		(*freefunc)(t->value);
	}
	free(t->child);free(t->fullkey);
	free(t);
}

unsigned int trie_size (const trie_t trie){
	if(trie->first !=NULL){
		int k=0;trie_pos_t t=trie->first;
		mDFS(trie, t, &k);
		return k;
	}
	else{return 0;}
}

// mDGS is similar to DGS, but is used to walk through trie and count its size
static void mDFS(trie_t trie, trie_pos_t t, int *k){
	int i;
	for(i=0;i<t->length;i++){
		mDFS(trie, *(t->child+i), k);
	}
	if(t->count !=0){*k=*k+1;}
}

//since trie is connected, a simplified version of DFS can be used
bool trie_walk (trie_t trie, trie_walk_t walkfunc, void * priv){
	if(trie->first->length ==0){return true;}
	else if(trie->first != NULL && walkfunc != NULL){
		int k=0;trie_pos_t t=trie->first;
		DFS(trie, walkfunc, priv, t, &k);
		if(k<0){return false;}
		else{return true;}
	}
	else{return false;}
}

// use DFS to walk through trie
static void DFS(trie_t trie, trie_walk_t walkfunc, void * priv, trie_pos_t t, int *k){
	int i;
	if(*k==-1){return;}//to exit the recurrence
	if(t->count ==0 || (*walkfunc)(trie,t,t->fullkey,priv)==true){
		for(i=0;i<t->length;i++){
			DFS(trie, walkfunc, priv, *(t->child+i), k);
		}		
	}
	else{*k=-1;return;}
	if(t->count !=0){k=k+1;}
}


bool trie_insert (trie_t trie, const char * str, void * newval,
		trie_pos_t * newpos){
	trie_pos_t t, result; int test=0;
    if(trie->first ==NULL || str==NULL || *str=='\0')
       return false;
    else{
	    t=trie->first;//pointer to the root node
	    char *p=(char *)str;
	    while(*p!='\0'){
	    	result=searchnode(t,*p);
		    if(result==NULL){
		         trie_pos_t tmp=NewTrieNode();
		         t->length++;//add a child
		         if(t->child != NULL){ 
					t->child=(trie_pos_t*)realloc(t->child,(t->length)*sizeof(trie_pos_t));
			     }
			     else{
			     	t->child=(trie_pos_t*)malloc(sizeof(trie_pos_t));
			     }
		         *(t->child+t->length-1)=tmp;
		         tmp->key=*p;tmp->parent=t;
		         t=tmp;
		         test++;
		    }
		    else{t=result;}
		    p++;
	    }
	    if(test){
	    	t->value=newval;
			t->count++;
			t->fullkey=(char*)malloc((strlen(str)+1)*sizeof(char));
			strcpy(t->fullkey,str);
			if(newpos){
				// trie_pos_t pt=(trie_pos_t)malloc(sizeof(trie_node_t));
				// pt=t;
				*newpos=t;
			}
		    return true;
		}
	    else{return false;}
    }
}

void * trie_get_value (const trie_t trie, trie_pos_t pos){
	if(pos->count !=0 && pos !=TRIE_INVALID_POS && trie->first != NULL){
		return pos->value;
	}
	else{return NULL;}
}

void trie_set_value (trie_t trie, trie_pos_t pos, void * nvalue){
	if(pos->count !=0 && pos !=TRIE_INVALID_POS && trie->first != NULL){
		pos->value=nvalue;
	}
}

//check if node has a child with (part of) key == pkey
static trie_pos_t searchnode(trie_pos_t node, const char pkey){
	if(node==TRIE_INVALID_POS){return NULL;}
	else{
		for(int i=0;i<node->length;i++){
			if(((*(node->child+i))->key)==pkey){
				return *(node->child+i);
			}
		}
		return NULL;
	}
}


trie_pos_t trie_find (const trie_t trie, const char * key){
	trie_pos_t t, result;
	if(trie==NULL || *key=='\0'){
	    return TRIE_INVALID_POS;
    }
    else{
    	t=trie->first;//pointer to the root node
	    char *p=(char *)key;
	    while(*p!='\0'){
	    	result=searchnode(t,*p);
		    if(result!=NULL){
		         t=result;
		         p++;   
		    }
		    else{break;}
	    }
	    if(*p=='\0'){
			if(t->count==0){return TRIE_INVALID_POS;}
			else{return t;}
	    }
	    else{return TRIE_INVALID_POS;}
    }
}

bool trie_remove (trie_t trie, const char * key, void ** data){
	trie_pos_t t=trie_find(trie,key);
	if(t !=TRIE_INVALID_POS){
		if(data){
			*data=t->value;
		}
		t->count=0;
		//*t=nodenull();
		//free(t);//t是pointer
		return true;
	}
	else{return false;}
}





