#include "Shader.h"
#include "Snek.h"

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

Snek* snek;

int main()
{
	snek = new Snek();
	snek->GetWindow()->SetSize(900, 1200);
	snek->GetWindow()->SetTitle("Snek");

	snek->GetWindow()->CreateWindow();
	snek->GetWindow()->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	snek->GetWindow()->Use();
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	snek->GetWindow()->Viewport();

	Shader* sh = new Shader("VertexShader.glsl", "FragmentShader.glsl");

	Rect::buf = new Buffers(RectVertices, RectIndices, 12, 6);

	snek->CreateTilemap(15, 15, 900, vec2(0, -150));
	snek->CreatePlayer();

	mat4 view = mat4(1.0f);
	view = glm::translate(view, vec3(0.0f, 0.0f, -3.0f));

	vec2 WinSize = snek->GetWindow()->GetSize();

	mat4 projection;
	projection = glm::ortho(-WinSize.x / 2, WinSize.x / 2, -WinSize.y / 2, WinSize.y / 2, 0.1f, 100.0f);

	Brush backgrundBrush = Brush();
	backgrundBrush.colour = vec4(0.1f, 0.1f, 0.1f, 1.0f);

	Brush RectBrush = Brush();
	RectBrush.colour = vec4(0.5, 0.75, 0.55, 1.0);
	RectBrush.projection = projection;
	RectBrush.view = view;
	RectBrush.sh = sh;

	Brush State0Brush = Brush();
	State0Brush.colour = vec4(0.5, 0.75, 0.25, 1.0);
	State0Brush.projection = projection;
	State0Brush.view = view;
	State0Brush.sh = sh;

	Brush State1Brush = Brush();
	State1Brush.colour = vec4(0.9, 0.15, 0.25, 1.0);
	State1Brush.projection = projection;
	State1Brush.view = view;
	State1Brush.sh = sh;

	Brush OutBrush = Brush();
	OutBrush.colour = vec4(0.05, 0.45, 0.14, 1.0);
	OutBrush.projection = projection;
	OutBrush.view = view;
	OutBrush.sh = sh;

	snek->SetTilemapBrush(0, &State0Brush);
	snek->SetTilemapBrush(1, &State1Brush);
	snek->SetTilemapBrush(-1, &OutBrush);
	snek->SetPlayerBrush(&RectBrush);

	snek->GenApel();

	while (!snek->GetWindow()->ShouldClose()) {
		snek->CalcDeltaTime();
		//Process
		ProcessInput();
		snek->MovePlayer();
		//Draw
		backgrundBrush.ClearWindow();
		snek->Draw();
		//EndRender
		snek->GetWindow()->RenderEnd();

	}
	snek->GetWindow()->Terminate();
	delete sh;
	delete snek;
}

void ProcessInput() {
	if (snek->GetWindow()->GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		snek->GetWindow()->SetShouldClose();
	}
	if (snek->GetWindow()->GetKey(GLFW_KEY_UP) == GLFW_PRESS && !(snek->bannedDir == 8)) {
		snek->direction = 0;
		snek->direction |= 8;
	}
	if (snek->GetWindow()->GetKey(GLFW_KEY_DOWN) == GLFW_PRESS && !(snek->bannedDir == 2)) {
		snek->direction = 0;
		snek->direction |= 2;
	}
	if (snek->GetWindow()->GetKey(GLFW_KEY_RIGHT) == GLFW_PRESS && !(snek->bannedDir == 1)) {
		snek->direction = 0;
		snek->direction |= 1;
	}
	if (snek->GetWindow()->GetKey(GLFW_KEY_LEFT) == GLFW_PRESS && !(snek->bannedDir == 4)) {
		snek->direction = 0;
		snek->direction |= 4;
	}
}