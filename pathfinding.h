/*
 * Αρχείο: pathfinding.h
 * Συγγραφείς: Λάμπρος-Ιάσων Ευαγγελόπουλος 11862, Δημήτρης Κωνσταντινίδης 11910
 * Άδεια: GPLv3 (Open Source)
 */

#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "map.h"
#include <raylib.h>
#include <stdbool.h>

typedef struct {
    int x;
    int y;
} Point;

/*
 * Υπολογίζει τη συντομότερη διαδρομή μεταξύ δύο σημείων χρησιμοποιώντας BFS.
 *
 * Παράμετροι:
 * map     - Δείκτης στον χάρτη (για έλεγχο εμποδίων/δρόμων).
 * start   - Σημείο εκκίνησης (συντεταγμένες πλακιδίου).
 * end     - Σημείο προορισμού (συντεταγμένες πλακιδίου).
 * outPath - Πίνακας όπου θα αποθηκευτούν τα σημεία της διαδρομής.
 * maxLen  - Το μέγιστο μήκος του πίνακα outPath (για αποφυγή overflow).
 *
 * Επιστρέφει: int - Το πλήθος των βημάτων της διαδρομής (0 αν δεν βρέθηκε διαδρομή).
 */
int FindPath(const Map *map, Point start, Point end, Point *outPath, int maxLen);

/*
 * Σχεδιάζει οπτικά τη διαδρομή στην οθόνη ως γραμμή.
 *
 * Παράμετροι:
 * path    - Ο πίνακας με τα σημεία της διαδρομής.
 * pathLen - Το πλήθος των σημείων (μήκος διαδρομής).
 *
 * Επιστρέφει: void.
 */
void DrawPath(Point *path, int pathLen);

#endif