#pragma once
#include "Module.h"
#include "Globals.h"

#define checkImageWidth 64
#define checkImageHeight 64

typedef unsigned char GLubyte;
typedef unsigned int GLuint;


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void firstCube();
	void secondCube();

public:

	uint my_indices = 0;
	uint my_vertex = 0;
	uint my_indices2 = 0;
	uint my_vertex2 = 0;
	int num_vertices;
	int num_indices;
	int num_vertices2;
	int num_indices2;
	float vert[24];
	uint index[36];
	float vert2[24];
	uint index2[36];

	GLubyte checkImage[checkImageHeight][checkImageWidth][4];
	GLuint texName;

};
