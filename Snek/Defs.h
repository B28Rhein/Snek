#pragma once
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <random>
#include <utility>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Shader.h"

typedef glm::vec4 vec4;
typedef glm::vec3 vec3;
typedef glm::vec2 vec2;
typedef glm::mat4 mat4;
typedef unsigned int uint;

struct Brush {
	vec4 colour;
	mat4 projection;
	mat4 view;
	Shader* sh = nullptr;
	void ClearWindow() {
		glClearColor(colour.r, colour.g, colour.b, colour.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	template<class T>
	void DrawRect(T* rect) {
		sh->use();
		sh->setVec4("Colour", colour);

		sh->setMat4("model", rect->GetModel());


		sh->setMat4("view", view);


		sh->setMat4("projection", projection);

		glBindVertexArray(rect->GetVao());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
};
