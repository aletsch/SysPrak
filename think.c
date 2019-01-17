#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>

#include "main.h"


int think() {

  int shmID;
  struct Spieldaten *spieldaten;
  shmID = shmget(KEY, SHMSIZE, 0666);
  spieldaten = (struct Spieldaten *) shmat(shmID, NULL, 0);


  switch (atoi(spieldaten.playerNumber)) {
    case 0:     //weiß
      for(int x=0, x<8, x++) {
        for (int y=7, y>=0, y--) {
          if (spieldaten.field[x][y] == ('w' || 'W')) {
            if (possibleMovesWhite() == 1) {
              return 0;
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
int possibleMovesWhite() {
  if (moveFound) {
    //TODO move in die pipe schreiben
    return 1;
  }
  return 0;
}

//hier werden alle Züge einer schwarzen Figur auf Gültigkeit geprüft
int possibleMovesBlack() {

  return 0;
}
