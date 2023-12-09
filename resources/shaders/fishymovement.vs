#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;
uniform float time;
uniform float time_scale = 1.;
uniform float side_to_side = 0.1;
uniform float pivot = 3.0;
uniform float wave = 0.5;
uniform float twist = 1.;

// Output vertex attributes (to fragment shader)
out vec2 fragTexCoord;
out vec4 fragColor;

vec3 vertexPos = vertexPosition;

void main()
{
    // Send vertex attributes to fragment shader
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;

    // implement fishy movement
    float t = time * time_scale;

    // side to side movement
    //vertexPos.z += cos(t) * side_to_side; 
    
    // pivotey movement
    //float pivot_angle = cos(t) * 0.1 * pivot;
    //mat2 rotation_matrix = mat2(vec2(cos(pivot_angle), -sin(pivot_angle)), vec2(sin(pivot_angle), cos(pivot_angle)));
    //vertexPos.xz *= rotation_matrix;

    // wavey fish
    float body = (vertexPos.x + 1.0);

    vertexPos.z += cos(t + body) * wave; 
    
    // Calculate final vertex position
    gl_Position = mvp*vec4(vertexPos, 1.0);
}
