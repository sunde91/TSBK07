#version 150

in vec3 var_Normal;
in vec2 var_TexCoord;
in vec3 var_Pos;

uniform sampler2D texUnit0;
uniform sampler2D texUnit1;
uniform mat4 cameraMatrix;

#define NUM_LIGHTS 4
uniform vec3 lightPosDir[NUM_LIGHTS];
uniform vec3 lightColor[NUM_LIGHTS];
uniform bool lightIsDir[NUM_LIGHTS];

out vec4 out_Color;

void main(void)
{
    const float k_d = 0.4;
    const float k_s = 0.7;
    const vec3 view = vec3(0,0,1);
    const float shadeAmbient = 0.25;

    mat3 rotationMatrix = mat3(cameraMatrix);
    vec3 normal = normalize(var_Normal);

    vec4 tex_Color_accum = vec4(0,0,0,1);
    int i;
    for(i = 0; i < NUM_LIGHTS-1; i++)
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
            lightStrength = 200.0 / (dot(posDiff, posDiff));
            lightSource = normalize(posDiff);
        }

        vec3 v_Pos = - normalize(var_Pos);
        vec3 reflection = 2 * dot(normal, lightSource) * normal - lightSource;
        float shadeDiffuse = clamp(dot(normal, lightSource), 0, 1);
        float shadeSpec = pow(clamp(dot(v_Pos, reflection), 0, 1), 25.0);

        float shade = shadeAmbient + k_d * shadeDiffuse + k_s * shadeSpec;
        shade = lightStrength * shade;
        vec4 tex_Color0 = shade * texture(texUnit0, var_TexCoord);
        vec4 tex_Color1 = shade * texture(texUnit1, var_TexCoord);
        vec4 tex_Color = lightColor[i].b * tex_Color0 + lightColor[i].g * tex_Color1
            + lightColor[i].r * ( tex_Color0 + tex_Color1  );
        tex_Color /= 4;
        //tex_Color.r *= lightColor[i].r;
        //tex_Color.g *= lightColor[i].g;
        //tex_Color.b *= lightColor[i].b;
        tex_Color_accum += tex_Color;
    }

    //tex_Color_accum += vec4(shadeAmbient, shadeAmbient, shadeAmbient, 0);

    tex_Color_accum.r = clamp(tex_Color_accum.r, 0, 1);
    tex_Color_accum.g = clamp(tex_Color_accum.g, 0, 1);
    tex_Color_accum.b = clamp(tex_Color_accum.b, 0, 1);
    tex_Color_accum.a = 1;
    out_Color = tex_Color_accum;
}
