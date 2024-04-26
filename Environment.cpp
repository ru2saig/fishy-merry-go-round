#include <Environment.hpp>
#include <Utility.hpp>
#include <raymath.h>
#include <rlgl.h>
#include <raylib.h>

Environment& Environment::instance()
{
  static Environment* env = new Environment();
  return *env;
}

Environment::Environment()
{
    auto staticVertexPath = shaderDir / "base.vs";
    auto wiggleVertexPath = shaderDir / "wiggle.vs";
    auto fragShaderPath = shaderDir / "transparent.fs";

    staticShader = LoadShader(staticVertexPath.c_str(), fragShaderPath.c_str());
    wiggleShader = LoadShader(wiggleVertexPath.c_str(), fragShaderPath.c_str());

    timeLoc = GetShaderLocation(wiggleShader, "time");
    float timeNow = 0.0f;
    SetShaderValue(wiggleShader, timeLoc, &timeNow, SHADER_UNIFORM_FLOAT);
    
    for (int i = 0; i < static_cast<int>(TextureIndex::TEXTURES); i++) {
	auto texturePath = texDir / textureNames[i];

        Image texture = LoadImage(texturePath.c_str());
	ImageFlipVertical(&texture);

        textures[i] = LoadTextureFromImage(texture);
	UnloadImage(texture);
    }
}

void Environment::Update()
{
    float timeNow = (float)GetTime();
    SetShaderValue(wiggleShader, timeLoc, &timeNow, SHADER_UNIFORM_FLOAT);
}

// TODO: How can I boost performance? Textures totally nuked it, but is there any way to use math to
//       reduce matrix operations (boost performance)?
void Environment::Draw()
{
    // TODO: programmatically obtain scaling data, depending on screen
    // resolution
    BeginShaderMode(staticShader); // all the static stuff
    DrawTexture3D(textures[static_cast<int>(TextureIndex::bg)], Vector3 { -20.0f, 0.0f, 0.0f }, 90.0, Vector3 { 0.0f, 1.0f, 0.0f }, 38.0, 19, WHITE);
    DrawTexture3D(textures[static_cast<int>(TextureIndex::mg)], Vector3 { 17.9f, -1.2f, 0.0f }, 90.0, Vector3 { 0.0f, 1.0f, 0.0f }, 5.3, 2.15, WHITE);
    DrawTexture3D(textures[static_cast<int>(TextureIndex::fg)], Vector3 { 18.0f, -0.8f, 0.0f }, 90.0, Vector3 { 0.0f, 1.0f, 0.0f }, 5.3, 2.15, WHITE);
    EndShaderMode();

    BeginShaderMode(wiggleShader);
    DrawTexture3D(textures[static_cast<int>(TextureIndex::kelp)], Vector3{-1.0f, -5.0f, 0.0f}, 90.0, Vector3{0.0f, 1.0f, 0.0f}, 21.0, 10.0, WHITE);    
    DrawTexture3D(textures[static_cast<int>(TextureIndex::mgDynamic)], Vector3{17.9f, -1.5f, 0.0f}, 90.0, Vector3{0.0f, 1.0f, 0.0f}, 5.3, 2.15, WHITE);
    DrawTexture3D(textures[static_cast<int>(TextureIndex::fgDynamic)], Vector3 { 18.0f, -0.8, 0.0f }, 90.0, Vector3 { 0.0f, 1.0f, 0.0f }, 5.3, 2.15, WHITE);
    EndShaderMode();
    
    rlPushMatrix();
    rlRotatef(-90.0, 0.0, 0.0, 1.0);
    // this "fades" further fish, couldn't get the shader to work, yet
    DrawPlane(Vector3Zero(), Vector2 { 100.0f, 100.0f }, Color { 0, 0, 0, 50 }); // a dark, blueish gray
    rlPopMatrix();
}

Environment::~Environment()
{
    for (int i = 0; i < static_cast<int>(TextureIndex::TEXTURES); i++)
	UnloadTexture(textures[i]);

    UnloadShader(staticShader);
    UnloadShader(wiggleShader);
}    
