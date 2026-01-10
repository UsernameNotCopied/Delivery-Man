/*
 * Αρχείο: traffic.h
 * Συγγραφείς: Λάμπρος-Ιάσων Ευαγγελόπουλος 11862, Δημήτρης Κωνσταντινίδης 11910
 * Άδεια: GPLv3 (Open Source)
 */

#ifndef TRAFFIC_H
#define TRAFFIC_H

#include "map.h"
#include <raylib.h>

#define MAX_CARS 80  
#define CAR_SPEED 60.0f
#define CAR_SIZE 10.0f  
#define LANE_OFFSET 7.0f

typedef struct {
    Vector2 position;
    Vector2 direction; 
    Color color;
    bool active;
    float speed;
    int id;
} Vehicle;

typedef struct {
    Vehicle cars[MAX_CARS];
    Texture2D carTexture;
} TrafficManager;

/*
 * Αρχικοποιεί το σύστημα κυκλοφορίας.
 *
 * Παράμετροι:
 * traffic - Δείκτης στο Traffic Manager.
 * map     - Δείκτης στον χάρτη.
 *
 * Επιστρέφει: void.
 */
void InitializeTraffic(TrafficManager *traffic, const Map *map);

/*
 * Θέτει τον συνολικό αριθμό ενεργών οχημάτων στον χάρτη.
 *
 * Παράμετροι:
 * traffic     - Δείκτης στο Traffic Manager.
 * map         - Δείκτης στον χάρτη.
 * targetCount - Ο επιθυμητός αριθμός οχημάτων.
 *
 * Επιστρέφει: void.
 */
void SetTrafficCount(TrafficManager *traffic, const Map *map, int targetCount);

/*
 * Ενημερώνει τη θέση και την κατάσταση όλων των οχημάτων.
 *
 * Παράμετροι:
 * traffic - Δείκτης στο Traffic Manager.
 * map     - Δείκτης στον χάρτη.
 * dt      - Ο χρόνος που πέρασε από το προηγούμενο καρέ.
 *
 * Επιστρέφει: void.
 */
void UpdateTraffic(TrafficManager *traffic, const Map *map, float dt);

/*
 * Σχεδιάζει όλα τα ενεργά οχήματα στην οθόνη.
 *
 * Παράμετροι:
 * traffic - Δείκτης στο Traffic Manager
 *
 * Επιστρέφει: void.
 */
void DrawTraffic(const TrafficManager *traffic);

/*
 * Ελέγχει αν ο παίκτης συγκρούστηκε με κάποιο όχημα.
 *
 * Παράμετροι:
 * traffic    - Δείκτης στο Traffic Manager.
 * playerRect - Το hitbox του παίκτη.
 *
 * Επιστρέφει: bool - true αν υπάρχει σύγκρουση, αλλιώς false.
 */
bool CheckTrafficCollision(const TrafficManager *traffic, Rectangle playerRect);

#endif