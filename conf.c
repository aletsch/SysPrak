//Bibliotheken einbinden
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct Configuration{
  char hostname[64];
  int portnumber;
  char gamekind[64];
};


char* getHostname(char* file){

  FILE* fpointer;
  char *line = (char*) malloc(sizeof(char)*256);
  char *ptr;

  fpointer = fopen(file, "r");
  while(!feof(fpointer)){
    fgets(line, 256, fpointer);
    ptr = strtok(line, " =");
    if(strcmp(ptr, "hostname") == 0){
      ptr = strtok(NULL, " =");
      fclose(fpointer);
      free(line);
      return ptr;
    }
  }
  free(line);
  return "hello";
}


int getPort(char* file){

  FILE* fpointer;
  char *line = (char*) malloc(sizeof(char)*256);
  char *ptr;

  fpointer = fopen(file, "r");
  while(!feof(fpointer)){
    fgets(line, 256, fpointer);
    ptr = strtok(line, " =");
    if(strcmp(ptr, "portnumber") == 0){
      ptr = strtok(NULL, " =");
      fclose(fpointer);
      free(line);
      return atoi(ptr);
    }
  }
  free(line);
  return 0;
}


char* getGamekind(char* file){

  FILE* fpointer;
  char *line = (char*) malloc(sizeof(char)*256);
  char *ptr;

  fpointer = fopen(file, "r");
  while(!feof(fpointer)){
    fgets(line, 256, fpointer);
    ptr = strtok(line, " =");
    if(strcmp(ptr, "gamekind") == 0){
      ptr = strtok(NULL, " =");
      fclose(fpointer);
      free(line);
      return ptr;
    }
  }
  free(line);
  return "hello";
}


struct Configuration setStructure(char* file){
  struct Configuration conf;
  strcpy(conf.hostname, getHostname(file));
  conf.portnumber = getPort(file);
  strcpy(conf.gamekind, getGamekind(file));
  return conf;
}


int main(){

  struct Configuration config = setStructure("client.conf");
  printf("Hostname ist: %s", config.hostname);
  printf("Port ist: %d\n", config.portnumber);
  printf("Gamekind ist: %s", config.gamekind);
  /*printf("Hostname ist: %s", getHostname("client.conf"));
  printf("Gamekind ist: %s", getGamekind("client.conf"));
  printf("Portnumber ist: %d\n", getPort("client.conf"));*/
}
