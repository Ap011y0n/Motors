#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui.h"

class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Init();

	update_status Update(float dt);
	bool CleanUp();
	void AboutMenu(bool window);
	void Configuration(bool config);

public:
	SDL_GLContext gl_context;

	bool show_demo_window;
	ImVec4 clear_color;
	bool show_another_window;
	bool active2 = false;
	bool show_About;

	bool show_Configuration;
};
