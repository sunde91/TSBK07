#version 150

in vec3 var_Normal;
in vec2 var_TexCoord;

uniform sampler2D texUnit;
uniform mat4 modelMatrix;
uniform mat4 cameraMatrix;

out vec4 out_Color;

void main(void)
{

        mat4 affineMatrix = cameraMatrix * modelMatrix;
        mat3 rotationMatrix = mat3(cameraMatrix);

        vec3 lightSource = normalize(rotationMatrix * vec3(0, 1, 1));
        const float shadeAmbient = 0.25;
        const vec3 view = vec3(0,0,1); //normalize(vec3(cameraMatrix[3]));

        vec3 normal = normalize(var_Normal);

        float shadeDiffuse = clamp(dot(normal, lightSource), 0, 1);

        vec3 reflection = 2 * dot(normal, lightSource) * normal - lightSource;
        float shadeSpec = pow(clamp(dot(view, reflection), 0, 1), 25.0);

        float shade = clamp(shadeAmbient + 0.4 * shadeDiffuse + 0.7 * shadeSpec, 0, 1);
        //float shade = clamp(shadeDiffuse, 0, 1);
        vec4 tex_Color = texture(texUnit, var_TexCoord);
        out_Color = shade  * tex_Color;
}