#include "shoot.h"
#include "board.h"
#include "ships.h"
#include "human.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isPositionShooted(element_t c) {
    return isEqual(c, ERROR) || isEqual(c, ASSERT);
}

element_t coordinates(element_t board[][TAM], chute_t c) {
  return board[c.x][c.y];
}

chute_t iaChute(element_t board[][TAM]) {
  chute_t chute;

  do {
    chute.x = rand() % TAM;
    chute.y = rand() % TAM;
  } while(isPositionShooted(coordinates(board, chute)));

  return chute;
}

int isShipDestroyed(element_t board[][TAM], element_t e, chute_t c) {
  int tam = e.tam;
  chute_t shipStart = {
    c.x,
    c.y
  }; //coordenada onde começa o navio;

  //TODO: improve this conditionals (receive the array of ships)
  if(e.type == 'S') {
    if(e.dir == 'H')
      shipStart.y = c.y - (SUBMARIN.tam - tam);
    else
      shipStart.x = c.x - (SUBMARIN.tam - tam);

    tam = SUBMARIN.tam;
  }
  else if(e.type == 'D') {
    if(e.dir == 'H')
      shipStart.y = c.y - (DESTROYER.tam - tam);
    else
      shipStart.x = c.x - (DESTROYER.tam - tam);

    tam = DESTROYER.tam;
  }
  else if(e.type == 'T') {
    if(e.dir == 'H')
      shipStart.y = c.y - (TANKER.tam - tam);
    else
      shipStart.x = c.x - (TANKER.tam - tam);

    tam = TANKER.tam;
  }
  else if(e.type == 'A') {
    if(e.dir == 'H')
      shipStart.y = c.y - (AIRCRAFT.tam - tam);
    else
      shipStart.x = c.x - (AIRCRAFT.tam - tam);

    tam = AIRCRAFT.tam;
  }

  for(int i = 0; i < tam; i++) {
    if(e.dir == 'H') {
      if(isEqual(board[shipStart.x][shipStart.y+i], e)) {
        return 0;
      }
    }
    else {
      if(isEqual(board[shipStart.x+i][shipStart.y], e)) {
        return 0;
      }
    }
  }
  return 1;
}

int shoot(element_t board[][TAM], const char* player) {
    chute_t s;
    if(strcmp(player, "ia") == 0) { 
      s = iaChute(board);
    }
    else {
      s = humanShoot(board);
    }

    element_t result = coordinates(board, s);

    if(isEqual(result, WATER)) {
        printf("\nAGUA!!\n\n");
        assign(&board[s.x][s.y], ERROR);
    }

    else {
        printf("\nBOMBA!!\n\n");
        assign(&board[s.x][s.y], ASSERT); 
        
        if(isShipDestroyed(board, result, s)) {
          return 1;
        }
    }

    return 0;
}
