#version 150

in vec3 var_Normal;
in vec2 var_TexCoord;

uniform sampler2D texUnit0;
uniform sampler2D texUnit1;

out vec4 out_Color;

void main(void)
{

        vec4 tex_Color0 = texture(texUnit0, var_TexCoord);
        vec4 tex_Color1 = texture(texUnit1, var_TexCoord);
        float t = var_TexCoord.t;
        float s = var_TexCoord.s;
        int it = int(10*(t*t + s*s)) % 2;
        if( it == 0 )
        {
            out_Color = tex_Color0;
        }
        else 
        {
             out_Color = tex_Color1;
        }
        //out_Color = (tex_Color0 + tex_Color1)/2;
        //out_Color = cos(var_TexCoord.t)*cos(var_TexCoord.t)*tex_Color0 + sin(var_TexCoord.t)*sin(var_TexCoord.t)*tex_Color1;
}