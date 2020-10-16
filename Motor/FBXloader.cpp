#include "Globals.h"
#include "Application.h"
#include "FBXloader.h"





FBXloader::FBXloader(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	LOG("init FBX loader");
}

FBXloader::~FBXloader()
{}

// Load assets
bool FBXloader::Start()
{
	LOG("start FBX loader");
	bool ret = true;

	return ret;
}

// Load assets
bool FBXloader::CleanUp()
{
	return true;
}

// Update: draw background
update_status FBXloader::PostUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}
