#include <cmath>
#include <random>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <utility>

// TODO: Drawing the fish using a mesh
// TODO 3: Find & load a fish texture, and translate the vertex shader from the
// docs! Add controls, for the uniforms and stuff (see shader comments).
// TODO 4: Make the fish follow a circle, after making it into a nice class
// TODO 5: Make multiple fish follow a circle, in a nice std::vector, with
// uniqe::ptrs? Was that just for models? See how well they work, here though! I
// might need it for the Shader and Texture. Test a 1000 fish (pick from a list of textures)
// TODO 6: Mod the fragment shader, to indicate the "depth" in the merry-go-round
// TODO 7: Add behaviours, and patterns to the fishies, varying the speeds, etc, and AI state machine!


Mesh GenMeshPlaneXY(float width, float length, int resX, int resY)
{
    Mesh mesh = { 0 };
    resX++;
    resY++;

    // Vertices definition
    int vertexCount = resX*resY; // vertices get reused for the faces

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


int main(void)
{
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

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
    Texture2D fishTex = LoadTextureFromImage(fishImage);
        
    Shader fishyShader = LoadShader("resources/shaders/fishymovement.vs", "resources/shaders/transparent.fs");
    int timeLoc = GetShaderLocation(fishyShader, "time");
    float timeNow = 0.0f;
    SetShaderValue(fishyShader, timeLoc, &timeNow, SHADER_UNIFORM_FLOAT);

    Material fishMat = LoadMaterialDefault();
    fishMat.maps[MATERIAL_MAP_DIFFUSE].texture = fishTex;
    fishMat.shader = fishyShader;

    Mesh fishMesh = GenMeshPlaneXY(4.0, 4.0, 10, 10);
            
    SetTargetFPS(60);
    DisableCursor();
    ToggleFullscreen();
    
    std::default_random_engine gen;
    std::uniform_real_distribution<float> uniform_dist(-20.0f, 20.0f);
    std::vector<std::pair<Vector3, float>> poses;

    // for(int i = 0; i < 1000; i++)
    //  	poses.emplace_back(Vector3 {uniform_dist(gen), uniform_dist(gen), uniform_dist(gen)}, uniform_dist(gen) * 10.0f);
    poses.emplace_back(Vector3 { 0.0f, 0.0f, 0.0f }, 0.0);

    rlDisableBackfaceCulling();
            
    // Main game loop
    while (!WindowShouldClose()) {
	// Update
	UpdateCamera(&camera, CAMERA_FREE);
	
	
	timeNow = (float) GetTime();
	SetShaderValue(fishyShader, timeLoc, &timeNow, SHADER_UNIFORM_FLOAT);

	// Draw
	BeginDrawing();
	ClearBackground(LIGHTGRAY);

	BeginMode3D(camera);
        DrawGrid(10, 1.0f);

	for (auto pose : poses) {
	    Matrix m = MatrixMultiply(MatrixTranslate(pose.first.x, pose.first.y, pose.first.z), MatrixRotate(Vector3 { 0.0, 1.0, 0.0 }, pose.second));
	    DrawMesh(fishMesh, fishMat, m);
	}

	EndMode3D();

	DrawFPS(10, 10);

	EndDrawing();

    }

    // De-Initialization
    UnloadTexture(fishTex);        // Unload texture
    UnloadImage(fishImage);
    UnloadShader(fishyShader);
    
    CloseWindow();              // Close window and OpenGL context

    return 0;
}
