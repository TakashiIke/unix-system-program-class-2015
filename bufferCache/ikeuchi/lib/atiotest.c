#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[]){
  int test = atoi(argv[1]);
  printf("test = %d\n", test);
  printf("argc = %d\n", argc);
  printf("isalpha = %d\n", isalpha(test));
  return 0;
}
