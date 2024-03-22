#version 460 core
layout (location = 0) in vec3 aPos;

struct InstanceData {
    vec3 Offset;
    float Scale;
};

layout (std430, binding = 0) buffer instanceUbo {
    InstanceData data[];
};

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
	vec3 aOffset = data[gl_InstanceID].Offset;
	float aScale = data[gl_InstanceID].Scale;
	vec3 normalizeOffset = aOffset / aScale;
	vec3 pos =  aPos + normalizeOffset;

    mat4 scaledModel = mat4(
        vec4(aScale, 0.0, 0.0, 0.0),
        vec4(0.0, aScale, 0.0, 0.0),
        vec4(0.0, 0.0, aScale, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    ) * model;

    gl_Position = lightSpaceMatrix * scaledModel * vec4(pos, 1.0);
}  
