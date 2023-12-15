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
    // TODO: major clean up on aile 15 to 36!
    staticShader = LoadShader("resources/shaders/base.vs", "resources/shaders/transparent.fs");
    wiggleShader = LoadShader("resources/shaders/base.vs", "resources/shaders/transparent.fs");
    
    // could use a loopdey-loop
    Image bgImage = LoadImage("resources/bg/background.png");
    ImageFlipVertical(&bgImage);
    bg = LoadTextureFromImage(bgImage);

    Image fgImage = LoadImage("resources/bg/fg.png");
    ImageFlipVertical(&fgImage);
    fg = LoadTextureFromImage(fgImage);

    Image fgDynamicImage = LoadImage("resources/bg/fg-dynamic.png");
    ImageFlipVertical(&fgDynamicImage);
    fgDynamic = LoadTextureFromImage(fgDynamicImage);

    Image mgImage = LoadImage("resources/bg/mg.png");
    ImageFlipVertical(&mgImage);
    mg = LoadTextureFromImage(mgImage);

    Image mgDynamicImage = LoadImage("resources/bg/mg-dynamic.png");
    ImageFlipVertical(&mgDynamicImage);
    mgDynamic = LoadTextureFromImage(mgDynamicImage);

    Image kelpImage = LoadImage("resources/bg/kelp.png");
    ImageFlipVertical(&kelpImage);
    kelp = LoadTextureFromImage(kelpImage);
    

    UnloadImage(kelpImage);
    UnloadImage(fgImage);
    UnloadImage(fgDynamicImage);
    UnloadImage(mgImage);
    UnloadImage(mgDynamicImage);
    UnloadImage(bgImage);
    
}

void Environment::Update() {}

// TODO: How can I boost performance? Textures totally nuked it, but is there any way to use math to
//       reduce matrix operations (boost performance)?
void Environment::Draw()
{
    // TODO: programmatically obtain scaling data, depending on screen resolution
    
    BeginShaderMode(staticShader); // all the static stuff
    DrawTexture3D(bg, Vector3 { -20.0f, 0.0f, 0.0f }, 90.0, Vector3 { 0.0f, 1.0f, 0.0f }, 38.0, 19, WHITE);
    DrawTexture3D(mg, Vector3 { 17.9f, -1.2f, 0.0f }, 90.0, Vector3 { 0.0f, 1.0f, 0.0f }, 5.3, 2.15, WHITE);
    DrawTexture3D(fg, Vector3 { 18.0f, -0.8f, 0.0f }, 90.0, Vector3 { 0.0f, 1.0f, 0.0f }, 5.3, 2.15, WHITE);
    EndShaderMode();


    BeginShaderMode(wiggleShader);
    // set an "offset" uniform for each texture, so that they all move a bit different!
    DrawTexture3D(kelp, Vector3 { -1.0f, -5.0f, 0.0f }, 90.0, Vector3 { 0.0f, 1.0f, 0.0f }, 21.0, 10.0, WHITE);
    DrawTexture3D(mgDynamic, Vector3 { 17.9f, -1.5f, 0.0f }, 90.0, Vector3 { 0.0f, 1.0f, 0.0f }, 5.3, 2.15, WHITE);
    DrawTexture3D(fgDynamic, Vector3 { 18.0f, -0.8, 0.0f }, 90.0, Vector3 { 0.0f, 1.0f, 0.0f }, 5.3, 2.15, WHITE);
    EndShaderMode();

    
    rlPushMatrix();
    rlRotatef(-90.0, 0.0, 0.0, 1.0);
    // this "fades" further fish, couldn't get the shader to work, yet
    DrawPlane(Vector3Zero(), Vector2 { 100.0f, 100.0f }, Color { 0, 0, 0, 50 }); // a dark, blueish gray
    rlPopMatrix();
}
