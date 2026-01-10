/*
 * Αρχείο: order.c
 * Συγγραφείς: Λάμπρος-Ιάσων Ευαγγελόπουλος 11862, Δημήτρης Κωνσταντινίδης 11910
 * Άδεια: GPLv3 (Open Source)
 */

#include "order.h"
#include "pathfinding.h"
#include <stdio.h>
#include <stdlib.h> 

static const Point PHARMACIES[3] = {
    {4, 4},    
    {24, 12},  
    {40, 28}   
};

static void SpawnSingleOrder(Order *o, Map *map, int id) {
    o->id = id;
    o->state = ORDER_AVAILABLE;

    int pIndex = id % 3; 
    o->shopX = PHARMACIES[pIndex].x;
    o->shopY = PHARMACIES[pIndex].y;

    int cx, cy;
    while (true) {
        cx = GetRandomValue(1, MAP_WIDTH - 2);
        cy = GetRandomValue(1, MAP_HEIGHT - 2);

        if (map->tiles[cy][cx] == TILE_ROAD) {
            if (abs(cx - o->shopX) + abs(cy - o->shopY) > 10) {
                break;
            }
        }
    }

    o->clientX = cx;
    o->clientY = cy;
    map->tiles[cy][cx] = TILE_CLIENT;

    Point start = {o->shopX, o->shopY};
    Point end   = {o->clientX, o->clientY};
    Point dummyPath[500];
    
    int dist = FindPath(map, start, end, dummyPath, 500);
    o->currentReward = (float)(50 + (dist * 4)); 
    o->reward = (int)o->currentReward;
}

void InitializeOrders(Orders *orders) {
    if (orders == NULL) return;
    orders->selectedIndex = 0;
    orders->totalMoney = 200; 
    orders->ordersCompleted = 0;
    orders->decayRate = 2.0f; 
}

void GenerateOrders(Orders *orders, Map *map) {
    for (int i = 0; i < MAX_ORDERS; i++) {
        SpawnSingleOrder(&orders->list[i], map, i);
    }
}

void SelectOrder(Orders *orders, int index) {
    if (index >= 0 && index < MAX_ORDERS) {
        orders->selectedIndex = index;
    }
}

int UpdateOrders(Orders *orders, Map *map, Rectangle playerRect, bool isPlayerActionPressed, float dt) {
    for (int i = 0; i < MAX_ORDERS; i++) {
        Order *o = &orders->list[i];
        if (o->state == ORDER_CARRYING) {
            o->currentReward -= orders->decayRate * dt;    
            if (o->currentReward < 10.0f) o->currentReward = 10.0f;
            o->reward = (int)o->currentReward;
        }
    }

    Order *ord = &orders->list[orders->selectedIndex];

    int pTileX = (int)((playerRect.x + playerRect.width/2) / TILE_SIZE);
    int pTileY = (int)((playerRect.y + playerRect.height/2) / TILE_SIZE);

    if (ord->state == ORDER_AVAILABLE && isPlayerActionPressed) {
        ord->state = ORDER_ACCEPTED;
        return 0;
    }
    else if (ord->state == ORDER_ACCEPTED) {
        if (pTileX == ord->shopX && pTileY == ord->shopY) {
            ord->state = ORDER_CARRYING;
            return 1;
        }
    }
    else if (ord->state == ORDER_CARRYING) {
        if (pTileX == ord->clientX && pTileY == ord->clientY) {
            orders->totalMoney += ord->reward;
            orders->ordersCompleted++;

            map->tiles[ord->clientY][ord->clientX] = TILE_ROAD;
            SpawnSingleOrder(ord, map, ord->id);
            ord->state = ORDER_AVAILABLE;
            
            return 1;
        }
    }
    
    return 0;
}

void DrawOrders(const Orders *orders) {
    int startY = 10;
    DrawText(TextFormat("Total: %d $", orders->totalMoney), 10, startY, 20, DARKGREEN);
    
    DrawText(TextFormat("Delivered: %d", orders->ordersCompleted), 150, startY, 20, DARKGRAY);
    
    startY += 30;

    for (int i = 0; i < MAX_ORDERS; i++) {
        Color c = BLACK;
        if (i == orders->selectedIndex) c = BLUE; 

        const Order *o = &orders->list[i];
        
        char statusText[50];
        switch(o->state) {
            case ORDER_AVAILABLE: TextCopy(statusText, "WAITING"); break;
            case ORDER_ACCEPTED:  TextCopy(statusText, ">> PHARMACY"); c = ORANGE; break;
            case ORDER_CARRYING:  TextCopy(statusText, ">> CLIENT"); c = RED; break;
            default: TextCopy(statusText, "DONE"); break;
        }

        DrawText(TextFormat("Order %d - %d$", i+1, o->reward), 10, startY, 20, c);
        DrawText(statusText, 180, startY, 20, c);
        startY += 25;
    }
}

Vector2 GetCurrentTarget(const Orders *orders, Vector2 playerPos) {
    const Order *o = &orders->list[orders->selectedIndex];
    Vector2 target = playerPos; 

    if (o->state == ORDER_AVAILABLE || o->state == ORDER_ACCEPTED) {
        target.x = (float)(o->shopX * TILE_SIZE + TILE_SIZE/2);
        target.y = (float)(o->shopY * TILE_SIZE + TILE_SIZE/2);
    } 
    else if (o->state == ORDER_CARRYING) {
        target.x = (float)(o->clientX * TILE_SIZE + TILE_SIZE/2);
        target.y = (float)(o->clientY * TILE_SIZE + TILE_SIZE/2);
    }
    return target;
}