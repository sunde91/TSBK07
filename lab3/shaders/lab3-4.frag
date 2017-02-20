#version 150

in vec3 var_Normal;
in vec2 var_TexCoord;
in vec3 var_Pos;

uniform sampler2D texUnit;
uniform mat4 cameraMatrix;

#define NUM_LIGHTS 5
uniform vec3 lightPosDir[NUM_LIGHTS];
uniform vec3 lightColor[NUM_LIGHTS];
uniform bool lightIsDir[NUM_LIGHTS];

out vec4 out_Color;

void main(void)
{
    const float k_d = 0.1;
    const float k_s = 0;
    const vec3 view = vec3(0,0,1);
    const float shadeAmbient = 0.25;

    mat3 rotationMatrix = mat3(cameraMatrix);
    vec3 normal = normalize(var_Normal);

    vec4 tex_Color_accum = vec4(0,0,0,0);
    int i;
    for(i = 0; i < NUM_LIGHTS; i++)
    {
        vec3 lightSource;
        float lightStrength = 10.0;
        if( lightIsDir[i] )
        {
            lightSource = normalize(rotationMatrix * lightPosDir[i]);
        }
        else
        {
            vec4 lightView = cameraMatrix * vec4(lightPosDir[i],1.0);
            lightSource = vec3(lightView / lightView.w);
            vec3 posDiff = lightSource - var_Pos;
            lightStrength = 33.0 / dot(posDiff, posDiff);
            lightSource = normalize(posDiff);
        }

        vec3 reflection = 2 * dot(normal, lightSource) * normal - lightSource;
        float shadeDiffuse = clamp(dot(normal, lightSource), 0, 1);
        float shadeSpec = pow(clamp(dot(view, reflection), 0, 1), 100.0);

        float shade = k_d * shadeDiffuse + k_s * shadeSpec;
        shade = lightStrength * shade;
        vec4 tex_Color = shade * texture(texUnit, var_TexCoord);
        tex_Color.r *= lightColor[i].r;
        tex_Color.g *= lightColor[i].g;
        tex_Color.b *= lightColor[i].b;
        tex_Color_accum += tex_Color;
    }
    // TODO : add clamping
    //tex_Color_accum.r = clamp(tex_Color_accum.r, 0, 1);
    //tex_Color_accum.g = clamp(tex_Color_accum.g, 0, 1);
    //tex_Color_accum.r = clamp(tex_Color_accum.b, 0, 1);
    out_Color = tex_Color_accum;
}
