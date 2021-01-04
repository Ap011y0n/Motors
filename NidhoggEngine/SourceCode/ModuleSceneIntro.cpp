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
	LOG("Loading Assets");
	bool ret = true;

	JointObj1 = nullptr;
	JointObj2 = nullptr;
	distance.Set(0, 0, 0);
	axis1.Set(1, 0, 0);
	axis2.Set(1, 0, 0);
	//GameObject* obj1 = new GameObject();
	//selectedObj = obj1;
	//obj1->CreateComponent(ComponentType::TRANSFORM);
	//obj1->CreateComponent(ComponentType::MESH);
	//App->FBX->ChangeMesh("library/Baker_house.mesh");
	//Collider* collider1 = (Collider*)obj1->CreateComponent(ComponentType::COLLIDER);
	//obj1->parent = scene;
	//scene->childs.push_back(obj1);

	//GameObject* obj2 = new GameObject();
	//selectedObj = obj2;
	//obj2->CreateComponent(ComponentType::TRANSFORM);
	//obj2->CreateComponent(ComponentType::MESH);
	//App->FBX->ChangeMesh("library/Baker_house.mesh");
	//Collider* collider2 = (Collider*)obj2->CreateComponent(ComponentType::COLLIDER);
	//obj2->parent = scene;
	//scene->childs.push_back(obj2);

	//App->Physics->AddConstraintP2P(obj1, obj2,
	//	btVector3{ 4.f,-0.,-0 }, btVector3{ 0, 0,0 });

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	culling = nullptr;

	CreatingJoint = false;

	char* buffer = nullptr;
	std::string file_path = "Assets/Street environment_V01.FBX";


	FolderIco = App->FBX->LoadTexBuffer("Assets/icons/folder-icon.png");
	FolderGoBack = App->FBX->LoadTexBuffer("Assets/icons/folder-icon-go_back.png");
	FbxIcon = App->FBX->LoadTexBuffer("Assets/icons/fbx-icon.png");
	PngIcon = App->FBX->LoadTexBuffer("Assets/icons/png-icon.png");
	TgaIcon = App->FBX->LoadTexBuffer("Assets/icons/tga-icon.png");
	MetaIcon = App->FBX->LoadTexBuffer("Assets/icons/meta-icon.png");
	MeshIcon = App->FBX->LoadTexBuffer("Assets/icons/mesh-icon.png");
	DdsIcon = App->FBX->LoadTexBuffer("Assets/icons/dds-icon.png");
	JsonIcon = App->FBX->LoadTexBuffer("Assets/icons/scene2-icon.png");
	ModelIcon = App->FBX->LoadTexBuffer("Assets/icons/model-icon.png");

	App->file_system->importAssetsFiles();

	wanttoload = true;

	vec4 coords(0, 1, 0, 0);
	App->PrimManager->CreatePlane(coords);
	
	vec3 size(2, 2, 2);
	vec3 pos(0, 3, 0);
	cube1 = App->PrimManager->CreateCube(size, pos);
	
	pos.Set(1, 6, 0);
	Primitive* cube2 = App->PrimManager->CreateCube(size, pos);
	App->Physics->AddConstraintP2P(*cube1, *cube2,
		btVector3{ 4.f,-0.,-0 }, btVector3{ 0, 0,0 });

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
		std::string file_path = "Assets/Street environment_V01.FBX";

		uint UID = App->ResManager->FindInAssets(file_path.c_str());
		Resource* NewResource = App->ResManager->RequestResource(UID);
		if (NewResource != nullptr)
		{
			LOG("Resource Found");
			App->serializer->LoadModel(NewResource);
		}

	}
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		
		std::string file_path = "Assets/BakerHouse.fbx";

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
		Collider* NewCollider = nullptr;
		for (int i = 0; i < parent->Components.size(); i++)
		{
			switch (parent->Components[i]->type)
			{
			case ComponentType::MESH:
				newMesh = (ComponentMesh * )parent->GetComponent(ComponentType::MESH);
				App->serializer->AddComponent(JsonComp, newMesh, newMesh->reference->GetLibraryFile(), newMesh->reference->GetUID());

				break;
			case ComponentType::MATERIAL:
				NewTex = (ComponentMaterial*)parent->GetComponent(ComponentType::MATERIAL);
				App->serializer->AddComponent(JsonComp, NewTex, NewTex->reference->GetLibraryFile(), NewTex->reference->GetUID());

				break;
			case ComponentType::TRANSFORM:
				NewTrans = (ComponentTransform*)parent->GetComponent(ComponentType::TRANSFORM);
				App->serializer->AddVec3(JsonTrans, NewTrans->pos.x, NewTrans->pos.y, NewTrans->pos.z);
				App->serializer->AddVec3(JsonScale, NewTrans->scale.x, NewTrans->scale.y, NewTrans->scale.z);
				App->serializer->AddVec4(JsonRot, NewTrans->rot.x, NewTrans->rot.y, NewTrans->rot.z, NewTrans->rot.w);
				break;
			case ComponentType::COLLIDER:
				NewCollider = (Collider*)parent->GetComponent(ComponentType::COLLIDER);
				App->serializer->AddComponent(JsonComp, NewCollider, "", 0);

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
	
	return newGameObject;
}

void ModuleSceneIntro::UpdateGameObject(GameObject* parent, float dt)
{
	if (parent->active)
	{
		parent->Update(dt);
		for (int i = 0; i < parent->childs.size(); i++)
		{
			UpdateGameObject(parent->childs[i], dt);
		}

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

void ModuleSceneIntro::ManageDropEvent(std::string file_path)
{

	char* buffer = nullptr;
	uint fileSize = 0;
	std::string fileStr, extensionStr;
	App->file_system->SplitFilePath(file_path.c_str(), &fileStr, &extensionStr);
	std::string relativePath = "";
	FileType type = App->file_system->SetFileType(extensionStr);
	uint UID;
	switch (type)
	{
	case FileType::UNKNOWN:
		break;
	case FileType::FBX:
		relativePath.append("Assets").append("/").append(fileStr).append(extensionStr);
		UID = App->ResManager->FindInAssets(relativePath.c_str());
		if (UID == 0)
		{
			App->ResManager->ImportFileStep1(relativePath.c_str());
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
		break;
	case  FileType::IMAGE:
		relativePath.append("Assets").append("/").append(fileStr).append(extensionStr);
		App->FBX->ChangeTexture(relativePath.c_str());
		break;
	case  FileType::MESH:
		relativePath.append("Assets/library/").append(fileStr).append(extensionStr);

		App->FBX->ChangeMesh(relativePath.c_str());
		break;
	}


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


