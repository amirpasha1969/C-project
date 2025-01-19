#include "ghosts_movement.h"
#include "base.h"
#include "map.h"
#include "raylib.h"
#include <stdlib.h>

void chaseplayer(int playerX, int playerY, int *ghostX, int *ghostY, float *timer, float delay) 
{
    *timer += GetFrameTime();

    if (*timer < delay) return;

    *timer = 0.0f;

    int dx[] = {0, 1, 0, -1};
    int dy[] = {-1, 0, 1, 0};
    int bestX = *ghostX;
    int bestY = *ghostY;
    int minDist = abs(playerX - *ghostX) + abs(playerY - *ghostY);

    for (int i = 0; i < 4; i++) 
    {
        int newX = *ghostX + dx[i];
        int newY = *ghostY + dy[i];

        if (newX >= 0 && newX < SIZE1 && newY >= 0 && newY < SIZE2 && a[newX][newY] != '#') 
        {
            int dist = abs(playerX - newX) + abs(playerY - newY);
            if (dist < minDist) 
            {
                minDist = dist;
                bestX = newX;
                bestY = newY;
            }
        }
    }

    *ghostX = bestX;
    *ghostY = bestY;
}
void random_move(int *ghostX, int *ghostY, int ghostIndex, int ghostCount, int ghostPositions[][2], char map[SIZE1][SIZE2], float *timer, float delay, int *direction) 
{   
    *timer += GetFrameTime();

    if (*timer < delay) return;

    *timer = 0.0f;
    char previousCells[4] = {'.','.','.','.'};
    static int prevX[4] = {-1, -1, -1, -1};
    static int prevY[4] = {-1, -1, -1, -1};

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    int moved = 0;

    while (!moved) 
    {
        *direction = rand() % 4;
        int newX = *ghostX + dx[*direction];
        int newY = *ghostY + dy[*direction];

        if (newX >= 0 && newX < SIZE1 && newY >= 0 && newY < SIZE2 && map[newX][newY] != '#' && !(newX == prevX[ghostIndex] && newY == prevY[ghostIndex])) 
        {
            map[*ghostX][*ghostY] = previousCells[ghostIndex];
            previousCells[ghostIndex] = map[newX][newY];
            prevX[ghostIndex] = *ghostX;
            prevY[ghostIndex] = *ghostY;
            *ghostX = newX;
            *ghostY = newY;
            ghostPositions[ghostIndex][0] = newX;
            ghostPositions[ghostIndex][1] = newY;
            
            moved = 1;
        }
    }
}
