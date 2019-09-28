#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"


class Entity {
public:

	glm::vec3 pad_scale;
	glm::vec3 pong_scale;

	glm::vec3 left_pos;
	glm::vec3 left_move;

	glm::vec3 right_pos;
	glm::vec3 right_move;

	glm::vec3 pong_pos;
	glm::vec3 pong_move;
	float pad_speed;
	float pong_speed_x;
	float pong_speed_y;


	Entity();

	void Update(float deltaTime);
	void Render(ShaderProgram* program);
};
