#ifndef __ModuleRender_H__
#define __ModuleRender_H__

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

	bool Init() override;
	update_status PreUpdate() override;
	update_status Update() override;
	update_status PostUpdate() override;
	bool CleanUp() override;

	void DrawGrid();
	void DrawAxis();

	void WindowResized(unsigned width, unsigned height);
	GLuint VAO, VBO, EBO;
	void* context;

	float4 bgColor;

	//The UI allows to modify these bools, which allow to modify these render variables
	bool enable_cull_face = false, counter_clock_orientation = true;
	bool front_face = true, back_face = false, front_and_back = false;
	bool enable_depth_test = true;

	bool enable_grid = true, enable_axis = true;
};

#endif