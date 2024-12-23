#include <raylib.h>
#include <iostream>
using namespace std;

Color Dark_Blue = Color{0, 255, 255, 255};
Color Light_Blue = Color{0, 191, 255, 255};
Color Yellow = Color{243, 213, 91, 255};

int player1_score = 0;
int player2_score = 0;
int cpu_score = 0;

class Ball {
 public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, ORANGE);
    }

    void Update() {
        x += speed_x;
        y += speed_y;


        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        }

        // Player 2 or CPU scores
        if (x + radius >= GetScreenWidth()) {
            cpu_score++;
            ResetBall();
        }

        // Player 1 scores
        if (x - radius <= 0) {
            player1_score++;
            ResetBall();
        }
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle {
 protected:
    void LimitMovement() {
        if (y <= 0) {
            y = 0;
        }
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

 public:
    float x, y;
    float width, height;
    int speed;

    void Draw() {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, RED);
    }
    void Draw1(){
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, BLACK);
    }

    void Update(bool isPlayer2 = false) {
        if (!isPlayer2) {
            if (IsKeyDown(KEY_UP)) y -= speed;
            if (IsKeyDown(KEY_DOWN)) y += speed;
        } else {
            if (IsKeyDown(KEY_W)) y -= speed;
            if (IsKeyDown(KEY_S)) y += speed;
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle {
 public:
    void Update(int ball_y) {
        if (y + height / 2 > ball_y) y -= speed;
        if (y + height / 2 <= ball_y) y += speed;
        LimitMovement();
    }
};

// Global objects
Ball ball;
Paddle player1, player2;
CpuPaddle cpu;

// Function declarations
void DrawMenu(int selection);
void PlayGame(bool versusCpu);

int main() {
    const int screen_width = 1280;
    const int screen_height = 800;

    InitWindow(screen_width, screen_height, "Pong - Group 10");
    SetTargetFPS(60);

    int menu_selection = 0;

    // Menu loop
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_UP)) menu_selection = (menu_selection - 1 + 3) % 3;
        if (IsKeyPressed(KEY_DOWN)) menu_selection = (menu_selection + 1) % 3;

        if (IsKeyPressed(KEY_ENTER)) {
            if (menu_selection == 0) {
                PlayGame(true);  // Play versus CPU
            } else if (menu_selection == 1) {
                PlayGame(false);  // 2 Players mode
            } else if (menu_selection == 2) {
                break;  // Exit
            }
        }

        BeginDrawing();
        ClearBackground(Dark_Blue);
        DrawMenu(menu_selection);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void DrawMenu(int selection) {
    const int fontSize = 40;
    const char* menuItems[] = {"Play versus CPU", "2 Players", "Exit"};
    ClearBackground(Dark_Blue);
    for (int i = 0; i < 3; i++) {
        Color color = (i == selection) ? YELLOW : WHITE;
        DrawText(menuItems[i], GetScreenWidth() / 2 - MeasureText(menuItems[i], fontSize) / 2,
                 GetScreenHeight() / 2 - 100 + i * 50, fontSize, color);
    }
}


void DrawGameOver(bool player1Wins, bool versusCpu) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Dark_Blue);

        const char* winnerText = player1Wins ? "PLAYER 1 WINS!" : (versusCpu ? "CPU WINS!" : "PLAYER 2 WINS!");
        DrawText(winnerText, GetScreenWidth() / 2 - MeasureText(winnerText, 50) / 2,
                 GetScreenHeight() / 2 - 100, 50, YELLOW);

        const char* returnText = "Press ENTER to return to the menu.";
        DrawText(returnText, GetScreenWidth() / 2 - MeasureText(returnText, 30) / 2,
                 GetScreenHeight() / 2, 30, WHITE);

        EndDrawing();

        if (IsKeyPressed(KEY_ENTER)) {
            break;
        }
    }
}

void PlayGame(bool versusCpu) {
const int WINNING_SCORE = 10;


    // Ball setup
    ball.radius = 20;
    ball.x = GetScreenWidth() / 2;
    ball.y = GetScreenHeight() / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    // Player 1 setup
    player1.width = 25;
    player1.height = 120;
    player1.x = GetScreenWidth() - player1.width - 10;
    player1.y = GetScreenHeight() / 2 - player1.height / 2;
    player1.speed = 6;

    // Player 2 or CPU setup
    if (versusCpu) {
        cpu.height = 120;
        cpu.width = 25;
        cpu.x = 10;
        cpu.y = GetScreenHeight() / 2 - cpu.height / 2;
        cpu.speed = 6;
    } else {
        player2.height = 120;
        player2.width = 25;
        player2.x = 10;
        player2.y = GetScreenHeight() / 2 - player2.height / 2;
        player2.speed = 6;
    }
    // Reset score
    player1_score = 0;
    cpu_score = 0;
    player2_score=0;

    // Game loop
    while (!WindowShouldClose()) {
        ball.Update();
        player1.Update();

        if (versusCpu) {
            cpu.Update(ball.y);
        } else {
            player2.Update(true);
        }

        // Collision checks
        if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius,
                                    {player1.x, player1.y, player1.width, player1.height})) {
            ball.speed_x *= -1;
        }

        if (versusCpu) {
            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius,
                                        {cpu.x, cpu.y, cpu.width, cpu.height})) {
                ball.speed_x *= -1;
            }
        } else {
            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius,
                                        {player2.x, player2.y, player2.width, player2.height})) {
                ball.speed_x *= -1;
            }
        }
        // Check for win condition
        if (player1_score == WINNING_SCORE || cpu_score == WINNING_SCORE) {
            DrawGameOver(player1_score == WINNING_SCORE, versusCpu);
            break;
        }
        // Drawing
        BeginDrawing();
        ClearBackground(Dark_Blue);
        DrawRectangle(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), Light_Blue);
        DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 150, BLUE);
        DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), WHITE);

        ball.Draw();
        player1.Draw();
        if (versusCpu) {
            cpu.Draw1();
        } else {
            player2.Draw1();
        }

        DrawText(TextFormat("%i", player1_score), 3 * GetScreenWidth() / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", versusCpu ? cpu_score : player2_score), GetScreenWidth() / 4 - 20, 20, 80, WHITE);

        EndDrawing();
    }
}
