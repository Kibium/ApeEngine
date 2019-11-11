#include "ModuleμSTimer.h"
#include "SDL.h"

ModuleμSTimer::ModuleμSTimer() {}

ModuleμSTimer::~ModuleμSTimer() {}

bool ModuleμSTimer::Start() {

	currentTime = 0;
	counting = true;
	return true;

}

void ModuleμSTimer::Read() {
	//In milliseconds
	currentTime = SDL_GetPerformanceCounter();
}

float ModuleμSTimer::Stop() {
	counting = false;
	return currentTime;
}