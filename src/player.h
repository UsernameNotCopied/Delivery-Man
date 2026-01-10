/*
 * Αρχείο: player.h
 * Συγγραφείς: Λάμπρος-Ιάσων Ευαγγελόπουλος 11862, Δημήτρης Κωνσταντινίδης 11910
 * Άδεια: GPLv3 (Open Source)
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include <stdbool.h>
#include "map.h"


typedef struct {
    Vector2 position;
    float speed;
    int width;
    int height;
    float hitboxWidth;
    float hitboxHeight;
    float rotation;
    Texture2D texture;
} Player;

/*
 * Επιστρέφει το τρέχον ορθογώνιο σύγκρουσης (Hitbox) του παίκτη.
 *
 * Παράμετροι:
 * player - Δείκτης στη δομή του παίκτη.
 *
 * Επιστρέφει: Rectangle - Το ορθογώνιο που περικλείει τον παίκτη.
 */
Rectangle GetPlayerRect(const Player *player);

/*
 * Αρχικοποιεί τον παίκτη σε συγκεκριμένη θέση και φορτώνει την εικόνα του.
 *
 * Παράμετροι:
 * player - Δείκτης στη δομή του παίκτη.
 * tileX  - Αρχική θέση X σε πλακίδια.
 * tileY  - Αρχική θέση Y σε πλακίδια.
 *
 * Επιστρέφει: void.
 */
void InitializePlayer(Player *player, int tileX, int tileY);

/*
 * Ενημερώνει τη θέση του παίκτη με βάση την είσοδο του πληκτρολογίου.
 *
 * Παράμετροι:
 * player - Δείκτης στη δομή του παίκτη.
 * map    - Δείκτης στον χάρτη (για έλεγχο εμποδίων).
 * dt     - Delta time (χρόνος από το προηγούμενο καρέ).
 *
 * Επιστρέφει: void.
 */
void UpdatePlayer(Player *player, const Map *map, float dt);

/*
 * Σχεδιάζει τον παίκτη στην οθόνη.
 *
 * Παράμετροι:
 * player - Δείκτης στη δομή του παίκτη.
 *
 * Επιστρέφει: void.
 */
void DrawPlayer(const Player *player);

#endif
