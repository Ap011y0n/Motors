#pragma once
#include "Module.h"
#include "Globals.h"

struct mesh
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint id_normals = 0; // normals in VRAM
	uint num_normals = 0;
	float* normals = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

	uint id_tex;
	uint num_tex;
	float* texCoords;

};



class FBXloader : public Module
{
public:
	FBXloader(Application* app, bool start_enabled = true);
	~FBXloader();

	bool Start();
	update_status PostUpdate(float dt);
	bool CleanUp();
	uint FillArrayBuffer(uint size, float* array);
	uint FillElementArrayBuffer(uint size, uint* array);
	bool LoadFBX(const char* buffer, uint size);
	void PrintMeshes();

private:

public:
	//mesh ourMesh;
	vector<mesh*> meshes;
};

