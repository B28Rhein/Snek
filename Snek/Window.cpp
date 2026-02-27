#include "Window.h"

void Window::CreateWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWinPtr = glfwCreateWindow(SizeX, SizeY, title.c_str(), NULL, NULL);
}

void Window::SetSize(float X, float Y)
{
	SizeX = X;
	SizeY = Y;
}

vec2 Window::GetSize()
{
	return vec2(SizeX, SizeY);
}

void Window::SetTitle(std::string title)
{
	this->title = title;
}

void Window::SetInputMode(int mode, int value) {
	glfwSetInputMode(glfwWinPtr, mode, value);
}

void Window::Use() {
	glfwMakeContextCurrent(glfwWinPtr);
}

void Window::Viewport() {
	glViewport(0, 0, SizeX, SizeY);
}

int Window::ShouldClose() {
	return glfwWindowShouldClose(glfwWinPtr);
}

void Window::SetShouldClose() {
	glfwSetWindowShouldClose(glfwWinPtr, true);
}

void Window::RenderEnd() {
	glfwPollEvents();
	glfwSwapBuffers(glfwWinPtr);
}

void Window::Terminate() {
	glfwTerminate();

}

int Window::GetKey(int key) {
	return glfwGetKey(glfwWinPtr, key);
}

float Window::GetTime() {
	return glfwGetTime();
}
