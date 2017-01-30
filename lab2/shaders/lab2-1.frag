#version 150

in vec3 var_Normal;
in vec2 var_TexCoord;

out vec4 out_Color;

void main(void)
{
        const vec3 lightSource = vec3(0.58, 0.58, -0.58);
        const float shadeAmbient = 0.15;
        const vec3 view = vec3(0,0,-1);

        vec3 normal = normalize(var_Normal);

        float shadeDiffuse = clamp(dot(normal, lightSource), 0, 1);

        vec3 reflection = 2 * dot(normal, lightSource) * normal - lightSource;
        float shadeSpec = pow(clamp(dot(view, reflection), 0, 1), 25.0);

        float shade = clamp(shadeAmbient + 0.3 * shadeDiffuse + 0.7 * shadeSpec, 0, 1);

	out_Color = vec4(shade * pow(var_TexCoord.s, 2) , shade * pow(var_TexCoord.t, 2), 0.0, 1.0); 
}
