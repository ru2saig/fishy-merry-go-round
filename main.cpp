#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <Fish.hpp>

int main(void)
{
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    InitWindow(screenWidth, screenHeight, "fishy merry-go-round");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = Vector3 { 5.0f, 2.0f, 0.0f };    // Camera position
    camera.target = Vector3 { 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3 { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Shader fishyShader = LoadShader("resources/shaders/fishymovement.vs", "resources/shaders/transparent.fs");
    int timeLoc = GetShaderLocation(fishyShader, "time");
    float timeNow = 0.0f;
    SetShaderValue(fishyShader, timeLoc, &timeNow, SHADER_UNIFORM_FLOAT);

    Fish fish = Fish(Vector2Zero(), Vector2 { 10.0f, 10.0f }, "resources/fish-1.png", fishyShader);
    Fish fish2 = Fish(Vector2 {1.0f, 0.0f}, Vector2 { 10.0f, 10.0f }, "resources/fish-1.png", fishyShader);

    SetTargetFPS(60);
    DisableCursor();
    ToggleFullscreen();
    
    // std::default_random_engine gen;
    // std::uniform_real_distribution<float> uniform_dist(-20.0f, 20.0f);
    // std::vector<std::pair<Vector3, float>> poses; 

    // for(int i = 0; i < 1000; i++)
    //  	poses.emplace_back(Vector3 {uniform_dist(gen), uniform_dist(gen), uniform_dist(gen)}, uniform_dist(gen) * 10.0f);
    
    rlDisableBackfaceCulling(); // required to render texture on both sides of the plane

    // Main game loop
    while (!WindowShouldClose()) {
	// Update
	UpdateCamera(&camera, CAMERA_FREE);
	
	// set shader uniforms
	timeNow = (float) GetTime();
	SetShaderValue(fishyShader, timeLoc, &timeNow, SHADER_UNIFORM_FLOAT);

	fish.Update(timeNow);
	fish2.Update(timeNow);
	    
	// Draw
	BeginDrawing();
	ClearBackground(LIGHTGRAY);

	BeginMode3D(camera);
        DrawGrid(10, 1.0f);

	rlPushMatrix();
	rlRotatef(90.0, 0.0, 0.0, 1.0);
	DrawPlane(Vector3Zero(), Vector2 { 100.0f, 100.0f }, Color { 0, 0, 0, 50 });
	rlPopMatrix();

        fish.Draw();
	fish2.Draw();
	
	EndMode3D();

	DrawFPS(10, 10);

	EndDrawing();

    }

    // De-Initialization
    CloseWindow();              // Close window and OpenGL context

    return 0;
}
