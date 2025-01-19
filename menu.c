#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include "base.h"
#include "raylib.h"
#include "records.h"
#include <string.h>

void GetPlayerName(Font customFont, Texture2D background, Music menuMusic);
void fade_loading(Font customfont , Texture2D background , Texture2D loading);
void Settings(Font customFont, Texture2D background, Music menuMusic , Sound menu_button , float *sfxVolume , float *musicVolume , Sound select);

void GetPlayerName(Font customFont, Texture2D background, Music menuMusic)
{
    memset(playername, 0, sizeof(playername));
    int charCount = 0;

    while (!WindowShouldClose()) 
    {
        UpdateMusicStream(menuMusic);

        if (IsKeyPressed(KEY_BACKSPACE) && charCount > 0) 
        {
            playername[--charCount] = '\0';
        }
        else if (charCount < sizeof(playername) - 1) 
        {
            int key = GetCharPressed();
            while (key > 0) 
            {
                if ((key >= 32) && (key <= 125)) 
                {
                    playername[charCount++] = (char)key;
                    playername[charCount] = '\0';
                }
                key = GetCharPressed();
            }
        }

        if (IsKeyPressed(KEY_ENTER) && charCount > 0) 
        {
            break;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        Vector2 position = {0, 0};
        float scale = 0.8f;
        DrawTextureEx(background, position, 0.0f, scale, WHITE);

        DrawTextEx(customFont, "Enter your name:", (Vector2){50, 200}, 20, 2, WHITE);
        DrawTextEx(customFont, playername, (Vector2){50, 240}, 40, 2, RED);
        DrawTextEx(customFont, "Press Enter to confirm", (Vector2){50, 300}, 20, 2, GRAY);

        EndDrawing();
    }
}
void fade_loading(Font customfont , Texture2D background , Texture2D loading)
{
    float alpha = 0.0f;
    while(alpha < 1.0f)
    {
        alpha += 0.01f;
        if (alpha > 1.0f) alpha =1.0f;

        BeginDrawing();
        ClearBackground(BLACK);
        float scale = 0.8f;
        Vector2 position = {0, 0};
        DrawTextureEx(background, position, 0.0f, scale, WHITE);
        DrawTextEx(customfont, "Enter your name:", (Vector2){50, 200}, 20, 2, WHITE);
        DrawTextEx(customfont, playername, (Vector2){50, 240}, 40, 2, RED);
        DrawTextEx(customfont, "Press Enter to confirm", (Vector2){50, 300}, 20, 2, WHITE);
        DrawRectangle(0, 0, 1110, 585, Fade(BLACK, alpha));
        EndDrawing();
    }

    usleep(500000);
    alpha = 1.0f;
    while(alpha > 0.0f)
    {
        alpha -= 0.005f;
        if(alpha < 0.0f) alpha = 0.0f;

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(loading , 0 , 0 , Fade(WHITE, 1.0f - alpha));
        EndDrawing();
    }
}


void Settings(Font customFont, Texture2D background, Music menuMusic , Sound menu_button , float *sfxVolume , float *musicVolume , Sound select)
{
    int settingsChoice = -1;
         
    while (!WindowShouldClose())
    {
        UpdateMusicStream(menuMusic);

        if (IsKeyPressed(KEY_UP)) 
        {
            settingsChoice = (settingsChoice - 1 + 3) % 3;
            PlaySound(menu_button);
        }
        if (IsKeyPressed(KEY_DOWN)) 
        {
            settingsChoice = (settingsChoice + 1) % 3;
            PlaySound(menu_button);
        }

        if (IsKeyPressed(KEY_ENTER)) 
        {
            if (settingsChoice == 0) 
            {
                *musicVolume = (*musicVolume == 1.0f) ? 0.0f : 1.0f;
                SetMusicVolume(menuMusic, *musicVolume);
            }
            else if (settingsChoice == 1) 
            {
                *sfxVolume = (*sfxVolume == 1.0f) ? 0.0f : 1.0f;
                SetSoundVolume(menu_button, *sfxVolume);
                SetSoundVolume(select, *sfxVolume);
            }
            else if (settingsChoice == 2) 
            {
                break; 
            }

            PlaySound(select);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        Vector2 position = {0, 0};
        DrawTextureEx(background, position, 0.0f, 1.0f, WHITE);

        DrawTextEx(customFont , "Setting" , (Vector2){50 , 100} , 50 , 2 , WHITE);

        DrawTextEx(customFont, settingsChoice == 0 ? 
                   (*musicVolume == 1.0f ? "-> Music: ON" : "-> Music: OFF") : 
                   (*musicVolume == 1.0f ? "   Music: ON" : "   Music: OFF"), 
                   (Vector2){50, 200}, 30, 2, ORANGE);

        DrawTextEx(customFont, settingsChoice == 1 ? 
                   (*sfxVolume == 1.0f ? "-> SFX: ON" : "-> SFX: OFF") : 
                   (*sfxVolume == 1.0f ? "   SFX: ON" : "   SFX: OFF"), 
                   (Vector2){50, 250}, 30, 2, ORANGE);

        DrawTextEx(customFont, settingsChoice == 2 ? "-> Back" : "   Back", 
                   (Vector2){50, 300}, 30, 2, ORANGE);

        EndDrawing();
    }
}
int main()
{
    int choice = 0; 
    float musicVolume = 1.0f;
    float sfxVolume = 1.0f;
    const int screenWidth = 1110;
    const int screenHeight = 585;
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(screenWidth, screenHeight, "Pacman");
    InitAudioDevice(); 
    Texture2D background = LoadTexture("../textures/background.jpg"); 
    Music menuMusic = LoadMusicStream("../musics/Neon Genesis Evangelion - Thanatos (Symphony).wav");
    Sound select = LoadSound("../effects/select.wav");
    Sound menu_button = LoadSound("../effects/menu-button.wav");
    Font customFont = LoadFont("../font/Orbitron-ExtraBold.ttf");
    Texture2D loading_texture = LoadTexture("../textures/loading.png");
    Texture2D setting = LoadTexture("../textures/setting.png");
    Texture2D records = LoadTexture("../textures/records.png");
    Texture2D frames[19];
    int q=2;
    for (int i = 0; i < 19; i++) 
    {
        char filename[64];
        sprintf(filename, "../textures/ezgif-7-0f0a390d7e-gif-im/frame_%02d.gif", q);
        frames[i] = LoadTexture(filename);
        ++q;
    }
    PlayMusicStream(menuMusic); 
    int currentFrame = 0;
    float frameTime = 0.07f; 
    float elapsedTime = 0.0f;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {   
        UpdateMusicStream(menuMusic);

        float deltaTime = GetFrameTime();
        elapsedTime += deltaTime;

        if (elapsedTime >= frameTime) 
        {
            currentFrame++;
            if (currentFrame >= 19) 
            {
                currentFrame = 0;
            }
            elapsedTime = 0.0f;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        Vector2 position = {0, 0};
        float scale = 0.8f; 
        DrawTextureEx(frames[currentFrame] , position , 0.0f , scale , WHITE);
        DrawTextEx(customFont, choice == 0 ? "-> Play" : "   Play", (Vector2){50, 200}, 30, 2, WHITE);
        DrawTextEx(customFont, choice == 1 ? "-> Records" : "   Records", (Vector2){50, 250}, 30, 2, WHITE);
        DrawTextEx(customFont, choice == 2 ? "-> Settings" : "   Settings", (Vector2){50, 300}, 30, 2, WHITE);
        DrawTextEx(customFont, choice == 3 ? "-> Exit" : "   Exit", (Vector2){50, 350}, 30, 2, WHITE);

        if (IsKeyPressed(KEY_UP)) 
        {
            choice = (choice - 1 + 4) % 4;
            PlaySound(menu_button);
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            choice = (choice + 1) % 4;
            PlaySound(menu_button);
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            PlaySound(select);
            if (choice == 0)
            {
                GetPlayerName(customFont , background , menuMusic);
                fade_loading(customFont , background , loading_texture);
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j <= 3; j++)
                    {
                        BeginDrawing();
                        ClearBackground(RAYWHITE);
                        DrawTextureEx(loading_texture, position, 0.0f, 1.0f, WHITE);

                        const char *loadingText = "LOADING";
                        int textWidth = MeasureText(loadingText, 30);
                        DrawTextEx(customFont, loadingText, (Vector2){35, 530}, 45, 2, WHITE);

                        for (int k = 0; k < j; k++) 
                        {
                            DrawTextEx(customFont, ".", (Vector2){100 + textWidth + (k * 10), 530}, 45, 2, WHITE);
                        }

                        EndDrawing();
                        usleep(500000); 
                    }
                }

                StopMusicStream(menuMusic);
                UnloadMusicStream(menuMusic);
                UnloadTexture(background);
                UnloadSound(select);
                UnloadSound(menu_button);
                UnloadFont(customFont);
                UnloadTexture(loading_texture);
                for (int i = 0; i < 19; i++)
                UnloadTexture(frames[i]);
                CloseWindow();
                CloseAudioDevice();
                character_and_ghost_movement();
                SetConfigFlags(FLAG_WINDOW_UNDECORATED);
                InitWindow(screenWidth, screenHeight, "Pacman");
                InitAudioDevice(); 
                background = LoadTexture("D:/pacman/src/textures/background.jpg"); 
                menuMusic = LoadMusicStream("D:/pacman/src/musics/Neon Genesis Evangelion - Thanatos (Symphony).wav");
                select = LoadSound("D:/pacman/src/effects/select.wav");
                menu_button = LoadSound("D:/pacman/src/effects/menu-button.wav");
                customFont = LoadFont("D:/pacman/src/font/Orbitron-ExtraBold.ttf");
                loading_texture = LoadTexture("../textures/loading.png");
                setting = LoadTexture("../textures/setting.png");
                records = LoadTexture("../textures/records.png");
                q=2;
                for (int i = 0; i < 19; i++) 
                {
                    char filename[64];
                    sprintf(filename, "../textures/ezgif-7-0f0a390d7e-gif-im/frame_%02d.gif", q);
                    frames[i] = LoadTexture(filename);
                    ++q;
                }
                PlayMusicStream(menuMusic); 
            }
            else if (choice == 1)
            {
                ManageScores(customFont, false, 0, records, menuMusic);           
            }
            else if (choice == 2)
            {
                Settings(customFont, setting, menuMusic , menu_button , &sfxVolume , &musicVolume , select);
            }
            else if (choice == 3)
            {   
                StopMusicStream(menuMusic); 
                UnloadMusicStream(menuMusic);
                CloseAudioDevice(); 
                UnloadTexture(background);
                for (int i = 0; i < 19; i++)
                UnloadTexture(frames[i]);
                UnloadSound(select);
                UnloadFont(customFont);
                CloseWindow();
                break;
            }
        }

        EndDrawing();
    }

    return 0;
}
