/*
 * Αρχείο: player.c
 * Συγγραφείς: Λάμπρος-Ιάσων Ευαγγελόπουλος 11862, Δημήτρης Κωνσταντινίδης 11910
 * Άδεια: GPLv3 (Open Source)
 */

#include <stddef.h>
#include "player.h"
#include <stdio.h>


/*
 * Eλέγχει αν η νέα θέση του παίκτη
 * προκαλεί σύγκρουση με κτίρια ή βγαίνει εκτός ορίων του χάρτη.
 *
 * Παράμετροι:
 * player - Δείκτης στη δομή του παίκτη.
 * map    - Δείκτης στον χάρτη.
 * newPos - Η προτεινόμενη νέα θέση του παίκτη.
 *
 * Επιστρέφει: bool - true αν υπάρχει σύγκρουση/πρόβλημα, false αν η κίνηση είναι έγκυρη.
 */
static bool CheckMapCollision(const Player *player, const Map *map, Vector2 newPos)
{
    if (player == NULL || map == NULL) return true;

    Rectangle playerRect = {
        newPos.x - player->hitboxWidth / 2.0f,
        newPos.y - player->hitboxHeight / 2.0f,
        player->hitboxWidth,
        player->hitboxHeight
    };

    if (playerRect.x < 0 ||
        playerRect.y < 0 ||
        playerRect.x + playerRect.width  > MAP_WIDTH  * TILE_SIZE ||
        playerRect.y + playerRect.height > MAP_HEIGHT * TILE_SIZE)
    {
        return true;
    }

    int leftTile   = (int)(playerRect.x) / TILE_SIZE;
    int rightTile  = (int)(playerRect.x + playerRect.width  - 1) / TILE_SIZE;
    int topTile    = (int)(playerRect.y) / TILE_SIZE;
    int bottomTile = (int)(playerRect.y + playerRect.height - 1) / TILE_SIZE;

    for (int y = topTile; y <= bottomTile; y++) {
        for (int x = leftTile; x <= rightTile; x++) {
            if (!isValid(map->tiles[y][x]))
                return true;
        }
    }

    return false;
}

Rectangle GetPlayerRect(const Player *player)
{
    Rectangle r = {
        player->position.x - player->hitboxWidth/2.0f,
        player->position.y - player->hitboxHeight/2.0f,
        player->hitboxWidth,
        player->hitboxHeight
    };
    return r;
}

void InitializePlayer(Player *player, int tileX, int tileY)
{
    if (player == NULL) return;

    player->width  = TILE_SIZE / 3; 
    player->height = TILE_SIZE / 4;

    player->position.x = tileX * TILE_SIZE + TILE_SIZE/2;
    player->position.y = tileY * TILE_SIZE + TILE_SIZE/2;

    player->hitboxWidth  = 6.0f;
    player->hitboxHeight = 6.0f;

    player->rotation = 0.0f;
    player->speed = 120.0f;

    player->texture = LoadTexture("assets/player.png");
}

void UpdatePlayer(Player *player, const Map *map, float dt)
{
    if (player == NULL || map == NULL) return;

    Vector2 newPos = player->position;
    float rotation = player->rotation;

    Vector2 tempPos = newPos;
    if (IsKeyDown(KEY_A)) { tempPos.x -= player->speed * dt; rotation = 180.0f; }
    if (IsKeyDown(KEY_D)) { tempPos.x += player->speed * dt; rotation = 0.0f; }
    if (!CheckMapCollision(player, map, tempPos)) newPos.x = tempPos.x;

    tempPos = newPos;
    if (IsKeyDown(KEY_W)) { tempPos.y -= player->speed * dt; rotation = 270.0f; }
    if (IsKeyDown(KEY_S)) { tempPos.y += player->speed * dt; rotation = 90.0f; }
    if (!CheckMapCollision(player, map, tempPos)) newPos.y = tempPos.y;

    player->position = newPos;
    player->rotation = rotation;
}

void DrawPlayer(const Player *player)
{
    if (player == NULL) return;

    Rectangle sourceRec = { 0, 0, (float)player->texture.width, (float)player->texture.height };
    Rectangle destRec   = { player->position.x, player->position.y, player->width, player->height };
    Vector2 origin = { player->width / 2.0f, player->height / 2.0f };
    
    DrawTexturePro(player->texture, sourceRec, destRec, origin, player->rotation, WHITE);
}