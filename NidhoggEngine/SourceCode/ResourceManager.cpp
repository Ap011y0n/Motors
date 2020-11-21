#include "Application.h"
#include "ResourceManager.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "parson/parson.h"

#include <algorithm>    

ResourceManager::ResourceManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{

	
}


ResourceManager::~ResourceManager()
{
	
}

bool ResourceManager::Init()
{
	// Determine if the PhysicsFS library is initialized, we can check it for avoid errors.
	LOG("Initializing ResourceManager");
	return true;
}

bool ResourceManager::CleanUp()
{
	return false;
}

uint ResourceManager::Find(const char* file_in_assets)
{
	uint ret = 0;

	return ret;
}

uint ResourceManager::ImportFile(const char* new_file_in_assets)
{
	uint ret = 0;
	
	ResourceType type = ReturnType(new_file_in_assets);
	if (type == ResourceType::UNKNOWN)
	{
		LOG("Failed to import resource, unknown type");
		return ret;
	}
	
	Resource* resource = CreateNewResource(new_file_in_assets, type);
	char* buffer = nullptr;
	uint fileSize = 0;
	fileSize = App->file_system->Load(new_file_in_assets, &buffer);

	switch (resource->GetType())
	{
	case ResourceType::UNKNOWN:
		break;
	case ResourceType::MODEL:
		LOG("importing model from %s", new_file_in_assets);
		App->FBX->LoadFBX(buffer, fileSize, (ResourceModel*)resource);
		break;
	case ResourceType::TEXTURE:
		LOG("importing texture from %s", new_file_in_assets);
		break;
	case ResourceType::MESH:
		break;
	}
	//load buffers from physfs
	//add info to resource
	//save resource

	return ret = resource->GetUID();
}

ResourceType ResourceManager::ReturnType(const char* assetsFile)
{
	ResourceType ret = ResourceType::UNKNOWN;
	std::string file;
	std::string extension;

	App->file_system->SplitFilePath(assetsFile, &file, &extension);

	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

	if (extension == "fbx")
	{
		ret = ResourceType::MODEL;
	}
	if (extension == "png" || extension == "dds" || extension == ".tga")
	{
		ret = ResourceType::TEXTURE;
	}
	return ret;
}
uint ResourceManager::GenerateNewUID()
{
	LCG rand;
	uint UID = rand.Int();
	return UID;
}
Resource* ResourceManager::CreateNewResource(const char* assetsFile, ResourceType type)
{
	Resource* ret = nullptr;

	

	uint uid = GenerateNewUID();

	switch (type) {
	case ResourceType::MODEL: ret = (Resource*) new ResourceModel(uid); break;
	case ResourceType::TEXTURE: ret = (Resource*) new ResourceTexture(uid); break;
//	case ResourceType::MESH: ret = (Resource*) new ResourceMesh(uid); break;

	}

	if (ret != nullptr)
	{
		resources[uid] = ret;
		ret->SetAssetPath(assetsFile);
		ret->GenLibraryPath(ret);
	}
	return ret;
}

Resource* ResourceManager::RequestResource(uint UID)
{
	std::map<uint, Resource*>::iterator it = resources.find(UID);
	if (it != resources.end())
	{
		//it->second->referenceCount++;
		if (!it->second->isLoaded)
		{
			it->second->loadResource();
			it->second->isLoaded = true;
		}
		/*fileSize = App->file_system->Load(NewMeshResource->GetLibraryFile(), &buffer);
		MeshImporter::Load(buffer, fileSize, NewMesh);*/

		return it->second;
	}
	else
		return nullptr;

}

//-------------------------------------------------------------------------------

Resource::Resource(uint id)
{
	UID = id;
	type = ResourceType::UNKNOWN;
	assetsFile = "";
	LibraryFile = "";
	isLoaded = false;
}

Resource::~Resource()
{

}

uint Resource::GetUID() const
{
	return UID;
}

const char* Resource::GetAssetFile() const
{
	return assetsFile.c_str();
}

const char* Resource::GetLibraryFile() const
{
	return LibraryFile.c_str();
}
ResourceType Resource::GetType() const
{
	return type;
}

void Resource::SetAssetPath(const char* AssetsPath)
{
	assetsFile = AssetsPath;
}

void Resource::SetLibraryPath(const char* LibraryPath)
{
	LibraryFile = LibraryPath;
}

void Resource::GenLibraryPath(Resource* resource)
{
	std::string file;
	std::string extension;
	App->file_system->SplitFilePath(resource->assetsFile.c_str(), &file, &extension);

	switch (type)
	{
	case ResourceType::UNKNOWN:
		break;
	case ResourceType::MODEL:
		LibraryFile = "Assets/library/" + file + "model";
		break;
	case ResourceType::TEXTURE:
		LibraryFile = "Assets/library/" + file + "dds";
		break;
	case ResourceType::MESH:
		break;
	}
}

void Resource::loadResource()
{
	LOG("Unespecified resource type");
}
//-------------------------------------------------------------------------------

ResourceModel::ResourceModel(uint id) : Resource(id)
{
	type = ResourceType::MODEL;
	root_value = json_value_init_object();
	root_object = json_value_get_object(root_value);
	branch = json_value_init_array();
	leaves = json_value_get_array(branch);

	json_object_set_value(root_object, "Game Objects", branch);
}

ResourceModel::~ResourceModel()
{

}

//-------------------------------------------------------------------------------

ResourceMesh::ResourceMesh(uint id) : Resource(id)
{
	type = ResourceType::MESH;
}

ResourceMesh::~ResourceMesh()
{

}

void ResourceMesh::loadResource()
{
	char* buffer = nullptr;
	uint fileSize = 0;

	fileSize = App->file_system->Load(GetLibraryFile(), &buffer);
	MeshImporter::Load(buffer, fileSize, this);
}
//-------------------------------------------------------------------------------

ResourceTexture::ResourceTexture(uint id) : Resource(id)
{
	type = ResourceType::TEXTURE;
}

ResourceTexture::~ResourceTexture()
{

}

void ResourceTexture::loadResource()
{
	char* buffer = nullptr;
	uint fileSize = 0;

	fileSize = App->file_system->Load(GetLibraryFile(), &buffer);
	MaterialImporter::Load(buffer, fileSize, this);
}