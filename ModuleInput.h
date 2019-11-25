#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL.h"
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
	float pitch = 0, yaw = -90;

	float currT = 0, currP = 0;
	float oldT = 0, oldP = 0;
	float theta = 0, phi = 0;
	float radius = 0;

	bool once = false;
	bool modelOnce = false;
	bool altPressed = false;
	float xOffset, yOffset;
	std::string extension;
	std::string directory;
};