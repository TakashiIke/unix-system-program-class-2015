#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "buf.h"
#include "dlist.h"
#include "state.h"
#include "getblk.h"
#include <assert.h>

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
int ParseStatus(char *argv);

//helper function

buf *GetBuf(int index);
void PrintState(buf *p);
void PrintBufferOne(int index);
void PrintBufferAll();
void PrintHashLine(int hkey);
void PrintHashAll();
void PrintFree();
int SearchNum(int blkno);
buf *init();
void SetStatus(buf *h, int stat);
void ResetStatus(buf *h, int stat);
buf *Clone(int blkno);
struct command_table{
  char *cmd;
  void (*func)(int, char *[]);
};

struct command_table cmd_tbl[] = 
{
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

int setbit = 1;
//--------------------------------


//----------------------------------
int parseline(char *cmdline, char **av){
  char array[100];
  char *buf = array;
  //char *buf = malloc(strlen(cmdline) + 1);
  //int add = buf;
  char *delim;
  int argc = 0;
  strcpy(buf, cmdline);
  buf[strlen(buf) - 1] = ' '; // replace null terminator to space
  while(*buf && (*buf == ' ')){ //ignore leading space
    buf++;
  }
  argc = 0;
 
  delim = strchr(buf, ' ');

  while(delim){
    *delim = '\0';
    av[argc++] = buf;
    //*delim = '\0'; // I don't know why we need this
    buf = delim + 1;
    while(*buf && (*buf == ' ')){
      buf++;
    }
    delim = strchr(buf, ' ');
  }
  av[argc] = NULL;
  if(argc == 0){
    printf("No argument is obtained\n");
    return 0;
  }
  else{
    for(int i = 0; i < argc; i++){
      puts(av[i]);
    }
  }
  //*buf = 1;
  return argc;  
}

//61101233
//Takashi Ikeuchi
//I am Takashi Ikeuchi, bitch!
int main(int argc, char *argv[]){
  char cmdline[100];
  while(setbit){
    printf("$ ");
    if(fgets(cmdline, 100, stdin) == NULL){
      printf("Oops something is wrong in reading char from commad\n");
      exit(1);
    }
    struct command_table *p;
    int ac = 0;
    char *av[16];
    ac = parseline(cmdline, av);
    if(!ac){
      continue;
    }
    for(p = cmd_tbl; p -> cmd; p++){
      if(strcmp(av[0], p -> cmd) == 0){
	(*p -> func)(ac, av);
	break;
      }
    }
    if(p -> cmd == NULL){
      fprintf(stderr, "unknown cmmand: %s\n", av[0]);
    }
  }
}

void help_proc(int num, char *name[]){
  //init
  printf("init  \n");
  printf("\tinitialize hash list and free list and make the \n");
  printf("\tinitial state shown in the figure 2.15. \n\n");
  //buf
  printf("buf[n ...] \n");
  printf("\tIf there is no input value, ");
  printf("display all the status of the buffers.\n");
  printf("\tIf there is an input block number, ");
  printf("display the status of the buffer at the block number.\n\n");
  //hash
  printf("hash[n ...] \n");
  printf("\tIf there is no input value, ");
  printf("display all the hash list.\n");
  printf("\tIf there is an input hash key, ");
  printf("display the hash list at the hash key given from user.\n\n");
  //free
  printf("free\n");
  printf("\tDisplay free list\n\n");
  //getblk 
  printf("getblk n\n");
  printf("\ttake the blkno from the user, execute getblk(n)\n\n");
  //brelse  
  printf("brelse n\n");
  printf("\ttake the blkno from the user, execute brelse(bp), \n");
  printf("\twhere bp is the pointer to buffer header with blkno = n");
  printf("\n\n");
  //set
  printf("set n stat [stat]\n");
  printf("\tset the status of the buffer of blkno n to stat\n\n");
  //reset
  printf("reset n stat [stat]\n");
  printf("\treset the status of the buffer of blkno n to stat\n\n");
  //quit
  printf("exit the software\n\n");
}

void init_proc(int argc, char *argv[]){
  //init process
  for(int i = 0; i < NHASH; i++){
    h_head[i].hash_fp = &h_head[i];
    h_head[i].hash_bp = &h_head[i];
    h_head[i].stat = 0;
    h_head[i].free_fp = &f_head;
    h_head[i].free_bp = &f_head;
    h_head[i].cache_data = NULL;
    buf *trace = &h_head[i];
    buf *p = malloc(sizeof(buf));
    buf *q = malloc(sizeof(buf));
    buf *r = malloc(sizeof(buf));
      if(p == NULL | q == NULL | r == NULL){
	printf("unable to assign memory\n");
	abort();
      }
      assert(p != NULL && q != NULL && r != NULL);
    
    insert_list(&h_head[i], p, HASHHEAD);
    insert_list(h_head[i].hash_fp, q, HASHHEAD);
    insert_list(((h_head[i].hash_fp) -> hash_fp), r, HASHHEAD);
  }
  buf *freebuffer = malloc(sizeof(buf));
  freebuffer -> free_fp = freebuffer;
  freebuffer -> free_bp = freebuffer;
  freebuffer -> hash_fp = NULL;
  freebuffer -> hash_bp = NULL;
  freebuffer -> stat = 0;
  freebuffer -> cache_data = NULL;
  f_head = *freebuffer;
  //adding buffers
  //mod = 0
  buf *trac = h_head[0].hash_fp;
  trac -> blkno = 28;
  SetStatus(trac, STAT_VALID);
  trac = trac -> hash_fp;
  trac -> blkno = 4;
  SetStatus(trac, STAT_VALID);
  trac = trac -> hash_fp;
  trac -> blkno = 64;
  SetStatus(trac, STAT_VALID | STAT_LOCKED);
  //mod = 1
  trac = h_head[1].hash_fp;
  trac -> blkno = 17;
  SetStatus(trac, STAT_VALID | STAT_LOCKED);
  trac = trac -> hash_fp;
  trac -> blkno = 5;
  SetStatus(trac, STAT_VALID);
  trac = trac -> hash_fp;
  trac -> blkno = 97;
  SetStatus(trac, STAT_VALID);
  //mod = 2
  trac = h_head[2].hash_fp;
  trac -> blkno = 98;
  SetStatus(trac, STAT_VALID | STAT_LOCKED);
  trac = trac -> hash_fp;
  trac -> blkno = 50;
  SetStatus(trac, STAT_VALID | STAT_LOCKED);
  trac = trac -> hash_fp;
  trac -> blkno = 10;
  SetStatus(trac, STAT_VALID);
  //mod = 3
  trac = h_head[3].hash_fp;
  trac -> blkno = 3;
  SetStatus(trac, STAT_VALID);
  trac = trac -> hash_fp;
  trac -> blkno = 35;
  SetStatus(trac, STAT_VALID | STAT_LOCKED);
  trac = trac -> hash_fp;
  trac -> blkno = 99;
  SetStatus(trac, STAT_VALID | STAT_LOCKED);

  //adding freelist
  trac = &f_head;
  //trac -> free_fp = NULL;
  insert_list(trac, h_head[3].hash_fp, FREEHEAD);
  trac = trac -> free_fp;
  insert_list(trac, (h_head[1].hash_fp) -> hash_fp, FREEHEAD);
  trac = trac -> free_fp;
  insert_list(trac, (h_head[0].hash_fp) -> hash_fp, FREEHEAD);
  trac = trac -> free_fp;
  insert_list(trac, h_head[0].hash_fp, FREEHEAD);
  trac = trac -> free_fp;
  insert_list(trac, (h_head[1].hash_fp) -> hash_fp -> hash_fp, FREEHEAD);
  trac = trac -> free_fp;
  insert_list(trac, (h_head[2].hash_fp) -> hash_fp -> hash_fp, FREEHEAD);

}
void buf_proc(int argc, char *argv[]){
  if(argc <= 1){
    PrintBufferAll();
  }
  else{
    for(int i = 1; i < argc; i++){
      if(!isalpha((argv[i][0]))){
	if(0 <= atoi(argv[i]) && atoi(argv[i]) <= 11){
	  PrintBufferOne(atoi(argv[i]));
	  printf("\n");
	}
	else{
	  printf("buffer number must be within 0 ~ 11\n");
	}
      }
      else{
	printf("INPUT VALUE MUST BE NUMBER, ASS WHOLE\n");
	printf("NOTHING PRINTED FOR THIS SHITTY REQUEST\n");
      }
    }
  }
}
void hash_proc(int argc, char *argv[]){
  if(argc <= 1){
    PrintHashAll();
  }
  else{
    for(int i = 1; i < argc; i++){
      if(!isalpha((argv[i][0]))){
	if(0 <= atoi(argv[i]) && atoi(argv[i]) <= 3){
	  PrintHashLine(atoi(argv[i-1]));
	}
	else{
	  printf("INPUT VALUE MUST BE WITHIN 0 ~ 3, DUSHBAG\n");
	  printf("NOTHING PRINTED FOR THIS SHITTY REQUEST\n");
	}
      }
      else{
	printf("YOU SHIT NUMBER PLEASE\n");
      }
    }
  }
}
void free_proc(int argc, char *argv[]){
  PrintFree();
}
void getblk_proc(int argc, char *argv[]){
  if(argc <= 1){
    printf("COMMON MAN, WHICH BLOCK ARE YOU TALKING ABOUT\n");
    printf("You Should Specify the block number BY YOURSELF SITTY ASS\n");
  }
  else{
    if(!isalpha((argv[1][0]))){
      int t = atoi(argv[1]);
      struct buf_header *blockedbuf = getblk(t);
    }
    else{
      printf("INPUT VALUE MUST BE NUMBER, ASS WHOLE\n");
    }
  }
}
void brelse_proc(int argc, char *argv[]){
  if(argc <= 2){
    printf("COMMON MAN, WHICH BLOCK ARE YOU TALKING ABOUT\n");
    printf("You Should Specify the block number BY YOURSELF SITTY ASS\n");
  }
  else{
    if(!isalpha(atoi(argv[2]))){
      if(1 <= atoi(argv[2]) && atoi(argv[2]) <= 12){
	Clone(atoi(argv[2]));
      }
      else{
	printf("INPUT VALUE MUST BE WITHING 1~12\n");
      }
    }
    else{
      printf("INPUT VALUE MUST BE NUMBER, ASS WHOLE\n");
    }
  }
}
void set_proc(int argc, char *argv[]){
  if(argc <= 3){
    printf("JUST DON'T DO THE SHITS\n");
  }
  else{
    int state = 0;
    for(int i = 3; i < argc; i++){
      int num = ParseStatus(argv[i]);
      state += num;
    }
    buf *buffer = Clone(atoi((argv[3])));
    int blkno = atoi((argv[3]));
    if(!isalpha(blkno)){
      if(1 <= blkno && blkno <=12){
	if(buffer == NULL){
	  printf("invaid cannot get buffer\n");
	}
	buffer -> stat | state;
      }
      else{
	printf("Errror input value for block number should be within 1~12\n");
      }
    }
    else{
      printf("This is not number but alphabet\n");
    }
  }
}

void reset_proc(int argc, char *argv[]){
  if(argc <= 3){
    printf("JUST DON'T DO THE SHITS\n");
  }
  else{
    int state = 0;
    for(int i = 3; i < argc; i++){
      int num = ParseStatus(argv[i]);
      state += num;
    }
    buf *buffer = Clone(atoi((argv[3])));
    int blkno = atoi((argv[3]));
    if(!isalpha(blkno)){
      if(1 <= blkno && blkno <=12){
	if(buffer == NULL){
	  printf("invaid cannot get buffer\n");
	}
	int midstate = (buffer -> stat) & state;
	buffer -> stat & midstate;
      }
      else{
	printf("Errror input value for block number should be within 1~12\n");
      }
    }
    else{
      printf("This is not number but alphabet\n");
    }
  }
}
void quit_proc(int argc, char *argv[]){
  setbit = 0;
  return;  
}

void PrintState(buf *p){
  int state = p -> stat;
  if(state & 0x20)  printf("O");
  else  printf("-");
  state = state << 1;

  if(state & 0x20)  printf("W");
  else  printf("-");
  state = state << 1;

  if(state & 0x20)  printf("K");
  else  printf("-");
  state = state << 1;

  if(state & 0x20)  printf("D");
  else  printf("-");
  state = state << 1;

  if(state & 0x20)  printf("V");
  else  printf("-");
  state = state << 1;

  if(state & 0x20)  printf("L");
  else  printf("-");

}
void PrintBufferOne(int index){
  int hkey = index / 3;
  buf *p = &h_head[hkey];
  for(int i = index % 3; i >= 0; i--){
    p = p -> hash_fp;
  }
  printf("[ %d : %d ", index, p -> blkno);
  PrintState(p);
  printf("]");
}

void PrintBufferAll(){
  //printf("buf[n ...]\n");
  int index = 0;
  for(int i = 0; i < NHASH; i++){
    for(buf *p = h_head[i].hash_fp; p != &h_head[i]; p = p -> hash_fp){
      if(index >= 10){
	if(p -> blkno >= 10){
	  printf("[%d : %d ", index, p -> blkno);
	}
	else{
	  printf("[%d :  %d ", index, p -> blkno);
	}
      }
      else{
	if(p -> blkno >= 10){
	  printf("[ %d : %d ", index, p -> blkno);
	}
	else{
	  printf("[ %d :  %d ", index, p -> blkno);
	}
      }
      PrintState(p);
      printf("]\n");
      index++; 
    }
  }
}

void PrintHashLine(int hkey){
  int index = hkey * 4;
  for(buf *p = h_head[hkey].hash_fp; p != &h_head[hkey]; p = p -> hash_fp){
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
    for(buf *p = h_head[i].hash_fp; p != &h_head[i]; p = p -> hash_fp){
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
    for(buf *p = h_head[i].hash_fp; p != &h_head[i]; p = p -> hash_fp){
      if(p == buffer) return index;
      index++; 
    }    
  }
}

buf *init(){
  buf *p = malloc(sizeof(buf));
  p -> hash_fp = NULL;
  p -> hash_bp = NULL;
  p -> stat = 0;
  p -> free_fp = NULL;
  p -> free_bp = NULL;
  p -> cache_data = NULL;

  return p;
}

void SetStatus(buf *h, int state){
  h -> stat = state;
}

void ResetStatus(buf *h, int stat){
  h -> stat ^ stat;
}

buf *GetBuf(int index){
  int quotient = index / 4;
  int remainder = index % 4;
  buf *p = h_head[quotient].hash_fp;
  for(int i = 0; i < remainder; i++){
    p -> hash_fp;
  }
  return p;
}

int ParseStatus(char *argv){
  char val = argv[0];
  switch(val){
  case('L'):
    return 1;
  case('V'):
    return 2;
  case('D'):
    return 4;
  case('K'):
    return 8;
  case('W'):
    return 16;
  case('O'):
    return 32;
  default:
    return 0;
  }
}
 
buf *Clone(int blkno){
  int hkey = blkno % 4;
  buf *p;
  for(p = h_head[hkey].hash_fp; p != &h_head[hkey];p = p -> hash_fp){
    if(p -> blkno == blkno){
      return p;
    }
    return NULL;
  }
}
  
