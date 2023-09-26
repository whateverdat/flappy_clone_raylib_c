#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"

int LoadHighScore();
void SaveHighScore(int);

int main(void)
{
    const int screenW = 1600, screenH = 900;
    InitWindow(screenW, screenH, "Avoid Obstacles!");
    
    InitAudioDevice();
    Sound flap = LoadSound("sfx/flap.wav");
    Sound score = LoadSound("sfx/score.wav");
    Sound gameover = LoadSound("sfx/gameover.wav");
    
    Texture2D background = LoadTexture("assets/background.png"), midground = LoadTexture("assets/midground.png"), foreground = LoadTexture("assets/foreground.png");
    Texture2D obstacleTexture = LoadTexture("assets/obstacle.png"), ballTexture = LoadTexture("assets/ball.png");
    float scrollingBack = 0.0f, scrollingMid = 0.0f, scrollingFore = 0.0f;
    
    srand(time(NULL));
    bool gameOver = false;
    int Score = 0, HiScore = LoadHighScore();
    float randH;
    
    Vector2 ballPosition = { (float)screenW / 2, (float)screenH / 2 };
    Rectangle obstacle = { screenW, 300, 100, screenH};
    Rectangle obstacleMirror = { screenW, 0, 100, 100 };
    
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {           
        if (IsKeyDown('E') && ballPosition.y > 0 ) ballPosition.y -= 20.0f;
            
        if (IsKeyPressed('E') && !gameOver) PlaySound(flap);
        
        
        if (obstacle.x == (screenW / 2) - 100 && !gameOver)
        {
            Score++;
            PlaySound(score);
            if (Score > HiScore) HiScore = Score;
        } 
        
        if (ballPosition.y < 900) ballPosition.y += 5.0f;
        
        if (CheckCollisionCircleRec(ballPosition, 50, obstacle) || CheckCollisionCircleRec(ballPosition, 50, obstacleMirror) || ballPosition.y == 900)
        {
            if (!gameOver) PlaySound(gameover);
            gameOver = true;
        }
        
        if (obstacle.x == 0)
        {
            randH = rand() % (screenH - 500) + 300;
            
            obstacle.y = randH;
            obstacleMirror.height = randH - 200;
           
            obstacle.x = screenW;
            obstacleMirror.x = screenW;
        }
        else
        {
            obstacle.x -= 20.0f;
            obstacleMirror.x -= 20.0f;
        }
        
        scrollingBack -= 0.1f;
        scrollingMid -= 0.5f;
        scrollingFore -= 1.0f;
        
        if (scrollingBack <= -background.width*2) scrollingBack = 0;
        if (scrollingMid <= -midground.width*2) scrollingMid = 0;
        if (scrollingFore <= -foreground.width*2) scrollingFore = 0;
        
        BeginDrawing();
        
            ClearBackground(RAYWHITE);
            if (gameOver) 
            {
                Score = 0;
                DrawText("Game is over, press 'R' to restart.", screenW / 2.0 - 200, screenH / 2.0, 20, LIGHTGRAY);
                if (IsKeyPressed('R'))
                {
                    ballPosition.y = (float)screenH / 2;
                    obstacle.x = (float)screenW;
                    obstacleMirror.x = (float)screenW;
                    gameOver = false;
                }
            }
            else 
            {   
                DrawTextureEx(background, (Vector2){ scrollingBack, 20 }, 0.0f, 2.0f, WHITE);
                DrawTextureEx(background, (Vector2){ background.width*2 + scrollingBack, 20 }, 0.0f, 2.0f, WHITE);
                DrawTextureEx(midground, (Vector2){ scrollingMid, 20 }, 0.0f, 2.0f, WHITE);
                DrawTextureEx(midground, (Vector2){ midground.width*2 + scrollingMid, 20 }, 0.0f, 2.0f, WHITE);
                DrawTextureEx(foreground, (Vector2){ scrollingFore, 70 }, 0.0f, 2.0f, WHITE);
                DrawTextureEx(foreground, (Vector2){ foreground.width*2 + scrollingFore, 70 }, 0.0f, 2.0f, WHITE);

                DrawCircleV(ballPosition, 50, MAROON);
                DrawTexturePro(ballTexture, (Rectangle) {0, 0, 256, 256}, (Rectangle){ballPosition.x -50, ballPosition.y -50, 100, 100}, (Vector2){0, 0}, 0.0f, WHITE);
                DrawTextureRec(obstacleTexture, (Rectangle){ 0, 0, obstacle.width, obstacle.height }, (Vector2){ obstacle.x, obstacle.y }, WHITE);
                DrawTextureRec(obstacleTexture, (Rectangle) {0, 0, obstacleMirror.width, obstacleMirror.height}, (Vector2){ obstacleMirror.x, obstacleMirror.y }, WHITE);
                
                DrawText("Press 'E' to make the circle go higher!", 20, 40, 20, WHITE);
                DrawText(TextFormat("%i", Score), screenW / 2, 100, 100, WHITE);
                DrawText(TextFormat("High Score: %i", HiScore), screenW - 200, 40, 25, WHITE);
            }
            
        EndDrawing();
    }
    SaveHighScore(HiScore);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

int LoadHighScore() {
    int highScore = 0;
    FILE *file = fopen("score.dat", "rb");
    if (file != NULL) {
        fread(&highScore, sizeof(int), 1, file);
        fclose(file);
    }
    return highScore;
}

void SaveHighScore(int highScore) {
    FILE *file = fopen("score.dat", "wb");
    if (file != NULL) {
        fwrite(&highScore, sizeof(int), 1, file);
        fclose(file);
    }
}
