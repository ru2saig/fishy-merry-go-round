#include <Utility.hpp>
#include <rlgl.h>
#include <raylib.h>

// Could have just rotated the plane, in the transform, but that seemed too much of a pain! (it rhymes)
Mesh Utility::GenMeshPlaneXY(float width, float length, int resX, int resY)
{
    Mesh mesh = { 0 };
    resX++;
    resY++;

    // Vertices definition
    int vertexCount = resX*resY; // Vertices get reused for the faces

    Vector3 *vertices = (Vector3 *)RL_MALLOC(vertexCount*sizeof(Vector3));
    for (int y = 0; y < resY; y++)
	{
	    // [-length/2, length/2]
	    float yPos = ((float)y/(resY - 1) - 0.5f)*length;
	    for (int x = 0; x < resX; x++)
		{
		    // [-width/2, width/2]
		    float xPos = ((float)x/(resX - 1) - 0.5f)*width;
		    vertices[x + y*resX] = Vector3 { xPos, yPos, 0.0f };
		}
	}

    // Normals definition
    Vector3 *normals = (Vector3 *)RL_MALLOC(vertexCount*sizeof(Vector3));
    for (int n = 0; n < vertexCount; n++) normals[n] = Vector3 { 0.0f, 0.0f, 1.0f }; 

    // TexCoords definition
    Vector2 *texcoords = (Vector2 *)RL_MALLOC(vertexCount*sizeof(Vector2));
    for (int v = 0; v < resY; v++)
	{
	    for (int u = 0; u < resX; u++)
		{
		    texcoords[u + v*resX] = Vector2 { (float)u/(resX - 1), (float)v/(resY - 1) };
		}
	}

    // Triangles definition (indices)
    int numFaces = (resX - 1)*(resY - 1);
    int *triangles = (int *)RL_MALLOC(numFaces*6*sizeof(int));
    int t = 0;
    for (int face = 0; face < numFaces; face++)
	{
	    // Retrieve lower left corner from face ind
	    int i = face + face / (resX - 1);

	    triangles[t++] = i + resX;
	    triangles[t++] = i + 1;
	    triangles[t++] = i;

	    triangles[t++] = i + resX;
	    triangles[t++] = i + resX + 1;
	    triangles[t++] = i + 1;
	}

    mesh.vertexCount = vertexCount;
    mesh.triangleCount = numFaces*2;
    mesh.vertices = (float *)RL_MALLOC(mesh.vertexCount*3*sizeof(float));
    mesh.texcoords = (float *)RL_MALLOC(mesh.vertexCount*2*sizeof(float));
    mesh.normals = (float *)RL_MALLOC(mesh.vertexCount*3*sizeof(float));
    mesh.indices = (unsigned short *)RL_MALLOC(mesh.triangleCount*3*sizeof(unsigned short));

    // Mesh vertices position array
    for (int i = 0; i < mesh.vertexCount; i++)
	{
	    mesh.vertices[3*i] = vertices[i].x;
	    mesh.vertices[3*i + 1] = vertices[i].y;
	    mesh.vertices[3*i + 2] = vertices[i].z;
	}

    // Mesh texcoords array
    for (int i = 0; i < mesh.vertexCount; i++)
	{
	    mesh.texcoords[2*i] = texcoords[i].x;
	    mesh.texcoords[2*i + 1] = texcoords[i].y;
	}

    // Mesh normals array
    for (int i = 0; i < mesh.vertexCount; i++)
	{
	    mesh.normals[3*i] = normals[i].x;
	    mesh.normals[3*i + 1] = normals[i].y;
	    mesh.normals[3*i + 2] = normals[i].z;
	}

    // Mesh indices array initialization
    for (int i = 0; i < mesh.triangleCount*3; i++) mesh.indices[i] = triangles[i];

    RL_FREE(vertices);
    RL_FREE(normals);
    RL_FREE(texcoords);
    RL_FREE(triangles);


    // Upload vertex data to GPU (static mesh)
    UploadMesh(&mesh, false);

    return mesh;
}

// rotation angle is in degrees
// TODO: Optimize this, without using matrices! And remove rlRotate stuff, in EnvironmentManager!
// add a boolean value for backface, although it's not used in this project...
void Utility::DrawTexture3D(Texture2D texture, Vector3 position, float rotation, Vector3 axis, float scaleX, float scaleY, Color tint)
{
    rlSetTexture(texture.id);

    rlPushMatrix();

    // apply transformations
    rlTranslatef(position.x, position.y, position.z);
    rlRotatef(rotation, axis.x, axis.y, axis.z);
    rlScalef(scaleX, scaleY, 1.0f);
    

    rlBegin(RL_QUADS);
    rlColor4ub(tint.r, tint.g, tint.b, tint.a);

    // draw the front(??) face, just one face, heh
    rlNormal3f(0.0f, 0.0f, 1.0f);
    // draws counter-clockwise from bottom-left -> bottom-right -> top-right -> top-left, to
    // ensure that the textures face the same way. Another method would be flipping the
    // vertexes instead
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(-1.0f, -1.0f, 0.0f);
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(1.0f, -1.0f, 0.0f);
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(1.0f, 1.0f, 0.0f);
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(-1.0f, 1.0f, 0.0f);

    rlEnd();
    rlPopMatrix();

    rlSetTexture(0);
}
