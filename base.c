#include "base.h"
#include "map.h"
#include "stars.h"
#include "ghosts_movement.h"
#include "raylib.h"
#include "game_over.h"
#include "records.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

float scale = 0.146 , scale2 = 0.114 , scale3 = 0.08 , scale_star = 0.0699 , scale_enemy1 = 0.17 , scale_enemy2 = 0.09 , scale_berry = 0.074, scale_health = 0.05 , scale_pepper = 0.1188 , scale_apple = 0.074 , scale_mushroom = 0.1 , scale_wall_2 = 0.08;
int x3 , y3 , x2 , y2;
int ghostPositions[4][2];
float ghostTimers[4]={0.0f , 0.0f , 0.0f , 0.0f};
int ghostDirections[4];
int s1_ghosts[4] , s2_ghosts[4];
int count_ghosts_star[4]={0 , 0 , 0 , 0};
int count_ghosts_berry[4]={0 , 0 , 0 , 0};
int count_ghosts_pepper[4]={0, 0 , 0 , 0};
int count_ghosts_apple[4]={0, 0 , 0 , 0};
int count_ghosts_mushroom[4]={0, 0 , 0 , 0};
int point = 0;
int s1 , s2 , count_spawn[5] = {0 , 0 , 0 , 0 , 0};
static int playerLives = 3;
bool mouthopen = true;
bool wallchange = true;
bool whiteeye = true;
bool isimmortal = false;
float immortaltime = 0.0f;
float damagecooldown = 3.0f;
float animationtimer = 0.0f;
float animationtimer1= 0.0f;
const float animationtimer2 = 0.2f;
const float animationtimer_ghost = 0.1f;
float wallanimation = 0.0f;
const float wallanimation1 = 0.2;
float ghostTimer_one = 0.0f;
int cherryX, cherryY;
bool isCherryVisible = false;
float cherrySpawnTimer = 0.0f;
float cherrySpawnInterval = 15.0f;
float powerUpTime = 0.0f; 
const float maxPowerUpTime = 7.4f; 
float movetimer = 0.0f;
float movetimer1 = 0.2f;
const float originalMovetimer1 = 0.2f;
int pepperX, pepperY;
bool isPepperVisible = false;
float pepperSpawnTimer = 0.0f; 
const float pepperSpawnInterval = 10.0f;
bool isSpeedBoostActive = false;
float speedBoostTime = 0.0f;
const float speedBoostDuration = 4.5f; 
const float speedBoostFactor = 0.1f;
int appleX, appleY;
bool isappleVisible = false;
float appleSpawnTimer = 0.0f; 
const float appleSpawnInterval = 30.0f;
int mushroomX, mushroomY;
bool ismushroomVisible = false;
float mushroomSpawnTimer = 0.0f; 
const float mushroomSpawnInterval = 10.0f;
bool isPoweredUp = false , isdead_ghosts[5]={false , false , false , false , false}; 
float ghostspawntime=5.0f;
float ghostspawntime1[5] = {0.0f , 0.0f , 0.0f , 0.0f , 0.0f};

Texture2D evaUp, evaDown, evaLeft, evaRight , evaUpopen , evaDownopen , evaRightopen , evaLeftopen;
Texture2D wall1 , wall2 , path , star , enemy1 , enemy1_white , ghostdown , ghostup , ghostleft , ghostright;
Texture2D health , cherryTexture , pathimmortal , pepper , apple , mushroom , health_1;
Music mainmusic ;
Sound walking , eating_effect , pepper_effect , rage , hurt_1 , hurt_2 , mushroom_effect ,apple_effect;
Font customFont;
void LoadTexturesAndEffects() 
{
    evaUp = LoadTexture("../textures/unit-1 top.png");
    evaDown = LoadTexture("../textures/unit-1 below.png");
    evaLeft = LoadTexture("../textures/unit-1 left.png");
    evaRight = LoadTexture("../textures/unit-1 right.png");
    evaUpopen = LoadTexture("../textures/unit-1 top open.png");
    evaDownopen = LoadTexture("../textures/unit-1 below open.png");
    evaLeftopen = LoadTexture("../textures/unit-1 left open.png");
    evaRightopen = LoadTexture("../textures/unit-1 right open.png");
    wall1 = LoadTexture("../textures/wall 1.png");
    wall2 = LoadTexture("../textures/wall 2.png");
    path = LoadTexture("../textures/path.png");
    star = LoadTexture("../textures/star.png");
    enemy1 = LoadTexture("../textures/chase_ghost.png");
    enemy1_white = LoadTexture("../textures/chase_ghost_white.png");
    ghostup = LoadTexture("../textures/random_ghost_top.png");
    ghostdown = LoadTexture("../textures/random_ghost_below.png");
    ghostleft = LoadTexture("../textures/random_ghost_left.png");
    ghostright = LoadTexture("../textures/random_ghost_right.png");
    health = LoadTexture("../textures/health_1.png");
    cherryTexture = LoadTexture("../textures/cherry.png");
    pathimmortal = LoadTexture("../textures/path.png");
    pepper = LoadTexture ("../textures/pepper.png");
    apple = LoadTexture ("../textures/apple.png");
    mushroom = LoadTexture("../textures/mushroom.png");
    health_1 = LoadTexture("../textures/health_2.png");
    mainmusic = LoadMusicStream("../musics/DECISIVE BATTLE.wav");
    walking = LoadSound("../effects/walking.wav");
    rage = LoadSound("../effects/rage.wav");
    pepper_effect = LoadSound("../effects/pepper.mp3");
    eating_effect = LoadSound("../effects/eating.wav");
    hurt_1 = LoadSound("../effects/hurt(1).wav");
    hurt_2 = LoadSound("../effects/hurt(2).wav");
    mushroom_effect = LoadSound("../effects/mushroom.mp3");
    apple_effect = LoadSound("../effects/apple.mp3");
}

void ResetGameState() 
{
    x3 = y3 = x2 = y2 = 0;
    playerLives = 3;
    point = 0;
    for (int i = 0; i<5; i++) 
    {
        s1_ghosts[i] = s2_ghosts[i] = 0;
        isdead_ghosts[i] = false;
        ghostspawntime1[i] = 0.0f;
    }
    for(int z=0 ; z<4;z++)
    {
        ghostTimers[z] = 0.0f;
        ghostPositions[z][0] = 0;
        ghostPositions[z][1] = 0;
        ghostDirections[z] = 0;
        count_ghosts_star[z] = count_ghosts_berry[z] = count_ghosts_apple[z] = count_ghosts_pepper[z] = 0;
    }
    s1 = s2 = 0;
    count_spawn[1] = count_spawn[2] = count_spawn[3] = count_spawn[0] = count_spawn[4] = 0 ;

    mouthopen = true;
    wallchange = true;
    whiteeye = true;
    isimmortal = false;
    immortaltime = 0.0f;
    animationtimer = 0.0f;
    animationtimer1 = 0.0f;
    cherrySpawnTimer = 0.0f;
    isCherryVisible = false;
    powerUpTime = 0.0f;
    isPoweredUp = false;
    pepperSpawnTimer = 0.0f;
    isPepperVisible = false;
    isSpeedBoostActive = false;
    movetimer1 = originalMovetimer1;
    bool isappleVisible = false;
    float appleSpawnTimer = 0.0f; 
    bool ismushroomVisible = false;
    float mushroomSpawnTimer = 0.0f; 
    const float mushroomSpawnInterval = 10.0f;
    const float appleSpawnInterval = 30.0f;
    const float pepperSpawnInterval = 10.0f;
    const float speedBoostDuration = 4.5f; 
    for (int i = 0; i < SIZE1; i++) 
    {
        for (int j = 0; j < SIZE2; j++) 
        {
            if (a[i][j] == '@' || a[i][j] == '*' || a[i][j] == 'C' || 
                (a[i][j] >= '1' && a[i][j] <= '5') || a[i][j] == 'P' || a[i][j]=='A' || a[i][j]=='M') 
            {
                a[i][j] = '.';
            }
        }
    }
}

void DrawCurrentGameState() 
{
    for (int i = 0; i < SIZE1; i++) 
    {
        for (int j = 0; j < SIZE2; j++) 
        {
            Vector2 position = {j * 57, i * 57};

            if (a[i][j] == '@') {
                DrawTextureEx(evaRight, position, 0.0f, scale, WHITE);
            } 
            else if (a[i][j] == '#') {
                DrawTextureEx(wall1, position, 0.0f, scale2, WHITE);
            } 
            else if (a[i][j] == '*') {
                DrawTextureEx(star, position, 0.0f, scale_star, WHITE);
            } 
            else if (a[i][j] >= '2' && a[i][j] <= '5') 
            {
                int ghostIndex = a[i][j] - '2';
                switch (ghostDirections[ghostIndex]) 
                {
                    case 0:
                        DrawTextureEx(ghostup, position, 0.0f, scale_enemy2, WHITE);
                        break;
                    case 1:
                        DrawTextureEx(ghostdown, position, 0.0f, scale_enemy2, WHITE);
                        break;
                    case 2:
                        DrawTextureEx(ghostleft, position, 0.0f, scale_enemy2, WHITE);
                        break;
                    case 3:
                        DrawTextureEx(ghostright, position, 0.0f, scale_enemy2, WHITE);
                        break;
                }
            } 
            else if (a[i][j] == '1') 
                DrawTextureEx(enemy1, position, 0.0f, scale_enemy1, WHITE);
            else if(a[i][j]== '.')
                DrawTextureEx(path, position, 0.0f, scale3, WHITE);
            else if(a[i][j]=='C')
                DrawTextureEx(cherryTexture, position, 0.0f, scale_berry, WHITE);
            else if(a[i][j]=='P')
                DrawTextureEx(pepper, position, 0.0f,  scale_pepper, WHITE);
            else if(a[i][j]=='A')
                DrawTextureEx(apple, position, 0.0f,  scale_apple, WHITE);
            else if(a[i][j]=='M')
                DrawTextureEx(mushroom, position, 0.0f,  scale_mushroom, WHITE);

        }
    }

    for (int i = 0; i < playerLives; i++) 
    {
        Vector2 position = {10 + i * 60, 810};
        DrawTextureEx(health, position, 0.0f, scale_star, WHITE);
    }

    DrawText(TextFormat("SCORE: %d", point), 10, 10, 20, WHITE);
}

void FreeResources() 
{
    UnloadTexture(evaUp);
    UnloadTexture(evaDown);
    UnloadTexture(evaLeft);
    UnloadTexture(evaRight);
    UnloadTexture(evaUpopen);
    UnloadTexture(evaDownopen);
    UnloadTexture(evaLeftopen);
    UnloadTexture(evaRightopen);
    UnloadTexture(wall1);
    UnloadTexture(wall2);
    UnloadTexture(path);
    UnloadTexture(star);
    UnloadTexture(enemy1);
    UnloadTexture(enemy1_white);
    UnloadTexture(ghostdown);
    UnloadTexture(ghostleft);
    UnloadTexture(ghostright);
    UnloadTexture(ghostup);
    UnloadTexture(health);
    UnloadTexture(cherryTexture);
    UnloadTexture(pathimmortal);
    UnloadTexture(pepper);
    UnloadTexture(apple);
    UnloadTexture(mushroom);
    UnloadMusicStream(mainmusic);
    UnloadSound(walking);
    UnloadSound(eating_effect);
    UnloadSound(rage);
    UnloadSound(pepper_effect);
    UnloadSound(mushroom_effect);
    UnloadSound(apple_effect);
    CloseWindow();
    CloseAudioDevice();
}
void StopMusicAndEffects()
{
    StopMusicStream(mainmusic);
}
void character_and_ghost_movement()
{
    InitWindow( 1140 , 855 , "pacman" );
    InitAudioDevice();
    LoadTexturesAndEffects();
    PlayMusicStream(mainmusic);
    int direction=0;

    stars();

    do 
    {
        x3 = rand() % SIZE1;
        y3 = rand() % SIZE2;
    } 
    while (a[x3][y3] != '.');

    a[x3][y3] = '@';

    do
    {
        x2 = rand() % SIZE1;
        y2 = rand() % SIZE2;
    } 
    while(a[x2][y2]!='.' || a[x2][y2]=='@' || a[x2][y2]=='*');

    a[x2][y2] = '1';

    for (int i = 0; i < 4; i++) 
    {
    ghostPositions[i][0] = rand() % SIZE1;
    ghostPositions[i][1] = rand() % SIZE2;
    while (a[ghostPositions[i][0]][ghostPositions[i][1]] != '.') 
    {
        ghostPositions[i][0] = rand() % SIZE1;
        ghostPositions[i][1] = rand() % SIZE2;
    }
    a[ghostPositions[i][0]][ghostPositions[i][1]] = '2' + i;
    }

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        UpdateMusicStream(mainmusic);
        bool ghostEatenThisFrame = false;
        int e1 = x3, e2 = y3;
        if(IsKeyPressed(KEY_ESCAPE))
        {
            return;
        }
        movetimer = movetimer + GetFrameTime();
        if(movetimer>=movetimer1)
        {
        if(IsKeyDown(KEY_RIGHT) && e2 + 1 < SIZE2 && a[e1][e2 + 1] != '#') 
        {
            if(!isSpeedBoostActive)
            PlaySound(walking);
            if(a[e1][e2 + 1]=='C')
            {
                PlaySound(rage);
                point += 5;
                a[e1][e2+1] = '.';
                isPoweredUp = true;
                powerUpTime = maxPowerUpTime;
                isCherryVisible = false;
            }
            if(a[e1][e2 + 1] == 'P')
            {
                PlaySound(pepper_effect);
                point += 10; 
                a[e1][e2 + 1] = '.';
                isSpeedBoostActive = true;
                speedBoostTime = speedBoostDuration;
                movetimer1 = speedBoostFactor; 
                isPepperVisible = false;
            }
            if(a[e1][e2 + 1] == 'A')
            {
                PlaySound(apple_effect);
                ++playerLives;
                point += 5;
                a[e1][e2 + 1] = '.';
                isappleVisible = false;
            }
            if(a[e1][e2 + 1] == 'M')
            {
                --playerLives;
                if(playerLives != 0)
                PlaySound(mushroom_effect);
                point += 5;
                a[e1][e2 + 1] = '.';
                ismushroomVisible = false;
                isimmortal = true;
                immortaltime = damagecooldown;
            }
            if(a[e1][e2 + 1]== '*')
            {
                point += 5;
            }
            a[e1][e2] = '.';
            e2++;
            direction=3;
            movetimer = 0.0f;
            

        }
        if(IsKeyDown(KEY_LEFT) && e2 - 1 >= 0 && a[e1][e2 - 1] != '#')
        {
            if(!isSpeedBoostActive)
            PlaySound(walking);
            if(a[e1][e2 - 1]=='C')
            {
                PlaySound(rage);
                point += 5;
                a[e1][e2-1] = '.';
                isPoweredUp = true;
                powerUpTime = maxPowerUpTime;
                isCherryVisible = false;
            }
            if(a[e1][e2 - 1] == 'P')
            {
                PlaySound(pepper_effect);
                point += 10; 
                a[e1][e2 - 1] = '.';
                isSpeedBoostActive = true;
                speedBoostTime = speedBoostDuration;
                movetimer1 = speedBoostFactor; 
                isPepperVisible = false;
            }    
            if(a[e1][e2 - 1] == 'A')
            {
                PlaySound(apple_effect);
                ++playerLives;
                point += 5;
                a[e1][e2 - 1] = '.';
                isappleVisible = false;
            }
            if(a[e1][e2 - 1] == 'M')
            {
                --playerLives;
                if(playerLives != 0)
                PlaySound(mushroom_effect);
                point += 5;
                a[e1][e2 - 1] = '.';
                ismushroomVisible = false;
                isimmortal = true;
                immortaltime = damagecooldown;
            }
            if(a[e1][e2 - 1]== '*')
            {
                point += 5;
            }
            a[e1][e2] = '.';
            e2--;
            direction=2;
            movetimer = 0.0f;
        }
        if(IsKeyDown(KEY_DOWN) && e1 + 1 < SIZE1 && a[e1 + 1][e2] != '#') 
        {
            if(!isSpeedBoostActive)
            PlaySound(walking);
            if(a[e1 + 1][e2]=='C')
            {
                PlaySound(rage);
                point += 5;
                a[e1+1][e2] = '.';
                isPoweredUp = true;
                powerUpTime = maxPowerUpTime;
                isCherryVisible = false;
            }
            if(a[e1 + 1][e2] == 'P')
            {
                PlaySound(pepper_effect);
                point += 10; 
                a[e1 + 1][e2] = '.';
                isSpeedBoostActive = true;
                speedBoostTime = speedBoostDuration;
                movetimer1 = speedBoostFactor; 
                isPepperVisible = false;
            }
            if(a[e1 + 1][e2] == 'A')
            {
                PlaySound(apple_effect);
                ++playerLives;
                point += 5;
                a[e1 + 1][e2] = '.';
                isappleVisible = false;
            }
            if(a[e1 + 1][e2] == 'M')
            {
                --playerLives;
                if(playerLives != 0)
                PlaySound(mushroom_effect);
                point += 5;
                a[e1 + 1][e2] = '.';
                ismushroomVisible = false;
                isimmortal = true;
                immortaltime = damagecooldown;
            }
            if(a[e1 - 1][e2]== '*')
            {
                point += 5;
            }
            a[e1][e2] = '.';
            e1++;
            direction=1;
            movetimer = 0.0f;
        }
        if(IsKeyDown(KEY_UP) && e1 - 1 >= 0 && a[e1 - 1][e2] != '#') 
        {
            if(!isSpeedBoostActive)
            PlaySound(walking);
            if(a[e1 - 1][e2]=='C')
            {
                PlaySound(rage);
                point += 5;
                a[e1-1][e2] = '.';
                isPoweredUp = true;
                powerUpTime = maxPowerUpTime;
                isCherryVisible = false;
            }
            if(a[e1 - 1][e2] == 'P')
            {
                PlaySound(pepper_effect);
                point += 10; 
                a[e1 - 1][e2] = '.';
                isSpeedBoostActive = true;
                speedBoostTime = speedBoostDuration;
                movetimer1 = speedBoostFactor; 
                isPepperVisible = false;
            }
            if(a[e1 - 1][e2] == 'A')
            {
                PlaySound(apple_effect);
                ++playerLives;
                point += 5;
                a[e1 - 1][e2] = '.';
                isappleVisible = false;
            }
            if(a[e1 - 1][e2] == 'M')
            {
                --playerLives;
                if(playerLives != 0)
                PlaySound(mushroom_effect);
                point += 5;
                a[e1 - 1][e2] = '.';
                ismushroomVisible = false;
                isimmortal = true;
                immortaltime = damagecooldown;
            }
            if(a[e1 - 1][e2]== '*')
            {
                point += 5;
            }
            a[e1][e2] = '.';
            e1--;
            direction=0;
            movetimer = 0.0f;
        }
        
        }
        a[x2][y2] = '.';
        int q=1;
        for (int i = 0; i < 4; i++) 
        {
            if(!isdead_ghosts[q])
            {
            random_move(&ghostPositions[i][0], &ghostPositions[i][1], i, 4, ghostPositions, a, &ghostTimers[i], 0.4 , &ghostDirections[i]);

            if(count_ghosts_star[i]>0)
            {
                a[s1_ghosts[i]][s2_ghosts[i]] = '*';
                count_ghosts_star[i]--;
            }
            if(count_ghosts_berry[i]>0)
            {
                a[s1_ghosts[i]][s2_ghosts[i]] = 'C';
                count_ghosts_berry[i]--;  
                isCherryVisible = true;              
            }
            if(count_ghosts_pepper[i]>0)
            {
                a[s1_ghosts[i]][s2_ghosts[i]] = 'P';
                count_ghosts_pepper[i]--;
                isPepperVisible = true;
            }
            if(count_ghosts_apple[i]>0)
            {
                a[s1_ghosts[i]][s2_ghosts[i]] = 'A';
                count_ghosts_apple[i]--;
                isappleVisible = true;
            }
            if(count_ghosts_mushroom[i]>0)
            {
                a[s1_ghosts[i]][s2_ghosts[i]] = 'M';
                count_ghosts_mushroom[i]--;
                ismushroomVisible = true;
            }
            if(a[ghostPositions[i][0]][ghostPositions[i][1]] != '*' && a[ghostPositions[i][0]][ghostPositions[i][1]] != 'C' &&
            a[ghostPositions[i][0]][ghostPositions[i][1]] != 'P' && a[ghostPositions[i][0]][ghostPositions[i][1]] != 'A' && a[ghostPositions[i][0]][ghostPositions[i][1]] !='M')
            {
                a[ghostPositions[i][0]][ghostPositions[i][1]] = '2' + i;
            }
            else if(a[ghostPositions[i][0]][ghostPositions[i][1]] == '*')
            {
                s1_ghosts[i] = ghostPositions[i][0];
                s2_ghosts[i] = ghostPositions[i][1];
                a[ghostPositions[i][0]][ghostPositions[i][1]] = '2' + i;
                ++count_ghosts_star[i];
            }
            else if(a[ghostPositions[i][0]][ghostPositions[i][1]] == 'C')
            {
                s1_ghosts[i] = ghostPositions[i][0];
                s2_ghosts[i] = ghostPositions[i][1];
                a[ghostPositions[i][0]][ghostPositions[i][1]] = '2' + i;
                ++count_ghosts_berry[i];
            }
            else if(a[ghostPositions[i][0]][ghostPositions[i][1]] == 'P')
            {
                s1_ghosts[i] = ghostPositions[i][0];
                s2_ghosts[i] = ghostPositions[i][1];
                a[ghostPositions[i][0]][ghostPositions[i][1]] = '2' + i;
                ++count_ghosts_pepper[i];                
            }
            else if(a[ghostPositions[i][0]][ghostPositions[i][1]] == 'A')
            {
                s1_ghosts[i] = ghostPositions[i][0];
                s2_ghosts[i] = ghostPositions[i][1];
                a[ghostPositions[i][0]][ghostPositions[i][1]] = '2' + i;
                ++count_ghosts_apple[i];
            }
            else if(a[ghostPositions[i][0]][ghostPositions[i][1]] == 'M')
            {
                s1_ghosts[i] = ghostPositions[i][0];
                s2_ghosts[i] = ghostPositions[i][1];
                a[ghostPositions[i][0]][ghostPositions[i][1]] = '2' + i;
                ++count_ghosts_mushroom[i];
            }
            }
            else if(isdead_ghosts[q])
            {
                a[ghostPositions[i][0]][ghostPositions[i][1]]='.';
            }
            ++q;
        }

        if (!isdead_ghosts[0]) 
        {
            chaseplayer(e1, e2, &x2, &y2, &ghostTimer_one, 0.3f);
            if (count_spawn[0]>0) 
            {
            a[s1][s2] = '*';
            count_spawn[0]--;
            }
            if(count_spawn[1]>0)
            {
            a[s1][s2] = 'C';
            count_spawn[1]--;
            isCherryVisible = true;
            }
            if(count_spawn[2]>0)
            {
            a[s1][s2] = 'A';
            count_spawn[2]--;
            isappleVisible = true;
            }
            if(count_spawn[3]>0)
            {
            a[s1][s2] = 'P';
            count_spawn[3]--;
            isPepperVisible = true;  
            }
            if(count_spawn[4]>0)
            {
            a[s1][s2] = 'M';
            count_spawn[4]--;
            ismushroomVisible = true;  
            }
            if (a[x2][y2] != '*' && a[x2][y2] != 'C' && a[x2][y2] != 'P' && a[x2][y2] != 'A' && a[x2][y2] != 'M') 
            {
            a[x2][y2] = '1';
            } 
            else if(a[x2][y2] == '*')
            {
            s1 = x2;
            s2 = y2;
            a[x2][y2] = '1';
            count_spawn[0]++;
            }
            else if(a[x2][y2] == 'C')
            {
            s1 = x2;
            s2 = y2;
            a[x2][y2] = '1';
            count_spawn[1]++;
            }
            else if(a[x2][y2] == 'A')
            {
            s1 = x2;
            s2 = y2;
            a[x2][y2] = '1';
            count_spawn[2]++;
            }
            else if(a[x2][y2] == 'P')
            {
            s1 = x2;
            s2 = y2;
            a[x2][y2] = '1';
            count_spawn[3]++;
            }
            else if(a[x2][y2] == 'M')
            {
            s1 = x2;
            s2 = y2;
            a[x2][y2] = '1';
            count_spawn[4]++;
            }
        } 
        else if (isdead_ghosts[0]) 
        {
            a[e1][e2] = '.';
        }

        

        a[e1][e2] = '@';

        x3 = e1;
        y3 = e2;

        animationtimer = animationtimer + GetFrameTime();
        if(animationtimer>=animationtimer2)
        {
            mouthopen = !mouthopen;
            animationtimer=0.0f;
        }
        wallanimation = wallanimation + GetFrameTime();
        if(wallanimation >= wallanimation1)
        {
            wallchange = !wallchange;
            wallanimation = 0.0f;
        }
        animationtimer1 = animationtimer1 + GetFrameTime();
        if(animationtimer1>=animationtimer_ghost)
        {
            whiteeye = !whiteeye;
            animationtimer1=0.0f;
        }
        float frametime = GetFrameTime();
        if(isimmortal)
        {
            immortaltime -=frametime;
            if(immortaltime <= 0.0f)
            isimmortal=false;
        }
        if(!isCherryVisible)
        {
        cherrySpawnTimer += GetFrameTime();
        if(!isCherryVisible && cherrySpawnTimer >= cherrySpawnInterval) 
        {
        do 
        {
        cherryX = rand() % SIZE1;
        cherryY = rand() % SIZE2;
        } while (a[cherryX][cherryY] != '.'); 

        a[cherryX][cherryY] = 'C'; 
        isCherryVisible = true;
        cherrySpawnTimer = 0.0f;
        }
        }
        if(!isPepperVisible)
        {
        pepperSpawnTimer += GetFrameTime();
        if (!isPepperVisible && pepperSpawnTimer >= pepperSpawnInterval) 
        {
            do 
            {
                pepperX = rand() % SIZE1;
                pepperY = rand() % SIZE2;
            } 
            while (a[pepperX][pepperY] != '.'); 

            a[pepperX][pepperY] = 'P';
            isPepperVisible = true;
            pepperSpawnTimer = 0.0f;
        }
        }
        if(isSpeedBoostActive)
        {
            speedBoostTime -= GetFrameTime();
            if(speedBoostTime <= 0.0f)
            {
                isSpeedBoostActive = false;
                movetimer1 = originalMovetimer1;
            }
        }
        if(!isappleVisible)
        {
        appleSpawnTimer += GetFrameTime();
        if (!isappleVisible && appleSpawnTimer >= appleSpawnInterval) 
        {
            do 
            {
                appleX = rand() % SIZE1;
                appleY = rand() % SIZE2;
            } 
            while (a[appleX][appleY] != '.'); 

            a[appleX][appleY] = 'A';
            isappleVisible = true;
            appleSpawnTimer = 0.0f;
        }

        mushroomSpawnTimer += GetFrameTime();
        if (mushroomSpawnTimer >= mushroomSpawnInterval) 
        {
            do 
            {
                mushroomX = rand() % SIZE1;
                mushroomY = rand() % SIZE2;
            } 
            while (a[mushroomX][mushroomY] != '.'); 

            a[mushroomX][mushroomY] = 'M';
            ismushroomVisible = true;
            mushroomSpawnTimer = 0.0f;
        }
        
        }
        int z=1;
        for (int i = 0; i < 4; i++) 
        {
            if (isdead_ghosts[z]) 
            {
                ghostspawntime1[z] += GetFrameTime();
                if (ghostspawntime1[z] >= ghostspawntime) 
                {
                    isdead_ghosts[z] = false;
                    ghostspawntime1[z] = 0.0f;
                    a[ghostPositions[i][0]][ghostPositions[i][1]] = '.';

                    int randomX, randomY;
                    do 
                    {
                    randomX = rand() % SIZE1;
                    randomY = rand() % SIZE2;
                    } while (a[randomX][randomY] != '.');

                    ghostPositions[i][0] = randomX;
                    ghostPositions[i][1] = randomY;

                    a[randomX][randomY] = '2' + i;
                }
            }
            ++z;
        }
        if(isdead_ghosts[0])
        {
            ghostspawntime1[0] += GetFrameTime();
            if(ghostspawntime1[0] >= ghostspawntime)
            {
                isdead_ghosts[0] = false;
                ghostspawntime1[0] = 0.0f;
                a[x2][y2] = '.';
                
                int randomX, randomY;
                do 
                {
                randomX = rand() % SIZE1;
                randomY = rand() % SIZE2;
                } while (a[randomX][randomY] != '.');

                x2 = randomX;
                y2 = randomY;

                a[randomX][randomY] = '1';
            }
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for(int i = 0; i < SIZE1; i++) 
        {
            for(int j = 0; j < SIZE2; j++) 
            {
                if(a[i][j] == '@')
                {
                    if(!isimmortal)
                    {
                    Vector2 position={j*57 , i*57};
                    switch (direction)
                    {
                    case 0:
                            DrawTextureEx(mouthopen ? evaUpopen : evaUp , position , 0.0f , scale , WHITE);
                            break;
                    case 1:
                            DrawTextureEx(mouthopen ? evaDownopen : evaDown , position , 0.0f , scale , WHITE);
                            break;
                    case 2:
                            DrawTextureEx(mouthopen ? evaLeftopen : evaLeft , position , 0.0f , scale , WHITE);
                            break;
                    case 3:
                            DrawTextureEx(mouthopen ? evaRightopen : evaRight , position , 0.0f , scale , WHITE);
                            break;
                    
                    default:
                        break;
                    }
                    }
                    else if(isimmortal)
                    {
                    Vector2 position={j*57 , i*57};
                    switch (direction)
                    {
                    case 0:
                            DrawTextureEx(mouthopen ? pathimmortal : evaUp , position , 0.0f ,mouthopen ? scale3 : scale , WHITE);
                            break;
                    case 1:
                            DrawTextureEx(mouthopen ? pathimmortal : evaDown , position , 0.0f , mouthopen ? scale3 : scale , WHITE);
                            break;
                    case 2:
                            DrawTextureEx(mouthopen ? pathimmortal : evaLeft , position , 0.0f , mouthopen ? scale3 : scale , WHITE);
                            break;
                    case 3:
                            DrawTextureEx(mouthopen ? pathimmortal : evaRight , position , 0.0f , mouthopen ? scale3 : scale , WHITE);
                            break;
                    
                    default:
                        break;
                    }
                    }
                }
                else if (a[i][j] == '#') 
                {
                    Vector2 position = {j*57 , i*57};
                    if(!isPoweredUp)
                    DrawTextureEx( wall1 , position , 0.0f , scale2 , WHITE);
                    if(isPoweredUp)
                    {
                    DrawTextureEx(wallchange ? wall1 : wall2 , position , 0.0f , wallchange ? scale2 : scale_wall_2 , WHITE);
                    }
                }
                else if (a[i][j] == '*') 
                {
                    Vector2 position = {j*57 , i*57};
                    DrawTextureEx(star , position , 0.0f , scale_star , WHITE);
                }
                else if(a[i][j]== '.')
                {
                    Vector2 position = {j*57 , i*57};
                    DrawTextureEx(path , position , 0.0f , scale3 , WHITE);
                }
                else if(a[i][j] == 'P')
                {
                Vector2 Position = {pepperY * 57, pepperX * 57};
                DrawTextureEx(pepper, Position, 0.0f, scale_pepper, WHITE);
                }
                else if(a[i][j] == 'A')
                {
                Vector2 Position = {appleY * 57, appleX * 57};
                DrawTextureEx(apple, Position, 0.0f, scale_apple, WHITE);
                }
                else if(a[i][j] == 'M')
                {
                Vector2 Position = {j * 57, i * 57};
                DrawTextureEx(mushroom, Position, 0.0f, scale_mushroom, WHITE);
                }
                else if(a[i][j] == '1')
                {
                    if(!isdead_ghosts[0])
                    {
                    Vector2 position = {j*57 , i*57};
                    DrawTextureEx(whiteeye ? enemy1 : enemy1_white , position , 0.0f , scale_enemy1 , WHITE);
                    }
                    else if(isdead_ghosts[0])
                    {
                    Vector2 position = {j*57 , i*57};
                    DrawTextureEx(path , position , 0.0f , scale3 , WHITE);
                    }
                }
                else if (a[i][j]=='2')
                {
                if(!isdead_ghosts[1])
                {
                Vector2 position = {j * 57, i * 57};
                if (ghostDirections[0] == 0)
                DrawTextureEx(ghostup, position, 0.0f, scale_enemy2, WHITE);
                else if (ghostDirections[0] == 1)
                DrawTextureEx(ghostdown, position, 0.0f, scale_enemy2, WHITE);
                else if (ghostDirections[0] == 2)
                DrawTextureEx(ghostleft, position, 0.0f, scale_enemy2, WHITE);
                else if (ghostDirections[0] == 3)
                DrawTextureEx(ghostright, position, 0.0f, scale_enemy2, WHITE);
                }
                else if(isdead_ghosts[1])
                {
                    Vector2 position = {j*57 , i*57};
                    DrawTextureEx(path , position , 0.0f , scale3 , WHITE);
                }
                }
                else if(a[i][j]=='3')
                {
                if(!isdead_ghosts[2])
                {
                Vector2 position = {j * 57, i * 57};
                if (ghostDirections[1] == 0)
                DrawTextureEx(ghostup, position, 0.0f, scale_enemy2, WHITE);
                else if (ghostDirections[1] == 1)
                DrawTextureEx(ghostdown, position, 0.0f, scale_enemy2, WHITE);
                else if (ghostDirections[1] == 2)
                DrawTextureEx(ghostleft, position, 0.0f, scale_enemy2, WHITE);
                else if (ghostDirections[1] == 3)
                DrawTextureEx(ghostright, position, 0.0f, scale_enemy2, WHITE);  
                }
                else if(isdead_ghosts[2])
                {
                    Vector2 position = {j*57 , i*57};
                    DrawTextureEx(path , position , 0.0f , scale3 , WHITE);
                }  
                }
                else if(a[i][j]=='4')
                {
                if(!isdead_ghosts[3])
                {
                Vector2 position = {j * 57, i * 57};
                if (ghostDirections[2] == 0)
                DrawTextureEx(ghostup, position, 0.0f, scale_enemy2, WHITE);
                else if (ghostDirections[2] == 1)
                DrawTextureEx(ghostdown, position, 0.0f, scale_enemy2, WHITE);
                else if (ghostDirections[2] == 2)
                DrawTextureEx(ghostleft, position, 0.0f, scale_enemy2, WHITE);
                else if (ghostDirections[2] == 3)
                DrawTextureEx(ghostright, position, 0.0f, scale_enemy2, WHITE);
                }
                else if(isdead_ghosts[3])
                {
                    Vector2 position = {j*57 , i*57};
                    DrawTextureEx(path , position , 0.0f , scale3 , WHITE);
                }     
                }
                else if(a[i][j]=='5')
                {
                if(!isdead_ghosts[4])
                {
                Vector2 position = {j * 57, i * 57};
                if (ghostDirections[3] == 0)
                DrawTextureEx(ghostup, position, 0.0f, scale_enemy2, WHITE);
                else if (ghostDirections[3] == 1)
                DrawTextureEx(ghostdown, position, 0.0f, scale_enemy2, WHITE);
                else if (ghostDirections[3] == 2)
                DrawTextureEx(ghostleft, position, 0.0f, scale_enemy2, WHITE);
                else if (ghostDirections[3] == 3)
                DrawTextureEx(ghostright, position, 0.0f, scale_enemy2, WHITE); 
                }
                else if(isdead_ghosts[4])
                {
                    Vector2 position = {j*57 , i*57};
                    DrawTextureEx(path , position , 0.0f , scale3 , WHITE);
                }
                }
            }
        }
        if (isCherryVisible) 
        {
        Vector2 cherryPosition = {cherryY * 57, cherryX * 57};
        DrawTextureEx(cherryTexture, cherryPosition, 0.0f, scale_berry , WHITE);
        }
        for (int i = 0; i < playerLives; i++) 
        {
            Vector2 position = { 10 + i * 60, 810 }; 
            if(!isPoweredUp)
            DrawTextureEx(health, position, 0.0f, scale_berry, WHITE); 
            else if(isPoweredUp)
            DrawTextureEx(wallchange ? health : health_1, position, 0.0f, scale_berry, WHITE);
        }

        DrawText(TextFormat("SCORE: %d", point), 10, 10, 20, WHITE);

        int codition=0;
            for(int i=0;i<SIZE1;i++)
            {
                for(int j=0;j<SIZE2;j++)
                {
                    if(a[i][j]=='*')
                    ++codition;
                }
            }
            if(codition==0)
            {
                stars();
                for(int i=0;i<SIZE1;i++)
                {
                    for(int j=0;j<SIZE2;j++)
                    {
                        if(a[i][j]=='*')
                        {
                        Vector2 position ={j*57 , i*57};
                        DrawTextureEx(star , position , 0.0f , scale_star , WHITE);
                        }
                    }
                }
            }
            if(!isPoweredUp)
            {
            for(int i = 0; i < SIZE1; i++) 
            {
                for(int j = 0; j < SIZE2; j++) 
                {
                    if(a[i][j] == '@') 
                    {
                    if((i + 1 < SIZE1 && (a[i + 1][j] == '1' || a[i + 1][j] == '2' || a[i + 1][j] == '3' || a[i + 1][j] == '4' || a[i + 1][j] == '5')) ||
                    (i - 1 >= 0 && (a[i - 1][j] == '1' || a[i - 1][j] == '2' || a[i - 1][j] == '3' || a[i - 1][j] == '4' || a[i - 1][j] == '5')) ||
                    (j + 1 < SIZE2 && (a[i][j + 1] == '1' || a[i][j + 1] == '2' || a[i][j + 1] == '3' || a[i][j + 1] == '4' || a[i][j + 1] == '5')) ||
                    (j - 1 >= 0 && (a[i][j - 1] == '1' || a[i][j - 1] == '2' || a[i][j - 1] == '3' || a[i][j - 1] == '4' || a[i][j - 1] == '5')))
                    if(!isimmortal)
                    {
                        --playerLives;
                        if(playerLives!=0)
                        PlaySound(hurt_1);
                        isimmortal = true;
                        immortaltime = damagecooldown;
                    }
                    }
                }
            }
            }
        if(isPoweredUp)
        {
            for(int i=0;i<SIZE1;i++)
            {
                for(int j=0;j<SIZE2;j++)
                {
                    if(a[i][j] == '@') 
                    {
                        if(a[i+1][j]=='1' || a[i-1][j]=='1' || a[i][j+1]=='1' || a[i][j-1]=='1')
                        {
                            PlaySound(eating_effect);
                            isdead_ghosts[0] = true; 
                            point += 150;
                            a[x2][y2] ='.';
                            break;
                        }
                        else if(a[i+1][j]=='2' || a[i-1][j]=='2' || a[i][j+1]=='2' || a[i][j-1]=='2')
                        {
                            PlaySound(eating_effect);
                            isdead_ghosts[1]=true;
                            point += 100;
                            break;
                        }
                        else if(a[i+1][j]=='3' || a[i-1][j]=='3' || a[i][j+1]=='3' || a[i][j-1]=='3')
                        {
                            PlaySound(eating_effect);
                            isdead_ghosts[2]=true;
                            point += 100;
                            break;
                        }
                        else if(a[i+1][j]=='4' || a[i-1][j]=='4' || a[i][j+1]=='4' || a[i][j-1]=='4')
                        {
                            PlaySound(eating_effect);
                            isdead_ghosts[3]=true; 
                            point += 100; 
                            break; 
                        }
                        else if(a[i+1][j]=='5' || a[i-1][j]=='5' || a[i][j+1]=='5' || a[i][j-1]=='5')
                        {
                            PlaySound(eating_effect);
                            isdead_ghosts[4]=true;  
                            point += 100; 
                            break;
                        }
                    }
                }
            }
        }
        if(playerLives==0)
        {
            PlaySound(hurt_2);
            if(GameOverScreen(1140, 855 , DrawCurrentGameState , point))
            {
            ManageScores(customFont, true, point, path, mainmusic);
            StopMusicAndEffects();
            ResetGameState();
            FreeResources();
            return;
            }
        }  
        if (isPoweredUp) 
        {
            powerUpTime -= GetFrameTime();
            if(powerUpTime <= 0.0f) 
            {
            isPoweredUp = false;
            }
        }
   
        EndDrawing();

    }
    UnloadFont(customFont);
    FreeResources();
}
