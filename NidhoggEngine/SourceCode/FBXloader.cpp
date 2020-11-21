#include "Globals.h"
#include "Application.h"
#include "FBXloader.h"
#include "PrimitiveManager.h"
#include "FileSystem.h"
#include "GameObject.h"

#include "serializer.h"
#include "parson/parson.h"

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "SourceCode/Assimp/libx86/assimp.lib")


#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"

#pragma comment( lib, "SourceCode/DevIL/libx86/DevIL.lib" )

#pragma comment( lib, "SourceCode/DevIL/libx86/ILU.lib" )
#pragma comment( lib, "SourceCode/DevIL/libx86/ILUT.lib" )

#include "MathGeoLib/include/MathGeoLib.h"
#include <string.h>

namespace MaterialImporter
{
	void Import( char* buffer, uint fileSize)
	{
		ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, fileSize);
	}
	void Save( char** fileBuffer, const char* path, std::string* newpath)
	{
		std::string doc(path);

		ILuint size;
		ILubyte* data;
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
		size = ilSaveL(IL_DDS, nullptr, 0); // Get the size of the data buffer
		if (size > 0) {
			data = new ILubyte[size]; // allocate data buffer
			if (ilSaveL(IL_DDS, data, size) > 0)
			{
				*fileBuffer = (char*)data; // Save to buffer with the ilSaveIL function
	
				size_t pos_dot = doc.find_last_of(".");
				doc = doc.substr(0, pos_dot);
				doc.append(".dds");
				doc = "library/" + doc;
				App->file_system->Save(doc.c_str(), *fileBuffer, size, false);
				newpath->append("Assets/");
				newpath->append(doc.c_str());
				if (data != nullptr)
				{
					delete[]data;
					data = nullptr;
				}
			}	
		}

	}
	void Load(const char* fileBuffer, uint size, ComponentMaterial* ourMaterial)
	{
		uint texbuffer = 0;
		glGenTextures(1, &texbuffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		ILuint ImageName;
		ilGenImages(1, &ImageName);
		ilBindImage(ImageName);

		ilLoadL(IL_TYPE_UNKNOWN, (const void*)fileBuffer, size);
		texbuffer = ilutGLBindTexImage();

		ourMaterial->texture_h = ilGetInteger(IL_IMAGE_HEIGHT);
		ourMaterial->texture_w = ilGetInteger(IL_IMAGE_WIDTH);
		ourMaterial->texbuffer = texbuffer;
		ilDeleteImages(1, &ImageName);
	}

	void Load(const char* fileBuffer, uint size, ResourceTexture* resourceTexture)
	{
		uint texbuffer = 0;
		glGenTextures(1, &texbuffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		ILuint ImageName;
		ilGenImages(1, &ImageName);
		ilBindImage(ImageName);

		ilLoadL(IL_TYPE_UNKNOWN, (const void*)fileBuffer, size);
		texbuffer = ilutGLBindTexImage();

		resourceTexture->texture_h = ilGetInteger(IL_IMAGE_HEIGHT);
		resourceTexture->texture_w = ilGetInteger(IL_IMAGE_WIDTH);
		resourceTexture->texbuffer = texbuffer;
		ilDeleteImages(1, &ImageName);
	}
};

namespace MeshImporter
{
	
	void Load(char* fileBuffer, uint size, ComponentMesh* mesh)
	{
		char* cursor = fileBuffer;

		// amount of indices / vertices / colors / normals / texture_coords
		uint ranges[4];
		uint bytes = sizeof(ranges);
		memcpy(ranges, cursor, bytes);
		cursor += bytes;
		mesh->num_index = ranges[0];
		mesh->num_vertex = ranges[1];
		mesh->num_normals = ranges[2];
		mesh->num_tex = ranges[3];

		// Load indices
		bytes = sizeof(uint) * mesh->num_index;
		mesh->index = new uint[mesh->num_index];
		memcpy(mesh->index, cursor, bytes);
		cursor += bytes;
		// Load vertex
		bytes = sizeof(float) * mesh->num_vertex * 3;
		mesh->vertex = new float[mesh->num_vertex * 3];
		memcpy(mesh->vertex, cursor, bytes);
		cursor += bytes;
		// Load vertex
		bytes = sizeof(float) * mesh->num_normals * 3;
		mesh->normals = new float[mesh->num_normals * 3];
		memcpy(mesh->normals, cursor, bytes);
		cursor += bytes;
		// Load vertex
		bytes = sizeof(float) * mesh->num_tex;
		mesh->texCoords = new float[mesh->num_tex];
		memcpy(mesh->texCoords, cursor, bytes);
	
	}
	void Load(char* fileBuffer, uint size, ResourceMesh* ResourceMesh)
	{
		char* cursor = fileBuffer;

		// amount of indices / vertices / colors / normals / texture_coords
		uint ranges[4];
		uint bytes = sizeof(ranges);
		memcpy(ranges, cursor, bytes);
		cursor += bytes;
		ResourceMesh->num_index = ranges[0];
		ResourceMesh->num_vertex = ranges[1];
		ResourceMesh->num_normals = ranges[2];
		ResourceMesh->num_tex = ranges[3];

		// Load indices
		bytes = sizeof(uint) * ResourceMesh->num_index;
		ResourceMesh->index = new uint[ResourceMesh->num_index];
		memcpy(ResourceMesh->index, cursor, bytes);
		cursor += bytes;
		// Load vertex
		bytes = sizeof(float) * ResourceMesh->num_vertex * 3;
		ResourceMesh->vertex = new float[ResourceMesh->num_vertex * 3];
		memcpy(ResourceMesh->vertex, cursor, bytes);
		cursor += bytes;
		// Load vertex
		bytes = sizeof(float) * ResourceMesh->num_normals * 3;
		ResourceMesh->normals = new float[ResourceMesh->num_normals * 3];
		memcpy(ResourceMesh->normals, cursor, bytes);
		cursor += bytes;
		// Load vertex
		bytes = sizeof(float) * ResourceMesh->num_tex;
		ResourceMesh->texCoords = new float[ResourceMesh->num_tex];
		memcpy(ResourceMesh->texCoords, cursor, bytes);

		ResourceMesh->id_vertex = App->FBX->FillArrayBuffer(ResourceMesh->num_vertex * 3, ResourceMesh->vertex);

		ResourceMesh->id_tex = App->FBX->FillArrayBuffer(ResourceMesh->num_tex, ResourceMesh->texCoords);

		ResourceMesh->id_normals = App->FBX->FillArrayBuffer(ResourceMesh->num_normals * 3, ResourceMesh->normals);

		ResourceMesh->id_index = App->FBX->FillElementArrayBuffer(ResourceMesh->num_index, ResourceMesh->index);

	}
	void Save(ComponentMesh* mesh, std::string* path, const char* name)
	{
		std::string doc;

		uint ranges[4] = { mesh->num_index, mesh->num_vertex, mesh->num_normals, mesh->num_tex };
		uint size = sizeof(ranges) + sizeof(uint) * mesh->num_index + sizeof(float) * mesh->num_vertex * 3 + sizeof(float) * mesh->num_normals * 3 + sizeof(float) * mesh->num_tex;

		char* fileBuffer = new char[size]; // Allocate
		char* cursor = fileBuffer;

		uint bytes = sizeof(ranges); // First store ranges
		memcpy(cursor, ranges, bytes);
		cursor += bytes;
		// Store indices
		bytes = sizeof(uint) * mesh->num_index;
		memcpy(cursor, mesh->index, bytes);
		cursor += bytes;
		// Store vertices
		bytes = sizeof(float) * mesh->num_vertex * 3;
		memcpy(cursor, mesh->vertex, bytes);
		cursor += bytes;
		// Store normals
		bytes = sizeof(float) * mesh->num_normals * 3;
		memcpy(cursor, mesh->normals, bytes);
		cursor += bytes;
		// Store textures
		bytes = sizeof(float) * mesh->num_tex;
		memcpy(cursor, mesh->texCoords, bytes);


		doc.append(name);
		doc.append(".mesh");
		doc = "library/" + doc;
		App->file_system->Save(doc.c_str(), fileBuffer, size, false);
		path->append("Assets/");
		path->append(doc.c_str());
	}

	void Save(ComponentMesh* mesh, ResourceMesh* resource, const char* name)
	{
		std::string doc;

		uint ranges[4] = { mesh->num_index, mesh->num_vertex, mesh->num_normals, mesh->num_tex };
		uint size = sizeof(ranges) + sizeof(uint) * mesh->num_index + sizeof(float) * mesh->num_vertex * 3 + sizeof(float) * mesh->num_normals * 3 + sizeof(float) * mesh->num_tex;

		char* fileBuffer = new char[size]; // Allocate
		char* cursor = fileBuffer;

		uint bytes = sizeof(ranges); // First store ranges
		memcpy(cursor, ranges, bytes);
		cursor += bytes;
		// Store indices
		bytes = sizeof(uint) * mesh->num_index;
		memcpy(cursor, mesh->index, bytes);
		cursor += bytes;
		// Store vertices
		bytes = sizeof(float) * mesh->num_vertex * 3;
		memcpy(cursor, mesh->vertex, bytes);
		cursor += bytes;
		// Store normals
		bytes = sizeof(float) * mesh->num_normals * 3;
		memcpy(cursor, mesh->normals, bytes);
		cursor += bytes;
		// Store textures
		bytes = sizeof(float) * mesh->num_tex;
		memcpy(cursor, mesh->texCoords, bytes);


		doc.append(name);
		doc.append(".mesh");
		std::string path;
		doc = "library/" + doc;
		App->file_system->Save(doc.c_str(), fileBuffer, size, false);
		path.append("Assets/");
		path.append(doc.c_str());
		resource->SetLibraryPath(path.c_str());
	}
};

FBXloader::FBXloader(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	numberGO = 0;
}

FBXloader::~FBXloader()
{}

// Load assets
bool FBXloader::Start()
{
	bool ret = true;
	LOG("start FBX loader");
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	texture_w=0;
	texture_h=0;

	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

	return ret;
}

void FBXloader::ChangeTexture(const char* path)
{
	LOG("changing all textures to %s", path);
	ComponentMaterial* mat = nullptr;
	if (App->UI->selectedObj != nullptr)
	{
		for (int i = 0; i < App->UI->selectedObj->Components.size(); i++)
		{
			if (App->UI->selectedObj->Components[i]->type == ComponentType::MATERIAL)
			{
				mat = (ComponentMaterial*)App->UI->selectedObj->Components[i];
			}
		}


		if (mat != nullptr)
		{
			glDeleteTextures(1, &(GLuint)mat->texbuffer);
			mat->texbuffer = LoadTexBuffer(path);
			mat->texture_path = path;
			mat->texture_h = texture_h;
			mat->texture_w = texture_w;
			if (mat->texbuffer != 0)
				mat->hastexture = true;
			else
				mat->hastexture = false;
		}

	}


}
// Load assets
bool FBXloader::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

// Update: draw background
update_status FBXloader::PostUpdate(float dt)
{
	//PrintMeshes();
	
	return UPDATE_CONTINUE;
}

void FBXloader::PrintMeshes()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, meshes[i]->id_vertex);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, meshes[i]->id_normals);
		glNormalPointer(GL_FLOAT, 0, NULL);


		glBindBuffer(GL_ARRAY_BUFFER, meshes[i]->id_tex);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		if (meshes[i]->hastexture)
		{
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_CULL_FACE);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, meshes[i]->texbuffer);
		}
	

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->id_index);

		

		glDrawElements(GL_TRIANGLES, meshes[i]->num_index, GL_UNSIGNED_INT, NULL);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisable(GL_TEXTURE_2D);

	}

}

uint FBXloader::FillArrayBuffer( uint size, float *array)
{
	uint id = 0;
	glGenBuffers(1, (GLuint*)&(id));
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, array, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return id;
}

uint FBXloader::FillElementArrayBuffer(uint size, uint* array)
{
	uint id = 0;
	glGenBuffers(1, (GLuint*)&(id));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * size, array, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return id;
}
void FBXloader::LoadNode(const aiScene* scene, aiNode* node, GameObject* father)
{
	// Use scene->mNumMeshes to iterate on scene->mMeshes array
	//for (int i = 0; i < scene->mNumMeshes; i++)
	std::string name = node->mName.C_Str();

	LOG("loading %s", node->mName.C_Str());

	if (name == "RootNode")
	{
		numberGO++;
		std::string obj = std::to_string(numberGO);
		
		name = "GameObject";
		name.append(obj);

	}
	GameObject* object = App->scene_intro->CreateGameObject(name.c_str(), father);
	JSON_Object* JsonObj = App->serializer->AddObjectToArray(App->serializer->leaves);
	App->serializer->AddFloat(JsonObj, "UID", object->UID);
	App->serializer->AddFloat(JsonObj, "ParentUID", object->parent->UID);
	App->serializer->AddString(JsonObj, "Name", name.c_str());
	JSON_Array* JsonTrans = App->serializer->AddArray(JsonObj, "Translation");
	JSON_Array* JsonScale = App->serializer->AddArray(JsonObj, "Scale");
	JSON_Array* JsonRot = App->serializer->AddArray(JsonObj, "Rotation");
	JSON_Array* JsonComp = App->serializer->AddArray(JsonObj, "Components");

	ComponentTransform* NewTrans = (ComponentTransform*)object->CreateComponent(ComponentType::TRANSFORM);

	aiVector3D translation, scaling;
	aiVector3D euler;
	aiQuaternion rotation;


	//ComponentTransform* fathertrans = nullptr;

	node->mTransformation.Decompose(scaling, rotation, translation);

	/*for (int i = 0; i < father->Components.size(); i++)
	{
		if (father->Components[i]->type == ComponentType::TRANSFORM)
		{
			 fathertrans = (ComponentTransform*)father->Components[i];
			
		}
	}*/
	
		NewTrans->pos.Set(translation.x, translation.y, translation.z);
		NewTrans->scale.Set(scaling.x, scaling.y, scaling.z);
		NewTrans->rot.Set(rotation.x, rotation.y, rotation.z, rotation.w);
	
	

	//NewTrans->rot = NewTrans->rot * quat;
	App->serializer->AddVec3(JsonTrans, NewTrans->pos.x, NewTrans->pos.y, NewTrans->pos.z);
	App->serializer->AddVec3(JsonScale, NewTrans->scale.x, NewTrans->scale.y, NewTrans->scale.z);
	App->serializer->AddVec4(JsonRot, NewTrans->rot.x, NewTrans->rot.y, NewTrans->rot.z, NewTrans->rot.w);

	NewTrans->transform = float4x4::FromTRS(NewTrans->pos, NewTrans->rot, NewTrans->scale);


	for (int i = 0; i < node->mNumMeshes; i++)
	{


		ComponentMesh* NewMesh = (ComponentMesh*)object->CreateComponent(ComponentType::MESH);
		//const aiMesh* mesh = scene->mMeshes[i];
		const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		aiString str;
	

		aiMaterial* newMaterial = scene->mMaterials[mesh->mMaterialIndex];
	
		if (newMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &str) != 0)
		{

			LOG("couldn't find diffuse texture");
		}
		else
		{
			std::string file, extension;
			App->file_system->SplitFilePath(str.C_Str(), &file, &extension );
			ComponentMaterial* NewTex = (ComponentMaterial*)object->CreateComponent(ComponentType::MATERIAL);
			//
			uint fileSize = 0;
			char* buffer = nullptr;
		
			file += extension;
			fileSize = App->file_system->Load(file.c_str(), &buffer);

			MaterialImporter::Import(buffer, fileSize);
			std::string path;
			MaterialImporter::Save(&buffer, file.c_str(), &path);
			NewTex->texture_path = path.c_str();
			App->serializer->AddComponent(JsonComp, ComponentType::MATERIAL, NewTex->texture_path.c_str());
			//TODO_Json: create a json component mesh with this path. No need to load anything
		//	fileSize = App->file_system->Load(path.c_str(), &buffer);
		//	MaterialImporter::Load(buffer, fileSize, NewTex);

			//

		/*	NewTex->texbuffer = LoadTexBuffer(str.C_Str());
			
			NewTex->texture_h = texture_h;
			NewTex->texture_w = texture_w;
			NewTex->texture_path = str.C_Str();*/
			if (NewTex->texbuffer != 0)
				NewTex->hastexture = true;
			else
				NewTex->hastexture = false;
		}


		NewMesh->num_vertex = mesh->mNumVertices;
		
		NewMesh->vertex = new float[NewMesh->num_vertex * 3];
		memcpy(NewMesh->vertex, mesh->mVertices, sizeof(float) * NewMesh->num_vertex * 3);
		LOG("New mesh with %d vertices", NewMesh->num_vertex);

		NewMesh->num_normals = mesh->mNumVertices;
		NewMesh->normals = new float[NewMesh->num_vertex * 3];

		memcpy(NewMesh->normals, mesh->mNormals, sizeof(float) * NewMesh->num_vertex * 3);
		if (mesh->HasTextureCoords(0)) {  // Assuming only one texture is attached to this mesh

			NewMesh->texCoords = new float[mesh->mNumVertices * 2];
			NewMesh->num_tex = mesh->mNumVertices * 2;
			for (unsigned int k = 0; k < mesh->mNumVertices; k++) {

				NewMesh->texCoords[k * 2] = mesh->mTextureCoords[0][k].x;
				NewMesh->texCoords[k * 2 + 1] = mesh->mTextureCoords[0][k].y;

			}
		}
		if (mesh->HasFaces())
		{
			NewMesh->num_index = mesh->mNumFaces * 3;
			NewMesh->index = new uint[NewMesh->num_index]; // assume each face is a triangle
			for (uint j = 0; j < mesh->mNumFaces; j++)
			{
				if (mesh->mFaces[j].mNumIndices != 3)
				{
					LOG("WARNING, geometry face with != 3 indices!");
				}
				else
				{
					vec3 origin(0, 0, 0);
					vec3 dest(0, 0, 0);
					for (int x = 0; x < 3; x++)
					{
						NewMesh->index[j * 3 + x] = mesh->mFaces[j].mIndices[x];

					}
				}
			}
		}

		std::string path;
		uint fileSize = 0;
		char* buffer = nullptr;

		MeshImporter::Save(NewMesh, &path, name.c_str());
		
		App->serializer->AddComponent(JsonComp, ComponentType::MESH, path.c_str());

		// Unnnecesary
	//	fileSize = App->file_system->Load(path.c_str(), &buffer);
	//	MeshImporter::Load(buffer, fileSize, NewMesh);

	//	NewMesh->id_vertex = FillArrayBuffer(NewMesh->num_vertex * 3, NewMesh->vertex);

	//	NewMesh->id_tex = FillArrayBuffer(NewMesh->num_tex, NewMesh->texCoords);

	//	NewMesh->id_normals = FillArrayBuffer(NewMesh->num_normals * 3, NewMesh->normals);

	//	NewMesh->id_index = FillElementArrayBuffer(NewMesh->num_index, NewMesh->index);
		// Unnnecesary

		//	CreateAABB(NewMesh);

	}
	App->serializer->SaveScene();

	

	for (int n = 0; n < node->mNumChildren; n++)
	{
		
		LoadNode(scene, node->mChildren[n], object);

	}
	object->to_delete = true;
}
// Load assets
bool FBXloader::LoadFBX(const char* buffer, uint size)
{
	bool ret = true;
	

	const aiScene* scene = aiImportFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);
	if (scene != nullptr)
	{
		aiNode* node = scene->mRootNode;
		

	
	//if (scene != nullptr && scene->HasMeshes())

	if (node != nullptr)
	{
		LoadNode(scene, node ,App->scene_intro->scene);
		aiReleaseImport(scene);
	}
	}
	else
		LOG("Error loading scene");


	return ret;
}

void FBXloader::LoadNode(const aiScene* scene, aiNode* node, ResourceModel* model, GameObject* father)
{
	std::string name = node->mName.C_Str();

	LOG("loading %s", node->mName.C_Str());

	if (name == "RootNode")
	{
		numberGO++;
		std::string obj = std::to_string(numberGO);

		name = "GameObject";
		name.append(obj);

	}
	GameObject* object = App->scene_intro->CreateGameObject(name.c_str(), father);
	JSON_Object* JsonObj = App->serializer->AddObjectToArray(model->leaves);

	App->serializer->AddFloat(JsonObj, "UID", object->UID);
	if(object->parent != nullptr)
	App->serializer->AddFloat(JsonObj, "ParentUID", object->parent->UID);
	else
		App->serializer->AddFloat(JsonObj, "ParentUID", 0);

	App->serializer->AddString(JsonObj, "Name", name.c_str());

	JSON_Array* JsonTrans = App->serializer->AddArray(JsonObj, "Translation");
	JSON_Array* JsonScale = App->serializer->AddArray(JsonObj, "Scale");
	JSON_Array* JsonRot = App->serializer->AddArray(JsonObj, "Rotation");
	JSON_Array* JsonComp = App->serializer->AddArray(JsonObj, "Components");

	ComponentTransform* NewTrans = (ComponentTransform*)object->CreateComponent(ComponentType::TRANSFORM);

	aiVector3D translation, scaling;
	aiVector3D euler;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scaling, rotation, translation);


	NewTrans->pos.Set(translation.x, translation.y, translation.z);
	NewTrans->scale.Set(scaling.x, scaling.y, scaling.z);
	NewTrans->rot.Set(rotation.x, rotation.y, rotation.z, rotation.w);



	//NewTrans->rot = NewTrans->rot * quat;
	App->serializer->AddVec3(JsonTrans, NewTrans->pos.x, NewTrans->pos.y, NewTrans->pos.z);
	App->serializer->AddVec3(JsonScale, NewTrans->scale.x, NewTrans->scale.y, NewTrans->scale.z);
	App->serializer->AddVec4(JsonRot, NewTrans->rot.x, NewTrans->rot.y, NewTrans->rot.z, NewTrans->rot.w);

	NewTrans->transform = float4x4::FromTRS(NewTrans->pos, NewTrans->rot, NewTrans->scale);


	for (int i = 0; i < node->mNumMeshes; i++)
	{
		//For now, we manually create the compoentns without checking if they are repeated or not, we'll only check taht for Models

		ComponentMesh* NewMesh = (ComponentMesh*)object->CreateComponent(ComponentType::MESH);
		const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		aiString str;

		//****TEMP
		uint meshUID = App->ResManager->GenerateNewUID();
		ResourceMesh* meshResource =  new ResourceMesh(meshUID);
		App->ResManager->resources[meshUID] = (Resource*)meshResource;

		//****TEMP

		aiMaterial* newMaterial = scene->mMaterials[mesh->mMaterialIndex];

		if (newMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &str) != 0)
		{

			LOG("couldn't find diffuse texture");
		}
		else
		{
			std::string file, extension;
			App->file_system->SplitFilePath(str.C_Str(), &file, &extension);
			ComponentMaterial* NewTex = (ComponentMaterial*)object->CreateComponent(ComponentType::MATERIAL);

			//****TEMP
			uint texUID = App->ResManager->GenerateNewUID();
			ResourceTexture* TexResource = new ResourceTexture(texUID);
			App->ResManager->resources[texUID] = TexResource;
			//****TEMP

			uint fileSize = 0;
			char* buffer = nullptr;

			file += extension;
			fileSize = App->file_system->Load(file.c_str(), &buffer);

			MaterialImporter::Import(buffer, fileSize);
			std::string path;
			MaterialImporter::Save(&buffer, file.c_str(), &path);
			NewTex->texture_path = path.c_str();

			TexResource->SetAssetPath(str.C_Str());
			TexResource->SetLibraryPath(NewTex->texture_path.c_str());

			App->serializer->AddResourceComponent(JsonComp, ComponentType::MATERIAL, TexResource->GetUID());

			if (NewTex->texbuffer != 0)
				NewTex->hastexture = true;
			else
				NewTex->hastexture = false;
		}


		NewMesh->num_vertex = mesh->mNumVertices;

		NewMesh->vertex = new float[NewMesh->num_vertex * 3];
		memcpy(NewMesh->vertex, mesh->mVertices, sizeof(float) * NewMesh->num_vertex * 3);
		LOG("New mesh with %d vertices", NewMesh->num_vertex);

		NewMesh->num_normals = mesh->mNumVertices;
		NewMesh->normals = new float[NewMesh->num_vertex * 3];

		memcpy(NewMesh->normals, mesh->mNormals, sizeof(float) * NewMesh->num_vertex * 3);
		if (mesh->HasTextureCoords(0)) {  // Assuming only one texture is attached to this mesh

			NewMesh->texCoords = new float[mesh->mNumVertices * 2];
			NewMesh->num_tex = mesh->mNumVertices * 2;
			for (unsigned int k = 0; k < mesh->mNumVertices; k++) {

				NewMesh->texCoords[k * 2] = mesh->mTextureCoords[0][k].x;
				NewMesh->texCoords[k * 2 + 1] = mesh->mTextureCoords[0][k].y;

			}
		}
		if (mesh->HasFaces())
		{
			NewMesh->num_index = mesh->mNumFaces * 3;
			NewMesh->index = new uint[NewMesh->num_index]; // assume each face is a triangle
			for (uint j = 0; j < mesh->mNumFaces; j++)
			{
				if (mesh->mFaces[j].mNumIndices != 3)
				{
					LOG("WARNING, geometry face with != 3 indices!");
				}
				else
				{
					vec3 origin(0, 0, 0);
					vec3 dest(0, 0, 0);
					for (int x = 0; x < 3; x++)
					{
						NewMesh->index[j * 3 + x] = mesh->mFaces[j].mIndices[x];

					}
				}
			}
		}



		uint fileSize = 0;
		char* buffer = nullptr;

		MeshImporter::Save(NewMesh, meshResource, name.c_str());

		App->serializer->AddResourceComponent(JsonComp, ComponentType::MESH, meshResource->GetUID());

		std::string file;
		std::string extension;
		App->file_system->SplitFilePath(model->GetLibraryFile(), &file, &extension);
		file.append("model");
		std::string directory = "library/";
		App->serializer->SaveValueAsFile(model->root_value, file.c_str(), directory);
	}

	



	for (int n = 0; n < node->mNumChildren; n++)
	{

		LoadNode(scene, node->mChildren[n], model, object);

	}
	object->to_delete = true;
}
bool FBXloader::LoadFBX(const char* buffer, uint size, ResourceModel* model)
{
	bool ret = true;


	const aiScene* scene = aiImportFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);
	if (scene != nullptr)
	{
		aiNode* node = scene->mRootNode;

		if (node != nullptr)
		{
			LoadNode(scene, node, model, nullptr);
			aiReleaseImport(scene);

		}
	}
	else
		LOG("Error loading scene");


	return ret;
}

uint FBXloader::LoadTexBuffer(const char* path)
{
	char* buffer = nullptr;
	uint fileSize = 0;

	uint texbuffer = 0;
	ILuint imageName = 0;

	fileSize = App->file_system->Load(path, &buffer);
	
	if (fileSize != 0)
	{
		glGenTextures(1, &texbuffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		ilGenImages(1, &imageName);
		ilBindImage(imageName);
	
		ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, fileSize);
		texture_h = ilGetInteger(IL_IMAGE_HEIGHT);
		texture_w = ilGetInteger(IL_IMAGE_WIDTH);
		
		texbuffer = ilutGLBindTexImage();
		ilDeleteImages(1, &imageName);

	}
	else
	{
		LOG("couldn't find texture with path %s", path);
	}
	
	return texbuffer;
}

void FBXloader::CreateAABB(ComponentMesh* NewMesh)
{
	NewMesh->bbox.SetNegativeInfinity();
	float3* vertexvert = (float3*)NewMesh->vertex;
	NewMesh->bbox.Enclose(vertexvert, NewMesh->num_vertex);

	float3 minvalue(inf, inf, inf);
	float3 maxvalue(-inf, -inf, -inf);

	for (int i = 0; i < NewMesh->num_vertex; i++)
	{
		if (vertexvert[i].x < minvalue.x)
		{
			minvalue.x = vertexvert[i].x;
		}
		if (vertexvert[i].y < minvalue.y)
		{
			minvalue.y = vertexvert[i].y;
		}
		if (vertexvert[i].z < minvalue.z)
		{
			minvalue.z = vertexvert[i].z;
		}

		if (vertexvert[i].x > maxvalue.x)
		{
			maxvalue.x = vertexvert[i].x;
		}
		if (vertexvert[i].y > maxvalue.y)
		{
			maxvalue.y = vertexvert[i].y;
		}
		if (vertexvert[i].z > maxvalue.z)
		{
			maxvalue.z = vertexvert[i].z;
		}

	}
	vec3 origin, destination;


	minvalue = NewMesh->bbox.minPoint;
	maxvalue = NewMesh->bbox.maxPoint;

	float3 array[8];

	NewMesh->bbox.GetCornerPoints(array);
	/*for (int i = 0; i < 8; i++)
	{
		origin.Set(array[i].x, array[i].y, array[i].z);
		if (i != 7)
		destination.Set(array[i+1].x, array[i+1].y, array[i+1].z);
		else
		destination.Set(array[0].x, array[0].y, array[0].z);

		App->PrimManager->CreateLine(origin, destination);
	}
	*/
	//0 = back left bot
	//2 = back left top
	//4 = back right bot
	//6 = back right top

	//3 = front left top
	//7 = front right top
	//1 = front left bot
	//5 = front right bot

	//origin.Set(array[0].x, array[0].y, array[0].z);
	//destination.Set(array[1].x, array[1].y, array[1].z);

	//App->PrimManager->CreateLine(origin, destination);

	//origin.Set(array[1].x, array[1].y, array[1].z);
	//destination.Set(array[3].x, array[3].y, array[3].z);

	//App->PrimManager->CreateLine(origin, destination);

	//origin.Set(array[3].x, array[3].y, array[3].z);
	//destination.Set(array[7].x, array[7].y, array[7].z);

	//App->PrimManager->CreateLine(origin, destination);

	//origin.Set(array[3].x, array[3].y, array[3].z);
	//destination.Set(array[2].x, array[2].y, array[2].z);

	//App->PrimManager->CreateLine(origin, destination);

	//origin.Set(array[0].x, array[0].y, array[0].z);
	//destination.Set(array[2].x, array[2].y, array[2].z);

	//App->PrimManager->CreateLine(origin, destination);

	//origin.Set(array[1].x, array[1].y, array[1].z);
	//destination.Set(array[5].x, array[5].y, array[5].z);

	//App->PrimManager->CreateLine(origin, destination);

	//origin.Set(array[7].x, array[7].y, array[7].z);
	//destination.Set(array[6].x, array[6].y, array[6].z);

	//App->PrimManager->CreateLine(origin, destination);


	//origin.Set(array[2].x, array[2].y, array[2].z);
	//destination.Set(array[6].x, array[6].y, array[6].z);

	//App->PrimManager->CreateLine(origin, destination);

	//origin.Set(array[6].x, array[6].y, array[6].z);
	//destination.Set(array[4].x, array[4].y, array[4].z);

	//App->PrimManager->CreateLine(origin, destination);

	//origin.Set(array[4].x, array[4].y, array[4].z);
	//destination.Set(array[0].x, array[0].y, array[0].z);

	//App->PrimManager->CreateLine(origin, destination);

	//origin.Set(array[7].x, array[7].y, array[7].z);
	//destination.Set(array[5].x, array[5].y, array[5].z);

	//App->PrimManager->CreateLine(origin, destination);

	//origin.Set(array[4].x, array[4].y, array[4].z);
	//destination.Set(array[5].x, array[5].y, array[5].z);

	//App->PrimManager->CreateLine(origin, destination);
	
}