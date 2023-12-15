#include <Fish.hpp>
#include <raylib.h>
#include <raymath.h>
#include <Utility.hpp>

Fish::Fish(Vector2 offsets, Vector2 axes, std::string texturePath, Shader shader, float timeScale)
    : timeScale { timeScale }, axes { axes }, offsets { offsets }, pos { Vector3Zero() }
{
    Image fishImage = LoadImage(texturePath.c_str());
    ImageFlipVertical(&fishImage);
    fishTex = LoadTextureFromImage(fishImage);

    fishMat = LoadMaterialDefault();
    fishMat.maps[MATERIAL_MAP_DIFFUSE].texture = fishTex;
    fishMat.shader = shader;

    fishMesh = GenMeshPlaneXY(1.0f, 1.0f, 5, 5); // TODO: Reduce these to 10? 15? 5?

    UnloadImage(fishImage);
}

void Fish::Update(float timeNow)
{
    // Move along the (ellipsoid) path
    timeNow += offsets.x;

    pos.x = axes.x * cos(-timeNow/timeScale);
    pos.z = axes.y * sin(-timeNow/timeScale);

    // Orient the fish texture along the direction of movement
    // pos.y = offset.y (as it's 0)
    transform = MatrixMultiply(MatrixRotateY(PI/2 + timeNow/timeScale), MatrixTranslate(pos.x, pos.y + offsets.y, pos.z));
}

void Fish::Draw()
{
    DrawMesh(fishMesh, fishMat, transform);
    // A plane for debugging purposes
    //DrawMesh(fishMesh, LoadMaterialDefault(), transform);
}

Fish::~Fish()
{
    UnloadTexture(fishTex);
    UnloadMesh(fishMesh);
    UnloadMaterial(fishMat);
}    
