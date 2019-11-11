#ifndef __MICROSTIMER__
#define __MICROSTIMER__

#include "Application.h"
#include "Module.h"

class ModuleμSTimer : public Module {

	ModuleμSTimer();
	~ModuleμSTimer();

	bool Start();
	void Read();
	float Stop();

	float currentTime = 0;
	bool counting = false;


};

#endif
