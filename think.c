#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>

#include "main.h"


char *getCoordinate(int x, int y){
  char* coordinate = malloc(sizeof(char) * 3);

  switch(x){
    case 0:
      strcpy(coordinate, "A");
      break;
    case 1:
      strcpy(coordinate, "B");
      break;
    case 2:
      strcpy(coordinate, "C");
      break;
    case 3:
      strcpy(coordinate, "D");
      break;
    case 4:
      strcpy(coordinate, "E");
      break;
    case 5:
      strcpy(coordinate, "F");
      break;
    case 6:
      strcpy(coordinate, "G");
      break;
    case 7:
      strcpy(coordinate, "H");
      break;
  }

  switch(y){
    case 0:
      strcat(coordinate, "1");
      break;
    case 1:
      strcat(coordinate, "2");
      break;
    case 2:
      strcat(coordinate, "3");
      break;
    case 3:
      strcat(coordinate, "4");
      break;
    case 4:
      strcat(coordinate, "5");
      break;
    case 5:
      strcat(coordinate, "6");
      break;
    case 6:
      strcat(coordinate, "7");
      break;
    case 7:
      strcat(coordinate, "9");
      break;
  }

  return coordinate;
}

int inBound(int x, int y) {
  if ((x<8)&&(x>-1)&&(y<8)&&(y>-1)) {
    return 1;
  } else {
    return 0;
  }
}

//hier werden alle züge einer weißen Figur auf Gültigkeit geprüft

struct moeglicherZug possibleMovesWhite(int x, int y, struct moeglicherZug bestMove, int geschlagen, char* moveBisher, char* currentField[8][8]) {

  char* ergebnis = malloc(sizeof(char)*64);

  struct moeglicherZug currentMove;
  currentMove.gewichtung = -1;

  //moveBisher  = malloc(sizeof(char)*64);
  strcat(moveBisher, getCoordinate(x,y));         //concate move
  strcat(moveBisher, ":");

  //pawn move
  if (*currentField[x][y] == 'w') {
    //nach links oben
    if (inBound(x-2, y+2) && (*currentField[x-1][y+1] == ('b' || 'B')) && (*currentField[x-2][y+2] == '*')){
      currentMove.gewichtung = currentMove.gewichtung +2;
      *currentField[x][y]      = '*';
      *currentField[x-2][y+2]  = 'w';
      *currentField[x-1][y+1]  = '*';
      //rekursiver Aufruf mit temporären Feld
      currentMove = possibleMovesWhite(x-2, x+2, currentMove, 1 , moveBisher, currentField);
      goto ZUGBEENDEN;
    } else if (inBound(x-1, y+1) && (*currentField[x-1][y+1] == '*') && (geschlagen == 0)) {
      strcat(moveBisher, getCoordinate(x-1, y+1));
      currentMove.gewichtung++;
      goto ZUGBEENDEN;
    }
    //nach rechts oben
    if (inBound(x+2, y+2) && (*currentField[x+1][y+1] == ('b' || 'B')) && (*currentField[x+2][y+2] == '*')){
      currentMove.gewichtung = currentMove.gewichtung +2;
      *currentField[x][y]      = '*';
      *currentField[x+2][y+2]  = 'w';
      *currentField[x+1][y+1]  = '*';
      //rekursiver Aufruf mit temporären Feld
      currentMove = possibleMovesWhite(x+2, x+2, currentMove, 1 , moveBisher, currentField);
      goto ZUGBEENDEN;
    } else if (inBound(x+1, y+1) && (*currentField[x+1][y+1] == '*') && (geschlagen == 0)) {
      strcat(moveBisher, getCoordinate(x+1, y+1));
      currentMove.gewichtung++;
      goto ZUGBEENDEN;
    }

  }
  //Queen move
  // if (spieldaten.field[x][y] == 'W') {
  //
  //   //TODO erstmal nachschauen ob schon ein schlagender Zug gefunden wurde
  //
  //   //TODO schauen ob Queen schlagen kann
  //
  //
  //
  //
  //   //nach nicht schlagenden Zügen suchen
  //     //hier fehlt noch: Dame kann theoretisch weiter als nur ein Feld laufen
  //
  //   //nach links oben
  //   if (inBound(x-1, y+1) && (spieldaten.field[x-1][y+1] == '*')) {
  //     //TODO diesen move in "nicht schlagende Züge"-Datei schreiben
  //   }
  //   //nach rechts oben
  //   if (inBound(x+1, y+1) && (spieldaten.field[x+1][y+1] == '*')) {
  //     //TODO diesen move in "nicht schlagende Züge"-Datei schreiben
  //   }
  //   //nach links unten
  //   if (inBound(x-1, y-1) && (spieldaten.field[x-1][y-1] == '*')) {
  //     //TODO diesen move in "nicht schlagende Züge"-Datei schreiben
  //   }
  //   //nach rechts unten
  //   if (inBound(x+1, y-1) && (spieldaten.field[x+1][y-1] == '*')) {
  //     //TODO diesen move in "nicht schlagende Züge"-Datei schreiben
  //   }
  // }


  ZUGBEENDEN:
    //moveBisher[(strlen(moveBisher)-1)] = "\0";

    strncat(ergebnis, moveBisher, strlen(moveBisher)-1);
    strcat(ergebnis, "\n");
    strcpy(currentMove.zug, ergebnis);
    if (currentMove.gewichtung > bestMove.gewichtung){
      return currentMove;
    } else {
      return bestMove;
    }
}

//hier werden alle Züge einer schwarzen Figur auf Gültigkeit geprüft
// int possibleMovesBlack() {
//
//   return 0;
// }


char* think() {

  int shmID;
  struct Spieldaten *spieldaten;
  shmID = shmget(KEY, SHMSIZE, 0666);
  spieldaten = (struct Spieldaten *) shmat(shmID, NULL, 0);
  char* ergebnis = malloc(sizeof(char)*64);

  struct moeglicherZug spielzug;

  char* currentField[8][8];
  memcpy(*currentField, spieldaten->field, sizeof(char)*8*8);

  switch (spieldaten->playerNumber) {
    case 0:     //weiß
      for(int x=0; x<8; x++) {
        for (int y=7; y>=0; y--) {
          if (spieldaten->field[x][y] == ('w' || 'W')) {
            char* moveBisher  = malloc(sizeof(char)*64);
            strcpy(moveBisher, "");
            spielzug = possibleMovesWhite(x,y, spielzug, 0, moveBisher, currentField);
            free(moveBisher);
            }
          }
        }
      }
    // case 1:     //schwarz
    //   for(int x=0, x<8, x++) {
    //     for (int y=7, y>=0, y--) {
    //       if (spieldaten.field[x][y] == ('b' || 'B')) {
    //         possibleMovesBlack();
    //       }
    //     }
    //   }
    strcpy(ergebnis, spielzug.zug);
    return ergebnis;
  }
