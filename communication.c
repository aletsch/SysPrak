#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "main.h"

void printBoard(){

  struct Spieldaten *spieldaten;

  int shmID = shmget(KEY, SHMSIZE, 0666);
  spieldaten = (struct Spieldaten *) shmat(shmID, NULL, 0);

  for(int i = 7; i >= 0; i--){
    for(int j = 0; j <= 7; j++){
      printf("%c ", spieldaten -> field[j][i]);
    }
    printf("\n");
  }
}

int updateBoard(char* board){

  int shmID;
  struct Spieldaten *spieldaten;

  //int shmSize = 2*sizeof(int)+BUF+2*sizeof(pid_t)+160;

  shmID = shmget(KEY, SHMSIZE, 0666);

  spieldaten = (struct Spieldaten *) shmat(shmID, NULL, 0);


  char * curLine = board;
  int zeile = 7;
  while(curLine)
  {
    char * nextLine = strchr(curLine, '\n');
    if (nextLine) *nextLine = '\0';  // temporarily terminate the current line

    char temp[64];
    char *ptr;
    strcpy(temp, curLine);

    ptr = strtok(temp, " +12345678\n");

    if (strcmp(ptr, "ENDBOARD") == 0){
      break;
    }

    for(int i = 0; i <= 7; i++){
      spieldaten -> field[i][zeile] = *ptr;
      ptr = strtok(NULL, " +12345678\n");

    }

    if (nextLine) *nextLine = '\n';  // then restore newline-char, just to be tidy
    curLine = nextLine ? (nextLine+1) : NULL;
    zeile--;
   }
  return 0;
}


int communication(int *socket, int pipe){

  int shmID;
  struct Spieldaten *spieldaten;
  shmID = shmget(KEY, SHMSIZE, 0666);
  spieldaten = (struct Spieldaten *) shmat(shmID, NULL, 0);

  int gameIsRunning = 1;
  char buffer[BUF];
  char *ptr;
  char temp[BUF];
  while(gameIsRunning){
    readServer(socket, buffer);
    char * curLine = buffer; //curline ist ab ist der start der aktuellen Zeile
    do{
      char * nextLine = strchr(curLine, '\n');  //pointer auf das erste \n in curLine
      if (nextLine) *nextLine = '\0';  //terminate the current line

      //SEGMENTATION FAULT BUFFER ZU KLEIN
      strcpy(temp, curLine);
      ptr = strtok(temp, " +\n");
      if(strcmp(ptr, "WAIT") == 0){
        writeServer(socket, buffer, "OKWAIT\n");
      } else if(strcmp(ptr, "GAMEOVER") == 0){
        readServer(socket, buffer);
        return -1;
      } else if(strcmp(ptr, "MOVE") == 0){
        //macht einfach gar nichts, weil wir nicht wissen ob wir neu lesen müssen, vielleicht Zeit für Spielzug rausnehmen(falls wir den thinker optimieren wollen)

      } else if(strcmp(ptr, "BOARD") == 0){
        //folgende Zeile wird verarbeitet
        curLine = nextLine ? (nextLine+1) : NULL;
        updateBoard(curLine);
        writeServer(socket, buffer, "THINKING\n");

        // thinker anstoßen
        kill(spieldaten -> thinker, SIGUSR1);


        break;
      } else if(strcmp(ptr, "OKTHINK") == 0){
        //Vom Thinker erhaltenen move übergeben
        char message[BUF];
        char anServer[BUF];
        read(pipe, message, BUF);
        strcpy(anServer, "PLAY ");
        strcat(anServer, message);
        writeServer(socket, buffer, anServer);
        readServer(socket, buffer);
      } else if(strcmp(ptr, "TOTAL") == 0){
        //nächste Zeilen überspringen: Ende der Prologphase
        curLine = nextLine ? (nextLine+1) : NULL;
        nextLine = strchr(curLine, '\n');
        curLine = nextLine ? (nextLine+1) : NULL;
        nextLine = strchr(curLine, '\n');
        curLine = nextLine ? (nextLine+1) : NULL;
      } else {
        printf("wtf was that\n%s\n", ptr);
        //printf("%s\n", curLine);
        return -1;
  }



      //gameIsRunning = inputVerarbeiten(curLine, socket);
      curLine = nextLine ? (nextLine+1) : NULL;
    }while(*curLine);
  }
  return 0;
}

