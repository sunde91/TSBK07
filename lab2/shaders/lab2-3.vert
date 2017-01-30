#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

uniform mat4 affineMatrix;
uniform mat4 projMatrix;
uniform float time;

out vec3 var_Normal;
out vec2 var_TexCoord;

void main(void)
{
        mat3 rotMatrix = mat3(affineMatrix);
        var_Normal = rotMatrix * in_Normal;
	gl_Position = projMatrix*affineMatrix*vec4(in_Position, 1.0);

    var_TexCoord = vec2(2 * in_TexCoord.s, 2 * in_TexCoord.t);
}
