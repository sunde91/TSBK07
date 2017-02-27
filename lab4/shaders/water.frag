#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 varNormal;
in vec3 varLight;
in vec3 varPos;

void main(void)
{

	//outColor = texture(tex, texCoord);

    vec3 normal = normalize(varNormal);
    vec3 light = normalize(varLight);
    float shadeAmbient = 0.5;
    float shadeDiffuse = clamp(dot(normal, light), 0, 1);

    vec3 reflection = 2 * dot(normal, light) * normal - light;
    //vec3 reflection = reflect(light, normal); //
    vec3 myPosDirection = - normalize(varPos);
    float shadeSpec = pow(clamp(dot(myPosDirection, reflection), 0, 1), 50.0);

    float shade = clamp(shadeAmbient + 0.5*shadeDiffuse + 0.5*shadeSpec,0,1);

    
    //vec4 texColor = texture(tex, texCoord);
    outColor = shade * vec4(133.0/255.0, 200.0/255.0, 242.0/255.0, 0.75);
}