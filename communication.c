#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define BUF 256


int printBoard(char* board){
  printf("%s\n", board);
  //TODO Zeile für Zeile auslesen und Board in shared memory übertragen
  return 0;
}


int inputVerarbeiten(char* buffer, int *socket){
  char *ptr;
  //SEGMENTATION FAULT BUFFER ZU KLEIN
  char temp[BUF];
  strcpy(temp, buffer);
  ptr = strtok(temp, " +\n");
  if(strcmp(ptr, "WAIT") == 0){
    writeServer(socket, buffer, "OKWAIT\n");
    return 1;
  } else if(strcmp(ptr, "GAMEOVER") == 0){
    readServer(socket, buffer);
    return 0;
  } else if(strcmp(ptr, "MOVE") == 0){
    //board wird ausgegeben
    readServer(socket, buffer);
    printBoard(buffer);
    writeServer(socket, buffer, "THINKING\n");

    //TODO thinker anstoßen

    return 1;
  } else if(strcmp(ptr, "OKTHINK\n") == 0){
    //Vom Thinker erhaltenen move übergeben
    writeServer(socket, buffer, "PLAY B6:C5\n");
    readServer(socket, buffer);
    return 1;
  } else {
    printf("wtf was that\n");
    return 0;
  }
}


int communication(int *socket){
  int gameIsRunning = 1;
  char buffer[BUF];
  while(gameIsRunning){
    readServer(socket, buffer);
    char * curLine = buffer;
    do{
      char * nextLine = strchr(curLine, '\n');
      if (nextLine) *nextLine = '\0';  // temporarily terminate the current line
      gameIsRunning = inputVerarbeiten(curLine, socket);
      curLine = nextLine ? (nextLine+1) : NULL;
    }while(*curLine);
  }
}

/*int communication(int *socket){
  int gameIsRunnino");
println();g = 1;
  char *ptr;
  //SEGMENTATION FAULT BUFFER ZU KLEIN
  char buffer[BUF];
  char temp[BUF];
  while(gameIsRunning){
    readServer(socket, buffer);
    strcpy(temp, buffer);
    ptr = strtok(temp, " +\n");
    if(strcmp(ptr, "WAIT") == 0){
       writeServer(socket, buffer, "OKWAIT\n");
    } else if(strcmp(ptr, "GAMEOVER") == 0){
      readServer(socket, buffer);
      gameIsRunning = 0;
    } else if(strcmp(ptr, "MOVE") == 0){
      //board wird ausgegeben
      readServer(socket, buffer);
      printBoard(buffer);
      writeServer(socket, buffer, "THINKING\n");

      //TODO thinker anstoßen


    } else if(strcmp(ptr, "OKTHINK\n")){
      //Vom Thinker erhaltenen move übergeben
      writeServer(socket, buffer, "PLAY B6:C5\n");
      readServer(socket, buffer);
    } else {
      printf("wtf was that\n");
      gameIsRunning = 0;
    }
  }
  return 0;
}*/
