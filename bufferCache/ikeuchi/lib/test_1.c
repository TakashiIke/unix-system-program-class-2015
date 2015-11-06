#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  printf("argc = %d\n", argc);
  //char *test[16];
  char *test[16];
  for(int i = 0; i < argc; i++){
    test[i] = (char *)calloc(1, sizeof(80));
    //memset(test[i], 0, sizeof(test[i]));
    //strcpy(test[i], argv[i]);
    strncpy(test[i], argv[i], sizeof(test[i]) - 1);
    puts(test[i]);
    printf("%d\n", i);
    free(test[i]);
  }
  printf("FINISHED\n");
  return 0;
}
