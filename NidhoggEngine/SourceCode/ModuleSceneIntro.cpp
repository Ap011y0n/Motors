#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PrimitiveManager.h"
#include "ModuleWindow.h"
#include "ModuleUI.h"
#include "GameObject.h"   
#include "serializer.h"
#include "ResourceManager.h"
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
	scene = CreateGameObject("scene");
	selectedObj = nullptr;

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

	culling = nullptr;
	//GameObject* object = CreateGameObject("test");
	//object->CreateComponent(ComponentType::MESH);

	
	char* buffer = nullptr;


	//uint fileSize = 0;
	//fileSize = App->file_system->Load(file_path.c_str(), &buffer);
	//App->FBX->LoadFBX(buffer, fileSize);

	std::string file_path = "Assets/p1character.FBX";
	//App->ResManager->ImportFile(file_path.c_str());

	//fileSize = App->file_system->Load(file_path.c_str(), &buffer);
	//App->FBX->LoadFBX(buffer, fileSize);

	file_path = "Assets/P1_PoldelaTorre.FBX";
	//App->ResManager->ImportFile(file_path.c_str());

	//fileSize = App->file_system->Load(file_path.c_str(), &buffer);
	//App->FBX->LoadFBX(buffer, fileSize);

	vec4 coords(0, 1, 0, 0);
	App->PrimManager->CreatePlane(coords);
//	vec3 size(1, 1, 1);
//	vec3 pos(0, 0, -2);
//	App->PrimManager->CreateCube(size, pos);

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
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		//App->serializer->LoadScene("Assets/library/TEST.json");
		
		//std::string file_path = "Assets/BakerHouse.fbx";
		std::string file_path = "Assets/Street environment_V01.FBX";

		uint UID = App->ResManager->FindInAssets(file_path.c_str());
		if (UID == 0)
		{
			UID = App->ResManager->ImportFile(file_path.c_str());
		}
		if (UID != 0)
		{
			Resource* NewResource = App->ResManager->RequestResource(UID);
			if (NewResource != nullptr)
			{
				LOG("Resource Found");
				App->serializer->LoadModel(NewResource);
			}
		}
	}

	UpdateGameObject(scene, dt);
	SetDelete(scene);
	DeleteGameObject(scene);
	
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
	//gameObjects.push_back(newGameObject);
	
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
	if (father->parent != nullptr)
	{
		if (father->parent->to_delete)
		{
			father->to_delete = true;
		}
	}
	for (int i = 0; i < father->childs.size(); i++)
	{
		SetDelete(father->childs[i]);
	}
	

}

bool ModuleSceneIntro::DeleteGameObject(GameObject* parent)
{
	
	for (int i = 0; i < parent->childs.size(); i++)
	{
		if (DeleteGameObject(parent->childs[i]))
			i--;
	}
	if (parent->to_delete)
	{
		if (parent->parent != nullptr)
		{
			for (int i = 0; i < parent->parent->childs.size(); i++)
			{
				if (parent->parent->childs[i] == parent)
				{
					parent->parent->childs.erase(parent->parent->childs.begin() + i);
					i--;
				}
			}
		}
		if (parent != NULL)
		{
			if (selectedObj = parent)
			{
				selectedObj = nullptr;
			}

			delete parent;
			parent = NULL;
		}
		return true;

	}
	return false;

}