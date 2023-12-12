#ifndef _FISH_HPP_
#define _FISH_HPP_
#include <raylib.h>
#include <string>

class Fish
{
public:
    Fish(Vector2 offset, Vector2 axes, std::string texturePath, Shader shader, float timeScale=10.0f);
    ~Fish();

    void Update(float timeNow);
    void Draw();
    
private:
    static Mesh GenMeshPlaneXY(float width, float length, int resX, int resY);
    float timeScale;
    
    Vector2 axes;
    Vector2 offsets;
    Vector3 pos;
    
    Matrix transform;

    Material fishMat;
    Mesh fishMesh;
    Texture2D fishTex;
};

#endif /* _FISH_HPP_ */
