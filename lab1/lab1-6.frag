#version 150

in vec3 var_Normal;
out vec4 out_Color;

void main(void)
{
        const vec3 lightSource = vec3(0.58, 0.58, 0.58);
        float shade = clamp(dot(normalize(var_Normal), lightSource), 0, 1);
	out_Color = vec4(shade, shade, shade, 1.0); 
}
