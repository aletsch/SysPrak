#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>

#include "main.h"

int getTeam(char spielstein){
  if(spielstein == 'w' || spielstein == 'W'){
    return 0;
  } else if(spielstein == 'b' || spielstein == 'B'){
    return 1;
  }
}


void printField(char field[8][8])
{
    for(int i=7; i>=0; i--)
    {
        for(int j=0; j<=7; j++)
        {
            printf("%c ", field[j][i]);
        }
        printf("\n");
    }
}


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
    //printf("Starte queenStrike\n");
    struct queenData temp;
    

    for (int distance = 1; distance<8; distance ++){
        int xnew = strike.x+(distance*rx);
        int ynew = strike.y+(distance*ry);
        if(inBound(xnew, ynew)&&inBound(xnew+rx,ynew+ry))
        {
            printField(strike.field);
            if((strike.field[xnew][ynew] == strike.enemyColour[0] || strike.field[xnew][ynew] == strike.enemyColour[1]) && strike.field[xnew+rx][ynew+ry] == '*')
                {
                    strcat(strike.moveATM,getCoordinate(xnew+rx,ynew+ry));
                    strcat(strike.moveATM, ":");
                    strcpy(strike.bestMove.zug,strike.moveATM);
                    strike.bestMove.gewichtung = strike.bestMove.gewichtung + 2;
                    strike.field[strike.x][strike.y]      = '*';
                    strike.field[xnew+rx][ynew+ry]  = strike.ownColour;
                    strike.field[xnew][ynew]  = '*';
                    strike.x=xnew+rx;
                    strike.y=ynew+ry;
                    printField(strike.field);
                    printf("\n");
                    

                    //rekursiver Aufruf für Folgeschläge
                    //links oben
                    temp=queenStrike(-1,1,strike);
                    if(temp.bestMove.gewichtung>strike.bestMove.gewichtung)
                    {
                        strike=temp;
                    }


                    //rechts oben
                    temp=queenStrike(1,1,strike);
                    if(temp.bestMove.gewichtung>strike.bestMove.gewichtung)
                    {
                        strike=temp;
                    }

                    //links unten
                    temp=queenStrike(-1,-1,strike);
                    if(temp.bestMove.gewichtung>strike.bestMove.gewichtung)
                    {
                        strike=temp;
                    }

                    //rechts unten
                    temp=queenStrike(-1,-1,strike);
                    if(temp.bestMove.gewichtung>strike.bestMove.gewichtung)
                    {
                        strike=temp;
                    }
                    
                }
                
            }
    }
return strike;
}



//enemy Colour 1 klein Buchstabe enemy Colour 2 Gossbuchstabe
//ownColour muss in Grossbuchstabe sein

struct moeglicherZug queenMove(int x, int y, struct moeglicherZug bestMove, int geschlagen, char* moveBisher, char field[8][8], char enemyColour[2], char ownColour)
{
    printf("Start queenMove\n");
    //create and fill queenData
    struct queenData strike;
    strike.x = x;
    strike.y = y;
    strike.success = 0;
    strcpy(strike.moveATM, getCoordinate(x,y));
    strcat(strike.moveATM, ":");
    memcpy(strike.field, field, sizeof(char)*8*8);
    strike.enemyColour[0] = enemyColour[0];
    strike.enemyColour[1]=enemyColour[1];
    strike.ownColour = ownColour;
    strike.bestMove = bestMove;

    
    //nach Schlagenden Zuegen suchen
    //links oben
    strike = queenStrike(-1,1,strike);
    //rechts oben
    strike = queenStrike(1,1,strike);
    //links unten
    strike = queenStrike(-1,-1,strike);
    //rechts unten
    strike = queenStrike(1,-1,strike);

    //TODO Nicht schlagende Zuege, wenn eine Dame nicht schlagen kann

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

    //prepare and start queenMove
    if(currentField[x][y] == 'W'){
        char enemyColour[2];
        enemyColour[0] = 'b';
        enemyColour[1] = 'B';
        char ownColour = 'W';
        currentMove=queenMove(x, y, bestMove, geschlagen, moveBisher, currentField, enemyColour, ownColour);
        moveBisher=currentMove.zug;
        goto ZUGBEENDEN;
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

struct moeglicherZug possibleMovesBlack(int x, int y, struct moeglicherZug bestMove, int geschlagen, char* moveBisher, char field[8][8]) {

  printf("Start possibleMovesBlack\n");

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


  //else
  //{
    //prepare and start queenMove
    if(currentField[x][y] == 'B'){
        char enemyColour[2];
        enemyColour[0] = 'w';
        enemyColour[1] = 'W';
        char ownColour = 'B';
        printField(currentField);
        currentMove=queenMove(x, y, bestMove, geschlagen, moveBisher, currentField, enemyColour, ownColour);
        moveBisher=currentMove.zug;
        
        goto ZUGBEENDEN;
        }
  //}

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
          //printf("Spielstein: %c\n", spielStein);
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
