#pragma once
#include "Module.h"
#include "Globals.h"

struct mesh
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;
	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;
};



class FBXloader : public Module
{
public:
	FBXloader(Application* app, bool start_enabled = true);
	~FBXloader();

	bool Start();
	update_status PostUpdate(float dt);
	bool CleanUp();

private:

public:
	mesh ourMesh;

};

