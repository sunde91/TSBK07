#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec3 in_Color;
uniform mat4 myMatrix;

out vec3 var_Normal;
out vec3 var_Color;

void main(void)
{
        mat3 normalMatrix = mat3(myMatrix);
        var_Normal = normalMatrix * in_Normal;
	gl_Position = myMatrix*vec4(in_Position, 1.0);
    var_Color = in_Color;
}
