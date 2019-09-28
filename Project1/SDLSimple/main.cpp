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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program_untextured;
ShaderProgram program_textured;
glm::mat4 viewMatrix, modelMatrix1, modelMatrix2, modelMatrix3, projectionMatrix;

float lastTicks = 0.0f;

float player_x = 0.0f;
float rotate_z = 0.0f;
float player_y = 0.0f;
float player_z = 0.0f;


GLuint playerTextureID1;
GLuint playerTextureID2;

GLuint LoadTexture(const char* filePath) {
	int w, h, n;
	unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

	if (image == NULL) {
		std::cout << "Unable to load image. Make sure the path is correct\n";
		assert(false);
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(image);
	return textureID;
}


void Initialize() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Simple 2D Scene!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);

	program_untextured.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
	
	program_textured.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
	playerTextureID1 = LoadTexture("me.png");
	playerTextureID2 = LoadTexture("dog.png");


	viewMatrix = glm::mat4(1.0f);
	modelMatrix1 = glm::mat4(1.0f);
	modelMatrix2 = glm::mat4(1.0f);
	modelMatrix3 = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	program_untextured.SetProjectionMatrix(projectionMatrix);
	program_untextured.SetViewMatrix(viewMatrix);
	program_untextured.SetColor(1.0f, 0.0f, 0.0f, 1.0f);

	program_textured.SetProjectionMatrix(projectionMatrix);
	program_textured.SetViewMatrix(viewMatrix);

	glUseProgram(program_untextured.programID);
	glUseProgram(program_textured.programID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			gameIsRunning = false;
		}
	}
}


void Update() {

	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;
	
	player_x += 1.0f * deltaTime;
	player_y += 0.4f * deltaTime;
	player_z += 0.6f * deltaTime;
	rotate_z += 50.0 * deltaTime;

}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	modelMatrix1 = glm::mat4(1.0f);
	modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(3.0f, 2.75f, 0.0f));
	modelMatrix1 = glm::rotate(modelMatrix1, glm::radians(rotate_z), glm::vec3(0.0f, 0.0f, 1.0f));

	program_untextured.SetModelMatrix(modelMatrix1);

	float vertices1[] = { 0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5f };
	glVertexAttribPointer(program_untextured.positionAttribute, 2, GL_FLOAT, false, 0, vertices1);
	glEnableVertexAttribArray(program_untextured.positionAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(program_untextured.positionAttribute);


	modelMatrix2 = glm::mat4(1.0f);

	if (player_x < 4.0f) {
		modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(-3.5f, 0.0f, 0.0f));
		modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(player_x, 0.0f, 0.0f));
	}
	else if (player_x > 4.0f) {
		modelMatrix2 = glm::rotate(modelMatrix2, glm::radians(rotate_z), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(-player_z, 0.0f, 0.0f));
	}
	

	program_textured.SetModelMatrix(modelMatrix2);
	
	float vertices2[] = { -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0 };
	float texCoords2[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	glBindTexture(GL_TEXTURE_2D, playerTextureID1);

	glVertexAttribPointer(program_textured.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
	glEnableVertexAttribArray(program_textured.positionAttribute);

	glVertexAttribPointer(program_textured.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords2);
	glEnableVertexAttribArray(program_textured.texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program_textured.positionAttribute);
	glDisableVertexAttribArray(program_textured.texCoordAttribute);



	modelMatrix3 = glm::mat4(1.0f);
	modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(3.5f, 0.0f, 0.0f));
	if (player_x < 3.5f) {
		modelMatrix3 = glm::rotate(modelMatrix3, glm::radians(rotate_z), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(player_y, 0.0f, 0.0f));
	}
	else if (player_x > 3.5f) {
		modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(-player_y, 0.0f, 0.0f));
	}

	program_textured.SetModelMatrix(modelMatrix3);

	float vertices3[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float texCoords3[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	glBindTexture(GL_TEXTURE_2D, playerTextureID2);

	glVertexAttribPointer(program_textured.positionAttribute, 2, GL_FLOAT, false, 0, vertices3);
	glEnableVertexAttribArray(program_textured.positionAttribute);

	glVertexAttribPointer(program_textured.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords3);
	glEnableVertexAttribArray(program_textured.texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program_textured.positionAttribute);
	glDisableVertexAttribArray(program_textured.texCoordAttribute);



	SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	Initialize();

	while (gameIsRunning) {
		ProcessInput();
		Update();
		Render();
	}

	Shutdown();
	return 0;
}
