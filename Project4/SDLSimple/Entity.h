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

enum  EntityType { PLAYER, PLATFORM, ENEMY, FONT };
enum AIType { WALKER };
enum AIState { AROUND, IDLE , JUMP };

class Entity {
public:

	EntityType entityType;
	AIType aitype;
	AIState aiState;

	bool isStatic;
	bool isActive;

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	float width;
	float height;

	float speed;

	int cols;
	int rows;
	int animIndex;

	GLuint textureID;

	Entity();

	bool CheckCollision(Entity other);

	void CheckOnTop(Entity object);

	void CheckCollisionsX(Entity* objects, int objectCount);
	void CheckCollisionsY(Entity* objects, int objectCount);

	void DrawSpriteFromTextureAtlas(ShaderProgram* program, int index);

	void AI(Entity player);
	void Update(float deltaTime, Entity player, Entity* objects, int objectCount, Entity* enemies, int enemyCount);
	void Render(ShaderProgram* program);
	void RenderFont(ShaderProgram* program);
	
	void Jump();
	bool gameover;
	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;

};



