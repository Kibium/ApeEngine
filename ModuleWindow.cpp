#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH;
		int height = SCREEN_HEIGHT;
		flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;

		

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

void ModuleWindow::SetFullscreen(bool b) {
	fullscreen = b;
}
void ModuleWindow::SetFulldesktop(bool b) {
	fulldesktop = b;
}
void ModuleWindow::SetBorderless(bool b) {
	borderless = b;
}
void ModuleWindow::SetResizable(bool b) {
	resizable = b;
}

bool ModuleWindow::GetFullscreen() {
	return fullscreen;
}
bool ModuleWindow::GetFulldesktop() {
	return fulldesktop;
}
bool ModuleWindow::GetBorderless() {
	return borderless;
}
bool ModuleWindow::GetResizable() {
	return resizable;
}

update_status ModuleWindow::Update() {
	if (fullscreen)
		flags |= SDL_WINDOW_FULLSCREEN;

	if (resizable)
		flags |= SDL_WINDOW_RESIZABLE;

	if (borderless)
		flags |= SDL_WINDOW_BORDERLESS;
	if (fulldesktop)
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

