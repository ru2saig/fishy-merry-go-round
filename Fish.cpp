#include <Fish.hpp>
#include <raylib.h>
#include <raymath.h>
#include <Utility.hpp>
#include <numbers>

Fish::Fish(Vector2 offsets, Vector2 axes, std::string texturePath, Shader shader, float timeScale)
    : timeScale { timeScale }, axes { axes }, offsets { offsets }, pos { Vector3Zero() }
{
    Image fishImage = LoadImage(texturePath.c_str());
    ImageFlipVertical(&fishImage);
    fishTex = LoadTextureFromImage(fishImage);

    fishMat = LoadMaterialDefault();
    fishMat.maps[MATERIAL_MAP_DIFFUSE].texture = fishTex;
    fishMat.shader = shader;

    pos.x = axes.x * cos(-offsets.x * timeScale);
    pos.z = axes.y * sin(-offsets.x * timeScale);
    pos.y = offsets.y;

    fishMesh = GenMeshPlaneXY(1.0f, 1.0f, 5, 5); // TODO: Reduce these to 10? 15? 5?

    UnloadImage(fishImage);
}

void Fish::Update(float t)
{
    // Move along the (ellipsoid) path
    t += offsets.x;

    pos.x = axes.x * cos(-t * timeScale);
    pos.z = axes.y * sin(-t * timeScale);
    pos.y = offsets.y;
    
    // Orient the fish texture along the direction of movement
    transform = MatrixMultiply(MatrixRotateY(PI/2 + t * timeScale), MatrixTranslate(pos.x, pos.y, pos.z));
}

void Fish::Draw()
{
    DrawMesh(fishMesh, fishMat, transform);

    // A plane and the path for debugging purposes
    //DrawMesh(fishMesh, LoadMaterialDefault(), transform); // Yes, this is the fastest but least performant way to test stuff
    //for (double i = 0.0, step = 0.1; i < std::numbers::pi_v<double> * 2; i += step)
    //DrawSphere({ axes.x * (float) cos(i), offsets.y, axes.y * (float) sin(i)}, 0.1, BLACK);
}

Fish::~Fish()
{ // is one of these unloading the shader as well?
    UnloadTexture(fishTex);
    UnloadMesh(fishMesh);
    UnloadMaterial(fishMat);
}    
