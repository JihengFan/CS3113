#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"
#include "EntityF.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

#define PLATFORM_COUNT 36

#define FONT_COUNT_SUCC 14
#define FONT_COUNT_FAIL 13

struct GameState {
	Entity player;
	Entity platforms[PLATFORM_COUNT];
	EntityF fontSucc[FONT_COUNT_SUCC];
	EntityF fontFail[FONT_COUNT_FAIL];
};

GameState state;

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
	displayWindow = SDL_CreateWindow("Lunar Lander", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);

	program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

	state.player.entityType = PLAYER;
	state.player.isStatic = false;
	state.player.position = glm::vec3(0, 3.2f, 0);
	state.player.acceleration = glm::vec3(0.0f, -0.1f, 0.0f);
	state.player.textureID = LoadTexture("ship.png");


	GLuint tileTextureID1 = LoadTexture("dirt.png");
	GLuint tileTextureID2 = LoadTexture("rock.png");

	for (int i = 0; i < 10; i++) {
		if (i == 5 || i == 6) {
			state.platforms[i].textureID = tileTextureID1;
		}
		else {
			state.platforms[i].textureID = tileTextureID2;
		}
		state.platforms[i].position = glm::vec3(-4.5f + i, -3.25f, 0);
	}


	for (int i = 0; i < 7; i++) {
		state.platforms[i + 10].textureID = tileTextureID2;
		state.platforms[i + 10].position = glm::vec3(-4.5f, -2.75f + i, 0);
	}

	for (int i = 0; i < 7; i++) {
		state.platforms[i + 17].textureID = tileTextureID2;
		state.platforms[i + 17].position = glm::vec3(4.5f, -2.75f + i, 0);
	}


	state.platforms[24].textureID = tileTextureID2;
	state.platforms[24].position = glm::vec3(-3.5f, 2.75f, 0);
	state.platforms[25].textureID = tileTextureID2;
	state.platforms[25].position = glm::vec3(-3.0f, 2.75f, 0);

	state.platforms[26].textureID = tileTextureID2;
	state.platforms[26].position = glm::vec3(3.5f, 2.75f, 0);
	state.platforms[27].textureID = tileTextureID2;
	state.platforms[27].position = glm::vec3(2.7f, 2.75f, 0);
	state.platforms[28].textureID = tileTextureID2;
	state.platforms[28].position = glm::vec3(3.5f, 1.75f, 0);

	state.platforms[29].textureID = tileTextureID2;
	state.platforms[29].position = glm::vec3(0.5f, 0.75f, 0);
	state.platforms[30].textureID = tileTextureID2;
	state.platforms[30].position = glm::vec3(-0.5f, 0.75f, 0);
	state.platforms[31].textureID = tileTextureID2;
	state.platforms[31].position = glm::vec3(0.3f, 0.0f, 0);
	state.platforms[32].textureID = tileTextureID2;
	state.platforms[32].position = glm::vec3(1.0f, 0, 0);

	state.platforms[33].textureID = tileTextureID2;
	state.platforms[33].position = glm::vec3(-3.5f, 0, 0);

	state.platforms[34].textureID = tileTextureID2;
	state.platforms[34].position = glm::vec3(-2.6f, -1.75f, 0);
	state.platforms[35].textureID = tileTextureID2;
	state.platforms[35].position = glm::vec3(-2.6f, -2.75f, 0);



	int indices_successful[FONT_COUNT_SUCC] = { 77,73,83,83,73,79,78,83,85,67,67,69,83,83 };

	GLuint fontTextureID = LoadTexture("font.png");
	for (int i = 0; i < FONT_COUNT_SUCC; i++) {
		state.fontSucc[i].animIndex = indices_successful[i];
		state.fontSucc[i].textureID = fontTextureID;
		if (i <= 6) {
			state.fontSucc[i].position = glm::vec3(i - 3, 1, 0);
		}
		else {
			state.fontSucc[i].position = glm::vec3(i - 10, 0, 0);
		}
	}

	int indices_failure[FONT_COUNT_FAIL] = { 77,73,83,83,73,79,78,70,65,73,76,69,68 };

	for (int i = 0; i < FONT_COUNT_FAIL; i++) {
		state.fontFail[i].animIndex = indices_failure[i];
		state.fontFail[i].textureID = fontTextureID;
		if (i <= 6) {
			state.fontFail[i].position = glm::vec3(i - 3, 1, 0);
		}
		else {
			state.fontFail[i].position = glm::vec3(i - 9.5, 0, 0);
		}
	}

	viewMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	glUseProgram(program.programID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE:
			gameIsRunning = false;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_SPACE:
				// Some sort of action
				break;

			}
			break;
		}
	}

	state.player.acceleration.x = 0;
	// Check for pressed/held keys below
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_A])
	{
		state.player.acceleration.x = -1.0f;
	}
	else if (keys[SDL_SCANCODE_D])
	{
		state.player.acceleration.x = 1.0f;
	}
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;

	deltaTime += accumulator;
	if (deltaTime < FIXED_TIMESTEP) {
		accumulator = deltaTime;
		return;
	}

	while (deltaTime >= FIXED_TIMESTEP) {
		// Update. Notice it's FIXED_TIMESTEP. Not deltaTime
		state.player.Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);

		deltaTime -= FIXED_TIMESTEP;
	}

	accumulator = deltaTime;
}


void Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	state.player.Render(&program);

	for (int i = 0; i < PLATFORM_COUNT; i++) {
		state.platforms[i].Render(&program);
	}

	if (state.player.isActive == false) {
		if (state.player.missionSucc == false) {
			for (int i = 0; i < FONT_COUNT_FAIL; i++) {
				state.fontFail[i].Render(&program);
			}
		}
		else {
			for (int i = 0; i < FONT_COUNT_SUCC; i++) {
				state.fontSucc[i].Render(&program);
			}
		}
	}


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