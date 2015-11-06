#include <stdio.h>
#include <stdib.h>
#include "buf.h"
#include "dlist.h"
#include "state.h"

struct buf_header *getblk(int blkno){
  while(h_head[blkno % 4]){
    if(buf *buffer = Search(blkno)){
      ASSERT(buffer != NULL);
      if(!IsStatus(buffer, STAT_LOCKED)){
	//scenario 5
	//sleep();
	printf("Process goes to sleep\n");
	return NULL;
	continue;
      }
      //scenario 1
      MakeStatus(buffer, (STAT_LOCKED | STAT_VALID));
      RemFromFreeList(buffer);
      return buffer;	
    }
    else{
      if(IsInFreeList(buffer)){
	//scenario 4
	//sleep();
	printf("Process goes to sleep\n");
	continue;
      }
      //scenario 3
      RemFromFreeList(buffer);
      if(CheckStatus(buffer, STAT_DWR)){
	//asynchronous write buffer to disk;
	continue;
      }
      //scenario 2
      buf *elem = GetBufFromFreeList(f_list);
      RemStatus(buffer, STAT_VALID);
      elem -> blkno = blkno;
      AddToHash(elem);
      /*
      printf("Kernel HDD access occuring\n");
      AddStatus(buffer, STAT_KRDWR);
      printf("Kernel HDD access finihe\n");
      RemStatus(buffer, STAT_KRDWR);
      AddStatus(buffer, STAT_VALID);
      */
      return elem;
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
    insert_list(f_list, buffer, FREETAIL);
  }
  else{
    insert_list(f_list, buffer, FREEHEAD);
  }
  //lower_cpu_level();
}


// based on the blkno, search the hash key, and 
// if there exist the value in the hash list, return 
// the buffer that contains blkno, return NULL otherwise
buf *Search(int blkno){
  int hkey = blkno % 4;
  buf *p;
  for(p = h_head[hkey].hash_fp; p != &hash_head[hkey]; p = p -> hasp_fp){
    if(p -> blkno == blkno){
      return p;
    }
    return NULL;
  }
}

//
int IsInHash(int blkno){
  
}

// check wheather buffer is locked or not
// take & with STAT_LOCKED and see the bit level operation
int IsStatus(buf *buffer, int state){
  return (buffer -> stat & state);
}
void AddStatus(buf *buffer, int state){
  (buffer -> stat) | state;
}
//// add STAT_LOCKED to the status of buffer  
//void Lock(buf *buffer){
//  buffer -> stat | STAT_LOCKED;
//}

void RemFromFreeList(buf *buf){
  buf *prev = buf -> free_bp;
  buf *next = buf -> free_fp;
  prev -> free_fp = next;
  next -> free_bp = prev;
  buf -> free_fp = NULL;
  buf -> free_bp = NULL;
}

buf *GetBufFromFreeList(buf *F_LIST){
  buf *buffer = F_LIST -> free_fp;
  if(!CheckStatus(buffer, STAT_DWR)){
    RemFromFreeList(buffer);
    reutrn buffer;
  }
  else{
    GetBufFromFreeList(buffer);
  }
}

void AddToHash(elem){
  int key = elem -> stat;
  int hkey = key % 4;
  AddStatus(elem, STAT_LOCKED);
  insert_tail(h_head[hkey], elem);
}
