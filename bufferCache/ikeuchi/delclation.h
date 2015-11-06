#ifndef DECLARATION_H_
#define DECLARATION_H_

struct command_table{
  char *cmd;
  void (*func)(int, char *[]);
};


typedef int checker_t;

checker_t setbit = 1;
checker_t malloced = 0;
checker_t initnum = 0;

#endif
