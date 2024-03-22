#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

using namespace glm;

struct Pyramid {
    struct Vertex {
        vec3 _position;
        vec3 _normal;
    };

    struct Translation {
        vec3 _offset;
        float _scale;
    };

    std::vector<Vertex> _vertices;
    std::vector<Translation> _translations;
    vec3 _position;
    int _depth;

    Pyramid() = default;

    Pyramid(const vec3& position, float depth, float scale) : _position(position), _depth(depth) {
        // clang-format off
		
		// front vertex
		_vertices.emplace_back(glm::vec3(0.f, 1.0f, 0.f), glm::vec3(0.f, 1.0f, 1.0f));
		_vertices.emplace_back(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.f, 1.0f, 1.0f));
		_vertices.emplace_back(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.f, 1.0f, 1.0f));

		// right vertex
		_vertices.emplace_back(glm::vec3(0.f, 1.0f, 0.f), glm::vec3(1.0f, 1.0f, 0.f));
		_vertices.emplace_back(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.f));
		_vertices.emplace_back(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 0.f));

		// back vertex
		_vertices.emplace_back(glm::vec3(0.f, 1.0f, 0.f), glm::vec3(0.f, 1.0f, -1.0f));
		_vertices.emplace_back(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.f, 1.0f, -1.0f));
		_vertices.emplace_back(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.f, 1.0f, -1.0f));

		// left vertex
		_vertices.emplace_back(glm::vec3(0.f, 1.0f, 0.f), glm::vec3(-1.0f, 1.0f, 0.f));
		_vertices.emplace_back(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, 0.f));
		_vertices.emplace_back(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 0.f));

		// base left vertex
		_vertices.emplace_back(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.f, -1.0f, 0.f));
		_vertices.emplace_back(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.f, -1.0f, 0.f));
		_vertices.emplace_back(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.f, -1.0f, 0.f));

		// base right vertex
		_vertices.emplace_back(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.f, -1.0f, 0.f));
		_vertices.emplace_back(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.f, -1.0f, 0.f));
		_vertices.emplace_back(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.f, -1.0f, 0.f));
        // clang-format on
        makeFractal(_position, _depth, scale);
    }

    void makeFractal(const vec3& position, int depth, float scale) {
        if (depth == 0) {
            _translations.emplace_back(glm::vec3(position.x, position.y + scale, position.z), scale);
            _translations.emplace_back(glm::vec3(position.x - scale, position.y - scale, position.z + scale), scale);
            _translations.emplace_back(glm::vec3(position.x - scale, position.y - scale, position.z - scale), scale);
            _translations.emplace_back(glm::vec3(position.x + scale, position.y - scale, position.z + scale), scale);
            _translations.emplace_back(glm::vec3(position.x + scale, position.y - scale, position.z - scale), scale);
        } else {
            makeFractal(glm::vec3(position.x, position.y + scale, position.z), depth - 1, scale / 2.0);
            makeFractal(glm::vec3(position.x - scale, position.y - scale, position.z - scale), depth - 1, scale / 2.0);
            makeFractal(glm::vec3(position.x - scale, position.y - scale, position.z + scale), depth - 1, scale / 2.0);
            makeFractal(glm::vec3(position.x + scale, position.y - scale, position.z - scale), depth - 1, scale / 2.0);
            makeFractal(glm::vec3(position.x + scale, position.y - scale, position.z + scale), depth - 1, scale / 2.0);
        }
    }
};
