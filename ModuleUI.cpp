#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX    0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX  0x9049

#include "Application.h"
#include "ModuleUI.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModelLoader.h"

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

void ModuleUI::ShowProperties() {
	ImGui::Begin("Properties");
	Separate();
	if (ImGui::CollapsingHeader("Transform")) {

		if(ImGui::TreeNode("Position")) {
			ImGui::TextColored(RED, "X: "); ImGui::SameLine();
			ImGui::TextColored(TURQ, "%0.3f", (float)App->modelLoader->model.scene->mRootNode->mTransformation.a4);
			ImGui::TextColored(GREEN, "Y: "); ImGui::SameLine();
			ImGui::TextColored(TURQ, "%0.3f", (float)App->modelLoader->model.scene->mRootNode->mTransformation.b4);
			ImGui::TextColored(BLUE, "Z: "); ImGui::SameLine();
			ImGui::TextColored(TURQ, "%0.3f", (float)App->modelLoader->model.scene->mRootNode->mTransformation.c4);
			ImGui::TreePop();
		}
		
		if (ImGui::TreeNode("Rotation")) {
			ImGui::TextColored(RED, "X: "); ImGui::SameLine();
			ImGui::TextColored(TURQ, "%0.3f", (float)App->modelLoader->model.scene->mRootNode->mTransformation.a4);
			ImGui::TextColored(GREEN, "Y: "); ImGui::SameLine();
			ImGui::TextColored(TURQ, "%0.3f", (float)App->modelLoader->model.scene->mRootNode->mTransformation.b4);
			ImGui::TextColored(BLUE, "Z: "); ImGui::SameLine();
			ImGui::TextColored(TURQ, "%0.3f", (float)App->modelLoader->model.scene->mRootNode->mTransformation.c4);
			ImGui::TreePop();

		}
		if (ImGui::TreeNode("Scale")) {
			ImGui::TextColored(RED, "X: "); ImGui::SameLine();
			ImGui::TextColored(TURQ, "%0.3f", (float)App->modelLoader->model.scene->mRootNode->mTransformation.a1);
			ImGui::TextColored(GREEN, "Y: "); ImGui::SameLine();
			ImGui::TextColored(TURQ, "%0.3f", (float)App->modelLoader->model.scene->mRootNode->mTransformation.b2);
			ImGui::TextColored(BLUE, "Z: "); ImGui::SameLine();
			ImGui::TextColored(TURQ, "%0.3f", (float)App->modelLoader->model.scene->mRootNode->mTransformation.c3);
			ImGui::TreePop();

		}
		
	}
	if (ImGui::CollapsingHeader("Geometry")) {
		ImGui::TextColored(TURQ, "Meshes:"); ImGui::SameLine();
		ImGui::TextColored(YELLOW, "%d", App->modelLoader->model.getMeshes());

		ImGui::TextColored(TURQ, "Polygons:"); ImGui::SameLine();
		ImGui::TextColored(YELLOW, "%d", App->modelLoader->model.getPolys());

		ImGui::TextColored(TURQ, "Vertices:"); ImGui::SameLine();
		ImGui::TextColored(YELLOW, "%d", App->modelLoader->model.getVertices());
	}
	if (ImGui::CollapsingHeader("Texture")) {
		//for(int i = 0; i< App->textures->getTextures().size(); ++i)
		ImGui::Image((void*)(intptr_t)App->textures->getTextures()[0].imageName, ImVec2(128, 128));
	}

	ImGui::End();
}

void ModuleUI::ShowConfig() {

	io = ImGui::GetIO();
	
	ImGui::Begin("Mateus Console");
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


	if (ImGui::CollapsingHeader("Camera")) {

		ImGui::TextColored(ImVec4(204, 204, 0, 1), "Camera Mode");
		if (ImGui::Button("Free View")) {
			App->camera->dirty = false;
			App->camera->once = false;
			App->camera->mode = true;

		}
		ImGui::SameLine();
		if (ImGui::Button("Auto Orbit")) {
			App->camera->ResetCamera(false);
			App->camera->dirty = true;
			App->camera->mode = false;

		}

		if (ImGui::TreeNode("Settings")) {


			if (ImGui::SliderFloat("Far Plane", &App->camera->farP, 50, 150)) {
				glUniformMatrix4fv(App->program->projLocation, 1, GL_TRUE, &App->camera->proj[0][0]);
				App->camera->ProcessMatrixs();

			}

			if (ImGui::SliderFloat("Near Plane", &App->camera->nearP, 0.1f, 40)) {
				glUniformMatrix4fv(App->program->projLocation, 1, GL_TRUE, &App->camera->proj[0][0]);
				App->camera->ProcessMatrixs();

			}
			Separate();

			if (ImGui::SliderFloat("FOV", &App->camera->vFov, 0.1f, math::pi)) {
				glUniformMatrix4fv(App->program->projLocation, 1, GL_TRUE, &App->camera->proj[0][0]);
				App->camera->ProcessMatrixs();

			}

			if (ImGui::SliderFloat("Aspect", &App->camera->aspectRatio, 1, 90)) {
				glUniformMatrix4fv(App->program->projLocation, 1, GL_TRUE, &App->camera->proj[0][0]);
				App->camera->ProcessMatrixs();

			}

			ImGui::TreePop();

		}	
	}

	if (ImGui::CollapsingHeader("Renderer")) {

		
		if(ImGui::TreeNode("Background Color")) {
			ImGui::ColorPicker4("##picker", (float*)&App->renderer->bgColor.x, misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
			ImGui::TreePop();
		}
	
		ImGui::Checkbox("Wireframes", &showLines);
		ImGui::SameLine();
		ImGui::Checkbox("Cull Face", &App->renderer->enable_cull_face);

		ImGui::Checkbox("Depth test", &App->renderer->enable_depth_test);
		ImGui::SameLine(); 
		/*ImGui::Checkbox("Render Front Faces", &App->renderer->front_face); 
		ImGui::SameLine();
		HelpMarker("This checkbox will be marked by default if you deactivate all of them!");

		ImGui::Checkbox("Render Back Faces", &App->renderer->back_face);
		ImGui::SameLine();
		ImGui::Checkbox("Render Front and Back Faces", &App->renderer->front_and_back);*/

		ImGui::Checkbox("Render Counter Clock Wise Mode", &App->renderer->counter_clock_orientation);


	}
	if (ImGui::CollapsingHeader("Window")) {

		ImGui::InputText("Mateus Engine", title, 25);
		SDL_SetWindowTitle(App->window->window, title);

		ImGui::Checkbox("Fullscreen", &App->window->fullscreen);
		ImGui::SameLine();
		ImGui::Checkbox("Bordered", &App->window->bordered);

		Separate();

		ImGui::SliderFloat("Brightness", &brightness, 0, 1);

		Separate();

		if (ImGui::SliderInt("Window width", &screenW, 300, 1200)) {
			SDL_SetWindowSize(App->window->window, screenW, screenH);
			glViewport(0, 0, screenW, screenH);
		}

		if (ImGui::SliderInt("Window height", &screenH, 300, 1200)) {
			SDL_SetWindowSize(App->window->window, screenW, screenH);
			glViewport(0, 0, screenW, screenH);
		}
	}

	if (ImGui::CollapsingHeader("Input")) {

		ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
		ImGui::Text("Mouse down:");     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (io.MouseDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
		ImGui::Text("Mouse clicked:");  for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
		ImGui::Text("Mouse dbl-clicked:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDoubleClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
		ImGui::Text("Mouse released:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseReleased(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }

		ImGui::Text("Keys down:");      for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (io.KeysDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("%d (0x%X) (%.02f secs)", i, i, io.KeysDownDuration[i]); }
		ImGui::Text("Keys pressed:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyPressed(i)) { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
		ImGui::Text("Keys release:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyReleased(i)) { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
	}

	if (ImGui::CollapsingHeader("Textures")) {

		ImGui::Checkbox("Checker", &App->modelLoader->activate_checker);

		if (ImGui::TreeNode("Texture Parameters"))
		{
			ImGui::Selectable("Toggle Repeat", &App->textures->repeat);
			ImGui::Selectable("Toggle Mirrored Repeat", &App->textures->mirrored);
			ImGui::Selectable("Toggle Clamp to the Edge", &App->textures->edge);
			ImGui::Selectable("Toggle Clamp to Border", &App->textures->border);
			ImGui::Selectable("Toggle Nearest Clamp", &App->textures->nearest);
			ImGui::Selectable("Toggle Mipmap", &App->textures->mipmap);

				
			ImGui::TreePop();
		}

	}

	if (ImGui::CollapsingHeader("Hardware Info")) {
		ImGui::Text("OS:");
		ImGui::SameLine(); ImGui::TextColored(YELLOW, "%s", SDL_GetCurrentVideoDriver());

		ImGui::Text("Number of Logical CPU cores:");
		ImGui::SameLine(); ImGui::TextColored(YELLOW, "%d", SDL_GetCPUCount());


		ImGui::Text("System RAM (Mb):");
		ImGui::SameLine(); ImGui::TextColored(YELLOW, "%d", SDL_GetSystemRAM());

		ImGui::Text("GPU Name:");
		ImGui::SameLine(); ImGui::TextColored(YELLOW, "%s", glGetString(GL_RENDERER));

		ImGui::Text("GPU Version:");
		ImGui::SameLine(); ImGui::TextColored(YELLOW, "%s", glGetString(GL_VERSION));

		ImGui::Text("Manufacturer:");
		ImGui::SameLine(); ImGui::TextColored(YELLOW, "%s", glGetString(GL_VENDOR));

		static GLint totalMemoryKb = 0;
		static GLint currentMemoryKb = 0;
		glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMemoryKb);
		glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &currentMemoryKb);
		ImGui::Text("VRAM Budget:");
		ImGui::SameLine();
		ImGui::TextColored(YELLOW, "%.1f Mb", (float)totalMemoryKb / 1000);
		ImGui::Text("VRAM Usage:");
		ImGui::SameLine();
		ImGui::TextColored(YELLOW, "%.1f Mb", ((float)totalMemoryKb / 1000) - ((float)currentMemoryKb / 1000));
		ImGui::Text("VRAM Available:");
		ImGui::SameLine();
		ImGui::TextColored(YELLOW, "%.1f Mb", (float)currentMemoryKb / 1000);
		ImGui::Text("VRAM Reserved:");
		ImGui::SameLine();
		ImGui::TextColored(YELLOW, "%.1f Mb", 0.0F);

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

	if (SDL_GetTicks() >= lastTicks + 1000) {
		fps_log.push_back(ImGui::GetIO().Framerate);
		ms_log.push_back(1000.0f / ImGui::GetIO().Framerate);
		lastTicks = (float)SDL_GetTicks();
	}

	FPSHistogram();
	SDL_SetWindowBrightness(App->window->window, brightness);
	ImGui::End();


	my_log.Draw("Log");


}

bool ModuleUI::Init() {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return false;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init("#version 330");

	//Flags for the bgColor
	misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

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

	ShowConfig();
	ShowProperties();
	//ImGui::ShowDemoWindow();
	return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate() {

	ImGui::Render();

	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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