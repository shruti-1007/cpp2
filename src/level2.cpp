#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include "hermione.h"

Hermione hermione; // Create Hermione object
bool gameWon = false; // Flag to track if the game is won
const int numrows = 20; // Number of rows
const int ncols = 36; // Number of columns
const int cellSize = 35; // Size of each cell
const int numGhosts = 5; // Number of ghosts

// Cell structure definition
struct Cell {
    bool visited = false;
    bool walls[4] = { true, true, true, true }; // Top, Right, Bottom, Left
    int r, c; // Row and Column
    std::vector<Cell*> neighbors; // Neighboring cells
};

// Player structure definition
struct Player {
    int r, c; // Player's row and column position
    Texture2D texture; // Player texture
};


// Ghost structure definition
struct Ghost {
    int r, c; // Ghost's row and column position
    Texture2D texture; // Ghost texture
};


// Global variables
std::vector<Cell> grid(numrows * ncols);
Cell* current = nullptr;
std::vector<Cell*> stack;
bool play = false;
Player player = { 0, 0 }; // Initialize player at the starting position
std::vector<Ghost> ghosts; // List of ghosts
Sound collide; // Collision sound
Music music; // Music stream

// Collision management variables
bool isColliding = false; // Track if the player has collided
float collisionTime = 0.0f; // Timer for how long to pause the music

// Function declarations
void InitGrid();
void DrawMaze();
void MazeGenerator();
void UpdatePlayer();
void InitGhosts();
void UpdateGhosts();
void DrawLevel2();
void UpdateLevel2();
// Function to initialize the grid
void InitGrid() {
    for (int r = 0; r < numrows; r++) {
        for (int c = 0; c < ncols; c++) {
            grid[r * ncols + c].r = r;
            grid[r * ncols + c].c = c;
            if (r > 0) grid[r * ncols + c].neighbors.push_back(&grid[(r - 1) * ncols + c]); // Top neighbor
            if (c < ncols - 1) grid[r * ncols + c].neighbors.push_back(&grid[r * ncols + (c + 1)]); // Right neighbor
            if (r < numrows - 1) grid[r * ncols + c].neighbors.push_back(&grid[(r + 1) * ncols + c]); // Bottom neighbor
            if (c > 0) grid[r * ncols + c].neighbors.push_back(&grid[r * ncols + (c - 1)]); // Left neighbor
        }
    }
}

void DrawMaze() {
    for (int r = 0; r < numrows; r++) {
        for (int c = 0; c < ncols; c++) {
            Cell& cell = grid[r * ncols + c];
            // Draw cell background
            if (cell.visited) {
                DrawRectangle(c * cellSize, r * cellSize, cellSize, cellSize, DARKGRAY);
            }
            // Draw walls
            if (cell.walls[0]) DrawLine(c * cellSize, r * cellSize, (c + 1) * cellSize, r * cellSize, BLACK); // Top
            if (cell.walls[1]) DrawLine((c + 1) * cellSize, r * cellSize, (c + 1) * cellSize, (r + 1) * cellSize, BLACK); // Right
            if (cell.walls[2]) DrawLine(c * cellSize, (r + 1) * cellSize, (c + 1) * cellSize, (r + 1) * cellSize, BLACK); // Bottom
            if (cell.walls[3]) DrawLine(c * cellSize, r * cellSize, c * cellSize, (r + 1) * cellSize, BLACK); // Left
        }
    }

    // Draw the player's frame and texture (Harry)
    int playerFrameX = player.c * cellSize; // X position of the player's frame
    int playerFrameY = player.r * cellSize; // Y position of the player's frame
    DrawRectangle(playerFrameX, playerFrameY, cellSize - 1, cellSize - 1, DARKGRAY); // Frame background

    // Draw the player texture (scaled)
    float playerScale = 0.1f; // Scale the player texture to 5% of original size
    DrawTextureEx(player.texture,
                  Vector2{playerFrameX + (cellSize - player.texture.width * playerScale) / 2,
                          playerFrameY + (cellSize - player.texture.height * playerScale) / 2},
                  0.0f, playerScale, WHITE);

    // Draw ghosts inside a frame
    float ghostScale = 0.05f; // Scale ghosts to 8% of original size
    for (const Ghost& ghost : ghosts) {
        int ghostFrameX = ghost.c * cellSize;
        int ghostFrameY = ghost.r * cellSize;
        DrawRectangle(ghostFrameX, ghostFrameY, cellSize - 1, cellSize - 1, DARKGRAY); // Frame background

        DrawTextureEx(ghost.texture,
                      Vector2{ghostFrameX + (cellSize - ghost.texture.width * ghostScale) / 2,
                              ghostFrameY + (cellSize - ghost.texture.height * ghostScale) / 2},
                      0.0f, ghostScale, WHITE); // Draw the ghost texture
    }
}


void MazeGenerator() {
    if (!current->visited) {
        current->visited = true;
        stack.push_back(current);
    }

    std::vector<Cell*> unvisited;
    for (Cell* neigh : current->neighbors) {
        if (!neigh->visited) {
            unvisited.push_back(neigh);
        }
    }

    if (!unvisited.empty()) {
        int randomIndex = std::rand() % unvisited.size();
        Cell* next = unvisited[randomIndex];

        // Remove walls
        if (current->r - next->r == 1) { // Top
            current->walls[0] = false;
            next->walls[2] = false;
        } else if (current->c - next->c == 1) { // Left
            current->walls[3] = false;
            next->walls[1] = false;
        } else if (current->r - next->r == -1) { // Bottom
            current->walls[2] = false;
            next->walls[0] = false;
        } else if (current->c - next->c == -1) { // Right
            current->walls[1] = false;
            next->walls[3] = false;
        }

        current = next;
    } else if (!stack.empty()) {
        current = stack.back();
        stack.pop_back();
    } else {
        play = true; // Maze generation completed
    }
}

void UpdatePlayer() {
    // Check for player movement
    if (IsKeyDown(KEY_UP) && !grid[player.r * ncols + player.c].walls[0]) { // Up
        player.r--;
    }
    if (IsKeyDown(KEY_RIGHT) && !grid[player.r * ncols + player.c].walls[1]) { // Right
        player.c++;
    }
    if (IsKeyDown(KEY_DOWN) && !grid[player.r * ncols + player.c].walls[2]) { // Down
        player.r++;
    }
    if (IsKeyDown(KEY_LEFT) && !grid[player.r * ncols + player.c].walls[3]) { // Left
        player.c--;}
        // Check for collision with Hermione
        if (player.r == hermione.r && player.c == hermione.c) { // Update this line to check against Hermione's position
            gameWon = true; // Set the win flag
        }
    
}
// Function to initialize ghosts
void InitGhosts() {
    for (int i = 0; i < numGhosts; i++) {
        Ghost ghost;
        ghost.r = std::rand() % numrows;
        ghost.c = std::rand() % ncols;
        ghost.texture = LoadTexture("./images/ghost.png"); // Load ghost texture
        ghosts.push_back(ghost);
    }
}

// Function to update ghosts
void UpdateGhosts() {
    for (Ghost& ghost : ghosts) {
        int direction = std::rand() % 4; // Random direction: 0 = up, 1 = right, 2 = down, 3 = left

        int newR = ghost.r;
        int newC = ghost.c;

        switch (direction) {
            case 0: newR--; break; // Up
            case 1: newC++; break; // Right
            case 2: newR++; break; // Down
            case 3: newC--; break; // Left
        }

        // Check for valid move
        if (newR >= 0 && newR < numrows && newC >= 0 && newC < ncols && 
            !grid[ghost.r * ncols + ghost.c].walls[direction]) {
            ghost.r = newR;
            ghost.c = newC;
        }

        // Check for collision with player
        if (ghost.r == player.r && ghost.c == player.c) {
            // Reset player to initial position
            PlaySound(collide); // Play collision sound
            player.r = 0;
            player.c = 0;

            // Stop the music and set collision flag
            StopMusicStream(music);
            isColliding = true;
            collisionTime = 3.0f; // Set collision duration (3 second)
        }
    }
}

// UPDATE level 2 function, including all elements
void UpdateLevel2() {
    static bool initialized = false; // Static variable to track initialization

    if (!initialized) {
        // Load sounds and music after initializing audio device
        collide = LoadSound("./Audio/ghostcollide.wav"); // Load collision sound
        music = LoadMusicStream("./Audio/sound.wav"); // Load music stream
        PlayMusicStream(music); // Start playing music

        std::srand(static_cast<unsigned>(std::time(0))); // Seed random number generator

        InitGrid(); // Initialize the grid
        current = &grid[0]; // Start from the first cell

        // Load player texture
        player.texture = LoadTexture("./images/harry.png"); // Ensure this path is correct
        hermione.texture = LoadTexture("./images/hermione.png");
        hermione.r = numrows - 1; // Set Hermione's position to the last row
        hermione.c = ncols - 1; // Set Hermione's position to the last column

        InitGhosts(); // Initialize ghosts

        initialized = true; // Set initialization flag to true
    }
     if (!play) {
        MazeGenerator(); // Continue generating the maze until complete
    } else {
        UpdatePlayer(); // Update player position once the maze is generated
        UpdateGhosts(); // Update ghosts
    }

    // Update the music stream
    UpdateMusicStream(music); // Ensure music plays smoothly

    // Handle collision timing
    if (isColliding) {
        collisionTime -= GetFrameTime(); // Decrease the collision timer
        if (collisionTime <= 0.0f) {
            PlayMusicStream(music); // Resume music after the pause
            isColliding = false; // Reset collision flag
        }
    }

}

// DRAW level 2 function
void DrawLevel2() {
     

    ClearBackground(RAYWHITE); // Clear the screen
    DrawMaze(); // Draw the maze
    DrawHermione(hermione, cellSize); // Draw Hermione

    // If the game is won, display the "You Win!" message
    if (gameWon) {
        DrawText("You Win!", ncols * cellSize / 2 - MeasureText("You Win!", 20) / 2, numrows * cellSize / 2 - 10, 20, GREEN);
    }
  
}
