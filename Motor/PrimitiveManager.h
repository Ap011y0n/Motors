#pragma once
#include "Module.h"
#include "Globals.h"

class Primitive;

class PrimitiveManager : public Module
{
public:
	PrimitiveManager(Application* app, bool start_enabled = true);
	~PrimitiveManager();

	bool Start();
	update_status PostUpdate(float dt);
	bool CleanUp();
	void CreateCube(vec3 size, vec3 pos = (0.f,0.f,0.f));
	void CreateSphere(float radius, vec3 pos = (0.f, 0.f, 0.f));
	void CreateCylinder(float radius, float height, vec3 pos = (0.f, 0.f, 0.f));
	void CreateLine(vec3 origin, vec3 dest);
	void CreatePlane(vec4 coords, bool axis = true);
	void MenuOptions();
private:
	vector<Primitive*> prim_list;
public:
	bool depth_bool;
	bool cullface_bool;
	bool texture2D_bool;
	bool LIGHTING_bool;
	bool Color_bool;
};
