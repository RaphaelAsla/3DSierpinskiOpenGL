#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

struct InstanceData {
    vec3 Offset;
    float Scale;
};

layout (std430, binding = 0) buffer instanceUbo {
    InstanceData data[];
};

out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main() {
	vec3 aOffset = data[gl_InstanceID].Offset;
	float aScale = data[gl_InstanceID].Scale;

    mat4 scaledModel = mat4(
        vec4(aScale, 0.0, 0.0, 0.0),
        vec4(0.0, aScale, 0.0, 0.0),
        vec4(0.0, 0.0, aScale, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    ) * model;

	vec3 normalizeOffset = aOffset / aScale;
    vec3 pos = aPos + normalizeOffset;
    FragPos = vec3(scaledModel * vec4(pos, 1.0));
    Normal = mat3(transpose(inverse(scaledModel))) * aNormal; // transform the normal with the scaled model matrix
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    gl_Position = projection * view * scaledModel * vec4(pos, 1.0f);
}
