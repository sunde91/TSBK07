#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 varNormal;
in vec3 varLight;
uniform sampler2D tex;

void main(void)
{

	//outColor = texture(tex, texCoord);

    //vec3 reflection = reflect(lightSource, normal); //2 * dot(normal, lightSource) * normal - lightSource;
    float shadeDiffuse = clamp(dot(varNormal, varLight), 0, 1);
    /*
    float shadeSpec = pow(clamp(dot(v_Pos, reflection), 0, 1), 25.0);

    float shade = shadeAmbient + k_d * shadeDiffuse + k_s * shadeSpec;
    shade = lightStrength * shade;
    vec4 tex_Color0 = shade * texture(texUnit0, var_TexCoord);
    vec4 tex_Color1 = shade * texture(texUnit1, var_TexCoord);
    vec4 tex_Color = lightColor[i].b * tex_Color0 + lightColor[i].g * tex_Color1
        + lightColor[i].r * ( tex_Color0 + tex_Color1  );
    tex_Color /= 4;
    */
    vec4 texColor = texture(tex, texCoord);
    outColor = shadeDiffuse * texColor;
}
