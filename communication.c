#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define BUF 256


int communication(int *socket){
  bool gameIsRunning = true
  char *ptr;
  //SEGMENTATION FAULT BUFFER ZU KLEIN
  char buffer[BUF];
  char temp[BUF];
  while(gameIsRunning){
    read(socket, buffer, BUF);
    strcpy(temp, buffer);
    ptr = strtok(temp, " +\n");
    if(strcmp(ptr, "WAIT") == 0){
       send(socket, "OKWAIT", strlen("OKWAIT"));
    } else if(strcmp(ptr, "GAMEOVER") == 0){
      //TODO 
    } else if(strcmp(ptr, "MOVE") == 0){
      //TODO
    } else {
      prtinf("wtf was that\n%s", buffer);
    }
  }
}
