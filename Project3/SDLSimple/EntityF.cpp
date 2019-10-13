#include "EntityF.h"

EntityF::EntityF()
{
	isActive = false;
	position = glm::vec3(0);
	animIndex = 0;
	rows = 16;
	cols = 16;
}

void EntityF::Update(float deltaTime) {}

void EntityF::DrawSpriteFromTextureAtlas(ShaderProgram* program, int index)
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

void EntityF::Render(ShaderProgram* program) {
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	program->SetModelMatrix(modelMatrix);

	DrawSpriteFromTextureAtlas(program, animIndex);
}