#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parseline(char *cmdline, char **av){
  char *buf = calloc(100, sizeof(char));
  //char *buf = malloc(strlen(cmdline) + 1);
  //int add = buf;
  char *delim;
  int argc = 0;
  strcpy(buf, cmdline);
  buf[strlen(buf) - 1] = ' '; // replace null terminator to space
  while(*buf && (*buf == ' ')){ //ignore leading space
    buf++;
  }
  argc = 0;
 
  delim = strchr(buf, ' ');

  while(delim){
    *delim = '\0';
    //av[argc] = calloc(strlen(buf) + 1, sizeof(char));
    //argc++;
    //strcpy(av[argc], buf);
    av[argc++] = buf;
    //*dlime = '\0';
    buf = delim + 1;
    while(*buf && (*buf == ' ')){
      buf++;
    }
    delim = strchr(buf, ' ');
  }
  av[argc] = NULL;
  if(argc == 0){
    printf("No argument is obtained\n");
  }
  //*buf = 1;
  
}

int main(int argc, char *argv[]){
  char cmdline[100];
  while(1){
    printf("$ ");
    if(fgets(cmdline, 100, stdin) == NULL){
      printf("Cannot read formally\n");
      return 1;
    }
    /*
      char *s = malloc(strlen(cmdline) + 1);
      strcpy(s, cmdline);
      printf("parsing the following stream\n");
      puts(s);
    */
    //puts(cmdline);
    char *av[100];
    parseline(cmdline, av);
  }
}
