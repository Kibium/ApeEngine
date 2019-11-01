#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL.h"
#include <vector>

#include "IMGUI/imgui.h"

class ExampleAppLog
{
	ImGuiTextBuffer     Buf;
	bool                ScrollToBottom = true;
	ImVector<char*>       Items;
	bool                  AutoScroll = true;
	static char* Strdup(const char *str) { size_t len = strlen(str) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)str, len); }
public:
	//void    Clear() { Buf.clear(); }

	void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
	{
		char buf[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
		buf[IM_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		Items.push_back(Strdup(buf));
	}

	void    Draw(const char* title)
	{
		//ImGui::SetNextWindowSize(ImVec2(500, 400));
		//ImGui::Begin(title);
		//if (ImGui::Button("Clear")) Clear();
		//ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::Separator();
		ImGui::BeginChild("scrolling");
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
		if (copy) ImGui::LogToClipboard();
		ImGui::TextUnformatted(Buf.begin());
		if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
			ImGui::SetScrollHereY(1.0f);
		ScrollToBottom = false;
		ScrollToBottom = false;
		ImGui::PopStyleVar();
		ImGui::EndChild();
		//ImGui::End();
	}
};

class ModuleUI : public Module {

public:
	ModuleUI();
	~ModuleUI();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	void MyConsole();
	void Separate();
	void FPSHistogram();

	bool CleanUp();

	ExampleAppLog my_log;
	bool showLog = false;
	ImGuiIO io;
	char* buf;
	float brightness = 1;
	int screenW = 500, screenH = 500;
	std::vector <float> fps_log;
	std::vector <float>	ms_log;
	bool addBar = false;
	float lastTicks = 0;
	
private:
		char title [25]= "Mateus";	
		bool test = true;

};


