/*
 * Αρχείο: main.c
 * Συγγραφείς: Λάμπρος-Ιάσων Ευαγγελόπουλος 11862, Δημήτρης Κωνσταντινίδης 11910
 * Άδεια: GPLv3 (Open Source)
 */

#include "order.h"
#include "map.h"
#include "player.h"
#include "pathfinding.h"
#include "traffic.h"
#include <raylib.h>
#include <stdio.h>

#define SCREEN_WIDTH (MAP_WIDTH * TILE_SIZE) / 1.5f
#define SCREEN_HEIGHT ((MAP_HEIGHT * TILE_SIZE + 80)) / 1.5f

/*
 * Σχεδιάζει το πλαίσιο ελέγχου και οδηγιών στην επάνω δεξιά γωνία.
 *
 * Παράμετροι: Καμία.
 * Επιστρέφει: void.
 */
void DrawControlsPanel() {
    int panelW = 280;
    int panelH = 140;
    int x = SCREEN_WIDTH - panelW - 20; 
    int y = 20;

    DrawRectangle(x, y, panelW, panelH, Fade(SKYBLUE, 0.85f));
    DrawRectangleLines(x, y, panelW, panelH, DARKBLUE);
    DrawText("CONTROLS", x + 10, y + 10, 20, DARKBLUE);
    DrawLine(x, y + 35, x + panelW, y + 35, DARKBLUE);
    DrawText("W A S D", x + 10, y + 50, 20, BLACK);
    DrawText(": Drive Vehicle", x + 110, y + 50, 18, DARKGRAY);
    DrawText("P", x + 10, y + 80, 20, BLACK);
    DrawText(": Accept / Deliver", x + 110, y + 80, 18, DARKGRAY);
    DrawText("1 / 2 / 3", x + 10, y + 110, 20, BLACK);
    DrawText(": Select Job", x + 110, y + 110, 18, DARKGRAY);
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Delivery Man");
    InitAudioDevice(); 
    SetTargetFPS(60);

    Player player;
    InitializePlayer(&player, 8, 8); 

    Map map;
    InitializeMap(&map); 

    Orders orders;
    InitializeOrders(&orders);
    GenerateOrders(&orders, &map); 

    TrafficManager traffic;
    InitializeTraffic(&traffic, &map); 

    Music soundtrack = LoadMusicStream("assets/soundtrack.mp3");
    PlayMusicStream(soundtrack);
    SetMusicVolume(soundtrack, 0.3f); // Set volume (0.0 to 1.0)
    Sound crashSound = LoadSound("assets/crash.mp3");

    Camera2D camera = {0};
    camera.target = player.position;
    camera.offset = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;

    Point pathBuffer[500];
    int pathLen = 0;
    
    int collisionTimer = 0;

    int crashPenalty = 50;  
    int currentTraffic = 20; 
    int lastCheckedOrders = 0;

    bool gameOver = false;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        
        UpdateMusicStream(soundtrack);

        Vector2 targetVec = {0}; 
        Point pTile = {0};
        Point tTile = {0};

        if (orders.totalMoney <= 0) {
            gameOver = true;
        }

        if (!gameOver) {
            if (orders.ordersCompleted > 0 && 
                orders.ordersCompleted % 3 == 0 && 
                orders.ordersCompleted != lastCheckedOrders) 
            {
                lastCheckedOrders = orders.ordersCompleted;
                crashPenalty *= 2;
                orders.decayRate *= 2.0f;
                currentTraffic += 10;
                SetTrafficCount(&traffic, &map, currentTraffic);
            }

            if (IsKeyPressed(KEY_ONE))   SelectOrder(&orders, 0);
            if (IsKeyPressed(KEY_TWO))   SelectOrder(&orders, 1);
            if (IsKeyPressed(KEY_THREE)) SelectOrder(&orders, 2);

            UpdatePlayer(&player, &map, dt);
            UpdateOrders(&orders, &map, GetPlayerRect(&player), IsKeyPressed(KEY_P), dt);
            UpdateTraffic(&traffic, &map, dt);
            
            if (CheckTrafficCollision(&traffic, GetPlayerRect(&player))) {
                if (collisionTimer == 0) {
                    PlaySound(crashSound);
                    orders.totalMoney -= crashPenalty;
                    collisionTimer = 60; 
                }
            }
            if (collisionTimer > 0) collisionTimer--;

            camera.target = player.position;

            pTile = (Point){ (int)(player.position.x / TILE_SIZE), (int)(player.position.y / TILE_SIZE) };
            targetVec = GetCurrentTarget(&orders, player.position);
            tTile = (Point){ (int)(targetVec.x / TILE_SIZE), (int)(targetVec.y / TILE_SIZE) };

            if (pTile.x != tTile.x || pTile.y != tTile.y) {
                pathLen = FindPath(&map, pTile, tTile, pathBuffer, 500);
            } else {
                pathLen = 0;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (gameOver) {
            const char* titleText = "GAME OVER";
            const char* reasonText = "You ran out of money!";
            const char* scoreText = TextFormat("Total Orders: %d", orders.ordersCompleted);

            int titleW = MeasureText(titleText, 80);
            int reasonW = MeasureText(reasonText, 30);
            int scoreW = MeasureText(scoreText, 20);

            DrawText(titleText, SCREEN_WIDTH/2 - titleW/2, SCREEN_HEIGHT/2 - 60, 80, RED);
            DrawText(reasonText, SCREEN_WIDTH/2 - reasonW/2, SCREEN_HEIGHT/2 + 40, 30, DARKGRAY);
            DrawText(scoreText, SCREEN_WIDTH/2 - scoreW/2, SCREEN_HEIGHT/2 + 80, 20, BLACK);
        } 
        else {
            BeginMode2D(camera);
                DrawMap(&map);
                DrawPath(pathBuffer, pathLen); 
                DrawTraffic(&traffic);         
                DrawPlayer(&player);           
                DrawCircleV(targetVec, 8.0f, MAROON); 
            EndMode2D();

            DrawRectangle(0, SCREEN_HEIGHT - 40, SCREEN_WIDTH, 40, LIGHTGRAY);
            DrawLine(0, SCREEN_HEIGHT - 40, SCREEN_WIDTH, SCREEN_HEIGHT - 40, DARKGRAY);
            DrawText(TextFormat("Crash Penalty: -%d$ | Traffic: %d", crashPenalty, currentTraffic), 10, SCREEN_HEIGHT - 30, 20, DARKGRAY);
            
            DrawOrders(&orders);
            DrawControlsPanel();
            
            if (collisionTimer > 0) {
                if ((collisionTimer / 10) % 2 == 0) {
                    const char* crashText = TextFormat("CRASH! -%d$", crashPenalty);
                    int crashW = MeasureText(crashText, 40);
                    DrawText(crashText, SCREEN_WIDTH/2 - crashW/2, SCREEN_HEIGHT/2 - 50, 40, RED);
                }
            }
        }

        EndDrawing();
    }

    UnloadMusicStream(soundtrack);
    UnloadSound(crashSound);
    
    UnloadTexture(player.texture);
    UnloadTexture(map.shopTexture);
    UnloadTexture(map.clientTexture);
    UnloadTexture(traffic.carTexture);
    
    CloseAudioDevice();
    CloseWindow();

    return 0;
}