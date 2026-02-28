#pragma once
#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>

class Texture
{
public:
	unsigned int ID;
	std::string path;

	Texture(std::string ImagePath);
	~Texture();
};
