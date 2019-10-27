#include "Entity.h"

Entity::Entity()
{
	entityType = PLATFORM;
	isStatic = true;
	isActive = true;
	position = glm::vec3(0);
	speed = 0;
	width = 1;
	height = 1;

	gameover = false;

	animIndex = 0;
	rows = 16;
	cols = 16;
}

bool Entity::CheckCollision(Entity other)
{
	if (isStatic == true) return false;
	if (isActive == false || other.isActive == false) return false;

	float xdist = fabs(position.x - other.position.x) - ((width + other.width) / 2.0f);
	float ydist = fabs(position.y - other.position.y) - ((height + other.height) / 2.0f);

	if (xdist < 0 && ydist < 0)
	{
		return true;
	}

	return false;
}

void Entity::CheckOnTop(Entity player) {
	float xdist = fabs(position.x - player.position.x) - ((width + player.width) / 2.0f);
	float ydist = fabs(position.y - player.position.y) - 1.1;
	if (xdist < 0 && ydist < 0) {
		if (player.gameover == false) {
			isActive = false;
		}
	}

}

void Entity::CheckCollisionsY(Entity* objects, int objectCount)
{
	for (int i = 0; i < objectCount; i++)
	{
		Entity object = objects[i];

		if (CheckCollision(object))
		{
			float ydist = fabs(position.y - object.position.y);
			float penetrationY = fabs(ydist - (height / 2) - (object.height / 2));
			if (velocity.y > 0) {
				position.y -= penetrationY;
				velocity.y = 0;
				collidedTop = true;
			}
			else if (velocity.y < 0) {
				position.y += penetrationY;
				velocity.y = 0;
				collidedBottom = true;
			}
		}
	}
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
	for (int i = 0; i < objectCount; i++)
	{
		Entity object = objects[i];

		if (CheckCollision(object))
		{
			float xdist = fabs(position.x - object.position.x);
			float penetrationX = fabs(xdist - (width / 2) - (object.width / 2));
			if (velocity.x > 0) {
				position.x -= penetrationX;
				velocity.x = 0;
				collidedRight = true;
				if (entityType == PLAYER && object.entityType == ENEMY) {
					isActive = false;
					gameover = true;
				}
			}
			else if (velocity.x < 0) {
				position.x += penetrationX;
				velocity.x = 0;
				collidedLeft = true;
				if (entityType == PLAYER && object.entityType == ENEMY) {
					isActive = false;
					gameover = true;
				}
			}
		}
	}
}


void Entity::Jump()
{
	if (collidedBottom)
	{
		velocity.y = 6.5;
	}
}


void Entity::AI(Entity player) {
	switch (aiState) {
	case AROUND:
		if (position.x <= -2) {
			velocity.x = 1;
		}
		if (position.x >= 0) {
			velocity.x = -1;
		}
		break;
	case IDLE:
		if (glm::distance(position, player.position) < 3.0f) {
			if (player.position.x > position.x) {
				velocity.x = 1.0f;
			}
			else {
				velocity.x = -1.0f;
			}
		}
		break;
	case JUMP:
		if (glm::distance(position, player.position) < 3.0f) {
			acceleration.y = -9.81f;
		}
		acceleration.y = -9.81f;
		if (collidedBottom) {
			velocity.y = 4;
		}
		break;
	}

}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, int index)
{
	float u = (float)(index % cols) / (float)cols;
	float v = (float)(index / cols) / (float)rows;

	float width = 1.0f / (float)cols;
	float height = 1.0f / (float)rows;

	float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
		u, v + height, u + width, v, u, v };

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

	glBindTexture(GL_TEXTURE_2D, textureID);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Update(float deltaTime, Entity player, Entity* objects, int objectCount, Entity* enemies, int enemyCount)
{
	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;

	velocity += acceleration * deltaTime;

	position.y += velocity.y * deltaTime;        // Move on Y
	CheckCollisionsY(objects, objectCount);    // Fix if needed
	
	position.x += velocity.x * deltaTime;        // Move on X
	CheckCollisionsX(objects, objectCount);    // Fix if needed
	

	if (entityType == ENEMY) {
		AI(player);
	}

	
	if (entityType == PLAYER) {
		CheckCollisionsY(enemies, enemyCount);
	}

	if (entityType == PLAYER) {
		CheckCollisionsX(enemies, enemyCount);
	}

	if (entityType == ENEMY) {
		CheckOnTop(player);
	}


}

void Entity::RenderFont(ShaderProgram* program) {
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	program->SetModelMatrix(modelMatrix);

	DrawSpriteFromTextureAtlas(program, animIndex);
}

void Entity::Render(ShaderProgram* program) {
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	program->SetModelMatrix(modelMatrix);

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	glBindTexture(GL_TEXTURE_2D, textureID);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

