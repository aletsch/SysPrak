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
  shmdt(spieldaten);
}

int updateBoard(char* board){

  int shmID = 0;
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

    char temp[64] = "";
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

int spielerBereit(char* line){
  char* curLine = line;
  char *ptr;
  char* nextLine = strchr(curLine, '\n');
  char* output = malloc(sizeof(char)*30);

  curLine = nextLine ? (nextLine+1) : NULL;
  ptr = strtok(curLine, " +");
  strcpy(output, "Spieler ");
  strcat(output, ptr);
  ptr = strtok(NULL, " +\n");
  strcat(output, " (");
  strcat(output, ptr);
  strcat(output, ") ist ");
  ptr = strtok(NULL, " +\n");
  if (atoi(ptr)){
    strcat(output, "bereit.\n");
  } else {
    strcat(output, "noch nicht bereit.\n");
  }
  printf("%s", output);
  free(output);
  return 0;
}


int communication(int *socket, int pipe){

  int shmID = 0;
  struct Spieldaten *spieldaten;
  shmID = shmget(KEY, SHMSIZE, 0666);
  spieldaten = (struct Spieldaten *) shmat(shmID, NULL, 0);

  int gameover = 0;
  int playerOneWon = 0;
  int playerTwoWon = 0;

  char buffer[BUF] = "";
  char *ptr;
  char temp[BUF] = "";
  while(1){
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
        gameover = 1;
      } else if(strcmp(ptr, "PLAYER0WON") == 0){
        ptr = strtok(NULL, " +\n");
        if (strcmp(ptr, "Yes") == 0){
          playerOneWon = 1;
        } else {
          playerOneWon = 0;
        }
      } else if(strcmp(ptr, "PLAYER1WON") == 0){
        ptr = strtok(NULL, " +\n");
        if (strcmp(ptr, "Yes") == 0){
          playerTwoWon = 1;
        } else {
          playerTwoWon = 0;
        }
      } else if(strcmp(ptr, "QUIT") == 0){
        if(playerOneWon == playerTwoWon){
          printf("playerone: %d  playertwo: %d\n", playerOneWon, playerTwoWon);
          printf("It's a tie.\n");
        } else if((playerOneWon > playerTwoWon && (spieldaten -> playerNumber) == 0)|| (playerTwoWon > playerOneWon && (spieldaten -> playerNumber) == 1)){
          printf("Congratulations, you won!\n");
        } else {
          printf("You lose\n");
        }
        shmdt(spieldaten);
        kill(spieldaten -> thinker, SIGTERM);
        return 0;
      } else if(strcmp(ptr, "MOVE") == 0){
        //macht einfach gar nichts, weil wir nicht wissen ob wir neu lesen müssen, vielleicht Zeit für Spielzug rausnehmen(falls wir den thinker optimieren wollen)

      } else if(strcmp(ptr, "BOARD") == 0){
        //folgende Zeile wird verarbeitet
        curLine = nextLine ? (nextLine+1) : NULL;
        char board[BUF] = "";
        strcpy(board, curLine);
        updateBoard(board);
        if (gameover == 0){
          writeServer(socket, buffer, "THINKING\n");

          // thinker anstoßen
          kill(spieldaten -> thinker, SIGUSR1);
        } else {
          for (int i = 0; i<9; i++){
            curLine = nextLine ? (nextLine+1) : NULL;
            nextLine = strchr(curLine, '\n');
          }
        }

      } else if(strcmp(ptr, "OKTHINK") == 0){
        //Vom Thinker erhaltenen move übergeben
        char message[BUF] = "";
        char anServer[BUF] = "";
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
      } else if(strcmp(ptr, "OKTHINK") == 0){

      } else {
        printf("wtf was that\n%s\n", ptr);
        shmdt(spieldaten);
        kill(spieldaten -> thinker, SIGTERM);
        return -1;
  }



      curLine = nextLine ? (nextLine+1) : NULL;
    }while(*curLine);
  }
}

