//Bibliotheken einbinden
#include "main.h" 

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
  return 0;
}


struct Configuration setConfig(char* file){
  struct Configuration conf;
  strcpy(conf.hostname, getHostname(file));
  conf.portnumber = getPort(file);
  strcpy(conf.gamekind, getGamekind(file));
  return conf;
}
