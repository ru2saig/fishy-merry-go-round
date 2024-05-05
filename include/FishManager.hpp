#ifndef _FISHMANAGER_HPP_
#define _FISHMANAGER_HPP_

#include <Fish.hpp>
#include <memory>
#include <raylib.h>
#include <vector>
#include <string>
#include <unordered_set>

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
    // these variables are used in CheckForNewfile
    std::unordered_set<std::string> fishFiles; // files (fingers crossed) loaded
    double lastTime; // last time when the fishDir was checked
    static float timeToWait; // how long to wait until checked for new files
    static int attempts; // spawm attempts

    // this stuff is used for adding new fish
    std::vector<std::unique_ptr<Fish>> fishies; // unique::ptr is required or seg faults occur
    std::unordered_set<std::string> pendingFish; // a list of fish that need to be loaded, as filePath, loadStatus. 
    Shader fishyShader;
    int timeLoc; // These make it easier to update stuff, could do without
    float timeNow;
    float minDist;
};

#endif /* _FISHMANAGER_HPP_ */
