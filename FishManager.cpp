#include <Fish.hpp>
#include <FishManager.hpp>
#include <algorithm>
#include <filesystem>
#include <Utility.hpp>
#include <string>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <cmath>

float FishManager::timeToWait = 1.0;
int FishManager::attempts = 5;

FishManager::FishManager()
    : minDist { 10.0 }
{
    auto vertexShaderPath = Utility::shaderDir / "fishymovement.vs";
    auto fragShaderPath = Utility::shaderDir / "transparent.fs";

    fishyShader = LoadShader(vertexShaderPath.c_str(), fragShaderPath.c_str());
    timeLoc = GetShaderLocation(fishyShader, "time");
    timeNow = (float) GetTime();
    SetShaderValue(fishyShader, timeLoc, &timeNow, SHADER_UNIFORM_FLOAT);

    lastTime = GetTime();
}

FishManager::~FishManager()
{
    UnloadShader(fishyShader);
}

void FishManager::updateMinDist()
{
    auto no = fishies.size();

    minDist = std::clamp(15.0/(1 + std::exp(no/40.0 - 0.5)), 1.2, 10.0);
}

void FishManager::Update()
{
    // Update the shader uniforms
    timeNow = (float) GetTime(); 
    SetShaderValue(fishyShader, timeLoc, &timeNow, SHADER_UNIFORM_FLOAT);

    for(auto &fish: fishies) // Update all the fish
	fish->Update(timeNow);

    // Attempt to add any pending fish
    int pendingMax = 10;
    for (auto pending = pendingFish.begin(); pendingMax > 0 && pending != pendingFish.end(); pendingMax--) {
	auto ret = AttemptToAddFish(*pending);

	// Yes, this is luck-based, wanted average O(1) for the set operations
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
    
    updateMinDist();
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

	    if (dist < minDist) {
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
    for (auto& file: std::filesystem::directory_iterator(Utility::fishDir)) {
	if (!fishFiles.contains(file.path().string())) {
	    auto texturePath = file.path().string();
	    fishFiles.insert(texturePath);

	    if (texturePath.find(".gitignore") == std::string::npos)
		pendingFish.insert(texturePath);
	}
    }
}

