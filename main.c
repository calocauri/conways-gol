/*******************************************************************************************
*
*   raylib [core] example - Basic 3d example
*
*   Welcome to raylib!
*
*   To compile example, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
int grid_w, grid_h;
int square_len = 10;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(int n, int w, int h, int (*a)[h], int (*b)[h]);          // Update and draw one frame
static void RandomizeGrid(int w, int h, int (*a)[h]); // Randomize grid

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;
    
    grid_w = screenWidth/square_len;
    grid_h = screenHeight/square_len;

    int state_a[grid_w][grid_h];
    int state_b[grid_w][grid_h];
   
    
    RandomizeGrid(grid_w, grid_h, state_a);

    InitWindow(screenWidth, screenHeight, "raylib");

    

    //--------------------------------------------------------------------------------------

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 15, 1);
#else
    SetTargetFPS(9);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    int n = 1;
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if(IsKeyPressed(KEY_R))
        {
            RandomizeGrid(grid_w, grid_h, n?state_a:state_b);
        }
        UpdateDrawFrame(n%2,grid_w,grid_h, n?state_a:state_b, n?state_b:state_a);
        n = !n;
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

static void RandomizeGrid(int w, int h, int (*a)[h])
{
    srand(time(NULL));
    for(int i=0; i<w; i++)
    {
        for(int j=0; j<h; j++)
        {
            a[i][j] = rand()%2;
        }
    }
}

// Update and draw game frame
static void UpdateDrawFrame(int n, int w, int h, int (*a)[h], int (*b)[h])
{
    
    // Update
    //----------------------------------------------------------------------------------
    for(int i=0; i<grid_w; i++)
    {
        for(int j=0; j<grid_h; j++)
        {
            int neighbors = 0;
            for(int di=-1; di<=1; di++)
            {
                for(int dj=-1; dj<=1; dj++)
                {
                    if(di==0 && dj==0) continue;
                    int ni = i + di;
                    int nj = j + dj;
                    if(ni>=0 && ni<grid_w && nj>=0 && nj<grid_h)
                    {
                        neighbors += a[ni][nj];
                    }
                }
            }

            // Apply Conway's Game of Life rules
            if(a[i][j])
            {
                if(neighbors < 2 || neighbors > 3)
                    b[i][j] = 0; // Cell dies
                else
                    b[i][j] = 1; // Cell lives
            }
            else
            {
                if(neighbors == 3)
                    b[i][j] = 1; // Cell becomes alive
                else
                    b[i][j] = 0; // Cell remains dead
            }
        }
    }
    
    //----------------------------------------------------------------------------------

    
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
        ClearBackground(BLACK);
        for(int i=0; i<grid_w; i++)
        {
            for(int j=0; j<grid_h; j++)
            {
                if(a[i][j])
                DrawRectangle(i*square_len+1,j*square_len+1, square_len-2, square_len-2, BEIGE);
            }
        }

    EndDrawing();
    //----------------------------------------------------------------------------------
}
