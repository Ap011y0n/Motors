#pragma once
#include "Module.h"
#include "Globals.h"

class Primitive;
class Cube;
class Pyramid;
class PrimSphere;
class PrimCylinder;
class PrimLine;

class PrimitiveManager : public Module
{
public:
	PrimitiveManager(Application* app, bool start_enabled = true);
	~PrimitiveManager();

	bool Start();
	update_status PostUpdate(float dt);
	bool CleanUp();
	Cube* CreateCube(vec3 size, vec3 pos = (0.f,0.f,0.f));
	Pyramid* CreatePyramid(vec3 size, vec3 pos = (0.f, 0.f, 0.f));
	PrimSphere* CreateSphere(float radius, unsigned int rings, unsigned int sectors, vec3 pos = (0.f, 0.f, 0.f));
	PrimCylinder* CreateCylinder(float radius, float height, int sides = 30, vec3 pos = (0.f, 0.f, 0.f));
	PrimLine* CreateLine(vec3 origin, vec3 dest);
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
