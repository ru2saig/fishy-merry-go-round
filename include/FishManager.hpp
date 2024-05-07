#ifndef _FISHMANAGER_HPP_
#define _FISHMANAGER_HPP_

#include <Fish.hpp>
#include <memory>
#include <raylib.h>
#include <vector>
#include <string>
#include <unordered_set>
#include <array>

class FishManager
{
public:
    FishManager();
    ~FishManager();

    void CheckForNewFiles();
    int AttemptToAddFish(std::string filePath);
    void Update();
    void Draw();

private:
    void UpdateMinDist(); // Dynamically minDist, according to the number of fish on screen
    void UpdateShaderUniforms(const std::array<float, 5>& values); // Update shader uniforms
    
    std::unordered_set<std::string> fishFiles; // Files (fingers crossed) loaded
    std::vector<std::unique_ptr<Fish>> fishies; // unique::ptr is required or seg faults occur
    std::unordered_set<std::string> pendingFish; // A list of fish that need to be loaded, as filePath, loadStatus.

    Shader fishyShader;

    static float timeToWait; // How long to wait until checked for new files
    static int attempts; // Spawm attempts
    
    double lastTime; // Last time when the fishDir was checked
    int timeLoc;
    float timeNow;
    float minDist; // The shortest distance from all other fish, so that a fish can be spawned
};

#endif /* _FISHMANAGER_HPP_ */
