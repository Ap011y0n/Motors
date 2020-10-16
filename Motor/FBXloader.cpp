#include "Globals.h"
#include "Application.h"
#include "FBXloader.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")



FBXloader::FBXloader(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

FBXloader::~FBXloader()
{}

// Load assets
bool FBXloader::Start()
{
	LOG("start FBX loader");
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	bool ret = true;

	return ret;
}

// Load assets
bool FBXloader::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

// Update: draw background
update_status FBXloader::PostUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}
