#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  char line[100];
/*
  for(int i = 0; i < argc; i++){
    
  }
*/
  fgets(line, 100, stdin);
  puts(line);

}
