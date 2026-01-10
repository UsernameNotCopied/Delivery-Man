/*
 * Αρχείο: pathfinding.c
 * Συγγραφείς: Λάμπρος-Ιάσων Ευαγγελόπουλος 11862, Δημήτρης Κωνσταντινίδης 11910
 * Άδεια: GPLv3 (Open Source)
 */

#include "pathfinding.h"
#include <string.h> 

#define MAX_QUEUE 2000

static const int DIR_X[] = {0, 1, 0, -1};
static const int DIR_Y[] = {-1, 0, 1, 0};

int FindPath(const Map *map, Point start, Point end, Point *outPath, int maxLen) {
    if (!isValid(map->tiles[start.y][start.x]) || !isValid(map->tiles[end.y][end.x])) {
        return 0; 
    }

    static int cameFrom[MAP_HEIGHT][MAP_WIDTH];
    for(int y=0; y<MAP_HEIGHT; y++) {
        for(int x=0; x<MAP_WIDTH; x++) cameFrom[y][x] = -1;
    }

    Point queue[MAX_QUEUE];
    int head = 0;
    int tail = 0;

    queue[tail++] = start;
    cameFrom[start.y][start.x] = -2; 

    bool found = false;

    while (head < tail) {
        Point current = queue[head++];

        if (current.x == end.x && current.y == end.y) {
            found = true;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nx = current.x + DIR_X[i];
            int ny = current.y + DIR_Y[i];

            if (nx < 0 || nx >= MAP_WIDTH || ny < 0 || ny >= MAP_HEIGHT) continue;

            if (cameFrom[ny][nx] == -1 && isValid(map->tiles[ny][nx])) {
                cameFrom[ny][nx] = current.y * MAP_WIDTH + current.x; 
                queue[tail++] = (Point){nx, ny};
            }
        }
    }

    if (!found) return 0;

    int steps = 0;
    Point trace = end;
    while (!(trace.x == start.x && trace.y == start.y)) {
        int parentVal = cameFrom[trace.y][trace.x];
        trace.x = parentVal % MAP_WIDTH;
        trace.y = parentVal / MAP_WIDTH;
        steps++;
    }

    if (steps >= maxLen) steps = maxLen - 1;

    Point curr = end;
    for (int i = steps - 1; i >= 0; i--) {
        outPath[i] = curr;
        int parentVal = cameFrom[curr.y][curr.x];
        curr.x = parentVal % MAP_WIDTH;
        curr.y = parentVal / MAP_WIDTH;
    }

    return steps;
}

void DrawPath(Point *path, int pathLen) {
    if (pathLen < 1) return;

    for (int i = 0; i < pathLen - 1; i++) {
        Vector2 p1 = { (float)(path[i].x * TILE_SIZE + TILE_SIZE/2), (float)(path[i].y * TILE_SIZE + TILE_SIZE/2) };
        Vector2 p2 = { (float)(path[i+1].x * TILE_SIZE + TILE_SIZE/2), (float)(path[i+1].y * TILE_SIZE + TILE_SIZE/2) };
        DrawLineEx(p1, p2, 4.0f, BLUE);
    }
}