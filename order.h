/*
 * Αρχείο: order.h
 * Συγγραφείς: Λάμπρος-Ιάσων Ευαγγελόπουλος 11862, Δημήτρης Κωνσταντινίδης 11910
 * Άδεια: GPLv3 (Open Source)
 */

#ifndef ORDER_H
#define ORDER_H

#include <raylib.h>
#include <stdbool.h>
#include "map.h"

#define MAX_ORDERS 3

typedef enum {
    ORDER_AVAILABLE = 0,
    ORDER_ACCEPTED,
    ORDER_CARRYING,
    ORDER_DELIVERED
} OrderState;

typedef struct {
    int id;
    int shopX, shopY;       
    int clientX, clientY;   
    
    int reward;             
    float currentReward;    
    
    OrderState state;
} Order;

typedef struct {
    Order list[MAX_ORDERS];
    int selectedIndex;      
    int totalMoney;
    
    int ordersCompleted;
    float decayRate;
} Orders;

/*
 * Αρχικοποιεί το σύστημα παραγγελιών και τα σύστημα αμοιβών.
 *
 * Παράμετροι:
 * orders - Δείκτης στη δομή Orders.
 *
 * Επιστρέφει: void.
 */
void InitializeOrders(Orders *orders);

/*
 * Δημιουργεί τις αρχικές τυχαίες παραγγελίες στον χάρτη.
 *
 * Παράμετροι:
 * orders - Δείκτης στη δομή Orders.
 * map    - Δείκτης στον χάρτη.
 *
 * Επιστρέφει: void.
 */
void GenerateOrders(Orders *orders, Map *map);

/*
 * Ενημερώνει την κατάσταση των παραγγελιών, τα οικονομικά και ελέγχει για παραλαβή/παράδοση.
 *
 * Παράμετροι:
 * orders                - Δείκτης στη δομή Orders.
 * map                   - Δείκτης στον χάρτη.
 * playerRect            - Hitbox παίκτη (για έλεγχο θέσης).
 * isPlayerActionPressed - Αν πατήθηκε το πλήκτρο δράσης (P).
 * dt                    - Ο χρόνος που πέρασε από το προηγούμενο καρέ.
 *
 * Επιστρέφει: int - 1 αν συνέβη σημαντικό γεγονός (παραλαβή/παράδοση), αλλιώς 0.
 */
int UpdateOrders(Orders *orders, Map *map, Rectangle playerRect, bool isPlayerActionPressed, float dt);

/*
 * Σχεδιάζει το UI των παραγγελιών στην οθόνη.
 *
 * Παράμετροι:
 * orders - Δείκτης στη δομή Orders.
 *
 * Επιστρέφει: void.
 */
void DrawOrders(const Orders *orders);

/*
 * Επιλέγει την ενεργή παραγγελία.
 *
 * Παράμετροι:
 * orders - Δείκτης στη δομή Orders.
 * index  - Ο δείκτης της παραγγελίας (0 έως MAX_ORDERS-1).
 *
 * Επιστρέφει: void.
 */
void SelectOrder(Orders *orders, int index);

/*
 * Υπολογίζει το Target.
 *
 * Παράμετροι:
 * orders    - Δείκτης στη δομή Orders.
 * playerPos - Η θέση του παίκτη.
 *
 * Επιστρέφει: Vector2 - Οι συντεταγμένες του στόχου (Φαρμακείο ή Πελάτης).
 */
Vector2 GetCurrentTarget(const Orders *orders, Vector2 playerPos);

#endif