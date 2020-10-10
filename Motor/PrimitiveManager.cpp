#include "Globals.h"
#include "Application.h"
#include "PrimitiveManager.h"
#include "Primitive.h"
#include "ModuleWindow.h"
#include "glew/include/glew.h"



PrimitiveManager::PrimitiveManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

PrimitiveManager::~PrimitiveManager()
{}

// Load assets
bool PrimitiveManager::Start()
{
	bool ret = true;
	depth_bool = false;
	cullface_bool = false;	
	return ret;
}

// Load assets
bool PrimitiveManager::CleanUp()
{
	LOG("Unloading Primitive Manager");

	return true;
}

// Update: draw background
update_status PrimitiveManager::PostUpdate(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		for (int i = 0; i < prim_list.size(); i++) {
			
			prim_list[i]->wire = !prim_list[i]->wire;
		}
	}
	if (depth_bool) 
	{
		glDisable(GL_DEPTH_TEST);
	}
	else 
	{
		glEnable(GL_DEPTH_TEST);
	}

	if (cullface_bool) 
	{
		glEnable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_CULL_FACE);
	}

	if (texture2D_bool) 
	{
		glDisable(GL_TEXTURE_2D);
	}
	else 
	{
		glEnable(GL_TEXTURE_2D);
	}

	if (LIGHTING_bool) {
		//glDisable(GL_LIGHTING);
	}
	else 
	{
		//glEnable(GL_LIGHTING);
	}
	
	if (Color_bool) 
	{
		//glEnable(GL_COLOR_MATERIAL);
	}
	else 
	{
		//glEnable(GL_COLOR_MATERIAL);
	}
	for (int i = 0; i < prim_list.size(); i++) {
		
		prim_list[i]->Render();
		
	}
	

	return UPDATE_CONTINUE;
}

Cube* PrimitiveManager::CreateCube(vec3 size, vec3 pos)
{
	Cube* cube = new Cube(size.x, size.y, size.z);
	cube->SetPos(pos.x, pos.y, pos.z);
	prim_list.push_back((Primitive*)cube);
	return cube;
}

void PrimitiveManager::CreateSphere(float radius, vec3 pos)
{
	Sphere* sphere = new Sphere(radius);
	sphere->SetPos(pos.x, pos.y, pos.z);
	prim_list.push_back((Primitive*)sphere);
}

void PrimitiveManager::CreateCylinder(float radius, float height, vec3 pos)
{
	Cylinder* cylinder = new Cylinder(radius, height);
	cylinder->SetPos(pos.x, pos.y, pos.z);
	prim_list.push_back((Primitive*)cylinder);
}

void PrimitiveManager::CreateLine(vec3 origin, vec3 dest)
{
	Line* line = new Line(dest.x, dest.y, dest.z);
	line->origin.Set(origin.x, origin.y, origin.z);
	prim_list.push_back((Primitive*)line);
}
void PrimitiveManager::CreatePlane(vec4 coords, bool axis)
{
	PrimPlane* plane = new PrimPlane(coords.x, coords.y, coords.z, coords.w);
	plane->axis = true;
	prim_list.push_back((Primitive*)plane);
}