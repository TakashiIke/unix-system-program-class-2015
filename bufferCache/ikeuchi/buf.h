#ifndef BUF_HEADER_
#define BUF_HEADER_
struct buf_header{
  int blkno;
  struct buf_header *hash_fp;
  struct buf_header *hash_bp;
  unsigned int stat;
  struct buf_header *free_fp;
  struct buf_header *free_bp;
  char *cache_data;
};

typedef struct buf_header buf;
#define NHASH 4
buf h_head[NHASH];
buf f_head;
#endif
