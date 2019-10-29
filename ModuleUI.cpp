#include "Application.h"
#include "ModuleUI.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include <GL/glew.h>
#include"IMGUI/imgui_impl_opengl3.h"
#include "IMGUI/imgui_impl_glfw.h"
#include "IMGUI/imgui_impl_sdl.h"


ModuleUI::ModuleUI() {}
ModuleUI::~ModuleUI() {}

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
	io.FontAllowUserScaling = true;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();


	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init("#version 330");
	io.WantCaptureKeyboard = true;
	io.WantTextInput = true;



	return true;
}

update_status ModuleUI::PreUpdate() {


	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleUI::Update() {

	ImGui::ShowDemoWindow();

	/*ImGui::Text("Hi bitches %d", 69);
	if(ImGui::Button("PRESS ME")) {
		showtext = true;
	}
	if (showtext) {
		ImGui::SameLine();
		ImGui::Text("Hah, you fool");

	}

	ImGui::InputText("<-- write here", "", 256);*/

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