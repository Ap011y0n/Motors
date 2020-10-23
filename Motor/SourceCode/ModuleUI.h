#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include<iostream> 
#include<string>
#include "Vec2.h"

using namespace std;


class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Init();

	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void AboutMenu(bool window);
	void Configuration(bool config);
	void StoreLog(const char* message);
	void PlotGraph();
	void ShowAppinDockSpace(bool* p_open);
	void HierarchyWin();
	void InspectorWin();
	void ShowExampleAppLayout(/*bool* p_open*/);
	void Change_Window_size(Vec2 newSize);
	void OnResize(int screen_width, int screen_height);
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
	bool Wireframe_bool;
	vector <float> fpsecond;
	bool show_Configuration;
	int max_fps = 61;
	int i = 0;
	int e = 0;
	bool p_open;
	ImVec2 MouseVelocity;
	ImVec2 windowSize = { 0, 0 };
	Vec2 win_size;
	Vec2 img_offset;
	Vec2 img_corner;
	Vec2 img_size;
	ImGuiWindowClass* windowClass = nullptr;
private:
	int c1;
	vector<string> consoleOutput;

};
