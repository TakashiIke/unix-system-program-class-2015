#include "buf.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define HASHHEAD 0
#define HASHTAIL 1
#define FREEHEAD 2
#define FREETAIL 3

void insert_hash_head(buf *h, buf *p);
void insert_free_head(buf *h, buf *p);
void insert_hash_tail(buf *h, buf *p);
void insert_free_tail(buf *h, buf *p);
void insert_list(buf *h, buf *p, int where);
void remove_hash(buf *p);

void insert_hash_head(buf *h, buf *p){
  p -> hash_bp = h;
  p -> hash_fp = h -> hash_fp;
  h -> hash_fp -> hash_bp = p;
  h -> hash_fp = p;
}

void insert_free_head(buf *h, buf *p){
  p -> free_bp = h;
  p -> free_fp = h -> free_fp;
  h -> free_fp -> free_bp = p;
  h -> free_fp = p;
}


void insert_hash_tail(buf *h, buf *p){
  p -> hash_fp = h;
  p -> hash_bp = h -> hash_bp;
  h -> hash_bp -> hash_fp = p;
  h -> hash_bp = p;
}

void insert_free_tail(buf *h, buf *p){
  p -> free_fp = h;
  p -> free_bp = h -> hash_bp;
  h -> free_bp -> free_fp = p;
  h -> free_bp = p;
}

void remove_hash(buf *p){
  h -> hash_fp = p -> hash_fp;
  h -> hash_fp -> hash_bp = p -> hash_bp;
  p -> hash_fp = NULL;
  p -> hash_bp = NULL;
}

// if where = 0, call insert_head call insert_tail otherwise
void insert_list(buf *h, buf *p, int where){
  if(where == HASHHEAD){
    insert_hash_head(h, p);
  }
  else if(where == HASHTAIL){
    insert_hash_tail(h, p);
  }
  else if(where == FREEHEAD){
    insert_free_head(h, p); 
  }
  else if(where == FREETAIL){
    insert_free_tail(h, p);
  }
  else{
    printf("wrong identification\n");
    return;
  }
  return;
}
