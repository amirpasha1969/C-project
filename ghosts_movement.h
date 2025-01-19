#ifndef GHOSTS_MOVEMENT_H
#define GHOSTS_MOVEMENT_H
#include "map.h"

void chaseplayer(int playerX, int playerY, int *ghostX, int *ghostY, float *timer, float delay);
void random_move(int *ghostX, int *ghostY, int ghostIndex, int ghostCount, int ghostPositions[][2], char map[SIZE1][SIZE2], float *timer, float delay, int *direction); 


#endif