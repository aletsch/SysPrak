#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define BUF 256


int communication(int *socket){
  int gameIsRunning = 1;
  char *ptr;
  //SEGMENTATION FAULT BUFFER ZU KLEIN
  char buffer[BUF];
  char temp[BUF];
  while(gameIsRunning){
    read(*socket, buffer, BUF);
    printf("%s", buffer);
    strcpy(temp, buffer);
    ptr = strtok(temp, " +\n");
    if(strcmp(ptr, "WAIT") == 0){
       write(*socket, "OKWAIT\n", strlen("OKWAIT\n"));
       printf("OKWAIT\n");
    } else if(strcmp(ptr, "GAMEOVER") == 0){
      read(*socket, buffer, BUF);
      printf("%s\n", buffer);
      gameIsRunning = 0;
    } else if(strcmp(ptr, "MOVE") == 0){
      //TODO thinker anstoßen
      printf("%s\n", buffer);
      write(*socket, "THINKING", strlen("THINKING"));
      read(*socket, buffer, BUF);
      printf("%s\n", buffer);
      write(*socket, "PLAY E3:D4", strlen("PLAY E3:D4"));
      read(*socket, buffer, BUF);
      printf("%s\n", buffer);
    } else {
      printf("wtf was that\n%s", buffer);
      gameIsRunning = 0;
    }
  }
}
