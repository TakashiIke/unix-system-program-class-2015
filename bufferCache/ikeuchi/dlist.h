#ifndef DLIST_H_
#define DLIST_H_

#include "buf.h"
#define HASHHEAD 0
#define HASHTAIL 1
#define FREEHEAD 2
#define FREETAIL 3

//void insert_hash(buf *h, buf *p, int where);
//void insert_free(buf *h, buf *p, int where);
//void remove_hash(buf *p);
//void insert_list(buf *h, buf *p, int where);

void insert_hash_head(buf *h, buf *p);
void insert_free_head(buf *h, buf *p);
void insert_hash_tail(buf *h, buf *p);
void insert_free_tail(buf *h, buf *p);
buf *remove_free_head();
buf *remove_hash_head();
void insert_list(buf *h, buf *p, int where);
buf *ref_free_head();
void remove_hash(buf *p);
void remove_free(buf *p);
int IsEmpty(int nnhashnum);
int IsEmptyFree();
#endif
