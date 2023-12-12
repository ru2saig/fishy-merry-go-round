#include <cmath>
#include <random>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <utility>
#include <Fish.hpp>

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

    Image fishImage = LoadImage("resources/fish-1.png");
    ImageFlipVertical(&fishImage);
    Texture2D fishTex = LoadTextureFromImage(fishImage);
        
    Shader fishyShader = LoadShader("resources/shaders/fishymovement.vs", "resources/shaders/transparent.fs");
    int timeLoc = GetShaderLocation(fishyShader, "time");
    float timeNow = 0.0f;
    SetShaderValue(fishyShader, timeLoc, &timeNow, SHADER_UNIFORM_FLOAT);

    int cameraPos = GetShaderLocation(fishyShader, "cameraPos");
    Vector3 cPos = camera.position;
    SetShaderValue(fishyShader, cameraPos, &cPos, SHADER_UNIFORM_VEC3);


    Material fishMat = LoadMaterialDefault();
    fishMat.maps[MATERIAL_MAP_DIFFUSE].texture = fishTex;
    fishMat.shader = fishyShader;

    Mesh fishMesh = GenMeshPlaneXY(1.0, 1.0, 20, 20);
    Material defaultMaterial = LoadMaterialDefault();
            
    SetTargetFPS(60);
    DisableCursor();
    ToggleFullscreen();
    
    // std::default_random_engine gen;
    // std::uniform_real_distribution<float> uniform_dist(-20.0f, 20.0f);
    // std::vector<std::pair<Vector3, float>> poses; 

    // for(int i = 0; i < 1000; i++)
    //  	poses.emplace_back(Vector3 {uniform_dist(gen), uniform_dist(gen), uniform_dist(gen)}, uniform_dist(gen) * 10.0f);

    Vector3 pos = Vector3Zero();
    float timeScale = 10; // this can be used for the swim speed of the fish!
    int a = 10;
    int b = 10;
    
    rlDisableBackfaceCulling(); // required to render texture on both sides of the plane

    // these require some tuning during the final application
    float START_FADE = 100.0f;
    float END_FADE = 1000.0f;
    
    // Main game loop
    while (!WindowShouldClose()) {
	// Update
	UpdateCamera(&camera, CAMERA_FREE);
	
	// set shader uniforms
	timeNow = (float) GetTime();
	cPos = camera.position;
	
	SetShaderValue(fishyShader, timeLoc, &timeNow, SHADER_UNIFORM_FLOAT);
	SetShaderValue(fishyShader, cameraPos, &cPos, SHADER_UNIFORM_VEC3);
	
	// follow along the ellipse, counter-clockwise
	pos.x = a * cos(-timeNow/timeScale); 
	pos.z = b * sin(-timeNow/timeScale);
	Matrix m = MatrixMultiply(MatrixRotateY(PI/2 + timeNow/timeScale), MatrixTranslate(pos.x, pos.y, pos.z));

	// Draw
	BeginDrawing();
	ClearBackground(LIGHTGRAY);

	BeginMode3D(camera);
        DrawGrid(10, 1.0f);

	// orient the fish-plane towards the direction of movement. 
	DrawMesh(fishMesh, fishMat, m);
	DrawMesh(fishMesh, defaultMaterial, m);
	
	EndMode3D();

	DrawFPS(10, 10);

	EndDrawing();

    }

    // De-Initialization
    UnloadTexture(fishTex);        // Unload texture
    UnloadMesh(fishMesh);
    UnloadMaterial(fishMat);
    UnloadImage(fishImage);
    
    CloseWindow();              // Close window and OpenGL context

    return 0;
}
