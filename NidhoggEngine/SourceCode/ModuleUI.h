#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include<iostream> 
#include<string>
#include "Vec2.h"
#include "ImGuizmo.h"

using namespace std;
class GameObject;

class TreeNode
{
public:
	TreeNode(GameObject* obj)
	{
		object = obj;
		isSelected = false;
	}
	~TreeNode();
public:
	GameObject* object;
	bool isSelected;
};
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
	void StoreLog(const char* message);
	void ShowAppinDockSpace(bool* p_open = NULL);

	void Configuration(bool config);
	void PlotGraph();
	void HierarchyWin(); //hierarchy window
	void InspectorWin(); //gameobjects list
	void ShowExampleAppLayout(/*bool* p_open*/);
	void Change_Window_size(Vec2 newSize);

	void GameObjectHierarchyTree(GameObject* node, int id);
	void DeactivateGameObjects(GameObject* father);
	void GameObjectInspector(GameObject* obj);
	void ChangeParent(GameObject* obj, GameObject* nextOwner);
	void SelectGameObject(GameObject* node);

	vec3 ReturnLookAtCamera(vec3 direction);
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

	 bool Hierarchy_open;
	 bool Inspector_open;
	 bool Console_open;

	vector <float> fpsecond;
	bool show_Configuration;
	int max_fps = 61;
	int i = 0;
	int e = 0;
	bool* open_docking;
	ImVec2 MouseVelocity;
	ImVec2 windowSize = { 0, 0 };
	Vec2 win_size;
	Vec2 image_offset;
	Vec2 img_corner;
	Vec2 image_size;
	ImGuiWindowClass* windowClass = nullptr;
	vec3 direction_camera;
	GameObject* selectedObj;
	ImVec2 winSize;
	ImVec2 winPos;

	ImGuizmo::OPERATION guizmo_type = ImGuizmo::OPERATION::TRANSLATE;
private:
	int c1;
	int cameras;
	vector<string> consoleOutput;

};

