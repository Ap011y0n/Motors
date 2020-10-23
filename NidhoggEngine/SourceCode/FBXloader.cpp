#include "Globals.h"
#include "Application.h"
#include "FBXloader.h"
#include "PrimitiveManager.h"
#include "FileSystem.h"
#include "GameObject.h"

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


#include <string.h>

FBXloader::FBXloader(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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


	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

	return ret;
}

void FBXloader::ChangeTexture(const char* path)
{
	LOG("changing all textures to %s", path);

	for (int i = 0; i < meshes.size(); i++)
	{
		glDeleteTextures(1, &(GLuint)meshes[i]->texbuffer);
		meshes[i]->texbuffer = LoadTexBuffer(path);
		if (meshes[i]->texbuffer != 0)
			meshes[i]->hastexture = true;
		else
			meshes[i]->hastexture = false;
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
	PrintMeshes();
	
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

// Load assets
bool FBXloader::LoadFBX(const char* buffer, uint size)
{
	bool ret = true;
	

	
	const aiScene* scene = aiImportFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);
	
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array

		for (int i = 0; i < scene->mNumMeshes; i++) {
			mesh* NewMesh = new mesh();

			const aiMesh* mesh = scene->mMeshes[i];


			aiString str;
			aiString folder;
			folder.Set("Assets/");
			NewMesh->texbuffer = 0;
			aiMaterial* newMaterial = scene->mMaterials[mesh->mMaterialIndex];
			if (newMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &str) != 0)
			{
				LOG("couldn't find diffuse texture")
			}
			else
			{
				folder.Append(str.C_Str());
				NewMesh->texbuffer = LoadTexBuffer(folder.C_Str());
				if (NewMesh->texbuffer != 0)
					NewMesh->hastexture = true;
				else
					NewMesh->hastexture = false;
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

			for (int i = 0; i < NewMesh->num_vertex; i++)
			{
				vec3 origin(NewMesh->vertex[i * 3], NewMesh->vertex[i * 3 + 1], NewMesh->vertex[i * 3 + 2]);
				vec3 destination(NewMesh->normals[i * 3], NewMesh->normals[i * 3 + 1], NewMesh->normals[i * 3 + 2]);
				destination *= 1;
				destination += origin;

				//App->PrimManager->CreateLine(origin, destination);
			}

			NewMesh->id_vertex = FillArrayBuffer(NewMesh->num_vertex * 3, NewMesh->vertex);

			NewMesh->id_tex = FillArrayBuffer(NewMesh->num_tex, NewMesh->texCoords);

			NewMesh->id_normals = FillArrayBuffer(NewMesh->num_normals * 3, NewMesh->normals);

			NewMesh->id_index = FillElementArrayBuffer(NewMesh->num_index, NewMesh->index);


			meshes.push_back(NewMesh);
		}
		

		aiReleaseImport(scene);
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

		texbuffer = ilutGLBindTexImage();
		ilDeleteImages(1, &imageName);

	}
	else
	{
		LOG("couldn't find texture with path %s", path);
	}
	
	return texbuffer;
}