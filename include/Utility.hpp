#ifndef _UTILITY_HPP_
#define _UTILITY_HPP_
#include <raylib.h>
#include <raymath.h>
#include <filesystem>

namespace Utility {
    // TODO: What about installing this? What ever would become of these?
    //       Should the luancher set the correct working directories?
    //       What are some good strategies for this?
    const std::filesystem::path texDir = std::filesystem::current_path().concat("/resources/bg/");
    const std::filesystem::path shaderDir = std::filesystem::current_path().concat("/resources/shaders/");
    const std::filesystem::path fishDir = std::filesystem::current_path().concat("/resources/textures/");

    Mesh GenMeshPlaneXY(float width, float length, int resX, int resY);
    void DrawTexture3D(Texture2D texture, Vector3 position, float rotation, Vector3 axis, float scaleX, float scaleY, Color tint);
};

#endif /* _UTILITY_HPP_ */
