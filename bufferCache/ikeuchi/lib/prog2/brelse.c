#include "dlist.h"
#include "buf.h"

void brelse(buf *buffer){
  //wakeup();
  printf("Wakeup processes wating for any buffer\n");

  //wakeup();
  printf("Wakeup processes waiting for buffer of blkno %d\n", buffer -> blkno);
  //raise_cpu_level();
  if(CheckStatus(buffer, STAT_VALID) & !CheckStatus(buffer, STAT_OLD)){
    AddToFreeList(buffer);
  }
  //lower_cpu_level();
}
