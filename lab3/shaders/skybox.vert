#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

uniform mat4 modelMatrix;
uniform mat4 cameraMatrix;
uniform mat4 projMatrix;

out vec3 var_Normal;
out vec2 var_TexCoord;

void main(void)
{
    
    mat4 affineMatrix = cameraMatrix * modelMatrix;
    mat3 rotationMatrix = mat3(affineMatrix);
    mat4 rotationMatrixView = affineMatrix;
    rotationMatrixView[0][3] = 0;
    rotationMatrixView[1][3] = 0;
    rotationMatrixView[2][3] = 0;
    var_Normal = rotationMatrix * in_Normal;
	gl_Position = projMatrix * rotationMatrixView*vec4(in_Position, 1.0);

    var_TexCoord = vec2(in_TexCoord.s, in_TexCoord.t);
}
