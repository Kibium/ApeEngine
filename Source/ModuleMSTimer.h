#ifndef __MSTIMER_H__
#define __MSTIMER_H__

#include "Application.h"
#include "Module.h"

class ModuleMSTimer : public Module {
public:
	ModuleMSTimer();
	~ModuleMSTimer();

	bool Start();
	void Read();
	float Stop();

	float currentTime = 0;
	bool counting = false;


};

#endif
