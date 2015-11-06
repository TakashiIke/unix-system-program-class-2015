#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"

int parseline(char *cmdline, char **av){
  //char array[100];
  char *buffer = calloc(100, sizeof(char));
  char *delim;
  int argc = 0;
  strcpy(buffer, cmdline);
  buffer[strlen(buffer) - 1] = ' ';
  while(*buffer && (*buffer == ' ')){ 
    buffer++;
  }
  argc = 0;
 
  delim = strchr(buffer, ' ');

  while(delim){
    *delim = '\0';
    av[argc++] = buffer;
    buffer = delim + 1;
    while(*buffer && (*buffer == ' ')){
      buffer++;
    }
    delim = strchr(buffer, ' ');
  }
  av[argc] = NULL;
  if(argc == 0){
    printf("No argument is obtained\n");
    return 0;
  }
  return argc;  
}
