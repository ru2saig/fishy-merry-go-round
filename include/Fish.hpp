#ifndef _FISH_HPP_
#define _FISH_HPP_
#include <raylib.h>
#include <string>
#include <array>

class Fish
{
public:
    Fish(Vector2 offset, Vector2 axes, std::string texturePath, Shader shader, float timeScale);
    ~Fish();

    void Update(float t);
    const std::array<float, 5>& getValues() { return uniformValues; };
    void Draw();
    Vector3 GetPosition() { return pos; }

private:
    float timeScale;

    std::array<float, 5> uniformValues;
    Vector2 axes;
    Vector2 offsets;
    Vector3 pos;
    
    Matrix transform;

    Material fishMat;
    Mesh fishMesh;
    Texture2D fishTex;
};

#endif /* _FISH_HPP_ */
