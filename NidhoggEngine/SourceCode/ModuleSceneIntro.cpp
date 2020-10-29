#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PrimitiveManager.h"
#include "ModuleWindow.h"
#include "ModuleUI.h"
#include "GameObject.h"   

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


#ifdef _DEBUG
#pragma comment( lib, "SourceCode/MathGeoLib/libx86/Debug/MathGeoLib.lib" )
#else
#pragma comment( lib, "SourceCode/MathGeoLib/libx86/Release/MathGeoLib.lib" )
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
	scene = CreateGameObject("scene");

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	//GameObject* object = CreateGameObject("test");
	//object->CreateComponent(ComponentType::MESH);

	std::string file_path = "Assets/BakerHouse.fbx";
	char* buffer = nullptr;
	uint fileSize = 0;
	fileSize = App->file_system->Load(file_path.c_str(), &buffer);
	App->FBX->LoadFBX(buffer, fileSize);
	//for (float i = -100; i < 100; i++)
	//{
	//	vec3 pos(i, 0.f, 0.f);
	//	vec3 pos2(0.f, 0.f, i+1);
	//	vec3 size(1.f, 1.f, 1.f);
	//	Cube* cube = App->PrimManager->CreateCube(size, pos);
	//	Cube* cube2 = App->PrimManager->CreateCube(size, pos2);
	//}
	//vec3 size(1.f, 1.f, 1.f);
	//Pyramid* pyramid = App->PrimManager->CreatePyramid(size);
	//pyramid->wire = false;
	//cube->SetPos(pos.x, pos.y, pos.z);
	//cube->SetRotation(45, pos);
	//cube->Scale(1.f, 2.f, 1.f);
	//cube->axis = true;
	//cube->wire = true;
	//vec3 pos(1.5f, 0.5f, -0.5f);
	//PrimSphere* sphere = App->PrimManager->CreateSphere(0.5, 12, 24, pos);
	//pos.Set(-1.3f, 0.f, 0.f);
	//Cube* cube = App->PrimManager->CreateCube(size, pos);

	//sphere->wire = true;
	//pos.Set(3.5f, 0.5f, -0.5f);

	//PrimSphere* sphere2 = App->PrimManager->CreateSphere(1, 100, 200, pos);
	//sphere2->wire = true;
	//pos.Set(3.f, 0.5f, -0.5f);
	//PrimCylinder *cylinder = App->PrimManager->CreateCylinder(0.5f, 1.f, 30, pos);

	vec4 coords(0, 1, 0, 0);
	App->PrimManager->CreatePlane(coords);
	//App->PrimManager->CreateLine(pos, size);
	/*firstCube();
	secondCube();*/
	//vec3 origin(1, 1, 0);
	//vec3 dest(0, 2, 2);

	//App->PrimManager->CreateLine(origin, dest);


	//glClearColor(0.0, 0.0, 0.0, 0.0);
	//glShadeModel(GL_FLAT);
	//glEnable(GL_DEPTH_TEST);


	int i, j, c;

	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
		checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		checkImage);

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
	/*if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		vec3 size(1.f, 1.f, 1.f);
		Cube* cube = App->PrimManager->CreateCube(size);

	}*/



	/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texName);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);
	
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
	glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);
	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);*/

	//ComponentTransform* transform = nullptr;
	//for (int i = 0; i < gameObjects.size(); i++)
	//{
	//	for (int j = 0; j < gameObjects[i]->Components.size(); j++)
	//	{
	//		if (gameObjects[i]->Components[j]->type == ComponentType::TRANSFORM)
	//		{
	//			transform = (ComponentTransform*)gameObjects[i]->Components[j];
	//		}
	//	}
	//	gameObjects[i]->Update(dt);
	//}

	UpdateGameObject(scene, dt);
	SetDelete(scene);
	DeleteGameObject(scene);
	/*if (transform != nullptr)
	{
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			transform->SetPos(transform->pos.x, transform->pos.y, transform->pos.z - 1 * dt);
		}
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			transform->SetPos(transform->pos.x, transform->pos.y, transform->pos.z + 1 * dt);
		}
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			transform->SetPos(transform->pos.x - 1 * dt, transform->pos.y, transform->pos.z);
		}
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			transform->SetPos(transform->pos.x + 1 * dt, transform->pos.y, transform->pos.z);
		}
		if (App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT)
		{
			transform->SetPos(transform->pos.x, transform->pos.y - 1 * dt, transform->pos.z);
		}
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		{
			transform->SetPos(transform->pos.x, transform->pos.y + 1 * dt, transform->pos.z);
		}
	}*/
	
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

GameObject* ModuleSceneIntro::CreateGameObject(const char* name, GameObject* father)
{
	GameObject* newGameObject = new GameObject(name, father);
	gameObjects.push_back(newGameObject);
	TreeNode* newnode = new TreeNode(newGameObject);
	App->UI->tree_nodes.push_back(newnode);
	return newGameObject;
}

void ModuleSceneIntro::UpdateGameObject(GameObject* father, float dt)
{
	father->Update(dt);
	for (int i = 0; i < father->childs.size(); i++)
	{
		UpdateGameObject(father->childs[i], dt);
	}

}

void ModuleSceneIntro::SetDelete(GameObject* father)
{
	if (father->father != nullptr)
	{
		if (father->father->to_delete)
		{
			father->to_delete = true;
		}
	}
	for (int i = 0; i < father->childs.size(); i++)
	{
		SetDelete(father->childs[i]);
	}
	

}
void ModuleSceneIntro::DeleteGameObject(GameObject* father)
{
	
	for (int i = 0; i < father->childs.size(); i++)
	{
		DeleteGameObject(father->childs[i]);
	}
	if (father->to_delete)
	{
		if (father->father != nullptr)
		{
			for (int i = 0; i < father->father->childs.size(); i++)
			{
				if (father->father->childs[i] == father)
				{
					father->father->childs.erase(father->father->childs.begin() + i);
					i--;
				}
			}
		}
		if (father != NULL)
		{
			if (App->UI->selectedObj = father)
			{
				App->UI->selectedObj = nullptr;
			}

			delete father;
			father = NULL;
		}

	}

}