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

	bool fullscreen = false;
	bool borderless = false;
	bool resizable = true;
	bool fulldesktop = false;

	Uint32 flags;

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

	void SetFullscreen(bool);
	void SetBorderless(bool);
	void SetResizable(bool);
	void SetFulldesktop(bool);

	bool GetFullscreen();
	bool GetBorderless();
	bool GetResizable();
	bool GetFulldesktop();
};

#endif // __ModuleWindow_H__