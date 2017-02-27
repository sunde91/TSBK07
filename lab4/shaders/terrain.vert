#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;
out vec3 varNormal;
out vec3 varLight;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;

void main(void)
{
    const vec3 lightSource = normalize(vec3(0,1,10));
	mat3 normalMatrix = mat3(mdlMatrix);
	texCoord = inTexCoord;
    varNormal = normalMatrix * inNormal;
    varLight = normalMatrix * lightSource;
	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
