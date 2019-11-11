#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX    0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX  0x9049

#include "Application.h"
#include "ModuleUI.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include <GL/glew.h>
#include"IMGUI/imgui_impl_opengl3.h"
#include "IMGUI/imgui_impl_glfw.h"
#include "IMGUI/imgui_impl_sdl.h"

#include <iostream>
using namespace std;


ModuleUI::ModuleUI() {
}
ModuleUI::~ModuleUI() {}

void ModuleUI::Separate() {
	ImGui::Separator();
	ImGui::Spacing();
}

void ModuleUI::FPSHistogram() {
	char title[25];
	sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
	ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

	sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
	ImGui::PlotHistogram("##framerate", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
}

void ModuleUI::MyConsole() {


	ImGui::Begin("Mateus Console");
	ImGui::Text("Mateus' Console allows you to look at different parameters of your PC");
	Separate();

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::MenuItem("ImGui"))
			App->RequestBrowser("https://github.com/ocornut/imgui");

		if (ImGui::MenuItem("Mateus Doc."))
			App->RequestBrowser("https://github.com/Kibium/Engine_Master");

		ImGui::EndMainMenuBar();
	}


	if (showLines)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (ImGui::CollapsingHeader("Engine")) {
		ImGui::InputText("Name", title, 25);
		SDL_SetWindowTitle(App->window->window, title);

		ImGui::Checkbox("MAG Filtering", &App->textures->MAGfilter);
		ImGui::SameLine();
		ImGui::Checkbox("MIN Filtering", &App->textures->MINfilter);
		Separate();

		ImGui::Checkbox("Enable Wrap_S", &App->textures->WRAPs);
		ImGui::SameLine();
		ImGui::Checkbox("Enable Wrap_T", &App->textures->WRAPt);
		Separate();

		//ImGui::Checkbox("Enable MipMap", &App->textures->mipmap);


	}

	if (ImGui::CollapsingHeader("Camera")) {
		if (ImGui::TreeNode("Settings")) {
			if (ImGui::SliderFloat("Far Plane", &App->camera->farP, 50, 150)) {
				glUniformMatrix4fv(App->program->projLocation, 1, GL_TRUE, &App->camera->proj[0][0]);
				App->camera->ProcessMatrixs();

			}

			if (ImGui::SliderFloat("Near Plane", &App->camera->nearP, 0.1, 40)) {
				glUniformMatrix4fv(App->program->projLocation, 1, GL_TRUE, &App->camera->proj[0][0]);
				App->camera->ProcessMatrixs();

			}
			Separate();

			if (ImGui::SliderFloat("FOV", &App->camera->vFov, 1, 90)) {
				glUniformMatrix4fv(App->program->projLocation, 1, GL_TRUE, &App->camera->proj[0][0]);
				App->camera->ProcessMatrixs();

			}

			if (ImGui::SliderFloat("Aspect", &App->camera->AR, 1, 90)) {
				glUniformMatrix4fv(App->program->projLocation, 1, GL_TRUE, &App->camera->proj[0][0]);
				App->camera->ProcessMatrixs();

			}

			Separate();

			if (ImGui::Button("Reset camera")) {
			
				App->camera->ResetCamera();
			}

			//if (ImGui::SliderFloat("Aspect Ratio", &App->camera->AR, 0.1, 40))
			//	glUniformMatrix4fv(App->program->projLocation, 1, GL_TRUE, &App->camera->proj[0][0]);

			ImGui::TreePop();

		}

		if (ImGui::TreeNode("Position")) {
			if (ImGui::SliderFloat("X", &App->camera->cameraPos.x, -10, 10)) {
				glUniformMatrix4fv(App->program->viewLocation, 1, GL_TRUE, &App->camera->view[0][0]);
				App->camera->ProcessMatrixs();

			}

			if (ImGui::SliderFloat("Y", &App->camera->cameraPos.y, -10, 10)) {
				glUniformMatrix4fv(App->program->viewLocation, 1, GL_TRUE, &App->camera->view[0][0]);
				App->camera->ProcessMatrixs();

			}

			if (ImGui::SliderFloat("Z", &App->camera->cameraPos.z, -10, 10)) {
				glUniformMatrix4fv(App->program->viewLocation, 1, GL_TRUE, &App->camera->view[0][0]);
				App->camera->ProcessMatrixs();

			}
			ImGui::TreePop();

		}

		if (ImGui::TreeNode("Rotation")) {
			if (ImGui::SliderFloat("X", &App->camera->rotX, -10, 10)) {
				glUniformMatrix4fv(App->program->modelLocation, 1, GL_TRUE, &App->camera->model[0][0]);
				App->camera->ProcessMatrixs();


			}

			if (ImGui::SliderFloat("Y", &App->camera->rotY, -10, 10)) {
				glUniformMatrix4fv(App->program->modelLocation, 1, GL_TRUE, &App->camera->model[0][0]);
				App->camera->ProcessMatrixs();


			}

			if (ImGui::SliderFloat("Z", &App->camera->rotZ, -10, 10)) {
				glUniformMatrix4fv(App->program->modelLocation, 1, GL_TRUE, &App->camera->model[0][0]);
				App->camera->ProcessMatrixs();


			}
			ImGui::TreePop();
		}
	}

	if (ImGui::CollapsingHeader("Images stuff")) {
		if (ImGui::TreeNode("Triangle stuff")) {
			ImGui::Checkbox("Show Lines", &showLines);
			ImGui::Checkbox("Triangle Mode", &App->renderer->mode);
			ImGui::TreePop();
		}

		Separate();

		ImGui::Text("Swap image button ->");
		ImGui::SameLine();
		if (ImGui::Button("Click me pls")) {
			App->textures->imageButtonValue++;
			App->textures->once = false;
		}
	}

	if (ImGui::CollapsingHeader("Window")) {

		ImGui::Checkbox("Fullscreen", &App->window->fullscreen);
		ImGui::SameLine();
		ImGui::Checkbox("Bordered", &App->window->bordered);
		Separate();
		ImGui::SliderFloat("Brightness", &brightness, 0, 1);
		Separate();
		if (ImGui::SliderInt("Window width", &screenW, 300, 800))
			SDL_SetWindowSize(App->window->window, screenW, screenH);

		if (ImGui::SliderInt("Window height", &screenH, 300, 800))
			SDL_SetWindowSize(App->window->window, screenW, screenH);
	}

	if (ImGui::CollapsingHeader("Hardware")) {
		ImGui::Text("OS:");
		ImGui::SameLine(); ImGui::TextColored(ImVec4(204, 204, 0, 1), "%s", SDL_GetCurrentVideoDriver());

		ImGui::Text("Number of Logical CPU cores:");
		ImGui::SameLine(); ImGui::TextColored(ImVec4(204, 204, 0, 1), "%d", SDL_GetCPUCount());


		ImGui::Text("System RAM (Mb):");
		ImGui::SameLine(); ImGui::TextColored(ImVec4(204, 204, 0, 1), "%d", SDL_GetSystemRAM());

		ImGui::Text("GPU Name:");
		ImGui::SameLine(); ImGui::TextColored(ImVec4(204, 204, 0, 1), "%s", glGetString(GL_RENDERER));

		ImGui::Text("GPU Version:");
		ImGui::SameLine(); ImGui::TextColored(ImVec4(204, 204, 0, 1), "%s", glGetString(GL_VERSION));

		ImGui::Text("Manufacturer:");
		ImGui::SameLine(); ImGui::TextColored(ImVec4(204, 204, 0, 1), "%s", glGetString(GL_VENDOR));

	}

	if (ImGui::CollapsingHeader("About")) {
		ImGui::Text("Engine name: %s", title);
		ImGui::Text("This engine's gonna be godlike.");
		ImGui::Text("The author of this engine is: Toni Ferrari.");
		ImGui::Text("Libraries used:");
		ImGui::BulletText("ImGui");
		ImGui::BulletText("OpenGL 3.0");
		ImGui::BulletText("SDL 2.1.0");
	}

	if (ImGui::CollapsingHeader("Log"))
		my_log.Draw("Log");

	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	if (SDL_GetTicks() >= lastTicks + 1000) {
		fps_log.push_back(ImGui::GetIO().Framerate);
		ms_log.push_back(1000.0f / ImGui::GetIO().Framerate);
		lastTicks = SDL_GetTicks();
	}

	FPSHistogram();
	SDL_SetWindowBrightness(App->window->window, brightness);
	ImGui::End();

}

bool ModuleUI::Init() {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}
	//my_log.AddLog("hola");
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;


	// Setup Dear ImGui style
	ImGui::StyleColorsDark();


	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init("#version 330");

	fps_log.push_back(0);
	ms_log.push_back(0);


	return true;
}

update_status ModuleUI::PreUpdate() {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleUI::Update() {

	MyConsole();
	//ImGui::ShowDemoWindow();
	return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate() {
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//Must be here in order to the UI window appear
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(App->renderer->context);
	SDL_DestroyWindow(App->window->window);
	SDL_Quit();
	return true;
}