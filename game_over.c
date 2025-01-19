#include <stdio.h>
#include "raylib.h"
#include <stdbool.h>
#include <unistd.h>
#include "game_over.h"
#include "records.h"

bool GameOverScreen(int screenWidth, int screenHeight, void (*DrawGameState)(), int score) 
{
    float alpha = 0.0f;
    Texture2D endImage = LoadTexture("../textures/end_game_screen.jpg");
    Font customFont = LoadFont("../font/Orbitron-ExtraBold.ttf");
    Music gameOverMusic = LoadMusicStream("../musics/Rei I.mp3");
    
    double startTime = GetTime();
    while (GetTime() - startTime < 3.0) 
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawGameState();
        EndDrawing();
    }

    while (alpha < 1.0f) 
    {
        alpha += 0.005f;
        if (alpha > 1.0f) alpha = 1.0f;

        BeginDrawing();
        ClearBackground(BLACK);
        DrawGameState();
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, alpha));
        EndDrawing();
    }

    usleep(500000);
    PlayMusicStream(gameOverMusic);
    alpha = 1.0f;
    while (alpha > 0.0f) 
    {
        UpdateMusicStream(gameOverMusic);
        alpha -= 0.005f;
        if (alpha < 0.0f) alpha = 0.0f;

        BeginDrawing();
        ClearBackground(BLACK);
        Vector2 position = {(screenWidth - endImage.width) / 2, (screenHeight - endImage.height) / 2};
        DrawTexture(endImage, position.x, position.y, Fade(WHITE, 1.0f - alpha));
        EndDrawing();
    }

    ManageScores(customFont, true, score, endImage, gameOverMusic);

    while (true) 
    {
        UpdateMusicStream(gameOverMusic);

        if (IsKeyPressed(KEY_ESCAPE)) 
        {
            StopMusicStream(gameOverMusic);
            UnloadMusicStream(gameOverMusic);
            UnloadTexture(endImage);
            UnloadFont(customFont);
            return true;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        Vector2 position = {(screenWidth - endImage.width) / 2, (screenHeight - endImage.height) / 2};
        DrawTexture(endImage, position.x, position.y, WHITE);

        DrawTextEx(
            customFont,
            "GAME OVER",
            (Vector2){ (float)( 600 + screenWidth - MeasureTextEx(customFont, "GAME OVER", 80, 2).x) / 2, screenHeight / 2 - 150 },
            80, 
            2,
            RED
        );

        DrawTextEx(
            customFont,
            TextFormat("Player: %s", playername),
            (Vector2){ (float)(800 + screenWidth - MeasureTextEx(customFont, TextFormat("Player: %s", playername), 40, 2).x) / 2, screenHeight / 2 - 50 },
            40, 
            2,
            WHITE
        );

        DrawTextEx(
            customFont,
            TextFormat("Score: %d", score),
            (Vector2){ (float)(800 + screenWidth - MeasureTextEx(customFont, TextFormat("Score: %d", score), 40, 2).x) / 2, screenHeight / 2 },
            40, 
            2,
            YELLOW
        );

        DrawTextEx(
            customFont,
            "Press ESC to Exit",
            (Vector2){ (float)(900 + screenWidth - MeasureTextEx(customFont, "Press ESC to Exit", 20, 2).x) / 2, screenHeight / 2 + 90 },
            20, 
            2,
            GRAY
        );

        EndDrawing();
    }
}
