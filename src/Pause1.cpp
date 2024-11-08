#include "raylib.h"
#include "Pause1.h"
#include <iostream>
#include "collectibles.h"

void UpdatePause1()
{
    // pause
}

void DrawPause1()
{
    DrawRectangle(0, 0, 1400, 900, Fade(DARKGRAY, 0.8f));
    DrawText("PAUSED", 550, 400, 40, WHITE);
    DrawText(("You earned total " + std::to_string(CollectibleCount) + " points").c_str(), 500, 450, 20, WHITE);
    DrawText("Press P to move to the next level", 500, 500, 20, WHITE);
}