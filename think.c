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

int think() {

  int shmID;
  struct Spieldaten *spieldaten;
  shmID = shmget(KEY, SHMSIZE, 0666);
  spieldaten = (struct Spieldaten *) shmat(shmID, NULL, 0);

  struct moeglicherZug spielzug;


  switch (atoi(spieldaten.playerNumber)) {
    case 0:     //weiß
      for(int x=0, x<8, x++) {
        for (int y=7, y>=0, y--) {
          if (spieldaten.field[x][y] == ('w' || 'W')) {
            spielzug = possibleMovesWhite();
            }
          }
        }
      }
    case 1:     //schwarz
      for(int x=0, x<8, x++) {
        for (int y=7, y>=0, y--) {
          if (spieldaten.field[x][y] == ('b' || 'B')) {
            possibleMovesBlack();
          }
        }
      }
  }
}





//hier werden alle züge einer weißen Figur auf Gültigkeit geprüft

//falls 'out of bounds'- artige Fehlermeldungen kommen -> if-statements jeweils um
//"&& (x+1<8) && (x+1>-1) && (y+1<8) && (y+1>-1)" oder Ähnliches ergänzen
struct moeglicherZug possibleMovesWhite(int x, int y, struct moeglicherZug bestMove/* field[8][8] field*/) {
  struct moeglicherZug currentMove;
  currentMove.gewichtung = -1;
  //pawn move
  if (spieldaten.field[x][y] == 'w') {
    //nach links oben
    if (spieldaten.field[x-1][y+1] == '*') {
      //TODO diesen move in "nicht schlagende Züge"-Datei schreiben
      //beispiel nicht schlagend:
      currentMove.gewichtung++;
      if (currentMove.gewichtung > bestMove.gewichtung){
        return curentMove;
      } else {
        return bestMove;
      }
    } else if ((spieldaten.field[x-1][y+1] == ('b' || 'B')) && (spieldaten.field[x-2][y+2] == '*')){
      //TODO diesen move in "schlagende Züge"-Datei schreiben
      //bzw eigentlich nach weiteren, den Zug vollendenden, schlagenden Zügen suchen (rekursiv?)
      //beispiel schlagend:
      currentMove.gewichtung = currentMove.gewichtung +2;
      //rekursiver Aufruf mit temporären Feld
      possibleMovesWhite(x-2, x+2, currentMove //, tempspielfeld);
      if (currentMove.gewichtung > bestMove.gewichtung){
        return currentMove;
      } else {
        return bestMove;
      }
    }
    //nach rechts open
    if (spieldaten.field[x+1][y+1] == '*') {
      //TODO diesen move in "nicht schlagende Züge"-Datei schreiben
    } else if ((spieldaten.field[x+1][y+1] == ('b' || 'B')) && (spieldaten.field[x+2][y+2] == '*')){
      //TODO diesen move in "schlagende Züge"-Datei schreiben
      //bzw eigentlich nach weiteren, den Zug vollendenden, schlagenden Zügen suchen (rekursiv?)
    }

  }
  //Queen move
  if (spieldaten.field[x][y] == 'W') {

    //TODO erstmal nachschauen ob schon ein schlagender Zug gefunden wurde

    //TODO schauen ob Queen schlagen kann




    //nach nicht schlagenden Zügen suchen
      //hier fehlt noch: Dame kann theoretisch weiter als nur ein Feld laufen

    //nach links oben
    if (spieldaten.field[x-1][y+1] == '*') {
      //TODO diesen move in "nicht schlagende Züge"-Datei schreiben
    }
    //nach rechts oben
    if (spieldaten.field[x+1][y+1] == '*') {
      //TODO diesen move in "nicht schlagende Züge"-Datei schreiben
    }
    //nach links unten
    if (spieldaten.field[x-1][y-1] == '*') {
      //TODO diesen move in "nicht schlagende Züge"-Datei schreiben
    }
    //nach rechts unten
    if (spieldaten.field[x+1][y-1] == '*') {
      //TODO diesen move in "nicht schlagende Züge"-Datei schreiben
    }
  }


// ünnötig geworden durch 2-Dateien Idee
  // if (moveFound) {
  //   //TODO move in die pipe schreiben
  //   return 1;
  // }
  return bestMove;
}

//hier werden alle Züge einer schwarzen Figur auf Gültigkeit geprüft
int possibleMovesBlack() {

  return 0;
}
