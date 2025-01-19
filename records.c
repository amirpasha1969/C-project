#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "records.h"

#define MAX_SCORES 10
#define MAX_NAME_LENGTH 256
#define MAX_DATE_LENGTH 64
#define MAX_TOTAL_SCORES 1000

char playername[MAX_NAME_LENGTH] = "";

void bubbleSort(char names[][MAX_NAME_LENGTH], int scores[], char dateTimes[][MAX_DATE_LENGTH], int total) 
{
    for (int i = 0; i < total - 1; i++) 
    {
        for (int j = 0; j < total - i - 1; j++) 
        {
            if (scores[j] < scores[j + 1]) 
            {
                int tempScore = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = tempScore;

                char tempName[MAX_NAME_LENGTH];
                strcpy(tempName, names[j]);
                strcpy(names[j], names[j + 1]);
                strcpy(names[j + 1], tempName);

                char tempDate[MAX_DATE_LENGTH];
                strcpy(tempDate, dateTimes[j]);
                strcpy(dateTimes[j], dateTimes[j + 1]);
                strcpy(dateTimes[j + 1], tempDate);
            }
        }
    }
}

void ManageScores(Font customFont, bool save, int score, Texture2D background, Music menuMusic) 
{
    const char *filename = "scores.txt";
    FILE *file;
    char names[MAX_TOTAL_SCORES][MAX_NAME_LENGTH];
    int scores[MAX_TOTAL_SCORES];
    char dateTimes[MAX_TOTAL_SCORES][MAX_DATE_LENGTH];
    int totalScores = 0;

    if (save) 
    {
        file = fopen(filename, "r");
        if (file) 
        {
            while (fscanf(file, "%255[^:]: %d (%63[^)])\n", names[totalScores], &scores[totalScores], dateTimes[totalScores]) == 3) 
            {
                totalScores++;
                if (totalScores >= MAX_TOTAL_SCORES) break;
            }
            fclose(file);
        }

        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char dateTime[MAX_DATE_LENGTH];
        strftime(dateTime, sizeof(dateTime), "%Y-%m-%d %H:%M:%S", t);

        strncpy(names[totalScores], playername, MAX_NAME_LENGTH - 1);
        names[totalScores][MAX_NAME_LENGTH - 1] = '\0';
        scores[totalScores] = score;
        strncpy(dateTimes[totalScores], dateTime, MAX_DATE_LENGTH - 1);
        dateTimes[totalScores][MAX_DATE_LENGTH - 1] = '\0';
        totalScores++;

        bubbleSort(names, scores, dateTimes, totalScores);

        if (totalScores > MAX_SCORES) 
        {
            totalScores = MAX_SCORES;
        }

        file = fopen(filename, "w");
        if (file) 
        {
            for (int i = 0; i < totalScores; i++) 
            {
                fprintf(file, "%s: %d (%s)\n", names[i], scores[i], dateTimes[i]);
            }
            fclose(file);
        }
    } 
    else 
    {
        file = fopen(filename, "r");
        if (file) 
        {
            while (fscanf(file, "%255[^:]: %d (%63[^)])\n", names[totalScores], &scores[totalScores], dateTimes[totalScores]) == 3) 
            {
                totalScores++;
                if (totalScores >= MAX_SCORES) break;
            }
            fclose(file);
        }

        bubbleSort(names, scores, dateTimes, totalScores);

        while (!WindowShouldClose()) 
        {
            UpdateMusicStream(menuMusic);

            if (IsKeyPressed(KEY_ESCAPE)) 
            {
                break;
            }

            BeginDrawing();
            ClearBackground(BLACK);

            Vector2 position = {700, -100};
            float scale = 0.8f;
            DrawTextureEx(background, position, 0.0f, scale, WHITE);

            DrawTextEx(customFont, "High Scores (Top 10):", (Vector2){50, 50}, 30, 2, WHITE);
            for (int i = 0; i < totalScores; i++) 
            {
                char scoreText[512];
                snprintf(scoreText, sizeof(scoreText), "%d. %s - %d (%s)", 
                         i + 1, names[i], scores[i], dateTimes[i]);
                DrawTextEx(customFont, scoreText, 
                           (Vector2){50, 100 + i * 30}, 20, 2, WHITE);
            }

            DrawTextEx(customFont, "Press ESC to go back.", 
                       (Vector2){50, 500}, 20, 2, GRAY);

            EndDrawing();
        }
    }
}
