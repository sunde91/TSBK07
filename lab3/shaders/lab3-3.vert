#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

uniform mat4 modelMatrix;
uniform mat4 cameraMatrix;
uniform mat4 projMatrix;
uniform float time;

out vec3 var_Normal;
out vec2 var_TexCoord;
out vec3 var_Pos;


void main(void)
{
    
    mat4 affineMatrix = cameraMatrix * modelMatrix;
    mat3 rotationMatrix = mat3(affineMatrix);
    vec4 worldPos = cameraMatrix * vec4(in_Position, 1.0);
    var_Pos = vec3(worldPos / worldPos.w);
    
    var_Normal = rotationMatrix * in_Normal;
	gl_Position = projMatrix*affineMatrix*vec4(in_Position, 1.0);

    var_TexCoord = vec2(30*in_TexCoord.s, 30*in_TexCoord.t);
}
