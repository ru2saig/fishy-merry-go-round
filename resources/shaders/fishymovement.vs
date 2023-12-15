#version 330
// Based heavily on the write up here: https://docs.godotengine.org/en/3.3/tutorials/3d/vertex_animation/animating_thousands_of_fish.html
// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;
uniform float time;
uniform float swim_speed = 1.0;
uniform float side_to_side = 0.1;
uniform float pivot = 1.0;
uniform float wave = .5;
uniform float waviness = 3.0;
uniform float twist = 0.1;
uniform float mask_black = 0.0;
uniform float mask_white = 1.0;
uniform vec3 cameraPos;

// Output vertex attributes (to fragment shader)
out vec2 fragTexCoord;
out vec4 fragColor;
//out float dist;

vec3 vertexPos = vertexPosition; // so that it can me modified
const float START_FADE = 100.0f; // tweak these, depending on taste
const float END_FADE = 200.0f;

void main()
{
    // Send vertex attributes to fragment shader
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;

    // used to fade the color inversely proportional to the distance from the camera
    // vec3 distanceVec = cameraPos - vertexPos;
    // dist = END_FADE - min(max(START_FADE, dot(distanceVec, distanceVec)), END_FADE);
    // dist = (dist + START_FADE)/END_FADE;
    
    float t = time * swim_speed;

    // side to side movement
    vertexPos.z += cos(t) * side_to_side; 
    
    // pivotey fish
    float pivot_angle = cos(t) * 0.1 * pivot;
    mat2 rotation_matrix = mat2(vec2(cos(pivot_angle), -sin(pivot_angle)), vec2(sin(pivot_angle), cos(pivot_angle)));
    vertexPos.xz *= rotation_matrix;

    // wavey fish
    float mask = smoothstep(mask_black, mask_white, 1.0 - vertexPos.x);
    vertexPos.z += cos(t + (vertexPos.x*waviness)) * wave;

    // twist fish
    float twist_angle = cos(t + vertexPos.x) * 0.3 * twist;
    mat2 twist_matrix = mat2(vec2(cos(twist_angle), -sin(twist_angle)), vec2(sin(twist_angle), cos(twist_angle)));

    vertexPos.zy = mix(vertexPos.zy, twist_matrix * vertexPos.zy, mask);
    
    // Calculate final vertex position
    gl_Position = mvp*vec4(vertexPos, 1.0);
}
