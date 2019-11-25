#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
#include "Globals.h"
#include "SDL.h"
#include "MathGeoLib.h"
typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:

	ModuleInput();
	~ModuleInput();

	bool Init() override;
	update_status Update() override;
	bool CleanUp() override;


private:

	void FreeMovement(float& x, float& y, float& _x, float& _y);
	void Orbit(float& x, float& y, float& _x, float& _y);
	void ProcessMovement(const float3& direction);

	SDL_MouseWheelEvent *wheel = NULL;
	bool enable_camera_movement = false;
	bool speed_boost = false;

	//Free camera variables
	float currentX = 0, lastX = 0, currentY = 0, lastY = 0;
	float pitch = 0, yaw = -90;

	//Orbiting variables
	float currT = 0, currP = 0;
	float oldT = 0, oldP = 0;
	float theta = 0, phi = 0;
	float radius = 0;

	bool once = false;
	bool altPressed = false;
	float xOffset, yOffset;

	//Dragged file extension
	std::string extension;

	//Dragged file directory!
	std::string directory;
};

#endif