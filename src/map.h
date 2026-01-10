/*
 * Αρχείο: map.h
 * Συγγραφείς: Λάμπρος-Ιάσων Ευαγγελόπουλος 11862, Δημήτρης Κωνσταντινίδης 11910
 * Άδεια: GPLv3 (Open Source)
 */

#ifndef MAP_H
#define MAP_H

#include <raylib.h>

#define TILE_SIZE 32

#define MAP_WIDTH 49
#define MAP_HEIGHT 36

typedef enum {
    TILE_ROAD = 0,
    TILE_BUILDING,
    TILE_SHOP,
    TILE_CLIENT
} TileType;

typedef struct {
    TileType tiles[MAP_HEIGHT][MAP_WIDTH];

    Texture2D shopTexture;
    Texture2D clientTexture;
} Map;

/*
 * Αρχικοποιεί τον χάρτη και φορτώνει τους πόρους (εικόνες).
 *
 * Παράμετροι:
 * map - Δείκτης στη δομή του χάρτη.
 *
 * Επιστρέφει: void.
 */
void InitializeMap(Map *map);

/*
 * Σχεδιάζει τον χάρτη (πλακίδια και εικονίδια) στην οθόνη.
 *
 * Παράμετροι:
 * map - Δείκτης στη δομή του χάρτη προς σχεδίαση.
 *
 * Επιστρέφει: void.
 */
void DrawMap(const Map *map);

/*
 * Ελέγχει αν ένα πλακίδιο είναι προσπελάσιμο από τον παίκτη (δηλαδή δεν είναι κτίριο).
 *
 * Παράμετροι:
 * tile - Ο τύπος του tile προς έλεγχο.
 *
 * Επιστρέφει: bool - true αν είναι μπορεί να οδηγήσει ο παίκτης εκεί, false αν είναι εμπόδιο.
 */
bool isValid(TileType tile);

#endif
