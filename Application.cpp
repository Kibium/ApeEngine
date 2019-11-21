#pragma once
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ModuleUI.h"
#include "ModuleTextures.h"
#include "ModelLoader.h"
#include "ModuleMSTimer.h"



using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(timer = new ModuleMSTimer());
	timer->Start();
	timer->Read();

	modules.push_back(window = new ModuleWindow());
	modules.push_back(input = new ModuleInput());
	modules.push_back(ui = new ModuleUI());

	modules.push_back(camera = new ModuleCamera());

	modules.push_back(renderer = new ModuleRender());

	modules.push_back(program = new ModuleProgram());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(modelLoader = new ModelLoader());

	ui->my_log.AddLog("------Engine Init!------\n");

	ui->my_log.AddLog("Time spent loading modules: %0.1f ms\n", timer->Stop());
}

Application::~Application()
{
	

	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }

}

bool Application::Init()
{
	timer->Start();
	timer->Read();
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();
	ui->my_log.AddLog("Time spent Initializing modules: %0.1f ms\n", timer->Stop());
	ui->my_log.AddLog("------------------------------\n");


	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}

void Application::RequestBrowser(char* web) {
	//Allows to open the web to the browser
	ShellExecute(0, 0, web, 0, 0, SW_SHOW);
}

