#include <iostream>
#include "raylib.h"
#include "raymath.h"

// DONE: Load fragment and vertex shaders
// TODO 2: Create a function to put textures into 3D space, based on the way
// billboard functions do it and the gists (see project TODO!)
// TODO 3: Find & load a fish texture, and translate the vertex shader from the
// docs! Add controls, for the uniforms and stuff (see shader comments).
// TODO 4: Make the fish follow a circle, after making it into a nice class
// TODO 5: Make multiple fish follow a circle, in a nice std::vector, with
// uniqe::ptrs? Was that just for models? See how well they work, here though! I
// might need it for the Shader and Texture. Test a 1000 fish (pick from a list of textures)
// TODO 6: Mod the fragment shader, to indicate the "depth" in the merry-go-round
// TODO 7: Add behaviours, and patterns to the fishies, varying the speeds, etc, and AI state machine!

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1080;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "fishy merry-go-round");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = Vector3 { 5.0f, 4.0f, 5.0f };    // Camera position
    camera.target = Vector3 { 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3 { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Texture2D bill = LoadTexture("resources/longish-fish.png");    

    Shader daShader = LoadShader("resources/shaders/fishymovement.vs", "resources/shaders/transparent.fs");
    int timeLoc = GetShaderLocation(daShader, "time");
    float timeNow = 0.0f;
    SetShaderValue(daShader, timeLoc, &timeNow, SHADER_UNIFORM_FLOAT);
    
    // Entire billboard texture, source is used to take a segment from a larger texture.
    Rectangle source = { 0.0f, 0.0f, (float)bill.width, (float)bill.height };

    Vector3 billUp = { 0.0f, 1.0f, 0.0f };

    // Rotate around origin
    // Here we choose to rotate around the image centre
    // NOTE: (-1, 1) is the range where origin.x, origin.y is inside the texture
    Vector2 rotateOrigin = { 0.0f };

    // Distance is needed for the correct billboard draw order
    // Larger distance (further away from the camera) should be drawn prior to smaller distance.
    float rotation = 0.0f;

    SetTargetFPS(60);                   // TODO: Remove this and instance 1000 fish

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
	{
	    // Update
	    UpdateCamera(&camera, CAMERA_FIRST_PERSON);

	    timeNow = (float) GetTime();
	    SetShaderValue(daShader, timeLoc, &timeNow, SHADER_UNIFORM_FLOAT);
	    
	    // Draw
	    BeginDrawing();
            ClearBackground(LIGHTGRAY);

            BeginMode3D(camera);

	    DrawGrid(10, 1.0f);        // Draw a grid

	    BeginShaderMode(daShader);
	    DrawBillboardPro(camera, bill, source, (Vector3) {0.0f, 0.0f, 0.0f}, billUp, (Vector2) {1.0f, 1.0f}, rotateOrigin, rotation, WHITE);
	    EndShaderMode();
                
            EndMode3D();

            DrawFPS(10, 10);

	    EndDrawing();

	}

    // De-Initialization
    UnloadTexture(bill);        // Unload texture
    CloseWindow();              // Close window and OpenGL context

    return 0;
}
