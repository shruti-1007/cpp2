#include "hermione.h"

void InitHermione(Hermione& hermione, const char* texturePath, int row, int col) {
    hermione.r = row;
    hermione.c = col;
    hermione.texture = LoadTexture(texturePath); // Load texture from provided path
}

void DrawHermione(const Hermione& hermione, int cellSize) {
    // Draw Hermione's frame
    int hermioneFrameX = hermione.c * cellSize; // X position
    int hermioneFrameY = hermione.r * cellSize; // Y position
    DrawRectangle(hermioneFrameX, hermioneFrameY, cellSize, cellSize, DARKGRAY); // Frame background

    // Scale Hermione's texture to 5% of original size
    float scale = 0.1f; 
    DrawTextureEx(hermione.texture,
                   Vector2{hermioneFrameX + (cellSize - hermione.texture.width * scale) / 2,
                            hermioneFrameY + (cellSize - hermione.texture.height * scale) / 2},
                   0.0f, // No rotation
                   scale, // Scale factor
                   WHITE); // Color tint
}
