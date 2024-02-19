#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;
uniform float wiggleMag=0.08;
uniform float time;
uniform float timeScale=1.0;


// Output vertex attributes (to fragment shader)
out vec2 fragTexCoord;
out vec4 fragColor;

vec3 vertexPos = vertexPosition;

// TODO: Make it kewler, perhaps sample from a noise texture? 
void main()
{
    // Send vertex attributes to fragment shader
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;
    float t = time * timeScale;

    vertexPos.x += wiggleMag * sin(t * timeScale);
    vertexPos.y += -0.01 * sin(t * timeScale);
    vertexPos.y += -0.01 * cos(t * timeScale);
    
    // Calculate final vertex position
    gl_Position = mvp*vec4(vertexPos, 1.0);
}
