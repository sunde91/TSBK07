#version 150

in vec3 var_Normal;
in vec2 var_TexCoord;
in vec3 var_Pos;

uniform sampler2D texUnit;
uniform mat4 modelMatrix;
uniform mat4 cameraMatrix;

out vec4 out_Color;

void main(void)
{

        mat4 affineMatrix = cameraMatrix * modelMatrix;
        mat3 rotationMatrix = mat3(cameraMatrix);

        vec3 lightSource = normalize(rotationMatrix * vec3(-10, 7, -6));
        const float shadeAmbient = 0.25;
        //const vec3 view = vec3(0,0,1); //normalize(vec3(cameraMatrix[3]));

        vec3 normal = normalize(var_Normal);

        float shadeDiffuse = clamp(dot(normal, lightSource), 0, 1);

        vec3 reflection = 2 * dot(normal, lightSource) * normal - lightSource;
        vec3 v_Pos = - normalize(var_Pos);
        float shadeSpec = pow(clamp(dot(v_Pos, reflection), 0, 1), 25.0);

        float shade = clamp(shadeAmbient + 0.4 * shadeDiffuse + 0.7 * shadeSpec, 0, 1);
        //float shade = clamp(shadeDiffuse, 0, 1);
        vec4 tex_Color = texture(texUnit, var_TexCoord);
        out_Color = shade  * tex_Color;
}