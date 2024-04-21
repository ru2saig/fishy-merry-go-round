#include <Fish.hpp>
#include <FishManager.hpp>
#include <filesystem>
#include <iostream> // TODO: REMOVE THIS
#include <string>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

std::string FishManager::fishDir = "resources/textures/";
float FishManager::timeToWait = 1.0;
int FishManager::attempts = 5;

FishManager::FishManager()
{
    fishyShader = LoadShader("resources/shaders/fishymovement.vs", "resources/shaders/transparent.fs");
    timeLoc = GetShaderLocation(fishyShader, "time");
    timeNow = (float) GetTime();
    SetShaderValue(fishyShader, timeLoc, &timeNow, SHADER_UNIFORM_FLOAT);

    lastTime = GetTime();
}

FishManager::~FishManager()
{
    UnloadShader(fishyShader);
}

void FishManager::Update()
{
    // Update the shader uniforms
    timeNow = (float) GetTime(); 
    SetShaderValue(fishyShader, timeLoc, &timeNow, SHADER_UNIFORM_FLOAT);

    for(auto &fish: fishies) // Update all the fish
	fish->Update(timeNow);

    // Attempt to add any pending fish
    for (auto pending = pendingFish.begin(); pending != pendingFish.end(); ) {
	auto ret = AttemptToAddFish(*pending);

        if (!ret) {  // Fishey found a place!
	    pending = pendingFish.erase(pending);
	} else // Can always try again!
	    pending++;
    }

    // Check for any new added files
    if (GetTime() - lastTime > FishManager::timeToWait) {
	CheckForNewFiles();
	lastTime = GetTime();
    }

    std::cout << fishies.size() <<  std::endl;
}

void FishManager::Draw()
{
    rlDisableBackfaceCulling(); // Required to render texture on both sides of the plane
    for(auto &fish: fishies)
	fish->Draw();
    rlEnableBackfaceCulling();
}

// Gets a random location, and sees if it's possible to add a fish. If it is, add it there, otherwise,
// add the filePath to pendingFish to try later.
int FishManager::AttemptToAddFish(std::string filePath)
{
    bool found = true;
    Vector2 offsets, axes;
    
    for (int attempt = 0; attempt < FishManager::attempts; attempt++) {
	offsets = { 0.0f, GetRandomValue(-18, 55)/10.0f };
	axes = { GetRandomValue(90, 100)/10.0f, GetRandomValue(140, 150)/10.0f };
	Vector3 potPos = Vector3 (axes.x, offsets.y, 0.0f); // axes.x * cos(t), offsets.y, axes.y * sin(t); simplified for t=0.0f

	for(auto& fish : fishies) { // Check for "collisions" with the existing fishies
	    Vector3 pos = fish->GetPosition();
	    auto dist = Vector3DistanceSqr(pos, potPos); 

	    if (dist < 6.0f) {
		found = false;
		break;
	    }
	}
    }
    
    if (found) {
	offsets.x = -GetTime();
	fishies.emplace_back(new Fish(offsets, axes, filePath, fishyShader, 0.02f ));
	return 0;
    }

    return 1;
}

void FishManager::CheckForNewFiles()
{
    for (auto& file: std::filesystem::directory_iterator(FishManager::fishDir)) {
	if (!fishFiles.contains(file.path().string())) {
	    auto texturePath = file.path().string();
	    fishFiles.insert(texturePath);

	    if (texturePath.find(".gitignore") == std::string::npos)
		pendingFish.insert(texturePath);
	}
    }
}

