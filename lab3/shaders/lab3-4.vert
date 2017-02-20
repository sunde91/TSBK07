#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

//uniform mat4 modelMatrix;
//uniform mat4 cameraMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projMatrix;

out vec3 var_Normal;
out vec2 var_TexCoord;
out vec3 var_Pos;

void main(void)
{
  
    var_Normal = normalMatrix * in_Normal;
    vec4 worldPos = modelViewMatrix * vec4(in_Position, 1.0);
    var_Pos = vec3(worldPos / worldPos.w);
	gl_Position = projMatrix * modelViewMatrix * vec4(in_Position, 1.0);

    var_TexCoord = vec2(30*in_TexCoord.s, 30*in_TexCoord.t);
}
