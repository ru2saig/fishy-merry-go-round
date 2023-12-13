#include <Fish.hpp>
#include <FishManager.hpp>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

// TODO: Add the scanner stuff!
int main()
{
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    InitWindow(screenWidth, screenHeight, "fishy merry-go-round");

    // TODO: Adjust this!
    Camera camera = { 0 };
    camera.position = Vector3 { 25.0f, 0.0f, 0.0f };    // Camera position
    camera.target = Vector3 { 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3 { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    FishManager fm = FishManager();
    
    SetTargetFPS(60);
    DisableCursor();
    ToggleFullscreen();

    // Main game loop
    while (!WindowShouldClose()) {
	// Update

        UpdateCamera(&camera, CAMERA_FREE); // TODO: best look?

	fm.Update();

			
	// Draw
	BeginDrawing();
	ClearBackground(LIGHTGRAY);

	BeginMode3D(camera);
        DrawGrid(10, 1.0f);

	// this "fades" further fish, couldn't get the shader to work, yet
	rlPushMatrix();
	rlRotatef(-90.0, 0.0, 0.0, 1.0);
	DrawPlane(Vector3Zero(), Vector2 { 100.0f, 100.0f }, Color { 0, 0, 0, 50 }); // a dark, blueish gray
	rlPopMatrix();
	
	fm.Draw();

	EndMode3D();

	DrawFPS(10, 10);

	EndDrawing();

    }

    // De-Initialization
    CloseWindow();              // Close window and OpenGL context

    return 0;
}
