#include "Globals.h"
#include "Application.h"
#include "FBXloader.h"
#include "PrimitiveManager.h"
#include "FileSystem.h"

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

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

	return ret;
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


		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i]->id_tex);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->id_index);

		glDrawElements(GL_TRIANGLES, meshes[i]->num_index, GL_UNSIGNED_INT, NULL);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
	mesh* NewMesh = new mesh();

	
	const aiScene* scene = aiImportFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);
	
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array

		for (int i = 0; i < scene->mNumMeshes; i++) {

			const aiMesh* mesh = scene->mMeshes[i];
			NewMesh->num_vertex = mesh->mNumVertices;
			NewMesh->vertex = new float[NewMesh->num_vertex * 3];
			memcpy(NewMesh->vertex, mesh->mVertices, sizeof(float) * NewMesh->num_vertex * 3);
			LOG("New mesh with %d vertices", NewMesh->num_vertex);

			NewMesh->num_normals = mesh->mNumVertices;
			NewMesh->normals = new float[NewMesh->num_vertex * 3];
		

			memcpy(NewMesh->normals, mesh->mNormals, sizeof(float) * NewMesh->num_vertex * 3);

			if (mesh->HasTextureCoords(0)) {  // Assuming only one texture is attached to this mesh

				NewMesh->texCoords = (float*)malloc(sizeof(float) * 2 * mesh->mNumVertices);
				NewMesh->num_tex = mesh->mNumVertices * 2;
				for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
					
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

							/*vec3 add_origin(ourMesh.vertex[ourMesh.index[j * 3 + x]],
								ourMesh.vertex[ourMesh.index[j * 3 + x] + 1],
								ourMesh.vertex[ourMesh.index[j * 3 + x] + 2]);
							vec3 add_dest(ourMesh.normals[ourMesh.index[j * 3 + x]],
								ourMesh.normals[ourMesh.index[j * 3 + x] + 1],
								ourMesh.normals[ourMesh.index[j * 3 + x] + 2]);
							origin += add_origin;
							dest += add_dest;*/

						}
						/*origin /= 3;
						dest *= -1;
						dest += origin;

						dest /= 3;
						dest.Set(1, 1, 1);
						App->PrimManager->CreateLine(origin, dest);*/
						//memcpy(&ourMesh.index[i * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint));
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

			App->PrimManager->CreateLine(origin, destination);
		}

		NewMesh->id_vertex = FillArrayBuffer(NewMesh->num_vertex * 3, NewMesh->vertex);
		
		NewMesh->id_tex = FillArrayBuffer(NewMesh->num_tex, NewMesh->texCoords);
		
		NewMesh->id_normals = FillArrayBuffer(NewMesh->num_normals * 3, NewMesh->normals);

		NewMesh->id_index = FillElementArrayBuffer(NewMesh->num_index, NewMesh->index);

		
		meshes.push_back(NewMesh);

		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene");


	return ret;
}