#ifndef _ENVIRONMENT_HPP_
#define _ENVIRONMENT_HPP_
#include <raylib.h>
#include <array>
#include <string>
#include <filesystem>

class Environment {
public:
    static Environment& instance();
    void Update();
    void Draw();
    ~Environment();
private:
    Environment();
    void SetWiggle(float wiggle); // this shouldn't be how it works? Why is it doing this?

    enum class TextureIndex {
	bg,
	kelp,
	mg,
	mgDynamic,
	fg,
	fgDynamic,
	TEXTURES
    };

    // TODO: What about installing this? What ever would become of these?
    const std::filesystem::path texDir = std::filesystem::current_path().concat("/resources/bg/");
    const std::filesystem::path shaderDir = std::filesystem::current_path().concat("/resources/shaders/");

    std::array<std::string, static_cast<int>(TextureIndex::TEXTURES)> textureNames = {
	"background.png", "kelp.png", "mg.png", "mg-dynamic.png", "fg.png", "fg-dynamic.png" };
    std::array<Texture2D, static_cast<int>(TextureIndex::TEXTURES)> textures;

    Shader staticShader;
    Shader wiggleShader;
    // locations of uniforms in the wiggle.vs
    int timeLoc;
};

#endif /* _ENVIRONMENT_HPP_ */
