#version 150

in vec3 var_Normal;
in vec2 var_TexCoord;

uniform sampler2D texUnit;

out vec4 out_Color;

void main(void)
{

        vec4 tex_Color = texture(texUnit, var_TexCoord);
        out_Color = tex_Color;
}