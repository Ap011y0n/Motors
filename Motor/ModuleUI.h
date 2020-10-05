#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include<iostream> 
#include<string>

using namespace std;


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
	void StoreLog(const char* message);
	void PlotGraph();

public:
	ImGuiIO* io;
	bool show_demo_window;
	bool show_console;
	ImVec4 clear_color;
	bool show_another_window;
	bool active2 = false;
	bool show_About;
	bool resizable_bool = false;
	bool border_bool;
	vector <float> fpsecond;
	bool show_Configuration;
	int max_fps = 144;
	int i = 0;
	int e = 0;
private:
	int c1;
	vector<string> consoleOutput;

};
