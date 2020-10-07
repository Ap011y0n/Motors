#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PrimitiveManager.h"
#include "ModuleWindow.h"
#include "ModuleUI.h"
           

#ifdef _DEBUG
#pragma comment( lib, "MathGeoLib/libx86/Debug/MathGeoLib.lib" )
#else
#pragma comment( lib, "MathGeoLib/libx86/Release/MathGeoLib.lib" )
#endif

#include "MathGeoLib/include/MathGeoLib.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	vec3 size(2.f, 2.f, 1.f);
	vec3 pos(0.f, 0.f, 0.f);
	App->PrimManager->CreateCube(size, pos);
	pos.Set(3.f, 0.f, 0.f);
	App->PrimManager->CreateSphere(0.5f, pos);
	pos.Set(6.f, 0.f, 0.f);
	App->PrimManager->CreateCylinder(0.5f, 0.5f, pos);
	vec4 coords(0, 1, 0, 0);
	App->PrimManager->CreatePlane(coords);
	App->PrimManager->CreateLine(pos, size);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{


	
	

	return UPDATE_CONTINUE;
}

