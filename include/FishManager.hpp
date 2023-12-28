#ifndef _FISHMANAGER_HPP_
#define _FISHMANAGER_HPP_

#include <Fish.hpp>
#include <memory>
#include <raylib.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

class FishManager
{
public:
    FishManager();
    ~FishManager();

    void CheckForNewFiles();
    void AttemptToAddFish(std::string filePath);
    void Update();
    void Draw();

private:
    // these variables are used in CheckForNewfile
    std::unordered_set<std::string> fishFiles; // Files (hopefully) loaded
    double lastTime; // Last time when the fishDir was checked
    static std::string fishDir; // Where processing fish files are stored
    static float timeToWait; // How long to wait until checked for new files

    std::vector<std::unique_ptr<Fish>> fishies; // unique::ptr is required or seg faults occur for some reason
    std::unordered_map<std::string, bool> pendingFish; // a list of fish that need to be loaded, as filePath, loadStatus. 
    Shader fishyShader;
    int timeLoc; // These make it easier to update stuff, could do without
    float timeNow;
};

#endif /* _FISHMANAGER_HPP_ */
