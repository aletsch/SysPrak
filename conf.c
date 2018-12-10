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
    //remove new line character
    char *newline = strchr(line, '\n');
    if(newline) *newline = 0;
    ptr = strtok(line, " =");
    if(strcmp(ptr, "hostname") == 0){
      ptr = strtok(NULL, " =");
      fclose(fpointer);
      free(line);
      return ptr;
    }
  }
  printf("Fehler in der Konfigurationsdatei");
  exit(EXIT_FAILURE);
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
    ptr = strtok(line, " =\n");
    if(strcmp(ptr, "portnumber") == 0){
      ptr = strtok(NULL, " =");
      fclose(fpointer);
      free(line);
      return atoi(ptr);
    }
  }
  printf("Fehler in der Konfigurationsdatei");
  exit(EXIT_FAILURE);
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
    //remove new line character
    char *newline = strchr(line, '\n');
    if(newline) *newline = 0;
    ptr = strtok(line, " =\n");
    if(strcmp(ptr, "gamekind") == 0){
      ptr = strtok(NULL, " =");
      fclose(fpointer);
      free(line);
      return ptr;
    }
  }
  printf("Fehler in der Konfigurationsdatei");
  exit(EXIT_FAILURE);
  free(line);
  return "hello";
}


struct Configuration setConfig(char* file){
  struct Configuration conf;
  strcpy(conf.hostname, getHostname(file));
  conf.portnumber = getPort(file);
  strcpy(conf.gamekind, getGamekind(file));
  return conf;
}


/*int main(){

  struct Configuration config = setConfig("client.conf");
  printf("Hostname ist: %s\n", config.hostname);
  printf("Port ist: %d\n", config.portnumber);
  printf("Gamekind ist: %s\n", config.gamekind);
  /*printf("Hostname ist: %s", getHostname("client.conf"));
  printf("Gamekind ist: %s", getGamekind("client.conf"));
  printf("Portnumber ist: %d\n", getPort("client.conf"));*/
//}
