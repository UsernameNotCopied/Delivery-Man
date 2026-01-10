/*
 * Αρχείο: traffic.c
 * Συγγραφείς: Λάμπρος-Ιάσων Ευαγγελόπουλος 11862, Δημήτρης Κωνσταντινίδης 11910
 * Άδεια: GPLv3 (Open Source)
 */

#include "traffic.h"
#include <stdlib.h>
#include <math.h>
#include <raymath.h> 


/*
 * Επιστρέφει ένα τυχαίο χρώμα για το αυτοκίνητο.
 *
 * Παράμετροι: Καμία.
 * Επιστρέφει: Color - Ένα τυχαίο χρώμα από την προκαθορισμένη λίστα.
 */
static Color GetRandomCarColor() {
    Color colors[] = { RED, MAROON, GREEN, LIME, PURPLE, VIOLET, BEIGE, BROWN };
    return colors[GetRandomValue(0, 7)];
}

/*
 * Ελέγχει αν μια θέση στον χάρτη είναι δρόμος.
 *
 * Παράμετροι:
 * map - Δείκτης στη δομή του χάρτη.
 * x   - Η συντεταγμένη x του πλακιδίου (tile).
 * y   - Η συντεταγμένη y του πλακιδίου (tile).
 *
 * Επιστρέφει: bool - true αν είναι δρόμος, false αν είναι κτίριο ή εκτός ορίων.
 */
static bool IsDrivable(const Map *map, int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return false;
    return (map->tiles[y][x] == TILE_ROAD);
}

/*
 * Υπολογίζει το κέντρο της λωρίδας κυκλοφορίας με βάση την κατεύθυνση (δεξιά πλευρά δρόμου).
 *
 * Παράμετροι:
 * tileX - Η συντεταγμένη x του πλακιδίου.
 * tileY - Η συντεταγμένη y του πλακιδίου.
 * dir   - Το διάνυσμα κατεύθυνσης του οχήματος.
 *
 * Επιστρέφει: Vector2 - Η ακριβής θέση (pixels) στο κέντρο της σωστής λωρίδας.
 */
static Vector2 GetLaneCenter(int tileX, int tileY, Vector2 dir) {
    float cx = (float)tileX * TILE_SIZE + TILE_SIZE / 2.0f;
    float cy = (float)tileY * TILE_SIZE + TILE_SIZE / 2.0f;

    if (dir.x > 0) cy += LANE_OFFSET;      
    else if (dir.x < 0) cy -= LANE_OFFSET; 
    
    if (dir.y > 0) cx -= LANE_OFFSET;      
    else if (dir.y < 0) cx += LANE_OFFSET; 

    return (Vector2){cx, cy};
}

/*
 * Δημιουργεί ένα όχημα σε τυχαία θέση δρόμου.
 *
 * Παράμετροι:
 * v   - Δείκτης στο όχημα προς αρχικοποίηση.
 * id  - Το αναγνωριστικό (ID) του οχήματος.
 * map - Δείκτης στον χάρτη.
 *
 * Επιστρέφει: void.
 */
static void SpawnCar(Vehicle *v, int id, const Map *map) {
    int attempts = 0;
    while(attempts < 100) {
        attempts++;
        int tx = GetRandomValue(0, MAP_WIDTH - 1);
        int ty = GetRandomValue(0, MAP_HEIGHT - 1);

        if (map->tiles[ty][tx] == TILE_ROAD) {
            v->id = id;
            v->active = true;
            v->color = GetRandomCarColor();
            v->speed = CAR_SPEED;

            Vector2 possibleDirs[4];
            int dirCount = 0;
            if (IsDrivable(map, tx+1, ty)) possibleDirs[dirCount++] = (Vector2){1, 0};
            if (IsDrivable(map, tx-1, ty)) possibleDirs[dirCount++] = (Vector2){-1, 0};
            if (IsDrivable(map, tx, ty+1)) possibleDirs[dirCount++] = (Vector2){0, 1};
            if (IsDrivable(map, tx, ty-1)) possibleDirs[dirCount++] = (Vector2){0, -1};

            if (dirCount > 0) {
                v->direction = possibleDirs[GetRandomValue(0, dirCount-1)];
                v->position = GetLaneCenter(tx, ty, v->direction);
                return;
            }
        }
    }
    v->active = false;
}

void SetTrafficCount(TrafficManager *traffic, const Map *map, int targetCount) {
    if (targetCount > MAX_CARS) targetCount = MAX_CARS;
    
    int currentActive = 0;
    for(int i=0; i<MAX_CARS; i++) {
        if(traffic->cars[i].active) currentActive++;
    }

    for(int i=0; i<MAX_CARS && currentActive < targetCount; i++) {
        if (!traffic->cars[i].active) {
            SpawnCar(&traffic->cars[i], i, map);
            if(traffic->cars[i].active) currentActive++;
        }
    }
}

void InitializeTraffic(TrafficManager *traffic, const Map *map) {
    traffic->carTexture = LoadTexture("assets/car.png");
    
    for(int i=0; i<MAX_CARS; i++) traffic->cars[i].active = false;
    
    SetTrafficCount(traffic, map, 20);
}

void UpdateTraffic(TrafficManager *traffic, const Map *map, float dt) {
    for (int i = 0; i < MAX_CARS; i++) {
        Vehicle *v = &traffic->cars[i];
        if (!v->active) continue;

        Rectangle sensor = {
            v->position.x + v->direction.x * 20.0f - CAR_SIZE/2,
            v->position.y + v->direction.y * 20.0f - CAR_SIZE/2,
            CAR_SIZE, CAR_SIZE
        };

        bool carAhead = false;
        for (int j = 0; j < MAX_CARS; j++) {
            if (i == j || !traffic->cars[j].active) continue;
            Rectangle otherCar = {
                traffic->cars[j].position.x - CAR_SIZE/2,
                traffic->cars[j].position.y - CAR_SIZE/2,
                CAR_SIZE, CAR_SIZE
            };
            if (CheckCollisionRecs(sensor, otherCar)) {
                carAhead = true;
                break;
            }
        }
        
        float targetSpeed = carAhead ? 0.0f : CAR_SPEED;
        v->speed += (targetSpeed - v->speed) * 0.1f; 
        if (v->speed < 0.1f) v->speed = 0.0f;

        v->position.x += v->direction.x * v->speed * dt;
        v->position.y += v->direction.y * v->speed * dt;

        int tx = (int)(v->position.x / TILE_SIZE);
        int ty = (int)(v->position.y / TILE_SIZE);

        Vector2 laneCenter = GetLaneCenter(tx, ty, v->direction);
        
        int nextX = tx + (int)v->direction.x;
        int nextY = ty + (int)v->direction.y;
        bool wallAhead = !IsDrivable(map, nextX, nextY);

        Vector2 toPos = Vector2Subtract(v->position, laneCenter);
        bool passedCenter = (Vector2DotProduct(toPos, v->direction) > 0);
        float dist = Vector2Distance(v->position, laneCenter);

        if (dist < 3.0f || (wallAhead && passedCenter)) {
            if (wallAhead && passedCenter) v->position = laneCenter;

            Vector2 validDirs[4];
            int validCount = 0;
            Vector2 dirs[] = { {0,-1}, {0,1}, {-1,0}, {1,0} };

            for (int d=0; d<4; d++) {
                if (dirs[d].x == -v->direction.x && dirs[d].y == -v->direction.y) continue;
                if (IsDrivable(map, tx + (int)dirs[d].x, ty + (int)dirs[d].y)) {
                    validDirs[validCount++] = dirs[d];
                }
            }

            if (validCount == 0) validDirs[validCount++] = (Vector2){ -v->direction.x, -v->direction.y };

            bool turn = true;
            if (!wallAhead && GetRandomValue(0, 100) < 70) turn = false;

            if (turn) {
                int r = GetRandomValue(0, validCount - 1);
                v->direction = validDirs[r];
                v->position = GetLaneCenter(tx, ty, v->direction); 
            }
        }
    }
}

void DrawTraffic(const TrafficManager *traffic) {
    float visualSize = 14.0f; 

    for (int i = 0; i < MAX_CARS; i++) {
        const Vehicle *v = &traffic->cars[i];
        if (!v->active) continue;

        float rotation = 0.0f;
        if (v->direction.x > 0) rotation = 270.0f;
        if (v->direction.y > 0) rotation = 0.0f;
        if (v->direction.x < 0) rotation = 90.0f;
        if (v->direction.y < 0) rotation = 180.0f;

        Rectangle sourceRec = { 0, 0, (float)traffic->carTexture.width, (float)traffic->carTexture.height };
        Rectangle destRec = { v->position.x, v->position.y, visualSize, visualSize };
        Vector2 origin = { visualSize / 2.0f, visualSize / 2.0f };

        DrawTexturePro(traffic->carTexture, sourceRec, destRec, origin, rotation, v->color);
    }
}

bool CheckTrafficCollision(const TrafficManager *traffic, Rectangle playerRect) {
    for (int i = 0; i < MAX_CARS; i++) {
        const Vehicle *v = &traffic->cars[i];
        if (!v->active) continue;

        Rectangle carRect = {
            v->position.x - CAR_SIZE/2,
            v->position.y - CAR_SIZE/2,
            CAR_SIZE, CAR_SIZE
        };

        if (CheckCollisionRecs(playerRect, carRect)) return true;
    }
    return false;
}