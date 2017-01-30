#version 150

in vec3 in_Position;
in vec3 in_Color;
uniform mat4 myMatrix;

out vec4 var_Color;

void main(void)
{
        var_Color = vec4(in_Color, 1.0);
	gl_Position = myMatrix*vec4(in_Position, 1.0);
}
