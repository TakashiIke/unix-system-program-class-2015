#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "getblk.h"
#include "buf.h"
#include "dlist.h"
#include "state.h"

buf *getblk(int blknum){
  while(&h_head[blknum % 4] != NULL){
    buf *buffer = Search(blknum);
    if(buffer != NULL){
      assert(buffer != NULL);
      if(IsStatus(buffer, STAT_LOCKED)){
	//sleep();
	printf("SCENARIO 5\n");
	printf("Process goes to sleep\n");
	AddStatus(buffer, STAT_WAITED);
	return NULL;
	continue;
      }
      //scenario 1
      printf("SCENARIO 1\n");
      MakeStatus(buffer, (STAT_LOCKED | STAT_VALID));
      RemFromFreeList(buffer);
      return buffer;	
    }
    else{
      //if(IsInFreeList(buffer)){
      if(IsEmptyFree()){
	//scenario 4
	//sleep();
	printf("SCENARIO 4\n");
	printf("Process goes to sleep\n");
	brelse(buffer);
	continue;
      }
      //RemFromFreeList(buffer);
      //RemoveFromFree()
      buf *ref = ref_free_head();
      if(CheckStatus(ref, STAT_DWR)){
	//scenario 3
	printf("SCENARIO 3\n");	//asynchronous write buffer to disk;
	buf *prev = ref -> free_bp;
	buf *next = ref -> free_fp;
	prev -> free_fp = next;
	next -> free_bp = prev;
	MakeStatus(ref, STAT_LOCKED | STAT_VALID | STAT_DWR | STAT_OLD);
	continue;
      }
      //scenario 2
      printf("SCENARIO 2\n");
      buf *additionalbuf = remove_free_head();
      RemStatus(additionalbuf, STAT_VALID);
      additionalbuf -> blkno = blknum;
      AddToHash(additionalbuf);
      printf("Kernel HDD access occuring\n");
      AddStatus(additionalbuf, STAT_KRDWR);
      printf("Kernel HDD access finished\n");
      RemStatus(additionalbuf, STAT_KRDWR);
      AddStatus(additionalbuf, STAT_VALID);
      return additionalbuf;
    }
  }
  printf("BUFFER NOT FOUND\n");
  return NULL;
}

void brelse(buf *buffer){
  //wakeup();
  printf("Wakeup processes wating for any buffer\n");
  if(CheckStatus(buffer, STAT_LOCKED | STAT_VALID | STAT_WAITED)){
    //wakeup();
    printf("Wakeup processes waiting for buffer of blkno %d\n", buffer -> blkno);
  }
  //raise_cpu_level();
  if(CheckStatus(buffer, STAT_VALID) & !CheckStatus(buffer, STAT_OLD)){
    insert_list(&f_head, buffer, FREETAIL);
    MakeStatus(buffer, STAT_VALID);
  }
  else{
    insert_list(&f_head, buffer, FREEHEAD);
    MakeStatus(buffer, STAT_VALID);
 }

  //lower_cpu_level();
}


// based on the blkno, search the hash key, and 
// if there exist the value in the hash list, return 
// the buffer that contains blkno, return NULL otherwise
buf *Search(int num){
  int hkey = num % 4;
  buf *p;
  for(p = h_head[hkey].hash_fp; p != &h_head[hkey]; p = p -> hash_fp){
    if(p -> blkno == num){
      return p;
    }
  }
  return NULL;
}

//
//int IsInHash(int blkno){
//  
//}

// check wheather buffer is locked or not
// take & with STAT_LOCKED and see the bit level operation
int IsStatus(buf *buffer, int state){
  return (buffer -> stat & state);
}
void AddStatus(buf *buffer, int state){
  buffer -> stat = (buffer -> stat) | state;
}
//// add STAT_LOCKED to the status of buffer  
//void Lock(buf *buffer){
//  buffer -> stat | STAT_LOCKED;
//}

void RemFromFreeList(buf *buffer){
  buf *prev = buffer -> free_bp;
  buf *next = buffer -> free_fp;
  prev -> free_fp = next;
  next -> free_bp = prev;
  buffer -> free_fp = NULL;
  buffer -> free_bp = NULL;
}

buf *GetBufFromFreeList(buf *F_LIST){
  buf *buffer = F_LIST -> free_fp;
  if(!CheckStatus(buffer, STAT_DWR)){
    RemFromFreeList(buffer);
    return buffer;
  }
  else{
    GetBufFromFreeList(buffer);
  }
}

void AddToHash(buf *elem){
  int key = elem -> blkno;
  int hkey = key % 4;
  AddStatus(elem, STAT_LOCKED);
  //insert_hash_head(h_head[hkey].hash_bp, elem);
  insert_list(&h_head[hkey], elem, HASHTAIL);
}

int IsInFreeList(buf *buffer){
  for(buf *p = &f_head; p != &f_head; p = p -> free_fp){
    if(p == buffer){
      return p -> blkno;
    }
  }
  return 0;
}

void MakeStatus(buf *buffer, int state){
  buffer -> stat = state;
}

int CheckStatus(buf *buffer, int state){
  int mask = buffer -> stat & state;

  return !(mask ^ state);
}

void RemStatus(buf *buffer, int state){
  buffer -> stat = buffer -> stat ^ state;
}
