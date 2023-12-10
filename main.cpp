#include <cmath>
#include <random>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <utility>

// IN-PROGRESS 3: Find & load a fish texture, and translate the vertex shader from the
// docs! Add controls, for the uniforms and stuff (see shader comments).
// TODO 4: Make the fish follow a circle, after making it into a nice class
// TODO 5: Make multiple fish follow a circle, in a nice std::vector, with
// uniqe::ptrs? Was that just for models? See how well they work, here though! I
// might need it for the Shader and Texture. Test a 1000 fish (pick from a list of textures)
// TODO 6: Mod the fragment shader, to indicate the "depth" in the merry-go-round
// TODO 7: Add behaviours, and patterns to the fishies, varying the speeds, etc, and AI state machine!

void DrawTexture3D(Texture2D texture, Vector3 position,
		   float rotation, Vector3 axis, float scale,
		   Color tint);
    
int main(void)
{
    const int screenWidth = 1080;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "fishy merry-go-round");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = Vector3 { 5.0f, 2.0f, 0.0f };    // Camera position
    camera.target = Vector3 { 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3 { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Image fishImage = LoadImage("resources/longish-fish.png");
    ImageFlipVertical(&fishImage);
    Texture2D fish = LoadTextureFromImage(fishImage);
        
    Shader fishyShader = LoadShader("resources/shaders/fishymovement.vs", "resources/shaders/transparent.fs");
    int timeLoc = GetShaderLocation(fishyShader, "time");
    float timeNow = 0.0f;
    SetShaderValue(fishyShader, timeLoc, &timeNow, SHADER_UNIFORM_FLOAT);

    SetTargetFPS(60);

    // std::default_random_engine gen;
    // std::uniform_real_distribution<float> uniform_dist(-20.0f, 20.0f);
    // std::vector<std::pair<Vector3, float>> poses;

    // for(int i = 0; i < 100000; i++)
    // 	poses.emplace_back(Vector3 {uniform_dist(gen), uniform_dist(gen), uniform_dist(gen)}, uniform_dist(gen) * 10.0f);
    Vector3 pos = Vector3 {0.0f, 0.0f, 0.0f};

    // Main game loop
    while (!WindowShouldClose()) {
	// Update
	UpdateCamera(&camera, CAMERA_FIRST_PERSON);
	
	
	timeNow = (float) GetTime();
	SetShaderValue(fishyShader, timeLoc, &timeNow, SHADER_UNIFORM_FLOAT);

	//pos.x = 5*sin(timeNow * 0.5);
	
	// Draw
	BeginDrawing();
	ClearBackground(LIGHTGRAY);

	BeginMode3D(camera);

	DrawGrid(10, 1.0f);

	BeginShaderMode(fishyShader);
	DrawTexture3D(fish, pos, 0.0f, Vector3 { 0.0, 1.0, 0.0 }, 1.0, WHITE);
	EndShaderMode();

	EndMode3D();

	DrawFPS(10, 10);

	EndDrawing();

    }

    // De-Initialization
    UnloadTexture(fish);        // Unload texture
    UnloadImage(fishImage);
    UnloadShader(fishyShader);
    
    CloseWindow();              // Close window and OpenGL context

    return 0;
}

void DrawTexture3D(Texture2D texture, Vector3 position, float rotation, Vector3 axis, float scale, Color tint)
{

    rlSetTexture(texture.id);


    rlPushMatrix();

    // apply transformations
    rlTranslatef(position.x, position.y, position.z);
    rlRotatef(rotation, axis.x, axis.y, axis.z);

    rlBegin(RL_QUADS);
    rlColor4ub(tint.r, tint.g, tint.b, tint.a);

    // draw the front face
    rlNormal3f(0.0f, 0.0f, 1.0f);
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(-scale, -scale, 0.0f);
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(-scale, scale, 0.0f);
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(scale, scale, 0.0f);
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(scale, -scale, 0.0f);

    // draw the back face
    rlNormal3f(0.0f, 0.0f, -1.0f);
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(-scale, -scale, 0.0f);
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(scale, -scale, 0.0f);
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(scale, scale, 0.0f);
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(-scale, scale, 0.0f);

    
    rlEnd();
    rlPopMatrix();
    
    rlSetTexture(0);
}
