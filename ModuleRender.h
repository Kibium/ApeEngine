#pragma once
#include "Module.h"
#include "Globals.h"
#include <GL/glew.h>

#include "MathGeoLib.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);
	GLuint VAO, VBO, EBO;
	void* context;

	float4 bgColor;

	bool enable_cull_face = false, counter_clock_orientation = true;
	bool front_face = true, back_face = false, front_and_back = false;
	bool enable_depth_test = true;
};
