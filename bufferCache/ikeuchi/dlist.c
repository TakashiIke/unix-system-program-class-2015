#include "buf.h"
#include "state.h"
#include "dlist.h"
#include "getblk.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

//void insert_hash_head(buf *h, buf *p);
//void insert_free_head(buf *h, buf *p);
///void insert_hash_tail(buf *h, buf *p);
//void insert_free_tail(buf *h, buf *p);
//void insert_list(buf *h, buf *p, int where);
//void remove_hash(buf *p);

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
  p -> free_bp = h -> free_bp;
  h -> free_bp -> free_fp = p;
  h -> free_bp = p;
}

void remove_hash(buf *p){
  buf *prev = p -> hash_bp;
  buf *next = p -> hash_fp;
  prev -> hash_fp = p -> hash_fp;
  next -> hash_bp = prev;
  //h_head -> hash_fp = p -> hash_fp;
  //h_head -> hash_fp -> hash_bp = p -> hash_bp;
  //p -> hash_fp = NULL;
  //p -> hash_bp = NULL;
}

void remove_free(buf *p){
  f_head.free_fp = f_head.free_fp -> free_fp;
  f_head.free_fp -> free_bp = &f_head;
  p -> free_fp = NULL;
  p -> free_bp = NULL;
  //free(p);
}

buf *ref_free_head(){
  buf *ret = f_head.free_fp;
  return ret;
}

buf *remove_free_head(){
  if(IsEmptyFree()){
    return NULL;
  }
  buf *ret = f_head.free_fp;
  while(CheckStatus(ret, STAT_DWR)){
    if(ret == &f_head){
      printf("Oops somethig wrong happening heer\n");
      return NULL;
    }
    ret = ret -> free_fp;
  }
 
  remove_hash(ret);
  f_head.free_fp = f_head.free_fp -> free_fp;
  f_head.free_fp -> free_bp = &f_head;
  return ret;
}

buf *remove_hash_head(int i){
  buf *ret = h_head[i].hash_fp;
  remove_hash(ret);
  //h_head[i].hash_fp = h_head[i].hash_fp -> hash_fp;
  //h_head[i].hash_fp -> hash_bp = &h_head[i];
  return ret;
}

int IsEmpty(int nnhashnum){
  nnhashnum %= 4;
  if( h_head[nnhashnum].hash_fp == &h_head[nnhashnum] &&
      h_head[nnhashnum].hash_bp == &h_head[nnhashnum] ){
    return 1;
  }
  return 0;
}

int IsEmptyFree(){
  if( f_head.free_fp == &f_head &&
      f_head.free_bp == &f_head ){
    return 1;
  }
  return 0;
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
