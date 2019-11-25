#ifndef __ModuleTextures_H__
#define __ModuleTextures_H__

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

	bool Init() override;
	update_status PreUpdate() override;
	update_status Update() override;
	update_status PostUpdate() override;
	bool CleanUp() override;

	ImageData CreateTexture(ILenum type, const char* path);
	const void RenderTextures();
	const std::vector<ImageData> getTextures();

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

	};

#endif