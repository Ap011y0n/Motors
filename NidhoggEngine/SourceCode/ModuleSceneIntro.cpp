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

	char* buffer = nullptr;

	std::string file_path = "Assets/Street environment_V01.FBX";

	//uint UID = App->ResManager->FindInAssets(file_path.c_str());
	//if (UID == 0)
	//{
	//	App->ResManager->ImportFileStep1(file_path.c_str());
	//}

	FolderIco = App->FBX->LoadTexBuffer("Assets/icons/folder-icon.png");

	App->file_system->importAssetsFiles();

	wanttoload = true;

	vec4 coords(0, 1, 0, 0);
	App->PrimManager->CreatePlane(coords);


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

	
	(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
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
	if (wanttoload && App->UI->importsvec.empty())
	{
		wanttoload = false;
		DeleteSceneObjects(scene);
		App->serializer->LoadScene("Assets/Scene.json");

	}
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		//App->serializer->LoadScene("Assets/library/TEST.json");
		
		std::string file_path = "Assets/BakerHouse.fbx";
		//std::string file_path = "Assets/Street environment_V01.FBX";

		uint UID = App->ResManager->FindInAssets(file_path.c_str());
		if (UID == 0)
		{
			App->ResManager->ImportFileStep1(file_path.c_str());
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
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		App->file_system->RefreshAssets();
	}


	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		App->serializer->CreateNewScene();
		SaveScene(scene);
		App->serializer->SaveScene("Scene.json");
	}
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		DeleteSceneObjects(scene);
		App->serializer->LoadScene("Assets/Scene.json");
	}

	UpdateGameObject(scene, dt);
	SetDelete(scene);
	DeleteGameObject(scene);
	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::WantToImport(ImportOptions* options)
{


	uint UID = App->ResManager->ImportFileStep2(options->path.c_str(), options);
	App->UI->importsvec.erase(App->UI->importsvec.begin());


	/*if (UID != 0)
	{
		Resource* NewResource = App->ResManager->RequestResource(UID);
		if (NewResource != nullptr)
		{
		switch (NewResource->GetType())
		{
		case ResourceType::MODEL:
			
				LOG("Resource Found");
				App->serializer->LoadModel(NewResource);
			break;
		}
		}
	}*/
}

void ModuleSceneIntro::SaveScene(GameObject * parent)
{
	if (parent != scene)
	{
		JSON_Object* JsonObj = App->serializer->AddObjectToArray(App->serializer->leaves);
		if (parent->UID == parent->parent->UID)
		{
			LCG();
			LCG rand;
			parent->UID = rand.Int();
		}
		App->serializer->AddFloat(JsonObj, "UID", parent->UID);
		App->serializer->AddFloat(JsonObj, "ParentUID", parent->parent->UID);
		App->serializer->AddString(JsonObj, "Name", parent->Name.c_str());
		JSON_Array* JsonTrans = App->serializer->AddArray(JsonObj, "Translation");
		JSON_Array* JsonScale = App->serializer->AddArray(JsonObj, "Scale");
		JSON_Array* JsonRot = App->serializer->AddArray(JsonObj, "Rotation");
		JSON_Array* JsonComp = App->serializer->AddArray(JsonObj, "Components");
		ComponentMesh* newMesh = nullptr;
		ComponentMaterial* NewTex = nullptr;
		ComponentTransform* NewTrans = nullptr;

		for (int i = 0; i < parent->Components.size(); i++)
		{
			switch (parent->Components[i]->type)
			{
			case ComponentType::MESH:
				newMesh = (ComponentMesh * )parent->GetComponent(ComponentType::MESH);
				App->serializer->AddComponent(JsonComp, ComponentType::MESH, newMesh->reference->GetLibraryFile(), newMesh->reference->GetUID());

				break;
			case ComponentType::MATERIAL:
				NewTex = (ComponentMaterial*)parent->GetComponent(ComponentType::MATERIAL);
				App->serializer->AddComponent(JsonComp, ComponentType::MATERIAL, NewTex->reference->GetLibraryFile(), NewTex->reference->GetUID());

				break;
			case ComponentType::TRANSFORM:
				NewTrans = (ComponentTransform*)parent->GetComponent(ComponentType::TRANSFORM);
				App->serializer->AddVec3(JsonTrans, NewTrans->pos.x, NewTrans->pos.y, NewTrans->pos.z);
				App->serializer->AddVec3(JsonScale, NewTrans->scale.x, NewTrans->scale.y, NewTrans->scale.z);
				App->serializer->AddVec4(JsonRot, NewTrans->rot.x, NewTrans->rot.y, NewTrans->rot.z, NewTrans->rot.w);
				break;

			}
		}
	}

	for (int i = 0; i < parent->childs.size(); i++)
	{
		SaveScene(parent->childs[i]);
	}
}



GameObject* ModuleSceneIntro::CreateGameObject(const char* name, GameObject* parent)
{
	GameObject* newGameObject = new GameObject(name, parent);
	//gameObjects.push_back(newGameObject);
	
	return newGameObject;
}

void ModuleSceneIntro::UpdateGameObject(GameObject* parent, float dt)
{
	parent->Update(dt);
	for (int i = 0; i < parent->childs.size(); i++)
	{
		UpdateGameObject(parent->childs[i], dt);
	}

}

void ModuleSceneIntro::DeleteSceneObjects(GameObject* parent)
{
	if (parent->parent != nullptr && parent != scene)
	{
		parent->to_delete = true;
	}
	for (int i = 0; i < parent->childs.size(); i++)
	{
		DeleteSceneObjects(parent->childs[i]);
	}


}

void ModuleSceneIntro::SetDelete(GameObject* parent)
{
	if (parent->parent != nullptr && parent != scene)
	{
		if (parent->parent->to_delete)
		{
			parent->to_delete = true;
		}
	}
	for (int i = 0; i < parent->childs.size(); i++)
	{
		SetDelete(parent->childs[i]);
	}
	

}

bool ModuleSceneIntro::DeleteGameObject(GameObject* parent)
{
	
	for (int i = 0; i < parent->childs.size(); i++)
	{
		if (DeleteGameObject(parent->childs[i]))
			i--;
	}
	if (parent != nullptr && parent->to_delete)
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


void ModuleSceneIntro::Camera_Editor_Window(ComponentCamera* camera)
{
	if (App->UI->Config_Camera_open) {
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		ImGui::Begin("Camera Config", &App->UI->Config_Camera_open);

		if (ImGui::TreeNodeEx("Camera Editor", node_flags))
		{
			ImGui::Separator();
			ImGui::Text("Camera configuration:");

			static bool Culling = false;
			ImGui::Checkbox("Culling", &Culling);
			if (Culling)
			{
				camera->cullingActive = true;
			}
			else {
				camera->cullingActive = false;
			}

			float p = camera->frustrum.farPlaneDistance;
			ImGui::SetNextItemWidth(200);
			ImGui::DragFloat("Far Plane", &p, 1, 0.01f, 999.0f);
			if (ImGui::IsItemActive())
			{
				camera->frustrum.farPlaneDistance = p;
			}

			float p1 = camera->frustrum.nearPlaneDistance;
			ImGui::SetNextItemWidth(200);
			ImGui::DragFloat("Near Plane", &p1, 0.5f, 0.01f, 999.0f);
			if (ImGui::IsItemActive())
			{
				camera->frustrum.nearPlaneDistance = p1;
			}

			float p2 = camera->GetFOV();
			ImGui::SetNextItemWidth(200);
			ImGui::DragFloat("Field of view", &p2, 0.5f, 50.0f, 120.0f);
			if (ImGui::IsItemActive())
			{
				camera->SetFOV(p2);
				//camera->frustrum.verticalFov = p2;
			}
			ImGui::Text("Horizontal FOV: %f", camera->GetHorizontalFov());
			ImGui::Text("Vertical FOV: %f", camera->GetFOV());
			ImGui::Text("Aspect ratio: %f", camera->aspectRatio);
			ImGui::TreePop();

		}

		ImGui::End();
	}
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