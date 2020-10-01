#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui.h"

class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:
	bool show_demo_window;
	ImVec4 clear_color;
	bool show_another_window;
};
