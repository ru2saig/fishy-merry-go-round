#include <Fish.hpp>
#include <FishManager.hpp>
#include <iostream>
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
	fish->Update(timeNow);

    // attempt to add any pending fish
    for (auto iter = pendingFish.begin(); iter != pendingFish.end();) {
	AttemptToAddFish(iter->first);

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

// TODO: Maybe add the statuses, in a refactor? Return a status, instead?
// Gets a random location, and sees if it's possible to add a fish. If it is, add it there, otherwise,
// add the filePath to pendingFish to try later.
void FishManager::AttemptToAddFish(std::string filePath)
{
    bool found = true;
    float timeScale = 20.0f;
    
    Vector2 offset = {-(timeScale + (float) GetTime()), GetRandomValue(-15, 40)/10.0f};
    Vector2 axes = {13.0f, GetRandomValue(140, 150)/10.0f};
    Vector3 potPos = Vector3 (axes.x * cos(-1.0), offset.y, axes.y * sin(-1.0));

    for(auto& fish : fishies) { // Check for "collisions" with the other fishies
	Vector3 pos = fish->GetPosition();
	auto dist = Vector2DistanceSqr(Vector2 {pos.y, pos.z}, Vector2{potPos.y, potPos.z});
	// Need a better metric. Overlapping planes?
	if (dist <= 260) {
	    found = false;
	    break; 
	}
    }

    
    if (found) {
	fishies.emplace_back(new Fish(offset, axes, filePath, fishyShader, timeScale));

        if (pendingFish.contains(filePath)) // :D we found a place!
	    pendingFish.at(filePath) = true;
    } else
	pendingFish.insert({filePath, false}); // :( Place not found
}

void FishManager::CheckForNewFiles()
{
    for (auto& file: std::filesystem::directory_iterator(FishManager::fishDir)) {
	if (!fishFiles.contains(file.path().string())) {
	    auto texturePath = file.path().string();
	    fishFiles.insert(texturePath);
	    AttemptToAddFish(texturePath.c_str());
	}
    }
}

