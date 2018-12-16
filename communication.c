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

int communication(int *socket){
  int gameIsRunning = 1;
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
}
