#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

uniform mat4 cameraMatrix;
uniform mat4 projMatrix;

out vec3 var_Normal;
out vec2 var_TexCoord;

void main(void)
{
    
    var_Normal = mat3(cameraMatrix) * in_Normal;
	gl_Position = projMatrix * cameraMatrix*vec4(in_Position, 1.0);

    var_TexCoord = vec2(in_TexCoord.s, in_TexCoord.t);
}
