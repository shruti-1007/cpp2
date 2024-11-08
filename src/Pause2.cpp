#include "raylib.h"
#include "Pause2.h"
#include <iostream>
#include "collectibles.h"
#include "globals.h"
#include "level2.h"

void UpdatePause2()
{
    // pause
}

void DrawPause2()
{
    DrawRectangle(0, 0, 1400, 900, Fade(DARKGRAY, 0.8f));
    DrawText("PAUSED", 550, 400, 40, WHITE);
    DrawText("You Win!", 36 * 35 / 2 - MeasureText("You Win!", 20) / 2, 20 * 35 / 2 - 10, 20, GREEN);
    DrawText(("You earned total " + std::to_string(coinsCollected) + " coins").c_str(), 500, 450, 20, WHITE);
    DrawText("Press l to move to the next level", 500, 500, 20, WHITE);
}