#include "main.h"

//gibt 1 bei weißen Stein, -1 bei schwarzen Stein, 0 bei leeren Feld(und andere Sachen die nicht auftreten sollten)
int getTeam(char spielstein){
  if(spielstein == 'w' || spielstein == 'W'){
    return 1;
  } else if(spielstein == 'b' || spielstein == 'B'){
    return -1;
  } else {
    return 0;
  }
}

//nimmt einen pointer entgegn, buffer in aufrufender funktion erstellen -> malloc -> am ende freen
char *getCoordinate(int x, int y, char *buffer){

  switch(x){
    case 0:
      strcpy(buffer, "A");
      break;
    case 1:
      strcpy(buffer, "B");
      break;
    case 2:
      strcpy(buffer, "C");
      break;
    case 3:
      strcpy(buffer, "D");
      break;
    case 4:
      strcpy(buffer, "E");
      break;
    case 5:
      strcpy(buffer, "F");
      break;
    case 6:
      strcpy(buffer, "G");
      break;
    case 7:
      strcpy(buffer, "H");
      break;
  }

  switch(y){
    case 0:
      strcat(buffer, "1");
      break;
    case 1:
      strcat(buffer, "2");
      break;
    case 2:
      strcat(buffer, "3");
      break;
    case 3:
      strcat(buffer, "4");
      break;
    case 4:
      strcat(buffer, "5");
      break;
    case 5:
      strcat(buffer, "6");
      break;
    case 6:
      strcat(buffer, "7");
      break;
    case 7:
      strcat(buffer, "8");
      break;
  }

  return buffer;
}



int inBound(int x, int y) {
  if ((x<8)&&(x>-1)&&(y<8)&&(y>-1)) {
    return 1;
  } else {
    return 0;
  }
}


//gibt ein Array mit den beiden gegnerischen Spielsteinen zurück
char * getEnemy (char spielstein)
{
//printf("\n\n\nStarte get Enemy\n\n\n"); 
char * ergebnis = malloc(sizeof(char)*2);
//char ergebnis[2];

  if(spielstein == 'w' || spielstein == 'W'){
    ergebnis[0] = 'b';
    ergebnis[1] = 'B';
  } else if(spielstein == 'b' || spielstein == 'B'){
    ergebnis[0] = 'w';
    ergebnis[1] = 'W';
  } else {
    ergebnis[0] = '*';
    ergebnis[1] = '*';
  }
//printf("getEnemy Input %c\n", spielstein);
//printf("Ergebnis get Enemy: %c %c\n",ergebnis[0], ergebnis[1]);
return ergebnis;
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


//überprüft nach Feinden in gegebene Richtung(rx,ry) an Position (x,y) und gibt bei gefunden und in schlagreichweite 1 zurück, colour gibt den eigenen Spielstein an (Dame/Bauer+Farbe)
//bei gefundenem Fein return 1 sonst 0
int checkEnemy(int rx, int ry, int x, int y, char field [8][8], char colour)
{
  char* buffer = malloc(sizeof(char)*3);
  int ergebnis = 0;
  char *enemy = getEnemy(colour);
  //memcpy(enemy,getEnemy,sizeof(char)*2);
  /*printf("Starte checkEnemy\n");
  //printf("enemy[0]=%c enemy[1]=%c\n",enemy[0],enemy[1]); 
  printf("rx %i ry %i\n", rx, ry);
  printf("Position x=%i y:%i\n", x, y);
  if(inBound(x,y))
    {
      printf("Position getCoordinate: %s\n", getCoordinate(x,y, buffer));
    }
  printf("check Bauer\n");*/

  //check nach Bauer
  if(inBound(x+rx,y+ry) && field[x+rx][y+ry] == enemy[0])
    {
      ergebnis = 1;
      //printf("Bauer gefunden\n");
    }

  //check nach Dame
  else
  {
    //printf("Suche Dame\n");
    for (int distance = 1; distance<8; distance ++){
      int xnew = x+(distance*rx);
      int ynew = y+(distance*ry);

      if(inBound(xnew,ynew))
      {
        if(field[xnew][ynew]== enemy[1])
          {
            ergebnis = 1;
            //printf("Dame gefunden\n");
          }
      }
      else
        {break;}
    }
  }
  //printf("CheckEnemy Finish: %i\n", ergebnis);
  free(enemy);
  free(buffer);
  return ergebnis;
}


//überprüft, ob der Stein an der Stelle (x,y) aus der Richtung (rx,ry) geschlagen werden kann
//input wie bei check Enemy
int strikeable(int rx, int ry, int x, int y, char field [8][8], char colour)
{
  int strikeable = 0;
  
  
  //printf("Strikeable Input: rx=%i ry=%i x=%i y=%i\n", rx, ry, x ,y);
  if(inBound(x+(-1*rx),y+(-1*ry)) && (field[x+(-1*rx)][y+(-1*ry)]=='*' || field[x+(-1*rx)][y+(-1*ry)]==colour))
    {strikeable=1;}
    
  //printf("Strikeable:  %i\n", strikeable);
  return strikeable;
}



//überprüft ob an der nicht schlagbaren Position ein Gegenschlag möglich ist
int checkCounterstrike(int x, int y, char field [8][8], char colour)
{
  int cs = 0;
  char *enemy = getEnemy(colour);
  
  //2*for zum durchgehen der 4 Richtungen
  for(int rx=-1; rx==1; rx+=2)
  {
    for(int ry=-1; ry==1; ry+=2)
    {
      //Figur ist Bauer und schlagbarer Feind auf Distanz 1
      if((colour=='b' || colour == 'w') && (field[x+rx][y+ry]==enemy[0] || (field[x+rx][y+ry]==enemy[1])) && (field[x+(rx*2)][y+(ry*2)]=='*'))
      {
        cs = 1;
      }
      //Figur ist eine Dame
      else if(colour=='B' || colour == 'W')
      {
        //checke auf der Diagonale nach schlagbaren feindlichen Steinen
        for(int distance=1; distance<8; distance ++)
        {
          //ist das zu schlagende Feld und das dahinter noch auf dem Board?
          if(inBound((x+(rx*distance)),(y+(ry*distance))) && inBound((x+(rx*distance)+ry),(y+(ry*distance)+ry)))
          {
            //wenn das Feld leer ist tu nichts und untersuche das nächste
            if(field[x+(rx*distance)][y+(ry*distance)]=='*')
            {}
            //ist das zu schlagende Feld ein Feind und das Feld dahinter frei, wenn ja cs=1 sonst stoppe Schleife
            else if((field[x+(rx*distance)][y+(ry*distance)]==enemy[0] || field[x+(rx*distance)][y+(ry*distance)]==enemy[1]) && field[x+(rx*distance)+rx][y+(ry*distance)+rx]=='*')
              {
                cs=1;
              }
              else
                {break;}
          }
        }
      }
    }
  }
  free(enemy);
  return cs;
}



//berechnet die Gewichtung für die Position(x,y) auf die man ziehen will und gibt diese zurück
// x,y der Position, das aktuelle Feld, Farbe des aktuell ziehenden Steins (wichtig, ob Dame oder Bauer!)
int getWeight(int x, int y, char field[8][8], char colour/*,int xakt, int yakt*/)
{
  printf("################ getWeight START ################\nInput x:%i y:%i Farbe: %c\n", x,y,colour);
  /*char* buffer = malloc(sizeof(char)*3);
  if(inBound(x,y))
    {
      printf("Position getCoordinate: %s\n", getCoordinate(x,y, buffer));
    }
  else
    {printf("Position nicht inBound x=%i y=%i", x, y);}
    
  free(buffer);*/

  //printField(field);
  //Wertungen
  int strike = -100;
  int lastRow = -20;
  int crown = 50;
  int counterStrike = 100;

  int gewichtung = 0;
  //char* enemy [2];
  char *enemy = getEnemy(colour);
  char *ownColour = getEnemy(enemy[0]);

  //überprüfe alle 4 Richtungen nach gegnerischen Steinen und ob sie schlagen können, falls ja erfolgt eine Abwertung
  
  
  printf("checkEnemy+strikeable\n");
  //alle vier Richtungen
  for(int rx=-1; rx==1; rx+=2)
  {
    for(int ry=-1; ry==1; ry+=2)
    {
      if(inBound(x,y))
      {
        //Feind ja
        if(checkEnemy(rx,ry,x,y,field,colour))
        {
          //schlagbar ja -> Abwertung
          if(strikeable(rx,ry,x,y,field,colour))
          {
            printf("Strikeable = true abzug von %i aktuelle Gewichtung %i\n", strike, gewichtung);
            gewichtung+=strike;
          }
          else
          {
            printf("Strikeable = false\n");
            if (checkCounterstrike(x,y,field,colour))
            { 
              gewichtung+= counterStrike;
              printf("Counterstrike possible! Aufwertung um %i aktuelle Gewichtung %i\n", counterStrike, gewichtung);
            }
          }
        }
      }
      else
        {break;}
    }
  }
/*  
  printf("rechts oben\n");
  //rechts oben
  if(checkEnemy(1,1,x,y,field,colour))
  {
    if(strikeable(1,1,x,y,field,colour))
    {
      gewichtung+=strike;
    }
    else if (checkCounterstrike(x,y,field,colour))
    { gewichtung+= counterStrike; }
  }
  printf("links unten\n");
  // links unten
  if(checkEnemy(-1,-1,x,y,field,colour))
  {
    if(strikeable(-1,-1,x,y,field,colour))
    {
      gewichtung+=strike;
    }
  }
  printf("rechts unten\n");
  //rechts unten
  if(checkEnemy(1,-1,x,y,field,colour))
  {
    if(strikeable(1,-1,x,y,field,colour))
    {
      gewichtung+=strike;
    }
  }
*/

  // Abwertung bei verlassen der letzten Reihe
  //???? aktuell nur für Bauern! (weiß nicht ob auch für Dame sinnvoll)
  if(colour=='b'||colour=='B')
  {
    if( y == 7 && colour =='b')
    {
      printf("pwan leaving last row malus von %i aktuelle Gewichtung %i\n", lastRow, gewichtung);
      gewichtung+=lastRow;
    }
  }
  else if(colour=='w'||colour=='W')
  {
    if( y == 0 && colour =='w')
    {
      printf("pawn leaving last row malus von %i aktuelle Gewichtung %i\n", lastRow, gewichtung);
      gewichtung+=lastRow;
    }
  }

  // Aufwertung, wenn Bauer gekrönt wird
  if(colour=='b' && y==0)
  {
    printf("crown bonus von %i aktuelle Gewichtung %i", crown, gewichtung);
    gewichtung+=crown;
  }
  else if(colour=='w'&& y==7)
  {
    printf("crown bonus von %i aktuelle Gewichtung %i", crown, gewichtung);
    gewichtung+=crown;
  }



  // Aufwertung wenn der Zug in die Mittelzone geht
  //überprüft den abstand von der Ziel-Koordinate zu E5
  gewichtung += (4 - abs(4-y));
  gewichtung += (4 - abs(4-x));
  printf("MittenBonus von %i aktuelle Gewichtung: %i\n", ((4 - abs(4-y))+(4 - abs(4-x))), gewichtung);




  // Abwertung, wenn man einen befreundeten Stein schlagbar macht!
  if( (inBound(x+1,y+1) && (field[x+1][y+1] == ownColour[0] || field[x+1][y+1] == ownColour[1]) && checkEnemy(1,1,x+1,y+1,field,colour)) ||
      (inBound(x+1,y-1) && (field[x+1][y-1] == ownColour[0] || field[x+1][y-1] == ownColour[1]) && checkEnemy(1,-1,x+1,y-1,field,colour)) ||
      (inBound(x-1,y+1) && (field[x-1][y+1] == ownColour[0] || field[x-1][y+1] == ownColour[1]) && checkEnemy(-1,1,x-1,y+1,field,colour)) ||
      (inBound(x-1,y-1) && (field[x-1][y-1] == ownColour[0] || field[x-1][y-1] == ownColour[1]) && checkEnemy(-1,-1,x-1,y-1,field,colour)) )
  {
    printf("Abwertung, weil befreundeter Stein schlagbar wird %i\n aktuelle Gewichtung: %i", strike, gewichtung);
    gewichtung+=strike;
  }
  
  printf("Berechnete Gewichtung für Position %i:%i - %i\n", (x), (y), gewichtung);
  printf("################ ENDE GET WEIGHT ################\n");
  free(enemy);
  free(ownColour);
  return gewichtung;

}




//rx Richung x: -1 links +1 rechts
//ry Richtung y: -1 unten +1 oben

struct queenData queenStrike(int rx, int ry, struct queenData strike)
{

    //buffer für Koordinaten
    char* buffer = malloc(sizeof(char)*3);
    //printf("Starte queenStrike\n");
    struct queenData temp;
    //temp.bestMove.gewichtung = -666666;
    printf("strike.bestMove.gewichtung: %i\n", strike.bestMove.gewichtung);
    printf("-------------------------Start queen Strike mit dem Feld:------------------------------\n\n");
    printField(strike.field);
    printf("\nInput: rx=%i ry=%i strike.x=%i strike.y=%i\n\n", rx, ry, strike.x, strike.y);


    for (int distance = 1; distance<8; distance ++){
        //printf("Forschleifenr. %i\n", distance);
        int xnew = strike.x+(distance*rx);
        int ynew = strike.y+(distance*ry);

        //zu schlagendes und neues Feld noch innerhalb des Feldes
        if(inBound(xnew, ynew) && inBound(xnew+rx,ynew+ry))
        {
        
            //Prüfung auf eigene Figuren in Schlagrichtung
            if (strike.field[xnew][ynew] != '*' && (strike.field[xnew][ynew] != strike.enemyColour[0] && strike.field[xnew][ynew] != strike.enemyColour[1])) {
                break;
            }
        
        
            //printField(strike.field);
            //printf("\n");
            //ist das zu schlagende Feld ein gegnerisches, dahinter frei und das Feld davor frei oder die eigene Figur
            if((strike.field[xnew][ynew] == strike.enemyColour[0] || strike.field[xnew][ynew] == strike.enemyColour[1]) && strike.field[xnew+rx][ynew+ry] == '*' && (strike.field[xnew-rx][ynew-ry] == strike.ownColour || strike.field[xnew-rx][ynew-ry] == '*'))
                {
                    //printf("Schlag gefunden in Schleifenr. %i\n", distance);
                    strcat(strike.moveATM,getCoordinate(xnew+rx,ynew+ry, buffer));
                    strcat(strike.moveATM, ":");
                    strcpy(strike.bestMove.zug,strike.moveATM);
                    strike.bestMove.gewichtung = strike.bestMove.gewichtung + 1000;
                    strike.field[strike.x][strike.y]      = '*';
                    strike.field[xnew+rx][ynew+ry]  = strike.ownColour;
                    strike.field[xnew][ynew]  = '*';
                    strike.x=xnew+rx;
                    strike.y=ynew+ry;
                    //printField(strike.field);
                    //printf("\n");


                    //rekursiver Aufruf für Folgeschläge
                    //links oben
                    printf("Start der Folgezüge:\nLinks Oben:\n");
                    temp=queenStrike(-1,1,strike);
                    if(temp.bestMove.gewichtung>strike.bestMove.gewichtung)
                    {
                        strike=temp;
                    }


                    //rechts oben
                    printf("FZ rechts oben");
                    temp=queenStrike(1,1,strike);
                    if(temp.bestMove.gewichtung>strike.bestMove.gewichtung)
                    {
                        strike=temp;
                    }

                    //links unten
                    printf("FZ links unten");
                    temp=queenStrike(-1,-1,strike);
                    if(temp.bestMove.gewichtung>strike.bestMove.gewichtung)
                    {
                        strike=temp;
                    }

                    //rechts unten
                     printf("FZ rechts unten");
                    temp=queenStrike(-1,-1,strike);
                    if(temp.bestMove.gewichtung>strike.bestMove.gewichtung)
                    {
                        strike=temp;
                    }

                }

            }


    }
    free(buffer);
    printf("Ende queen Strike mit dem Feld:\n");
    printField(strike.field);
    printf("ENDE-ENDE-ENDE-QUEENSTRIKE-ENDE-ENDE-ENDE\n");
    return strike;
}

//errechnet die bestmögliche Gewichtung für einen nicht schlagenden Dame Zug in eine Richtung
//rx Richtung x (-1,1); ry Richung ry (-1,1); Die dazugehörigen Daten (strike)
struct queenData queenMove(int rx, int ry, struct queenData strike)
{
  //int queenMoveDowngrade = -20;
  printf("\n\n----------------------------------------QueenMove START -------------------------------------------------\n\n");
    
    //wird benötigt, da moveATM hier immer gleich sein muss, da es der Ausgangspunkt für alle 4 Bewegungsmöglichkeiten ist
    char moveTemp[64];
    //printField(strike.field);
    //printf("Position x=%i y=%i Richtung rx=%i ry=%i\n",strike.x,strike.y,rx,ry);
    //buffer für Koordinaten
    char* buffer = malloc(sizeof(char)*3);
    //temporäre Wertung mit unmöglich niedrigem Wert befüllt
    int gewichtungTemp = -500;
    for(int distance = 1; distance<8; distance++)
    {
      //ist das angegeben Feld noch auf dem Board? && ist es leer? -> Abbruch bei nein
      if(inBound(strike.x+(distance*rx),strike.y+(distance*ry)) && strike.field[strike.x+(distance*rx)][strike.y+(distance*ry)] == '*')
      {
        //berechne gewichtung für das Feld
        //printf("Berechne Gewichtung für das Feld [%i][%i] mit Inhalt %c\n",strike.x+(distance*rx),strike.y+(distance*ry), strike.field[strike.x+(distance*rx)][strike.y+(distance*ry)]);
        gewichtungTemp = getWeight(strike.x+(distance*rx),strike.y+(distance*ry),strike.field,strike.field[strike.x][strike.y]); //+ queenMoveDowngrade;
        //checke, ob das Feld/der Move besser ist
        if(strike.bestMove.gewichtung<gewichtungTemp)
        {
          //wenn das Feld frei ist speicher es als bestes Feld sonst brich die Schleife ab
          strcpy(moveTemp, strike.moveATM);
          strcat(moveTemp, getCoordinate(strike.x+(distance*rx),strike.y+(distance*ry),buffer));
          //strcat(strike.moveATM, getCoordinate(strike.x-distance,strike.y+distance, buffer));
          strcat(moveTemp, ":");
          //printf("MoveTemp %s in for-Schleifenr. %i\n",moveTemp, distance);
         }
         
      }
      
      else
        {break;}
    }
    
    //besten Zug auswählen
    if(strike.bestMove.gewichtung<gewichtungTemp)
    {
      strcpy(strike.bestMove.zug, moveTemp);
      strike.bestMove.gewichtung = gewichtungTemp;
    }
    
    free(buffer);
    printf("\n\nQUEENMOVE -ENDE -ENDE- ENDE -ENDE - QUEENMOVE\n\naktuell bester Move: %s mit der Gewichtung %i", strike.bestMove.zug, strike.bestMove.gewichtung);
    return strike;
}



//enemy Colour 1 klein Buchstabe enemy Colour 2 Gossbuchstabe
//ownColour muss in Grossbuchstabe sein

struct moeglicherZug queenMain(int x, int y, struct moeglicherZug bestMove, char field[8][8], char enemyColour[2], char ownColour)
{
    //buffer für Koordinaten
    char* buffer = malloc(sizeof(char)*3);

    printf("Start queenMain\n");
    printField(field);
    //create and fill queenData
    struct queenData strike;
    strike.x = x;
    strike.y = y;
    strike.success = 0;
    strcpy(strike.moveATM, getCoordinate(x,y, buffer));
    strcat(strike.moveATM, ":");
    memcpy(strike.field, field, sizeof(char)*8*8);
    strike.enemyColour[0] = enemyColour[0];
    strike.enemyColour[1] = enemyColour[1];
    strike.ownColour = ownColour;
    strike.bestMove = bestMove;


    //nach Schlagenden Zuegen suchen
    //links oben
    printf("Vor QueenStrike x=%i y=%i\n", strike.x, strike.y);
    //printf("Field vor QueenStrike:         <<<<<--------\n\n");
    //printField(strike.field);
    strike = queenStrike(-1,1,strike);
    //rechts oben
    strike = queenStrike(1,1,strike);
    //links unten
    strike = queenStrike(-1,-1,strike);
    //rechts unten
    strike = queenStrike(1,-1,strike);    
    
    printf("Vor QueenMove x=%i y=%i\n", strike.x, strike.y);
    //printf("Field vor QueenMove:          <<<<<---------\n\n");
    //printField(strike.field);
    //Nicht schlagende Zuege, wenn eine Dame nicht schlagen kann
    //nach links oben gehen, rechts oben, links unten, rechts unten gehen
    strike = queenMove(-1, 1, strike);
    strike = queenMove(1, 1, strike);
    strike = queenMove(-1, -1, strike);
    strike = queenMove(1, -1, strike);
    


    //release variables
    free(buffer);
    return strike.bestMove;
}





//hier werden alle züge einer weißen Figur auf Gültigkeit geprüft
//Übergabewerte aktuelle Position "(x,y)"; der aktuelle beste Zug "bestMove"; hat schon geschlagen "geschlagen"/-1 = nur bewegt;
struct moeglicherZug possibleMovesWhite(int x, int y, struct moeglicherZug bestMove, int geschlagen, char* moveBisher, char field[8][8]) {

  //buffer für Koordinaten
  char* buffer = malloc(sizeof(char)*3);
  char* ergebnis = malloc(sizeof(char)*64);
  strcpy(ergebnis, "");
  
  char currentField[8][8];
  memcpy(currentField, field, sizeof(char)*8*8);
  struct moeglicherZug currentMove, tempMove;
  if(geschlagen){
    currentMove.gewichtung = bestMove.gewichtung;
    tempMove.gewichtung = bestMove.gewichtung;
  } else {
    currentMove.gewichtung = -500;
    tempMove.gewichtung = -500;
  }

  strcat(moveBisher, getCoordinate(x,y, buffer));         //concate move
  strcat(moveBisher, ":");

  strcpy(currentMove.zug, moveBisher);
  strcpy(tempMove.zug, moveBisher);
  //pawn move
  if (currentField[x][y] == 'w') {
    if (inBound(x-2, y+2) && (currentField[x-1][y+1] == 'b' || currentField[x-1][y+1] == 'B' ) && (currentField[x-2][y+2] == '*' && geschlagen != -1)){
      //nach links oben schlagen
      tempMove.gewichtung = tempMove.gewichtung + 1000;
      currentField[x][y]      = '*';
      currentField[x-2][y+2]  = 'w';
      currentField[x-1][y+1]  = '*';
      //rekursiver Aufruf mit temporären Feld
      tempMove = possibleMovesWhite(x-2, y+2, tempMove, 1 , moveBisher, currentField);
      tempMove.gewichtung = tempMove.gewichtung + getWeight(x-2, y+2, currentField, 'w');
      geschlagen = 1;
      //wenn der zurückgegebene Zug besser ist als der bisher beste Zug, currentMove ersetzen

     if(tempMove.gewichtung > currentMove.gewichtung){
        currentMove = tempMove;
     }
     
     //Feld und tempMove resetten, damit die nächste Möglichkeit richtig berechnet/ersetzt werden kann
     memcpy(currentField, field, sizeof(char)*8*8);
      if(geschlagen){
        tempMove.gewichtung = bestMove.gewichtung;
      } else {
        tempMove.gewichtung = -1;
      }
    }


    if (inBound(x+2, y+2) && (currentField[x+1][y+1] == 'b' || currentField[x+1][y+1] == 'B') && (currentField[x+2][y+2] == '*' && geschlagen != -1)){
      //nach rechts oben schlagen
      tempMove.gewichtung = tempMove.gewichtung + 1000;
      currentField[x][y]      = '*';
      currentField[x+2][y+2]  = 'w';
      currentField[x+1][y+1]  = '*';
      //rekursiver Aufruf mit temporären Feld
      tempMove = possibleMovesWhite(x+2, y+2, tempMove, 1 , moveBisher, currentField);
      tempMove.gewichtung = tempMove.gewichtung + getWeight(x+2, y+2, currentField, 'w');
      geschlagen = 1;
      //wenn der zurückgegebene Zug besser ist als der bisher beste Zug, currentMove ersetzen

     if(tempMove.gewichtung > currentMove.gewichtung){
        currentMove = tempMove;
     }
     
     //Feld und tempMove resetten, damit die nächste Möglichkeit richtig berechnet/ersetzt werden kann
     memcpy(currentField, field, sizeof(char)*8*8);
      if(geschlagen){
        tempMove.gewichtung = bestMove.gewichtung;
      } else {
        tempMove.gewichtung = -1;
      }
    }
    
    
    if (inBound(x-1, y+1) && (currentField[x-1][y+1] == '*') && (geschlagen == 0)) {
      //nach links oben bewegen
      currentField[x][y]      = '*';
      currentField[x-1][y+1]  = 'w';
      tempMove.gewichtung = tempMove.gewichtung + getWeight(x-1, y+1, currentField, 'w');
      tempMove = possibleMovesWhite(x-1, y+1, tempMove, -1 , moveBisher, currentField);
      geschlagen = -1;
      //printf("moveBewegtLinks: %s\n", moveBisher);
      //wenn der zurückgegebene Zug besser ist als der bisher beste Zug, currentMove ersetzen

     if(tempMove.gewichtung > currentMove.gewichtung){
        currentMove = tempMove;
     }
     
     //Feld und tempMove resetten, damit die nächste Möglichkeit richtig berechnet/ersetzt werden kann
     memcpy(currentField, field, sizeof(char)*8*8);
      if(geschlagen){
        tempMove.gewichtung = bestMove.gewichtung;
      } else {
        tempMove.gewichtung = -1;
      }
    }
    
    if (inBound(x+1, y+1) && (currentField[x+1][y+1] == '*') && (geschlagen == 0)) {
      //nach rechts oben bewegen
      currentField[x][y]      = '*';
      currentField[x+1][y+1]  = 'w';
      tempMove.gewichtung = tempMove.gewichtung + getWeight(x+1, y+1, currentField, 'w');
      tempMove = possibleMovesWhite(x+1, y+1, tempMove, -1 , moveBisher, currentField);
      geschlagen = -1;
      //printf("moveBewegtRechts: %s\n", moveBisher);
      //wenn der zurückgegebene Zug besser ist als der bisher beste Zug, currentMove ersetzen

     if(tempMove.gewichtung > currentMove.gewichtung){
        currentMove = tempMove;
     }
    }

}

    //prepare and start queenMain
  if(currentField[x][y] == 'W'){
    char enemyColour[2];
    enemyColour[0] = 'b';
    enemyColour[1] = 'B';
    char ownColour = 'W';
    currentMove=queenMain(x, y, bestMove, currentField, enemyColour, ownColour);
    moveBisher=currentMove.zug;
    //goto ZUGBEENDEN;
  }



//  ZUGBEENDEN:
    //moveBisher[(strlen(moveBisher)-1)] = "\0";

    strncat(ergebnis, moveBisher, strlen(moveBisher)-1);
    strcat(ergebnis, "\n");
    strcpy(currentMove.zug, ergebnis);
    free(ergebnis);
    free(buffer);
    //printf("momentaner Zug: %s", currentMove.zug);
    //printf("mit der Gewichtung: %d\n\n", currentMove.gewichtung);
    if (currentMove.gewichtung > bestMove.gewichtung){
      return currentMove;
    } else {
      return bestMove;
    }
}

struct moeglicherZug possibleMovesBlack(int x, int y, struct moeglicherZug bestMove, int geschlagen, char* moveBisher, char field[8][8]) {

  //buffer für Koordinaten
  char* buffer = malloc(sizeof(char)*3);

  char* ergebnis = malloc(sizeof(char)*64);
  strcpy(ergebnis, "");
  char currentField[8][8];
  memcpy(currentField, field, sizeof(char)*8*8);
  struct moeglicherZug currentMove, tempMove;
  if(geschlagen){
    currentMove.gewichtung = bestMove.gewichtung;
    tempMove.gewichtung = bestMove.gewichtung;
  } else {
    currentMove.gewichtung = -500;
    tempMove.gewichtung = -500;
  }

  strcat(moveBisher, getCoordinate(x,y, buffer));         //concate move
  strcat(moveBisher, ":");

  strcpy(currentMove.zug, moveBisher);
  strcpy(tempMove.zug, moveBisher);

  //pawn move
  if (currentField[x][y] == 'b') {
    if (inBound(x-2, y-2) && (currentField[x-1][y-1] == 'w' || currentField[x-1][y-1] == 'W' ) && (currentField[x-2][y-2] == '*' && geschlagen != -1)){
      //nach links unten schlagen
      tempMove.gewichtung = tempMove.gewichtung + 1000;
      currentField[x][y]      = '*';
      currentField[x-2][y-2]  = 'b';
      currentField[x-1][y-1]  = '*';
      //rekursiver Aufruf mit temporären Feld
      tempMove = possibleMovesBlack(x-2, y-2, tempMove, 1 , moveBisher, currentField);
      tempMove.gewichtung = tempMove.gewichtung + getWeight(x-2, y-2, currentField, 'b');
      geschlagen = 1;
      //wenn der zurückgegebene Zug besser ist als der bisher beste Zug, currentMove ersetzen

     if(tempMove.gewichtung > currentMove.gewichtung){
       currentMove = tempMove;
     } 
     
     //Feld und tempMove resetten, damit die nächste Möglichkeit richtig berechnet/ersetzt werden kann
     memcpy(currentField, field, sizeof(char)*8*8);
      if(geschlagen){
        tempMove.gewichtung = bestMove.gewichtung;
      } else {
        tempMove.gewichtung = -1;
      }
    }


    if (inBound(x+2, y-2) && (currentField[x+1][y-1] == 'w' || currentField[x+1][y-1] == 'W') && (currentField[x+2][y-2] == '*' && geschlagen != -1)){
      //nach rechts unten schlagen
      tempMove.gewichtung = tempMove.gewichtung + 1000;
      currentField[x][y]      = '*';
      currentField[x+2][y-2]  = 'b';
      currentField[x+1][y-1]  = '*';
      //rekursiver Aufruf mit temporären Feld
      tempMove = possibleMovesBlack(x+2, y-2, tempMove, 1 , moveBisher, currentField);
      tempMove.gewichtung = tempMove.gewichtung + getWeight(x+2, y-2, currentField, 'b');
      geschlagen = 1;
      //wenn der zurückgegebene Zug besser ist als der bisher beste Zug, currentMove ersetzen
      if(tempMove.gewichtung > currentMove.gewichtung){
         currentMove = tempMove;
      }
      
      //Feld und tempMove resetten, damit die nächste Möglichkeit richtig berechnet/ersetzt werden kann
     memcpy(currentField, field, sizeof(char)*8*8);
      if(geschlagen){
        tempMove.gewichtung = bestMove.gewichtung;
      } else {
        tempMove.gewichtung = -1;
      }
    }


    if (inBound(x-1, y-1) && (currentField[x-1][y-1] == '*') && (geschlagen == 0)) {
      //nach links unten bewegen
      currentField[x][y]      = '*';
      currentField[x-1][y-1]  = 'b';
      //printf("moveBewegtLinks: %s\n", moveBisher);
      tempMove.gewichtung = tempMove.gewichtung + getWeight(x-1, y-1, currentField, 'b');
      tempMove = possibleMovesBlack(x-1, y-1, tempMove, -1 , moveBisher, currentField);
      geschlagen = -1;
      //wenn der zurückgegebene Zug besser ist als der bisher beste Zug, currentMove ersetzen

      if(tempMove.gewichtung > currentMove.gewichtung){
        currentMove = tempMove;
      }
      
      
      //Feld und tempMove resetten, damit die nächste Möglichkeit richtig berechnet/ersetzt werden kann
      memcpy(currentField, field, sizeof(char)*8*8);
      if(geschlagen){
        tempMove.gewichtung = bestMove.gewichtung;
      } else {
        tempMove.gewichtung = -1;
      }
    }
    
    if (inBound(x+1, y-1) && (currentField[x+1][y-1] == '*') && (geschlagen == 0)) {
      //nach rechts unten bewegen
      currentField[x][y]      = '*';
      currentField[x+1][y-1]  = 'b';
      //printf("moveBewegtRechts: %s\n", moveBisher);
      tempMove.gewichtung = tempMove.gewichtung + getWeight(x+1, y-1, currentField, 'b');
      tempMove = possibleMovesBlack(x+1, y-1, tempMove, -1 , moveBisher, currentField);
      geschlagen = -1;
      //wenn der zurückgegebene Zug besser ist als der bisher beste Zug, currentMove ersetzen

     if(tempMove.gewichtung > currentMove.gewichtung){
        currentMove = tempMove;
     }
    }

  }


  if(currentField[x][y] == 'B'){
    char enemyColour[2];
    enemyColour[0] = 'w';
    enemyColour[1] = 'W';
    char ownColour = 'B';
    //printField(currentField);
    currentMove=queenMain(x, y, bestMove, currentField, enemyColour, ownColour);
    moveBisher=currentMove.zug;

        //goto ZUGBEENDEN;
        }
  //}

 // ZUGBEENDEN:
    //moveBisher[(strlen(moveBisher)-1)] = "\0";

    strncat(ergebnis, moveBisher, strlen(moveBisher)-1);
    strcat(ergebnis, "\n");
    strcpy(currentMove.zug, ergebnis);
    free(ergebnis);
    free(buffer);
    //printf("momentaner Zug: %s", currentMove.zug);
    //printf("mit der Gewichtung: %d\n\n", currentMove.gewichtung);
    if (currentMove.gewichtung > bestMove.gewichtung){
      return currentMove;
    } else {
      return bestMove;
    }
}



char* think() {

  struct Spieldaten *spieldaten;
  spieldaten = (struct Spieldaten *) shmat(shmID, NULL, 0);
  char* ergebnis = malloc(sizeof(char)*64);
  strcpy(ergebnis, "");

  struct moeglicherZug spielzug;
  strcpy(spielzug.zug, "");
  spielzug.gewichtung = -500;

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
  shmdt(spieldaten);
  return ergebnis;
}
