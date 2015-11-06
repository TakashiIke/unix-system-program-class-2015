#ifndef GETBLK_H_
#define GETBLK_H_

//#include <stdio.h>
//#include <stdlib.h>
#include "buf.h"
//#include "dlist.h"
//#include "state.h"

buf *getblk(int blknum);
void brelse(buf *buffer);
buf *Search(int blkno);
int IsStatus(buf *buffer, int state);
void AddStatus(buf *buffer, int state);
void RemFromFreeList(buf *buf);
buf *GetBufFromFreeList(buf *F_LIST);
void AddToHash(buf *elem);
int IsInFreeList(buf *buffer);
int CheckStatus(buf *buffer, int state);
void RemStatus(buf *buffer, int state);
void MakeStatus(buf *buffer, int state);

#endif
