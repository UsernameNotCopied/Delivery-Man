/*
 * Αρχείο: map.c
 * Συγγραφείς: Λάμπρος-Ιάσων Ευαγγελόπουλος 11862, Δημήτρης Κωνσταντινίδης 11910
 * Άδεια: GPLv3 (Open Source)
 */

#include "map.h"
#include <stddef.h>


/*
 * Δημιουργεί τη διάταξη του χάρτη.
 *
 * Παράμετροι:
 * map - Δείκτης στη δομή του χάρτη προς συμπλήρωση.
 *
 * Επιστρέφει: void.
 */
static void GenerateMap(Map *map)
{
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (x == 0 || y == 0 || x == MAP_WIDTH - 1 || y == MAP_HEIGHT - 1) {
                map->tiles[y][x] = TILE_ROAD;
            }
            else if (x % 4 == 0 || y % 4 == 0) {
                map->tiles[y][x] = TILE_ROAD;
            }
            else {
                map->tiles[y][x] = TILE_BUILDING;
            }
        }
    }
    map->tiles[4][4]   = TILE_SHOP;
    map->tiles[12][24] = TILE_SHOP;
    map->tiles[28][40] = TILE_SHOP;
}

void InitializeMap(Map *map)
{
    if (map == NULL) return;
    GenerateMap(map);
    map->shopTexture   = LoadTexture("assets/shop.png");
    map->clientTexture = LoadTexture("assets/client.png");
}

void DrawMap(const Map *map)
{
    if (map == NULL) return;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Rectangle tileRect = {
                (float)(x * TILE_SIZE), (float)(y * TILE_SIZE),
                (float)TILE_SIZE, (float)TILE_SIZE
            };

            if (map->tiles[y][x] == TILE_BUILDING) {
                DrawRectangleRec(tileRect, GRAY);
                DrawRectangleLinesEx(tileRect, 1, BLACK);
            } else {
                DrawRectangleRec(tileRect, DARKGRAY);
            }
            if (map->tiles[y][x] == TILE_SHOP) {
                DrawTexturePro(map->shopTexture, 
                    (Rectangle){0, 0, (float)map->shopTexture.width, (float)map->shopTexture.height}, 
                    tileRect, (Vector2){0, 0}, 0.0f, WHITE);
            }
            else if (map->tiles[y][x] == TILE_CLIENT) {
                DrawTexturePro(map->clientTexture, 
                    (Rectangle){0, 0, (float)map->clientTexture.width, (float)map->clientTexture.height}, 
                    tileRect, (Vector2){0, 0}, 0.0f, WHITE);
            }
        }
    }
}

bool isValid(TileType tile)
{
    return (tile != TILE_BUILDING);
}