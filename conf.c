//Bibliotheken einbinden
#include "main.h" 

char* getHostname(char* file){

  FILE* fpointer;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  char *ptr;
  char *ergebnis = malloc(sizeof(char)*64);

  fpointer = fopen(file, "r");
  
  while((read = getline(&line, &len, fpointer)) != -1){
    
    ptr = strtok(line, " =");
    if(strcmp(ptr, "hostname") == 0){
      ptr = strtok(NULL, " =");
      break;
    }
  }

  ptr[strlen(ptr)-1] = 0;
  fclose(fpointer);

  strcpy(ergebnis, ptr);
  free(line);
  return ergebnis;
}


int getPort(char* file){

  FILE* fpointer;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  char *ptr;
  int ergebnis;

  fpointer = fopen(file, "r");
  
  while((read = getline(&line, &len, fpointer)) != -1){
    
    ptr = strtok(line, " =");
    if(strcmp(ptr, "portnumber") == 0){
      ptr = strtok(NULL, " =");
      break;
    }
  }

  ptr[strlen(ptr)-1] = 0;
  fclose(fpointer);
  ergebnis = atoi(ptr);
  free(line);
  return ergebnis;
}



char* getGamekind(char* file){

  FILE* fpointer;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  char *ptr;
  char *ergebnis = malloc(sizeof(char)*64);

  fpointer = fopen(file, "r");
  
  while((read = getline(&line, &len, fpointer)) != -1){
    
    ptr = strtok(line, " =");
    if(strcmp(ptr, "gamekind") == 0){
      ptr = strtok(NULL, " =");
      break;
    }
  }

  ptr[strlen(ptr)-1] = 0;
  fclose(fpointer);
  strcpy(ergebnis, ptr);
  free(line);
  return ergebnis;
}


struct Configuration setConfig(char* file){
  struct Configuration conf;
  char* temp = getHostname(file);
  strcpy(conf.hostname, temp);
  free(temp);
  temp = getGamekind(file);
  strcpy(conf.gamekind, temp);
  free(temp);
  conf.portnumber = getPort(file);
  return conf;
}
