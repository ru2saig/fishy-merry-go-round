#include <Environment.hpp>
#include <Fish.hpp>
#include <FishManager.hpp>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>


int main()
{
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    InitWindow(screenWidth, screenHeight, "fishy merry-go-round");

    Camera camera = { 0 };
    camera.position = Vector3 { 25.0f, 0.0f, 0.0f };    // Camera position
    camera.target = Vector3 { 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3 { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    FishManager fm = FishManager();
    Environment env = Environment::instance();
        
    
    SetTargetFPS(60);
    DisableCursor();
    ToggleFullscreen();


    // Main game loop
    while (!WindowShouldClose()) {
	// Update

        UpdateCameraPro(&camera, Vector3Zero(), Vector3Zero(), 0.0f);
	//UpdateCamera(&camera, CAMERA_FREE);
			
	fm.Update();
	env.Update();
				
	// Draw
	BeginDrawing();
	ClearBackground(LIGHTGRAY);

	BeginMode3D(camera);

	env.Draw();
	fm.Draw();

	EndMode3D();
	EndDrawing();
    }

    CloseWindow();              // Close window and OpenGL context

    return 0;
}
