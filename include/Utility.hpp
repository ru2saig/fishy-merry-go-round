#ifndef _UTILITY_HPP_
#define _UTILITY_HPP_
#include <raylib.h>
#include <raymath.h>

Mesh GenMeshPlaneXY(float width, float length, int resX, int resY);
void DrawTexture3D(Texture2D texture, Vector3 position, float rotation, Vector3 axis, float scaleX, float scaleY, Color tint);

#endif /* _UTILITY_HPP_ */
