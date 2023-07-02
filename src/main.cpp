#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "camera.h"
#include "shader.h"

static constexpr float WIDTH = 1500.0f;
static constexpr float HEIGHT = 1000.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, WIDTH, HEIGHT);

void mouseCallback([[maybe_unused]] GLFWwindow* window, double xpos_in, double ypos_in) {
    camera.processMouseMovement(xpos_in, ypos_in);
}

void scrollCallback([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] double xoffset, double yoffset) {
    camera.processMouseScroll(static_cast<float>(yoffset));
}

void makeFractal(std::vector<glm::vec3>& translations, glm::vec3 position, int depth, double scale) {
    if (depth == 0) {
        translations.emplace_back(glm::vec3(position.x, position.y + scale, position.z));
        translations.emplace_back(glm::vec3(scale));
        translations.emplace_back(glm::vec3(position.x - scale, position.y - scale, position.z + scale));
        translations.emplace_back(glm::vec3(scale));
        translations.emplace_back(glm::vec3(position.x - scale, position.y - scale, position.z - scale));
        translations.emplace_back(glm::vec3(scale));
        translations.emplace_back(glm::vec3(position.x + scale, position.y - scale, position.z + scale));
        translations.emplace_back(glm::vec3(scale));
        translations.emplace_back(glm::vec3(position.x + scale, position.y - scale, position.z - scale));
        translations.emplace_back(glm::vec3(scale));
    } else {
        makeFractal(translations, glm::vec3(position.x, position.y + scale, position.z), depth - 1, scale / 2.0f);
        makeFractal(translations, glm::vec3(position.x - scale, position.y - scale, position.z - scale), depth - 1, scale / 2.0f);
        makeFractal(translations, glm::vec3(position.x - scale, position.y - scale, position.z + scale), depth - 1, scale / 2.0f);
        makeFractal(translations, glm::vec3(position.x + scale, position.y - scale, position.z - scale), depth - 1, scale / 2.0f);
        makeFractal(translations, glm::vec3(position.x + scale, position.y - scale, position.z + scale), depth - 1, scale / 2.0f);
    }
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "cube", nullptr, nullptr);

    if (window == nullptr) {
        throw std::runtime_error("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD\n");
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    std::vector<glm::vec3> vertices = {
        // clang-format off
        // front vertex
		glm::vec3(0.f, 1.0f, 0.f), glm::vec3(0.f, 1.0f, 1.0f),
		glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.f, 1.0f, 1.0f),
		glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.f, 1.0f, 1.0f),

		// right vertex
		glm::vec3(0.f, 1.0f, 0.f), glm::vec3(1.0f, 1.0f, 0.f),
		glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.f),
		glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 0.f),

		// back vertex
		glm::vec3(0.f, 1.0f, 0.f), glm::vec3(0.f, 1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.f, 1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.f, 1.0f, -1.0f),

		// left vertex
		glm::vec3(0.f, 1.0f, 0.f), glm::vec3(-1.0f, 1.0f, 0.f),
		glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, 0.f),
		glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 0.f),

		// base left vertex
		glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.f, -1.0f, 0.f),
		glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.f, -1.0f, 0.f),
		glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.f, -1.0f, 0.f),

		// base right vertex
		glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.f, -1.0f, 0.f),
		glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.f, -1.0f, 0.f),
		glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.f, -1.0f, 0.f),
        // clang-format on
    };

    std::vector<glm::vec3> translations;

    makeFractal(translations, glm::vec3(0.0f), 5, 1.0f);

    Shader shader("../shaders/shader.vert.glsl", "../shaders/shader.frag.glsl");

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);

    GLuint INSTANCE_VBO;
    glGenBuffers(1, &INSTANCE_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, INSTANCE_VBO);
    glBufferData(GL_ARRAY_BUFFER, translations.size() * sizeof(glm::vec3), translations.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, INSTANCE_VBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    glEnable(GL_DEPTH_TEST);

    float delta_time = 0.0f;
    float last_frame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        delta_time = currentFrame - last_frame;
        last_frame = currentFrame;

        camera.processKeyboard(window, delta_time);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), WIDTH / HEIGHT, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, static_cast<float>(glfwGetTime()) * glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setMat4("model", model);

        shader.setVec3("objectColor", glm::vec3(0.94f, 0.76f, 0.69f));
        shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setVec3("lightPos", glm::vec3(10.0f, 10.0f, 10.0f));
        shader.setVec3("viewPos", camera.Position);

        glDrawArraysInstanced(GL_TRIANGLES, 0, vertices.size(), translations.size());

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
