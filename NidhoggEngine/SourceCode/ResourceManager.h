#ifndef __RESOURCEMANAGER__
#define __RESOURCEMANAGER__

#include "Module.h"
#include <map>

enum class ResourceType
{
	UNKNOWN,
	MODEL,
	TEXTURE,
	MESH,
};
class Resource
{
public:
	Resource(uint uid);
	virtual ~Resource();
	uint GetUID() const;
	const char* GetAssetFile() const;
	const char* GetLibraryFile() const;
	void SetAssetPath(const char* AssetsPath);
	void SetLibraryPath(const char* LibraryPath);
	void GenLibraryPath(Resource* resource);
	ResourceType GetType() const;
	virtual void loadResource();
protected:
	uint UID;
	std::string assetsFile;
	std::string LibraryFile;
	ResourceType type = ResourceType::UNKNOWN;
public:
	bool isLoaded;
};
class ResourceModel : public Resource
{
public:
	ResourceModel(uint uid);
	virtual ~ResourceModel();


protected:
	uint MeshUID;
	uint TextureUID;
public:
	JSON_Value* root_value;
	JSON_Object* root_object;
	JSON_Value* branch;
	JSON_Array* leaves;
};

class ResourceMesh : public Resource
{
public:
	ResourceMesh(uint uid);
	virtual ~ResourceMesh();
	void loadResource();

public:
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint id_normals = 0; // normals in VRAM
	uint num_normals = 0;
	float* normals = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

	uint id_tex; //textures coords in VRAM
	uint num_tex;
	float* texCoords;
};

class ResourceTexture : public Resource
{
public:
	ResourceTexture(uint uid);
	virtual ~ResourceTexture();
	void loadResource();


	public:
		uint texbuffer;				//texture loaded in VRAM
		int texture_w;
		int texture_h;
	
};

//----------------------------------------------------------------------

class ResourceManager : public Module
{
public:

	ResourceManager(Application* app, bool start_enabled = true);
	~ResourceManager();

	bool Init();

	// Called before quitting
	bool CleanUp();
	
	uint Find(const char* file_in_assets);
	uint ImportFile(const char* new_file_in_assets);
	Resource* CreateNewResource(const char* assetsFile, ResourceType type);
	uint GenerateNewUID();
	Resource* RequestResource(uint UID);
	std::map<uint, Resource*> resources;

private:
	ResourceType ReturnType(const char* assetsFile);

};

#endif // __RESOURCEMANAGER__


