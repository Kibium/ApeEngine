#pragma once
#include "Application.h"
#include "Module.h"
#include "Globals.h"
#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include <vector>

struct ImageData {
	int width = 0;
	int height = 0;
	int size = 0;
	GLuint texture;
	ILubyte* data = nullptr;
	ILuint imageName;
};

class ModuleTextures : public Module {

public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();

	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	ImageData CreateTexture(ILenum type, const char* path);
	const void RenderTextures();
	const std::vector<ImageData> getTextures();


	bool CleanUp();
	GLuint texture1, texture2, texture3;
	//ILuint kirbo, lenna, muffin;

	int width;
	int height;

	bool repeat = false, mirrored = false, edge = false, border = false, nearest = true;

	//Which image to show
	int imageButtonValue = 0;
	bool once = false; //so it doesn't execude code all the time

	//Allows to draw the image
	bool mipmap = true;

private:
	std::vector<ImageData> images;
	std::vector<ILinfo*> Iinfo;
	ImageData kirb;
	ImageData lenna;
	ImageData muffin;
	};