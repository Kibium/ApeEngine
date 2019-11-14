#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL.h"

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();

private:
	SDL_MouseWheelEvent *wheel = NULL;
	bool enable_camera_movement = false;
	bool speed_boost = false;

	float currentX, lastX, currentY, lastY;
	float xOffset, yOffset;
	bool once = false;
};