#include "map.h"
#include "stars.h"
#include <stdlib.h>
#include <time.h>
void stars()
{
    srand(time(NULL));
    int count = 0;
    while(count<10)
    {
        int x = rand() % 15;
        int y = rand() % 20;
        if(a[x][y]=='.')
        {
            a[x][y]='*';
            ++count;
        }
    }
}