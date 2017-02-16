#version 150

in vec3 var_Normal;
in vec2 var_TexCoord;
in vec3 var_Pos;

uniform sampler2D texUnit;
uniform mat4 cameraMatrix;

#define NUM_LIGHTS 1
uniform vec3 lightPosDir[NUM_LIGHTS];
uniform vec3 lightColor[NUM_LIGHTS];
uniform bool lightIsDir[NUM_LIGHTS];

out vec4 out_Color;

void main(void)
{
    const vec3 view = vec3(0,0,1);
    const float shadeAmbient = 0.25;

    mat3 rotationMatrix = mat3(cameraMatrix);
    vec3 normal = normalize(var_Normal);

    vec4 tex_Color;
    int i;
    for(i = 0; i < NUM_LIGHTS; i++)
    {
        vec3 lightSource;
        float lightStrength = 1.0;
        if( lightIsDir[i] )
        {
            lightSource = normalize(rotationMatrix * lightPosDir[i]);
        }
        else
        {
            vec4 lightView = cameraMatrix * vec4(lightPosDir[i],1.0);
            lightSource = vec3(lightView / lightView.w);
            vec3 posDiff = lightSource - var_Pos;
            lightStrength = sqrt(100.0 / dot(posDiff, posDiff));
            lightSource = normalize(lightSource);
        }

        vec3 reflection = 2 * dot(normal, lightSource) * normal - lightSource;
        float shadeDiffuse = clamp(dot(normal, lightSource), 0, 1);
        float shadeSpec = pow(clamp(dot(view, reflection), 0, 1), 25.0);

        float shade = shadeAmbient + 0.4 * shadeDiffuse + 0.0 * shadeSpec;
        shade = lightStrength * shade;
        tex_Color = shade * texture(texUnit, var_TexCoord);
        tex_Color.r *= lightColor[i].r;
        tex_Color.g *= lightColor[i].g;
        tex_Color.b *= lightColor[i].b;
    }
    // TODO : add clamping
    out_Color = tex_Color;
}