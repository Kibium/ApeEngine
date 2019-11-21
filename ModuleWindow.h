#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	update_status Update();
	// Called before quitting
	bool CleanUp();

	float GetWidth();
	float GetHeight();

	bool fullscreen = false;
	bool bordered = true;
	bool fulldesktop = false;

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

	int width;
	int height;
	Uint32 flags;
};

#endif // __ModuleWindow_H__