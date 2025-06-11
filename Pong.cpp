// Tetris.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib.h"

Color Light_Blue = Color{ 69, 114, 147, 255 };
Color Blue = Color{ 25, 53, 122, 255 };
Color Dark_Blue = Color{ 17, 17, 80, 255 };
Color Yellow = Color{ 243, 213, 31, 255 };

int player_score = 0;
int ai_score = 0;

class Ball
{
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw()
    {
        DrawCircle(x, y, radius, Yellow);
    }

    void Update()
    {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
        }
        if (x + radius >= GetScreenWidth() )
        {
            ai_score++;
            ResetBall();
        }
        if (x - radius <= 0)
        {
            player_score++;
            ResetBall();
        }
    }

    void ResetBall()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_schoices[2] = {-1, 1};
        speed_x *= speed_schoices[GetRandomValue(0, 1)];
        speed_y *= speed_schoices[GetRandomValue(0, 1)];

    }
};

class Paddle
{
protected:
    void LimitMovement()
    {
        if (y <= 0)
        {
            y = 0;
        }
        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }
public:
    float x, y;
    float width, height;
    int speed;

    void Draw()
    {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }
    void Update()
    {
        if (IsKeyDown(KEY_UP))
        {
            y = y - speed;
        }
        else if (IsKeyDown(KEY_DOWN))
        {
            y = y + speed;
        }
        LimitMovement();
    }
};

class AIPaddle : public Paddle
{
public:
    void Update(int ball_y)
    {
        if (y + height / 2 > ball_y)
        {
            y = y - speed;
        }
        if (y + height / 2 <= ball_y)
        {
            y = y + speed;
        }
        LimitMovement();
    }
};

Ball ball;
Paddle player;
AIPaddle ai;

int main()
{
    std::cout << "Hello World!\n";

    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");
    SetTargetFPS(60);
    
    ball.radius = 20;
    ball.x = SCREEN_WIDTH / 2;
    ball.y = SCREEN_HEIGHT / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 120;
    player.x = SCREEN_WIDTH - player.width - 10;
    player.y = SCREEN_HEIGHT / 2 - player.height / 2;
    player.speed = 6; 

    ai.width = 25;
    ai.height = 120;
    ai.x = 10;
    ai.y = SCREEN_HEIGHT / 2 - ai.height / 2;
    ai.speed = 5;

    while (WindowShouldClose() == false)
    {
        BeginDrawing();

        //Updating
        ball.Update();
        player.Update();
        ai.Update(ball.y);

        // Check for collisions
        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ player.x, player.y, player.width, player.height }))
        {
            ball.speed_x *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ ai.x, ai.y, ai.width, ai.height }))
        {
            ball.speed_x *= -1;
        }

        //Drawing
        ClearBackground(Dark_Blue);
        DrawRectangle(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, Blue);
        DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 150, Light_Blue);
        DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_WIDTH, WHITE);

        ball.Draw();
        ai.Draw();
        player.Draw();
        DrawText(TextFormat("%i", ai_score), SCREEN_WIDTH / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 *SCREEN_WIDTH / 4 - 20, 20, 80, WHITE);

        EndDrawing();
    }
    CloseWindow();
}

