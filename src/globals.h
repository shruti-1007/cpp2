#ifndef GLOBALS_H
#define GLOBALS_H

#include "collectibles.h"
#include "character.h"

extern Collectibles collectible;
extern Character hp;
extern int CollectibleCount;
extern int coinsCollected; // Counter for collected coins
extern bool gameWon; // Flag to track if the game is won
extern bool gameOver; // Flag to track if the game is over

#endif // GLOBALS_H
