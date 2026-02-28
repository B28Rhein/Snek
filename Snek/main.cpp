#include "Shader.h"
#include "Snek.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
float RectVertices[] = {
	0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
};
uint RectIndices[] = {
	0, 1, 3,
	1, 2, 3
};

float LetterVerices[] = {
	0.5f,  0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
};
float LetterTexVertices[] = {
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f
};

void ProcessInput();

Snek* snek;
int WinMain();


int main()
{
	snek = new Snek();
	snek->isInTextureMode = true;
	stbi_set_flip_vertically_on_load(true);
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	Shader* sh = new Shader("VertexShader.glsl", "FragmentShader.glsl");
	Shader* texSh = new Shader("VertexShader.glsl", "FragTexShader.glsl");

	Rect::buf = new Buffers(RectVertices, RectIndices, 20, 6);
	Rect::letterBuf = new Buffers(LetterVerices, LetterTexVertices, RectIndices, 12, 6);

	snek->CreateTilemap(15, 15, 900, vec2(0, -150));
	snek->CreatePlayer();

	mat4 view = mat4(1.0f);
	view = glm::translate(view, vec3(0.0f, 0.0f, -3.0f));

	vec2 WinSize = snek->GetWindow()->GetSize();

	mat4 projection;
	projection = glm::ortho(-WinSize.x / 2, WinSize.x / 2, -WinSize.y / 2, WinSize.y / 2, 0.1f, 100.0f);

	Brush backgrundBrush = Brush();
	backgrundBrush.colour = vec4(0.1f, 0.1f, 0.1f, 1.0f);

	Brush headBrush = Brush();
	headBrush.colour = vec4(0.5, 0.75, 0.55, 1.0);
	headBrush.projection = projection;
	headBrush.view = view;
	headBrush.sh = sh;
	headBrush.texSh = texSh;
	headBrush.texture = new Texture("Textures\\snekHead.png");

	Brush mid1Brush = Brush();
	mid1Brush.colour = vec4(0.9, 0.55, 0.55, 1.0);
	mid1Brush.projection = projection;
	mid1Brush.view = view;
	mid1Brush.sh = sh;
	mid1Brush.texSh = texSh;
	mid1Brush.texture = new Texture("Textures\\snekStraight.png");

	Brush mid2Brush = Brush();
	mid2Brush.colour = vec4(0.3, 0.45, 0.95, 1.0);
	mid2Brush.projection = projection;
	mid2Brush.view = view;
	mid2Brush.sh = sh;
	mid2Brush.texSh = texSh;
	mid2Brush.texture = new Texture("Textures\\snekCurveR.png");

	Brush tailBrush = Brush();
	tailBrush.colour = vec4(1, 0.25, 0.15, 1.0);
	tailBrush.projection = projection;
	tailBrush.view = view;
	tailBrush.sh = sh;
	tailBrush.texSh = texSh;
	tailBrush.texture = new Texture("Textures\\snekEnd.png");

	Brush State0Brush = Brush();
	State0Brush.colour = vec4(0.5, 0.75, 0.25, 1.0);
	State0Brush.projection = projection;
	State0Brush.view = view;
	State0Brush.sh = sh;
	State0Brush.texSh = texSh;

	Brush State1Brush = Brush();
	State1Brush.colour = vec4(0.9, 0.15, 0.25, 1.0);
	State1Brush.projection = projection;
	State1Brush.view = view;
	State1Brush.sh = sh;
	State1Brush.texSh = texSh;
	State1Brush.texture = new Texture("Textures\\apel.png");

	Brush OutBrush = Brush();
	OutBrush.colour = vec4(0.05, 0.45, 0.14, 1.0);
	OutBrush.projection = projection;
	OutBrush.view = view;
	OutBrush.sh = sh;

	Brush LetterBrush = Brush();
	LetterBrush.colour = vec4(0);
	LetterBrush.projection = projection;
	LetterBrush.view = view;
	LetterBrush.sh = sh;
	LetterBrush.texSh = texSh;
	//https://frostyfreeze.itch.io/pixel-bitmap-fonts-png-xml
	LetterBrush.texture = new Texture("Textures\\characters.png");

	snek->SetTilemapBrush(0, &State0Brush);
	snek->SetTilemapBrush(1, &State1Brush);
	snek->SetTilemapBrush(-1, &OutBrush);
	snek->SetPlayerBrush(&headBrush, &mid1Brush, &mid2Brush, &tailBrush);
	snek->SetLetterBrush(&LetterBrush);

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

int WinMain() {
	return main();
}

void ProcessInput() {
	if (snek->GetWindow()->GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		snek->GetWindow()->SetShouldClose();
	}
	if (snek->GetWindow()->GetKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
		snek->PauseUnpause();
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