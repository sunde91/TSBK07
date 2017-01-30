#version 150

in vec3 in_Position;
in vec3 in_Normal;
uniform mat4 myMatrix;

out vec3 var_Normal;

void main(void)
{
        mat3 normalMatrix = mat3(myMatrix);
        var_Normal = normalMatrix * in_Normal;
	gl_Position = myMatrix*vec4(in_Position, 1.0);
}
