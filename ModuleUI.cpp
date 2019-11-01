#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX    0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX  0x9049

#include "Application.h"
#include "ModuleUI.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
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

void ModuleUI::MyConsole() {

	ImGui::Begin("Mateus Console");
	ImGui::Text("Mateus' Console allows you to look at different parameters of your PC");
	Separate();

	//Test stuff
	if (ImGui::CollapsingHeader("Stuff to Test")) {

		//The bool value changes depending on the checkbox's state
		if (ImGui::Checkbox("Check me!", &test)) {
			ImGui::SameLine(); ImGui::Text("Hi!");
		}
	}

	if (ImGui::CollapsingHeader("Window")) {

		ImGui::Checkbox("Fullscreen", &App->window->fullscreen);
		ImGui::SameLine();
		ImGui::Checkbox("Bordered", &App->window->bordered);
			
		
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
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();

	//App->window->SetFullscreen(fullscreen);
	//App->window->SetResizable(resizable);

}

bool ModuleUI::Init() {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

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

	

	return true;
}

update_status ModuleUI::PreUpdate() {


	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleUI::Update() {


	//ImGui::ShowDemoWindow();
	MyConsole();

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