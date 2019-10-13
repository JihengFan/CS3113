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


class EntityF {
public:
	
	bool isActive;

	glm::vec3 position;

	GLuint textureID;
	int cols;
	int rows;
	int animIndex;


	EntityF();

	void Update(float deltaTime);
	void Render(ShaderProgram* program);

	void DrawSpriteFromTextureAtlas(ShaderProgram* program, int index);

}; 
