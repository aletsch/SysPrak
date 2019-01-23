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
      strcat(coordinate, "8");
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
//rx Richung x: -1 links +1 rechts
//ry Richtung y: -1 unten +1 oben
struct queenData queenStrike(int rx, int ry, struct queenData strike)
{

    

    for (int distance = 1; distance<8; distance ++){
        int xnew = strike.x+(distance*rx);
        int ynew = strike.y+(distance*ry);
        if(inBound(xnew, ynew))
        {
            if((strike.field[xnew][ynew] == *strike.enemyColour[1] || strike.field[xnew][ynew] == *strike.enemyColour[2] ) && strike.field[xnew+rx][ynew+ry] == '*')
                {
                    strike.bestMove.gewichtung = strike.bestMove.gewichtung + 2;
                    strike.field[strike.x][strike.y]      = '*';
                    strike.field[xnew+rx][ynew+ry]  = *strike.ownColour;
                    strike.field[xnew][ynew]  = '*';
                    strcat(strike.moveATM, getCoordinate(xnew-rx,ynew+ry));
                    strcat(strike.moveATM, ":");
                    strike.success = 1;
                    strike.x=strike.x+(distance*rx);
                    strike.y=strike.y +(distance*ry);
                    
                    //rekursiver Aufruf mit temporärem Feld
                    strike = queenStrike(xnew-rx, ynew+ry, strike);
                }
        }
        else
        { break; }
    }


    return strike;
}

//enemy Colour 1 klein Buchstabe enemy Colour 2 Gossbuchstabe
//ownColour muss in Grossbuchstabe sein
struct moeglicherZug queenMove(int x, int y, struct moeglicherZug bestMove, int geschlagen, char* moveBisher, char field[8][8], char* enemyColour[2], char* ownColour)
{
    //create and fill queenData
    struct queenData strike;
    strike.x = x;
    strike.y = y;
    strike.success = 0;
    strcpy(strike.moveATM,moveBisher);
    memcpy(strike.field, field, sizeof(char)*8*8);
    memcpy(strike.enemyColour, enemyColour, sizeof(char)*2);
    strcpy(strike.ownColour, ownColour);   //, sizeof(char));
    strike.bestMove = bestMove;

    //nach links oben schlagen

    strike = queenStrike(-1,1,strike);

    /*for (int distance = 1; distance<8; distance ++){
        int xnew = x-distance;
        int ynew = y+distance;
        if(inBound(xnew, ynew))
        {
            if((currentField[xnew][ynew] == enemyColour[1] || currentField[xnew][ynew] == enemyColour[2] ) && currentField[xnew-1][ynew+1] == '*')
                {
                    currentMove.gewichtung = currentMove.gewichtung + 2;
                    currentField[xnew][ynew]      = '*';
                    currentField[xnew-1][ynew+1]  = ownColour;
                    currentField[xnew-1][ynew+1]  = '*';
                    strcat(moveBisher, getCoordinate(xnew-1,ynew+1));
                    strcat(moveBisher, ':');
                    
                    //rekursiver Aufruf mit temporärem Feld
                    currentMove = queenMove(xnew-1, ynew+1, currentMove, 1 , moveBisher, currentField, ownColour);
                }
        }
        else
        { break; }
    }*/

    

    //nach rechts oben schlagen

    //nach links unten schalgen

    //nach rechts unten schlagen


    //release variables
    return strike.bestMove;
} 

//hier werden alle züge einer weißen Figur auf Gültigkeit geprüft
//Übergabewerte aktuelle Position "(x,y)"; der aktuelle beste Zug "bestMove"; kann geschlagen werden "geschlagen"; 
struct moeglicherZug possibleMovesWhite(int x, int y, struct moeglicherZug bestMove, int geschlagen, char* moveBisher, char field[8][8]) {

  char* ergebnis = malloc(sizeof(char)*64);
  char currentField[8][8];
  memcpy(currentField, field, sizeof(char)*8*8);
  struct moeglicherZug currentMove;
  if(geschlagen){
    currentMove.gewichtung = bestMove.gewichtung;
  } else {
    currentMove.gewichtung = -1;
  }

  //moveBisher  = malloc(sizeof(char)*64);
  strcat(moveBisher, getCoordinate(x,y));         //concate move
  strcat(moveBisher, ":");
  //pawn move
  if (currentField[x][y] == 'w') {
    if (inBound(x-2, y+2) && (currentField[x-1][y+1] == 'b' || currentField[x-1][y+1] == 'B' ) && (currentField[x-2][y+2] == '*')){
      //nach links oben schlagen
      currentMove.gewichtung = currentMove.gewichtung + 2;
      currentField[x][y]      = '*';
      currentField[x-2][y+2]  = 'w';
      currentField[x-1][y+1]  = '*';
      //rekursiver Aufruf mit temporären Feld
      currentMove = possibleMovesWhite(x-2, y+2, currentMove, 1 , moveBisher, currentField);
      goto ZUGBEENDEN;
    } else if (inBound(x+2, y+2) && (currentField[x+1][y+1] == 'b' || currentField[x+1][y+1] == 'B') && (currentField[x+2][y+2] == '*')){
      //nach rechts oben schlagen
      currentMove.gewichtung = currentMove.gewichtung + 2;
      currentField[x][y]      = '*';
      currentField[x+2][y+2]  = 'w';
      currentField[x+1][y+1]  = '*';
      //rekursiver Aufruf mit temporären Feld
      currentMove = possibleMovesWhite(x+2, y+2, currentMove, 1 , moveBisher, currentField);
      goto ZUGBEENDEN;
    } else if (inBound(x-1, y+1) && (currentField[x-1][y+1] == '*') && (geschlagen == 0)) {
      //nach links oben bewegen
      strcat(moveBisher, getCoordinate(x-1, y+1));
      strcat(moveBisher, ":");
      //printf("moveBewegtLinks: %s\n", moveBisher);
      currentMove.gewichtung++;
      goto ZUGBEENDEN;
    } else if (inBound(x+1, y+1) && (currentField[x+1][y+1] == '*') && (geschlagen == 0)) {
      //nach rechts oben bewegen
      strcat(moveBisher, getCoordinate(x+1, y+1));
      strcat(moveBisher, ":");
      //printf("moveBewegtRechts: %s\n", moveBisher);
      currentMove.gewichtung++;
      goto ZUGBEENDEN;
    }

  }


    if(currentField[x][y] == 'W'){
        char* enemyColour[2];
        strcpy(enemyColour[1], "b");
        strcpy(enemyColour[2], "B");
        char* ownColour;
        strcpy(ownColour,"W");
        queenMove(x, y, bestMove, geschlagen, moveBisher, field, enemyColour, ownColour);
        }
    
  /*
  //Queen move
  if (currentField[x][y] == 'W') {
    //suche Schlag links oben
    if(inBound(x-2, y+2) && (currentField[x-1][y+1] == 'b' || currentField[x-1][y+1] == 'B' ) && (currentField[x-2][y+2] == '*')){
        
    

    //suche Schlag rechts oben

    //suche Schlag links unten

    //suche Schlag rechts unten



    //TODO erstmal nachschauen ob schon ein schlagender Zug gefunden wurde
    
    //TODO schauen ob Queen schlagen kann




    //nach nicht schlagenden Zügen suchen
      //hier fehlt noch: Dame kann theoretisch weiter als nur ein Feld laufen

    //nach links oben
    if (inBound(x-1, y+1) && (spieldaten.field[x-1][y+1] == '*')) {
      //TODO diesen move in "nicht schlagende Züge"-Datei schreiben
    }
    //nach rechts oben
    if (inBound(x+1, y+1) && (spieldaten.field[x+1][y+1] == '*')) {
      //TODO diesen move in "nicht schlagende Züge"-Datei schreiben
    }
    //nach links unten
    if (inBound(x-1, y-1) && (spieldaten.field[x-1][y-1] == '*')) {
      //TODO diesen move in "nicht schlagende Züge"-Datei schreiben
    }
    //nach rechts unten
    if (inBound(x+1, y-1) && (spieldaten.field[x+1][y-1] == '*')) {
      //TODO diesen move in "nicht schlagende Züge"-Datei schreiben
    }
  }
  */


  ZUGBEENDEN:
    //moveBisher[(strlen(moveBisher)-1)] = "\0";

    strncat(ergebnis, moveBisher, strlen(moveBisher)-1);
    strcat(ergebnis, "\n");
    strcpy(currentMove.zug, ergebnis);
    //printf("momentaner Zug: %s", currentMove.zug);
    //printf("mit der Gewichtung: %d\n\n", currentMove.gewichtung);
    if (currentMove.gewichtung > bestMove.gewichtung){
      return currentMove;
    } else {
      return bestMove;
    }
}

struct moeglicherZug possibleMovesBlack(int x, int y, struct moeglicherZug bestMove, int geschlagen, char* moveBisher, char field[8][8]) {

  char* ergebnis = malloc(sizeof(char)*64);
  char currentField[8][8];
  memcpy(currentField, field, sizeof(char)*8*8);
  struct moeglicherZug currentMove;
  if(geschlagen){
    currentMove.gewichtung = bestMove.gewichtung;
  } else {
    currentMove.gewichtung = -1;
  }

  //moveBisher  = malloc(sizeof(char)*64);
  strcat(moveBisher, getCoordinate(x,y));         //concate move
  strcat(moveBisher, ":");
  //pawn move
  if (currentField[x][y] == 'b') {
    if (inBound(x-2, y-2) && (currentField[x-1][y-1] == 'w' || currentField[x-1][y-1] == 'W' ) && (currentField[x-2][y-2] == '*')){
      //nach links unten schlagen
      currentMove.gewichtung = currentMove.gewichtung + 2;
      currentField[x][y]      = '*';
      currentField[x-2][y-2]  = 'b';
      currentField[x-1][y-1]  = '*';
      //rekursiver Aufruf mit temporären Feld
      currentMove = possibleMovesBlack(x-2, y-2, currentMove, 1 , moveBisher, currentField);
      goto ZUGBEENDEN;
    } else if (inBound(x+2, y-2) && (currentField[x+1][y-1] == 'w' || currentField[x+1][y-1] == 'W') && (currentField[x+2][y-2] == '*')){
      //nach rechts unten schlagen
      currentMove.gewichtung = currentMove.gewichtung + 2;
      currentField[x][y]      = '*';
      currentField[x+2][y-2]  = 'b';
      currentField[x+1][y-1]  = '*';
      //rekursiver Aufruf mit temporären Feld
      currentMove = possibleMovesBlack(x+2, y-2, currentMove, 1 , moveBisher, currentField);
      goto ZUGBEENDEN;
    } else if (inBound(x-1, y-1) && (currentField[x-1][y-1] == '*') && (geschlagen == 0)) {
      //nach links unten bewegen
      strcat(moveBisher, getCoordinate(x-1, y-1));
      strcat(moveBisher, ":");
      //printf("moveBewegtLinks: %s\n", moveBisher);
      currentMove.gewichtung++;
      goto ZUGBEENDEN;
    } else if (inBound(x+1, y-1) && (currentField[x+1][y-1] == '*') && (geschlagen == 0)) {
      //nach rechts unten bewegen
      strcat(moveBisher, getCoordinate(x+1, y-1));
      strcat(moveBisher, ":");
      //printf("moveBewegtRechts: %s\n", moveBisher);
      currentMove.gewichtung++;
      goto ZUGBEENDEN;
    }

  }

  ZUGBEENDEN:
    //moveBisher[(strlen(moveBisher)-1)] = "\0";

    strncat(ergebnis, moveBisher, strlen(moveBisher)-1);
    strcat(ergebnis, "\n");
    strcpy(currentMove.zug, ergebnis);
    //printf("momentaner Zug: %s", currentMove.zug);
    //printf("mit der Gewichtung: %d\n\n", currentMove.gewichtung);
    if (currentMove.gewichtung > bestMove.gewichtung){
      return currentMove;
    } else {
      return bestMove;
    }
}



char* think() {

  int shmID;
  struct Spieldaten *spieldaten;
  shmID = shmget(KEY, SHMSIZE, 0666);
  spieldaten = (struct Spieldaten *) shmat(shmID, NULL, 0);
  char* ergebnis = malloc(sizeof(char)*64);

  struct moeglicherZug spielzug;
  strcpy(spielzug.zug, "");
  spielzug.gewichtung = -2;

  char currentField[8][8];
  memcpy(&currentField, spieldaten->field, sizeof(char)*8*8);
  char spielStein;

  switch (spieldaten->playerNumber) {
    case 0:     //weiß
      for(int y=0; y<8; y++) {
        for (int x=0; x<8; x++) {
          spielStein = spieldaten->field[x][y];
          if (spielStein == 'w' || spielStein == 'W') {
            char* moveBisher  = malloc(sizeof(char)*64);
            strcpy(moveBisher, "");
            spielzug = possibleMovesWhite(x,y, spielzug, 0, moveBisher, currentField);
            printf("bester Zug bisher: %smit der Gewichtung: %d\n\n", spielzug.zug, spielzug.gewichtung);
            free(moveBisher);
            }
          }
        }
      break;
    case 1:     //schwarz
      for(int x=0; x<8; x++) {
        for (int y=7; y>=0; y--) {
          spielStein = spieldaten->field[x][y];
          if (spielStein == 'b' || spielStein == 'B') {
            char* moveBisher  = malloc(sizeof(char)*64);
            strcpy(moveBisher, "");
            spielzug = possibleMovesBlack(x,y, spielzug, 0, moveBisher, currentField);
            printf("bester Zug bisher: %smit der Gewichtung: %d\n\n", spielzug.zug, spielzug.gewichtung);
            free(moveBisher);
          }
        }
      }
    break;
  }

  strcpy(ergebnis, spielzug.zug);
  printf("thinker sagt%s \n", ergebnis);
  return ergebnis;
}
