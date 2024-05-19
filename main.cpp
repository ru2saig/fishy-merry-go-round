#include <Environment.hpp>
#include <Fish.hpp>
#include <FishManager.hpp>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <cstdlib>

int main(int argc, char** argv)
{
    InitWindow(GetScreenWidth(), GetScreenHeight(), "fishy merry-go-round");

    Camera camera = { 0 };
    camera.position = Vector3 { 25.0f, 0.0f, 0.0f };    // Camera position
    camera.target = Vector3 { 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3 { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    SetTargetFPS(60);
    DisableCursor();
    ToggleFullscreen();

    FishManager fm = FishManager();
    Environment env = Environment::instance();
    RenderTexture2D target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    Shader rays = LoadShader(0, "resources/shaders/rays.fs");

    int timeLoc = GetShaderLocation(rays, "time");
    int resLoc = GetShaderLocation(rays, "res");
    float res[2] = {(float) GetScreenWidth(), (float) GetScreenHeight()};
    SetShaderValue(rays, resLoc, res, SHADER_UNIFORM_VEC2);
        
    // Main game loop
    while (!WindowShouldClose()) {

	// Update
	UpdateCameraPro(&camera, Vector3Zero(), Vector3Zero(), 0.0f);
	fm.Update();
	env.Update();

	float time = GetTime();
	SetShaderValue(rays, timeLoc, &time, SHADER_UNIFORM_FLOAT);
				
	// Draw
	BeginTextureMode(target);
	ClearBackground(LIGHTGRAY);

	BeginMode3D(camera);
		
	env.Draw();
	fm.Draw();

	EndMode3D();
	EndTextureMode();

	BeginDrawing();
	ClearBackground(LIGHTGRAY);
	BeginShaderMode(rays);
	DrawTextureRec(target.texture, { 0, 0, (float)target.texture.width, (float)-target.texture.height }, { 0, 0 }, LIGHTGRAY);
	EndShaderMode();
	EndDrawing();
	
    }

    CloseWindow();
    UnloadShader(rays);
    
    return 0;
}
