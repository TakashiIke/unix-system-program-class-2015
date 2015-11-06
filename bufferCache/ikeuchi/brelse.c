#include <stdio.h>
#include "state.h"
#include "dlist.h"
#include "getblk.h"
#include "buf.h"

//int CheckStatus(buf *buffer, int state);

void brelse(buf *buffer){
  //wakeup();
  printf("Wakeup processes wating for any buffer\n");

  //wakeup();
  printf("Wakeup processes waiting for buffer of blkno %d\n", buffer -> blkno);
  //raise_cpu_level();
  if(CheckStatus(buffer, STAT_VALID) & !CheckStatus(buffer, STAT_OLD)){
    insert_list(&f_head, buffer, FREETAIL);
  }
  else{
    insert_list(&f_head, buffer, FREEHEAD);
  }
  //lower_cpu_level();
  
}

/*
int CheckStatus(buf *buffer, int state){
  return (buffer -> stat & state);
}
*/
