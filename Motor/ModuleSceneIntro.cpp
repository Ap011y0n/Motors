#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PrimitiveManager.h"
#include "ModuleWindow.h"
#include "ModuleUI.h"
           

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


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

	vec3 size(1.f, 1.f, 1.f);
	vec3 pos(0.f, 1.f, 0.f);
	Cube* cube = App->PrimManager->CreateCube(size);
	cube->SetPos(pos.x, pos.y, pos.z);
	cube->SetRotation(45, pos);
	cube->Scale(1.f, 2.f, 1.f);
	cube->axis = true;
	cube->wire = true;
	//pos.Set(3.f, 0.f, 0.f);
	//App->PrimManager->CreateSphere(0.5f, pos);
	//pos.Set(6.f, 0.f, 0.f);
	//App->PrimManager->CreateCylinder(0.5f, 0.5f, pos);
	vec4 coords(0, 1, 0, 0);
	App->PrimManager->CreatePlane(coords);
	//App->PrimManager->CreateLine(pos, size);
	/*firstCube();
	secondCube();*/
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

	//glLineWidth(2.0f);
	//glBegin(GL_TRIANGLES);
	//glVertex3f(1.f, 1.f, 0.f);
	//glVertex3f(0.f, 1.f, 0.f);
	//glVertex3f(0.f, 0.f, 0.f);

	//glVertex3f(0.f, 0.f, 0.f);
	//glVertex3f(1.f, 0.f, 0.f);
	//glVertex3f(1.f, 1.f, 0.f);

	//glVertex3f(1.f, 1.f, 0.f);
	//glVertex3f(1.f, 0.f, 0.f);
	//glVertex3f(1.f, 0.f, 1.f);

	//glVertex3f(1.f, 0.f, 1.f);
	//glVertex3f(1.f, 1.f, 1.f);
	//glVertex3f(1.f, 1.f, 0.f);

	//glVertex3f(1.f, 1.f, 0.f);
	//glVertex3f(1.f, 1.f, 1.f);
	//glVertex3f(0.f, 1.f, 1.f);

	//glVertex3f(0.f, 1.f, 1.f);
	//glVertex3f(0.f, 1.f, 0.f);
	//glVertex3f(1.f, 1.f, 0.f);

	//glVertex3f(1.f, 1.f, 0.f);
	//glVertex3f(1.f, 0.f, 0.f);
	//glVertex3f(1.f, 0.f, 1.f);

	//glVertex3f(1.f, 0.f, 1.f);
	//glVertex3f(1.f, 1.f, 1.f);
	//glVertex3f(1.f, 1.f, 0.f);

	//glEnd();
	//glLineWidth(1.0f);
	/*
	my_id = 0;
	num_vertices = 24;
	float vertices[72] =
	{
	1, 1, 0,
	0, 1, 0,
	0, 0, 0,
	0, 0, 0,
	1, 0, 0,
	1, 1, 0,
	1, 1, 0,
	1, 0, 0,
	1, 0, 1,
	1, 0, 1,
	1, 1, 1,
	1, 1, 0,
	1, 1, 0,
	1, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 0,
	1, 1, 0,
	1, 1, 0,
	1, 0, 0,
	1, 0, 1,
	1, 0, 1,
	1, 1, 1,
	1, 1, 0,
	};
	glGenBuffers(1, (GLuint*)&(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// … bind and use other buffers
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	glDisableClientState(GL_VERTEX_ARRAY);
	*/

	

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glEnableClientState(GL_VERTEX_ARRAY);

	//glBindBuffer(GL_ARRAY_BUFFER, my_vertex);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
	//
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);

	//glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);


	//glBindBuffer(GL_ARRAY_BUFFER, my_vertex2);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
	//
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices2);

	//glDrawElements(GL_TRIANGLES, num_indices2, GL_UNSIGNED_INT, NULL);

	////*---------
	//glDisableClientState(GL_VERTEX_ARRAY);
	


	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::firstCube()
{

	uint indices[36] = {
		// front
			0, 1, 2,
			2, 3, 1,
			// right
			1, 3, 5,
			3, 5, 7,
			//// back
			7, 4, 5,
			6, 7, 4,
			//// left
			6, 0, 4,
			0, 6, 2,
			//// bottom
			4, 5, 0,
			5, 1, 0,
			//// top
			6, 2, 3,
			3, 7, 6
	};
	for (int i = 0; i < 36; i++)
	{
		index[i] = indices[i];

	}
	num_indices = 36;

	float vertices[24] =
	{
		// front
			 0.0, 0.0,  0.0,
			 1.0, 0.0,  0.0,
			 0.0, 1.0,  0.0,
			 1.0, 1.0,  0.0,

			 // back
			  0.0, 0.0, -1.0,
			  1.0, 0.0, -1.0,
			  0.0, 1.0, -1.0,
			  1.0, 1.0, -1.0,
	};
	num_vertices = 8;

	for (int i = 0; i < 24; i++)
	{
		vert[i] = vertices[i];

	}
	my_indices = 0;
	my_vertex = 0;
	glGenBuffers(1, (GLuint*)&(my_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, my_vertex);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vert, GL_STATIC_DRAW);
	// … bind and use other buffers

	glGenBuffers(1, (GLuint*)&(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_indices, index, GL_STATIC_DRAW);
	
}

void ModuleSceneIntro::secondCube()
{

	uint indices[36] = {
		// front
			0, 1, 2,
			2, 3, 1,
			// right
			1, 3, 5,
			3, 5, 7,
			//// back
			7, 4, 5,
			6, 7, 4,
			//// left
			6, 0, 4,
			0, 6, 2,
			//// bottom
			4, 5, 0,
			5, 1, 0,
			//// top
			6, 2, 3,
			3, 7, 6
	};
	for (int i = 0; i < 36; i++)
	{
		index2[i] = indices[i];

	}
	num_indices2 = 36;

	float vertices[24] =
	{
		// front
			 0.0, 0.0,  3.0,
			 1.0, 0.0,  3.0,
			 0.0, 1.0,  3.0,
			 1.0, 1.0,  3.0,

			 // back
			  0.0, 0.0, 2.0,
			  1.0, 0.0, 2.0,
			  0.0, 1.0, 2.0,
			  1.0, 1.0, 2.0,
	};
	num_vertices2 = 8;

	for (int i = 0; i < 24; i++)
	{
		vert2[i] = vertices[i];

	}
	my_indices2 = 0;
	my_vertex2 = 0;
	glGenBuffers(1, (GLuint*)&(my_vertex2));
	glBindBuffer(GL_ARRAY_BUFFER, my_vertex2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices2 * 3, vert2, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&(my_indices2));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_indices2, index2, GL_STATIC_DRAW);
	
}