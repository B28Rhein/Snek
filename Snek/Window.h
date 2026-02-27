#pragma once
#include "Defs.h"
class Window
{
	GLFWwindow* glfwWinPtr = nullptr;
	float SizeX = 0;
	float SizeY = 0;
	std::string title;

public:
	void CreateWindow();
	void SetSize(float X, float Y);
	vec2 GetSize();
	void SetTitle(std::string title);
	void SetInputMode(int mode, int value);
	void Use();
	void Viewport();
	int ShouldClose();
	void SetShouldClose();
	void RenderEnd();
	void Terminate();
	int GetKey(int key);
	float GetTime();
};

