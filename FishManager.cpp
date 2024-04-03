#include <Fish.hpp>
#include <FishManager.hpp>
#include <filesystem>
#include <string>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

std::string FishManager::fishDir = "resources/textures/";
float FishManager::timeToWait = 1.0;

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
	fish->Update();

    // attempt to add any pending fish
    for (auto pending : pendingFish) {
	auto ret = AttemptToAddFish(pending.first);
	if (!ret) { 
	    // TODO: remove the fish from pendingFish
	}
    }

    for (auto iter = pendingFish.begin(); iter != pendingFish.end();) {
	// AttemptToAddFish(iter->first);

        // remove any spawned fish
	if (pendingFish.contains(iter->first) && iter->second)
	    iter = pendingFish.erase(iter);
	else
	    iter++;
    }

    // Check for any new added files
    if (GetTime() - lastTime > FishManager::timeToWait) {
	CheckForNewFiles();
	lastTime = GetTime();
    }


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
    Vector2 offsets = { 0, GetRandomValue(-15, 60)/10.0f };
    Vector2 axes = { GetRandomValue(135, 150)/10.0f, GetRandomValue(90, 100)/10.0f };
    Vector3 potPos = Vector3 (axes.x, offsets.y, axes.y);

    for(auto& fish : fishies) { // Check for "collisions" with the other fishies
	Vector3 pos = fish->GetPosition();
	auto dist = Vector3DistanceSqr(pos, potPos);
	
        if (dist < 113.0f) {
	    found = false;
	    break; 
	}
    }
    
    if (found) {
	fishies.emplace_back(new Fish(offsets, axes, filePath, fishyShader, 1.0f));

        if (pendingFish.contains(filePath)) // :D we found a place, so clear it!
	    pendingFish.at(filePath) = true; 	    // TODO: remove the fish from pendingFish
	return 0;
    }

    // TODO: remove the fish from pendingFish
    pendingFish.insert({filePath, false}); // :( Place not found
    return 1;
}

void FishManager::CheckForNewFiles()
{
    for (auto& file: std::filesystem::directory_iterator(FishManager::fishDir)) {
	if (!fishFiles.contains(file.path().string())) {
	    auto texturePath = file.path().string();
	    fishFiles.insert(texturePath);
	    pendingFish.insert({texturePath, false});
	}
    }
}

