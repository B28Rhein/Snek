#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include <random>

typedef glm::vec4 vec4;
typedef glm::vec3 vec3;
typedef glm::vec2 vec2;
typedef glm::mat4 mat4;
typedef unsigned int uint;

float RectVertices[] = {
	0.5f,  0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f
};
uint RectIndices[] = {
	0, 1, 3,
	1, 2, 3
};

void ProcessInput();

struct Buffers {
	uint VAO;
	uint VBO;
	uint EBO;
	void InitBuffers(float vertices[], uint indices[], int vertCount, int indiceCount) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceCount * sizeof(uint), &indices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);
	}
};

struct Rect {
	float x;
	float y;
	float dx;
	float dy;
	Shader* sh = nullptr;
	Buffers buf;
	Rect() {
		x = 0;
		y = 0;
		dx = 1;
		dy = 1;
		buf.InitBuffers(RectVertices, RectIndices, 12, 6);
		sh = nullptr;
	}
	mat4 GetModel() {
		mat4 model = mat4(1.0f);
		model = glm::translate(model, vec3(x, y, 0));
		model = glm::scale(model, vec3(dx, dy, 1));
		return model;
	}
};

struct Brush {
	vec4 colour;
	mat4 projection;
	mat4 view;
	void ClearWindow() {
		glClearColor(colour.r, colour.g, colour.b, colour.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void DrawRect(Rect rect) {
		rect.sh->use();
		rect.sh->setVec4("Colour", colour);

		rect.sh->setMat4("model", rect.GetModel());

		
		rect.sh->setMat4("view", view);

		
		rect.sh->setMat4("projection", projection);

		glBindVertexArray(rect.buf.VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
};

struct PlayerTile {
	PlayerTile* nextTile = nullptr;
	Rect r;
	PlayerTile() {
		r = Rect();
		r.x = 0;
		r.y = 0;
		r.dx = 20;
		r.dy = 20;
		r.sh = new Shader("VertexShader.glsl", "FragmentShader.glsl");
	}
	void AddTile() {
		if (nextTile == nullptr) {
			nextTile = new PlayerTile();
			nextTile->r.x = r.x;
			nextTile->r.y = r.y;
			nextTile->r.dx = r.dx;
			nextTile->r.dy = r.dy;
		}
		else {
			nextTile->AddTile();
		}
	}
	void MoveTiles() {
		if (nextTile == nullptr) {
			return;
		}
		if (!(nextTile->r.x == r.x && nextTile->r.y == r.y)) {
			nextTile->MoveTiles();
			nextTile->r.x = r.x;
			nextTile->r.y = r.y;
		}
	}
	void Draw(Brush b) {
		b.DrawRect(r);
		if(nextTile != nullptr)
			nextTile->Draw(b);
	}
	bool TileInPosition(float x, float y) {
		if (r.x == x && r.y == y) {
			return true;
		}
		if (nextTile == nullptr)
			return false;
		return nextTile->TileInPosition(x, y);
	}
};

struct Tile {
	Rect inR;
	Rect outR;
	int state;
};

struct TileMap {
	Tile* Map;
	int x;
	int y;
	float tileSizeX = 0;
	float tileSizeY = 0;
	Brush State0Brush;
	Brush State1Brush;
	Brush OutBrush;
	TileMap(int x, int y, float WinX, float WinY, Shader* sh) {
		Map = new Tile[x * y];
		this->x = x;
		this->y = y;
		float smallerDim = WinY;
		if (WinX < smallerDim) smallerDim = WinX;
		smallerDim *= 0.95;
		float dx = smallerDim / x;
		float dy = smallerDim / y;
		for (int i = 0; i < x * y; i++) {
			Map[i].inR.x = -smallerDim / 2 + dx / 2 + dx * (i % x);
			Map[i].inR.y = smallerDim / 2 - dy / 2 - dy * (i / y);
			Map[i].inR.dx = dx-dx/5;
			Map[i].inR.dy = dy-dy/5;
			Map[i].inR.sh = sh;
			Map[i].outR.x = -smallerDim / 2 + dx / 2 + dx * (i % x);
			Map[i].outR.y = smallerDim / 2 - dy / 2 - dy * (i / y);
			Map[i].outR.dx = dx;
			Map[i].outR.dy = dy;
			Map[i].outR.sh = sh;
			Map[i].state = 0;
		}
		tileSizeX = dx;
		tileSizeY = dy;
	}
	void Draw() {
		for (int i = 0; i < x * y; i++) {
			OutBrush.DrawRect(Map[i].outR);
			switch (Map[i].state)
			{
			case 0:
				State0Brush.DrawRect(Map[i].inR);
				break;
			case 1:
				State1Brush.DrawRect(Map[i].inR);
				break;
			default:
				break;
			}
			
		}
	}
};

struct Player {
	float* x;
	float* y;
	PlayerTile pt;
	Player() {
		x = &pt.r.x;
		y = &pt.r.y;
	}
	void Move(vec2 moveVector) {
		pt.MoveTiles();
		*x += moveVector.x;
		*y += moveVector.y;
	}
	void Draw(Brush b) {
		pt.Draw(b);
	}
	bool PlayerOnTile(int x, int y, TileMap map) {
		Tile t = map.Map[y * map.y + x];
		float coordX = t.inR.x;
		float coordY = t.inR.y;
		return pt.TileInPosition(coordX, coordY);
	}
};

struct Window {
	GLFWwindow* glfwWinPtr = nullptr;
	float SizeX = 0;
	float SizeY = 0;
	std::string title;
 	void CreateWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWinPtr = glfwCreateWindow(SizeX, SizeY, title.c_str(), NULL, NULL);
	}
	void SetInputMode(int mode, int value) {
		glfwSetInputMode(glfwWinPtr, mode, value);
	}
	void Use() {
		glfwMakeContextCurrent(glfwWinPtr);
	}
	void Viewport() {
		glViewport(0, 0, SizeX, SizeY);
	}
	int ShouldClose() {
		return glfwWindowShouldClose(glfwWinPtr);
	}
	void SetShouldClose() {
		glfwSetWindowShouldClose(glfwWinPtr, true);
	}
	void RenderEnd() {
		glfwPollEvents();
		glfwSwapBuffers(glfwWinPtr);
	}
	void Terminate() {
		glfwTerminate();

	}
	int GetKey(int key) {
		return glfwGetKey(glfwWinPtr, key);
	}
	float GetTime() {
		return glfwGetTime();
	}
};

struct Snek {
	Window window;
	Player* player = nullptr;
	TileMap* map = nullptr;
	int pointCounter = 0;
	float deltaTime = 0;
	float lastFrame = 0;
	float MoveCounter = 0;
	float moveSpeed = 5;
	char direction = 0;
	void MovePlayer(){
		MoveCounter += deltaTime;
		if (MoveCounter >= (1 / moveSpeed)) {
			char direction = this->direction;
			MoveCounter = 0;
			vec2 moveVector = vec2(0.0f);
			if (direction % 2 == 1) {
				moveVector += vec2(1.0, 0.0) * map->tileSizeX;
			}
			direction >>= 1;
			if (direction % 2 == 1) {
				moveVector += -vec2(0.0, 1.0) * map->tileSizeY;
			}
			direction >>= 1;
			if (direction % 2 == 1) {
				moveVector += -vec2(1.0, 0.0) * map->tileSizeX;
			}
			direction >>= 1;
			if (direction % 2 == 1) {
				moveVector += vec2(0.0, 1.0) * map->tileSizeY;
			}
			direction >>= 1;
			float playerNewX = *player->x + moveVector.x;
			float playerNewY = *player->y + moveVector.y;
			if (playerNewX < map->Map[0].inR.x) {
				moveVector.x = map->Map[map->x - 1].inR.x*2;
			}
			if (playerNewX > map->Map[map->x - 1].inR.x) {
				moveVector.x = -map->Map[map->x - 1].inR.x*2;
			}
			if (playerNewY > map->Map[0].inR.y) {
				moveVector.y = -map->Map[map->y - 1].inR.y*2;
			}
			if (playerNewY < map->Map[(map->y - 1)*map->y].inR.y) {
				moveVector.y = map->Map[map->y - 1].inR.y * 2;
			}
			Tile* t = CheckCollision(playerNewX, playerNewY);
			if (t->state == 1) {
				pointCounter += 1;
				t->state = 0;
				GenApel();
				player->pt.AddTile();
				moveSpeed += 0.5;
			}
			if (t->state != 2) {
				player->Move(moveVector);
			}
			
		}
		ShowPoints();

	}
	void CalcDeltaTime() {
		float currentFrame = window.GetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
	void GenApel() {
		std::random_device rand;
		std::uniform_int_distribution<int> distX(0, map->x-1);
		std::uniform_int_distribution<int> distY(0, map->y-1);
		int y = 0;
		int x = 0;
		do {
			x = distX(rand);
			y = distY(rand);
		} while (player->PlayerOnTile(x, y, *map));
		
		map->Map[y * map->y + x].state = 1;
	}
	Tile* CheckCollision(float X, float Y)
	{
		float smallerDim = map->tileSizeX * map->x;
		int W = round((X + smallerDim / 2 - map->tileSizeX / 2) / map->tileSizeX);
			//	X = -smallerDim/2 + map->tileSizeX/2 + map->tileSizeX * W
			// X + smallerDim/2 - map->tileSizeX/2 = map->tileSizeX * W
			// W = (X + smallerDim/2 - map->tileSizeX/2)/map->tileSizeX
			//	
		int U = round(-(Y - smallerDim / 2 + map->tileSizeY / 2) / map->tileSizeY);
		Tile* t = &map->Map[U * map->y + W];
		return t;
	}
	void ShowPoints() {
		system("cls");
		std::cout << pointCounter << "\n" << MoveCounter << "\n" << deltaTime << "\n" << 1/moveSpeed;
	}
};
Snek snek = Snek();



int main()
{
	snek.window.SizeX = 1200;
	snek.window.SizeY = 900;
	snek.window.title = "Snek";

	snek.window.CreateWindow();
	snek.window.SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	snek.window.Use();
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	snek.window.Viewport();

	Shader* sh = new Shader("VertexShader.glsl", "FragmentShader.glsl");

	snek.player = new Player();
	snek.map = new TileMap(11, 11, snek.window.SizeX, snek.window.SizeY, sh);

	snek.player->pt.r.dx = snek.map->tileSizeX;
	snek.player->pt.r.dy = snek.map->tileSizeY;

	mat4 view = mat4(1.0f);
	view = glm::translate(view, vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection;
	projection = glm::ortho(-snek.window.SizeX / 2, snek.window.SizeX / 2, -snek.window.SizeY / 2, snek.window.SizeY / 2, 0.1f, 100.0f);

	Brush backgrundBrush = Brush();
	backgrundBrush.colour = vec4(0.1f, 0.1f, 0.1f, 1.0f);

	Brush RectBrush = Brush();
	RectBrush.colour = vec4(0.5, 0.75, 0.55, 1.0);
	RectBrush.projection = projection;
	RectBrush.view = view;

	Brush State0Brush = Brush();
	State0Brush.colour = vec4(0.5, 0.75, 0.25, 1.0);
	State0Brush.projection = projection;
	State0Brush.view = view;

	Brush State1Brush = Brush();
	State1Brush.colour = vec4(0.9, 0.15, 0.25, 1.0);
	State1Brush.projection = projection;
	State1Brush.view = view;

	Brush OutBrush = Brush();
	OutBrush.colour = vec4(0.05, 0.45, 0.14, 1.0);
	OutBrush.projection = projection;
	OutBrush.view = view;

	snek.map->OutBrush = OutBrush;
	snek.map->State0Brush = State0Brush;
	snek.map->State1Brush = State1Brush;

	snek.GenApel();

	while (!snek.window.ShouldClose()) {
		snek.CalcDeltaTime();
		//Process
		ProcessInput();
		snek.MovePlayer();
		//Draw
		backgrundBrush.ClearWindow();
		snek.map->Draw();
		snek.player->Draw(RectBrush);
		//EndRender
		snek.window.RenderEnd();

	}
	snek.window.Terminate();
	delete snek.player->pt.r.sh;
	delete sh;
}

void ProcessInput() {
	if (snek.window.GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		snek.window.SetShouldClose();
	}
	if (snek.window.GetKey(GLFW_KEY_UP) == GLFW_PRESS) {
		snek.direction = 0;
		snek.direction |= 8;
	}
	if (snek.window.GetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
		snek.direction = 0;
		snek.direction |= 2;
	}
	if (snek.window.GetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
		snek.direction = 0;
		snek.direction |= 1;
	}
	if (snek.window.GetKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
		snek.direction = 0;
		snek.direction |= 4;
	}
}