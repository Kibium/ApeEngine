#include "ModuleMSTimer.h"
#include "SDL.h"

ModuleMSTimer::ModuleMSTimer(){}

ModuleMSTimer::~ModuleMSTimer(){}

bool ModuleMSTimer::Start() {

	currentTime = 0;
	counting = true;
	return true;

}

void ModuleMSTimer::Read() {
	//In milliseconds
	if(counting)
		currentTime = SDL_GetTicks();
}

float ModuleMSTimer::Stop() {
	counting = false;
	return currentTime;
}