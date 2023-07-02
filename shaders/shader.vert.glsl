#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aOffset;
layout (location = 3) in vec3 aScale;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 objectColor;

void main() {
    mat4 scaledModel = mat4(
        vec4(aScale.x, 0.0, 0.0, 0.0),
        vec4(0.0, aScale.y, 0.0, 0.0),
        vec4(0.0, 0.0, aScale.z, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    ) * model;

	vec3 normalizeOffset = aOffset / aScale;
    vec3 pos = aPos + normalizeOffset;
    gl_Position = projection * view * scaledModel * vec4(pos, 1.0f);
    FragPos = vec3(scaledModel * vec4(pos, 1.0));
    Normal = mat3(transpose(inverse(scaledModel))) * aNormal; // transform the normal with the scaled model matrix
}
