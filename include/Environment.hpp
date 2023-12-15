#ifndef _ENVIRONMENT_HPP_
#define _ENVIRONMENT_HPP_
#include <raylib.h>
#include <array>

class Environment {
public:
    static Environment& instance();
    void Update();
    void Draw();
    ~Environment();
private:
    Environment();
    void SetWiggle(float wiggle); // this shouldn't be how it works? Why is it doing this?

    Texture2D fg;
    Texture2D fgDynamic;
    Texture2D mg;
    Texture2D mgDynamic;
    Texture2D kelp;
    Texture2D bg;

    Shader staticShader;
    Shader wiggleShader;
    // locations of uniforms in the wiggle.vs
    int timeLoc;
};

#endif /* _ENVIRONMENT_HPP_ */
