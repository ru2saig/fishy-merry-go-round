#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in float dist;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;


// Output fragment color
out vec4 finalColor;

void main()
{
    vec4 texel = texture(texture0, fragTexCoord); 
    if (texel.a == 0.0) discard;

    finalColor = texel * vec4(fragColor.rgb * dist, 1.0) * colDiffuse;
}
