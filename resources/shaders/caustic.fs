#version 330
// Based heavily off this amazing work:
// https://www.shadertoy.com/view/4ljXWh

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform float itime;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables
const float TAU = 6.28318530718;
const int MAX_ITER = 5;

vec3 caustic(vec2 uv)
{
    vec2 p = mod(uv*TAU, TAU)-250.0;
    float time = itime * .5+23.0;

    vec2 i = vec2(p);
    float c = 1.0;
    float inten = .005;

    for (int n = 0; n < MAX_ITER; n++) 
	{
	    float t = time * (1.0 - (3.5 / float(n+1)));
	    i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));
	    c += 1.0/length(vec2(p.x / (sin(i.x+t)/inten),p.y / (cos(i.y+t)/inten)));
	}
    
    c /= float(MAX_ITER);
    c = 1.17-pow(c, 1.4);
    vec3 color = vec3(pow(abs(c), 8.0));
    color = clamp(color + vec3(0.0, 0.35, 0.5), 0.0, 1.0);
    color = mix(color, vec3(1.0,1.0,1.0),0.3);
    
    return color;
}

void main()
{
    // Texel color fetching from texture sampler
    vec3 skyColor = texture(texture0, fragTexCoord).rgb;
    vec3 sunLightColor = vec3(1.7, 0.65, 0.65);
    vec3 skyLightColor = vec3(0.8, 0.35, 0.15);
    vec3 indLightColor = vec3(0.4, 0.3, 0.2);
    vec3 horizonColor = vec3(0.0, 0.05, 0.2);
     
    vec2 p = -1.0 + 2.0 * fragTexCoord.xy;

    vec3 eye = vec3(0.0, 1.25, 1.5);
    
    vec3 ro = eye;
    vec3 ta = vec3(0.5, 1.8, 0.0);

    vec3 cw = normalize(ta - ro);
    vec3 cu = normalize(cross(vec3(0.0, 1.0, 0.0), cw));
    vec3 cv = normalize(cross(cw, cu));
    mat3 cam = mat3(cu, cv, cw);

    vec3 rd = cam * normalize(vec3(p.xy, 1.0));

    // background
    vec3 color = skyColor;
    float sky = 0.0;

    sky = clamp(0.8 * (1.0 - 0.8 * rd.y), 0.0, 1.0);
    color = sky * skyColor;
    color += ((0.3*caustic(vec2(p.x,p.y*1.0)))+(0.3*caustic(vec2(p.x,p.y*2.7))))*pow(p.y,4.0);

    color = mix(color * 2.8, skyColor, pow(1.0 - pow(rd.y,4.0), 0.6));
    
    finalColor = vec4(color, 1.0);    
}
