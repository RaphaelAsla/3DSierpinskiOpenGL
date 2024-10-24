#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "camera.hpp"
#include "pyramid.hpp"
#include "shader.hpp"

using namespace glm;

static Camera _camera = Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 1920, 1024);

static void mouseCallback([[maybe_unused]] GLFWwindow* window, double xpos_in, double ypos_in) {
    _camera.processMouseMovement(xpos_in, ypos_in);
}

static void scrollCallback([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] double xoffset, double yoffset) {
    _camera.processMouseScroll(static_cast<float>(yoffset));
}

struct Renderer {
    GLFWwindow* _window;
    Pyramid _fractal;
    Shader _shader;
    Shader _depth_shader;
    vec2 _window_size;
    vec2 _depth_map_size;
    GLuint _vao;
    GLuint _vbo;
    GLuint _ssbo;
    GLuint _depth_map_fbo;
    GLuint _depth_map_texture;
    float _last_render;

    Renderer(const vec2& window_size) : _window_size(window_size), _vao(0) {
        // Setup GLFW
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        _window = glfwCreateWindow(window_size.x, window_size.y, "Sierpinski Pyramid", nullptr, nullptr);

        // Setup OpenGL
        glfwMakeContextCurrent(_window);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glfwSwapInterval(1);

        glfwSetCursorPosCallback(_window, mouseCallback);
        glfwSetScrollCallback(_window, scrollCallback);

        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_FRAMEBUFFER_SRGB);

        _shader = {"../shaders/shader.vert.glsl", "../shaders/shader.frag.glsl"};
        _depth_shader = {"../shaders/depthShader.vert.glsl", "../shaders/depthShader.frag.glsl"};

        _fractal = Pyramid(vec3(0.0f, 0.0f, 0.0f), 5, 1);
        _depth_map_size = vec2(2024, 2024);

        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, _fractal._vertices.size() * sizeof(glm::vec3) * 2, _fractal._vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));

        // Create and bind the SSBO
        glGenBuffers(1, &_ssbo);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, _fractal._translations.size() * (sizeof(vec3) + sizeof(float)), _fractal._translations.data(), GL_STATIC_DRAW);

        // Bind the SSBO to a binding point
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _ssbo);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glGenFramebuffers(1, &_depth_map_fbo);

        glGenTextures(1, &_depth_map_texture);
        glBindTexture(GL_TEXTURE_2D, _depth_map_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _depth_map_size.x, _depth_map_size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        glBindFramebuffer(GL_FRAMEBUFFER, _depth_map_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth_map_texture, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        _shader.use();
        _shader.setInt("shadowMap", 0);
    }

    ~Renderer() {
        glfwDestroyWindow(_window);
        glfwTerminate();
    }

    void run() {
        float delta_time = 0.0f;
        float last_frame = 0.0f;

        glm::vec3 lightPos(2.0f, 2.0f, 3.0f);

        while (!glfwWindowShouldClose(_window)) {
            float currentFrame = static_cast<float>(glfwGetTime());
            delta_time = currentFrame - last_frame;
            last_frame = currentFrame;

            _camera.processKeyboard(_window, delta_time);

            float near_plane = 0.01f, far_plane = 15.0f;
            glm::mat4 lightProjection = glm::ortho(-8.0f, 8.0f, -8.0f, 8.0f, near_plane, far_plane);
            glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
            glm::mat4 lightSpaceMatrix = lightProjection * lightView;
            glm::mat4 projection = glm::perspective(glm::radians(_camera.Zoom), _window_size.x / _window_size.y, 0.1f, 100.0f);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            _depth_shader.use();
            _depth_shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

            glViewport(0, 0, _depth_map_size.x, _depth_map_size.y);
            glBindFramebuffer(GL_FRAMEBUFFER, _depth_map_fbo);
            glClear(GL_DEPTH_BUFFER_BIT);

            glCullFace(GL_FRONT);
            renderScene(_depth_shader);
            glCullFace(GL_BACK);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glViewport(0, 0, _window_size.x, _window_size.y);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            _shader.use();

            _shader.setMat4("projection", projection);
            _shader.setMat4("view", _camera.getViewMatrix());
            _shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

            _shader.setVec3("lightPos", vec3(2.0f, 2.0, 3.0));
            _shader.setVec3("viewPos", _camera.Position);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _depth_map_texture);
            renderScene(_shader);

            glfwPollEvents();
            glfwSwapBuffers(_window);
        }
    }

    void renderScene(const Shader& shader) {
        mat4 model = mat4(1.0f);
        model = glm::rotate(model, static_cast<float>(glfwGetTime()) * glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbo);
        glBindVertexArray(_vao);
        glDrawArraysInstanced(GL_TRIANGLES, 0, _fractal._vertices.size(), _fractal._translations.size());
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        glBindVertexArray(0);
    }
};
