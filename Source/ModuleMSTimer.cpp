#include "ModuleMSTimer.h"
#include "ModuleUI.h"
#include "SDL.h"

ModuleMSTimer::ModuleMSTimer(){}

ModuleMSTimer::~ModuleMSTimer(){}

bool ModuleMSTimer::Start() {
	App->ui->my_log.AddLog("Init Timer\n");

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