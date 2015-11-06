#include <stdio.h>
#include <stdib.h>
#include "buf.h"
#include "dlist.h"
#include "state.h"
#include "getblk.h"

void help_proc(int, char *[]);
void init_proc(int, char *[]);
void buf_proc(int, char *[]);
void hash_proc(int, char *[]);
void free_proc(int, char *[]);
void getblk_proc(int, char *[]);
void brelse_proc(int, char *[]);
void set_proc(int, char *[]);
void reset_proc(int, char *[]);
void quit_proc(int, char *[]);

struct command_table{
  char *cmd;
  void (*func)(int, char *[]);
};

struct command_table cmd_tbl[]{
  {"help", help_proc},
  {"init", init_proc},
  {"buf", buf_proc},
  {"hash", hash_proc},
  {"free", free_proc},
  {"getblk", getblk_proc},
  {"brelse", brelse_proc},
  {"set", set_proc},
  {"reset", reset_proc},
  {"quit", quit_proc},
  {NULL, NULL}
};

int main(){


}


void help_proc(int num, char *name[]){
  //init
  printf("init : \n");
  printf("\tinitialize hash list and free list and make the initial ");
  printf("state shown in the figure 2.15. \n ");
  //buf
  printf("init : \n");
  printf("\tinitialize hash list and free list and make the initial ");
  printf("state shown in the figure 2.15. \n ");
}
void init_proc(int, char *[]);
void buf_proc(int, char *[]);
void hash_proc(int, char *[]);
void free_proc(int, char *[]);
void getblk_proc(int, char *[]);
void brelse_proc(int, char *[]);
void set_proc(int, char *[]);
void reset_proc(int, char *[]);
void quit_proc(int, char *[]);

void PrintInit(){
  printf("init : \n");
  printf("\tinitialize hash list and free list and make the initial ");
  printf("state shown in the figure 2.15. \n ");
}

void PrintState(buf *p){
  int state = p -> stat;
  if(state & 0x1)  printf("L");
  else printf("-");
  state >> 1;

  if(state & 0x1)  printf("V");
  else printf("-");
  state >> 1;

  if(state & 0x1)  printf("D");
  else printf("-");
  state >> 1;

  if(state & 0x1)  printf("K");
  else printf("-");
  state >> 1;

  if(state & 0x1)  printf("W");
  else printf("-");
  state >> 1;

  if(state & 0x1)  printf("O");
  else printf("-");

}
void PrintBufferOne(int index){
  int hkey = index % 4;
  buf *p = hash_head[hkey].hash_fp;
  for(int i = index / 4; i > 0; i++){
    p = p -> hash_fp;
  }
  printf("\t[ %d : %d ", index, p -> blkno);
  PrintState(p);
  printf("]");
}

void PrintBufferAll(){
  //printf("buf[n ...]\n");
  int index = 0;
  for(int i = 0; i < NHASH; i++){
    for(buf *p = hash_head[i].hash_fp; p != &hash_head[i]; p = p -> hash_fp){
      printf("\t[ %d : %d ", index, p -> blkno);
      PrintState(p);
      printf("]");
      index++; 
    }
  }
}

void PrintHashLine(int hkey){
  int index = hkey * 4;
  for(buf *p = hash_head[hkey].hash_fp; p != &hash_head[hkey]; p = p -> hash_fp){
    printf("\t[ %d : %d ", index, p -> blkno);
    PrintState(p);
    printf("]");
    index++;
  }
}

void PrintHashAll(){
  printf("Hash[n ...]\n");
  int index = 0;
  for(int i = 0; i < NHASH; i++){
    printf("%d :", i);
    for(buf *p = hash_head[i].hash_fp; p != &hash_head[i]; p = p -> hash_fp){
      printf("\t[ %d : %d ", index, p -> blkno);
      PrintState(p);
      printf("]");
      index++; 
    }
    printf("\n");
  }
}

void PrintFree(){
  int index = 0; 
  for(buf *p = f_head.free_fp; p != &f_head; p = p -> free_fp){
    index = SearchNum(p -> blkno);
    printf("\t[ %d : %d ", index, p -> blkno);
    PrintState(p);
    printf("]");
  }
}

int SearchNum(int blkno){
  buf *buffer = Search(blkno);
  int index = 0; 
  for(int i = 0; i < NHASH; i++){
    for(buf *p = hash_head[i].hash_fp; p != &hash_head[i]; p = p -> hash_fp){
      if(p == buffer) return index;
      index++; 
    }    
  }
}
