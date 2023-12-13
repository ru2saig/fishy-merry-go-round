#include "Fish.hpp"
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

FishManager::~FishManager() {}

void FishManager::Update()
{
    // Update the shader uniforms
    timeNow = (float) GetTime(); 
    SetShaderValue(fishyShader, timeLoc, &timeNow, SHADER_UNIFORM_FLOAT);

    // attempt to add any pending fish
    for(auto& pending: pendingFish)
	AttemptToAddFish(pending);
    
    for(auto &fish: fishies) // Update all the fish
	fish->Update(timeNow);

    if (GetTime() - lastTime > FishManager::timeToWait) { // Check for any new added files
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
    
    Vector2 offset = {-(100.0f + (float) GetTime()), GetRandomValue(-30, 50)/10.0f};     
    Vector2 axes = {13.0f, GetRandomValue(140, 150)/10.0f};
    Vector3 potPos = Vector3 (axes.x * cos(-1.0), axes.y * sin(-1.0), offset.y);
    	
    for(auto& fish : fishies) { // Check for "collisions" with the other fishies
	Vector3 pos = fish->GetPosition();
	auto dist = Vector3Distance(pos, potPos);

	if (dist <= 3.0f) {
	    found = false;
	    break; 
	}

    }

    if (found) {
	fishies.emplace_back(new Fish(offset, axes, filePath, fishyShader, 100.0f));
	if (pendingFish.contains(filePath)) // remove filePath if it was in pendingFish
	    pendingFish.erase(filePath);
    } else
	pendingFish.insert(filePath); // :( Place not found
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

