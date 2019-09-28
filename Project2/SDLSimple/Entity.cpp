#include "Entity.h"

Entity::Entity()
{
	pad_scale = glm::vec3(0.2f, 1.2f, 1.0f);
	pong_scale = glm::vec3(0.2f, 0.2f, 1.0f);

	left_pos = glm::vec3(-4.9f, 0.0f, 0.0f);

	right_pos = glm::vec3(4.9f, 0.0f, 0.0f);
	pong_pos = glm::vec3(0);
	pad_speed = 1.5;
	pong_speed_x = 2;
	pong_speed_y = 2;
}

void Entity::Update(float deltaTime)
{
	left_pos += left_move * pad_speed * deltaTime;
	if (left_pos.y >= 3.1f) {
		left_pos.y = 3.1f;
	}
	if (left_pos.y <= -3.1f) {
		left_pos.y = -3.1f;
	}

	right_pos += right_move * pad_speed * deltaTime;
	if (right_pos.y >= 3.1f) {
		right_pos.y = 3.1f;
	}
	if (right_pos.y <= -3.1f) {
		right_pos.y = -3.1f;
	}

	pong_pos.x += cos(45 * 3.14 / 180) * deltaTime * pong_speed_x;
	pong_pos.y += sin(45 * 3.14 / 180) * deltaTime * pong_speed_y;

	if (pong_pos.y > 3.6f) {
		if (pong_speed_y > 0.0f) {
			pong_speed_y *= -1.0f;
		}
	}

	if (pong_pos.y < -3.6f) {
		if (pong_speed_y < 0.0f) {
			pong_speed_y *= -1.0f;
		}
	}

	if ((pong_pos.x + 0.1f < right_pos.x - 0.1f) || (pong_pos.x - 0.1f> right_pos.x + 0.1f) || (pong_pos.y + 0.1f < right_pos.y - 0.4f) || (pong_pos.y - 0.1f > right_pos.y + 0.4f)) {}
	else {
		if (pong_speed_x > 0.0f) {
			pong_speed_x *= -1.0f;
		}
	}

	if ((pong_pos.x + 0.1f < left_pos.x - 0.1f) || (pong_pos.x - 0.1f > left_pos.x + 0.1f) || (pong_pos.y + 0.1f < left_pos.y - 0.4f) || (pong_pos.y - 0.1f > left_pos.y + 0.4f)) {}
	else {
		if (pong_speed_x < 0.0f) {
			pong_speed_x *= -1.0f;
		}
	}


	if (pong_pos.x >= 4.9f) {
		pong_pos.x = 4.9f;
		pong_speed_x = 0.0f;
		pong_speed_y = 0.0f;
		pad_speed = 0.0f;
		std::cout << "Left Player Wins The Game!" << std::endl;
	}

	if (pong_pos.x <= -4.9f) {
		pong_pos.x = -4.9f;
		pong_speed_x = 0.0f;
		pong_speed_y = 0.0f;
		pad_speed = 0.0f;
		std::cout << "Right Player Wins The Game!" << std::endl;
	}
}

void Entity::Render(ShaderProgram* program) {
	glm::mat4 leftPad = glm::mat4(1.0f);
	leftPad = glm::translate(leftPad, left_pos);
	leftPad = glm::scale(leftPad, pad_scale);
	program->SetModelMatrix(leftPad);

	float vertices1[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices1);
	glEnableVertexAttribArray(program->positionAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);



	glm::mat4 rightPad = glm::mat4(1.0f);
	rightPad = glm::translate(rightPad, right_pos);
	rightPad = glm::scale(rightPad, pad_scale);
	program->SetModelMatrix(rightPad);

	float vertices2[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
	glEnableVertexAttribArray(program->positionAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);



	glm::mat4 pong = glm::mat4(1.0f);
	pong = glm::translate(pong, pong_pos);
	pong = glm::scale(pong, pong_scale);
	program->SetModelMatrix(pong);

	float vertices3[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices3);
	glEnableVertexAttribArray(program->positionAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
}