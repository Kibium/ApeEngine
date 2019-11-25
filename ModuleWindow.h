#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	virtual ~ModuleWindow();

	bool Init() override;
	update_status Update() override;
	bool CleanUp() override;

	const float GetWidth();
	const float GetHeight();

	bool fullscreen = false;
	bool bordered = true;
	bool fulldesktop = false;

	SDL_Window* window = NULL;
	SDL_Surface* screen_surface = NULL;

	int width;
	int height;
	Uint32 flags;
};

#endif // __ModuleWindow_H__