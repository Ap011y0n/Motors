#include "Globals.h"
#include "Application.h"
#include "FBXloader.h"

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

	std::string file_path("Assets/warrior.FBX");

	const aiScene* scene = aiImportFile(file_path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++) {
			ourMesh.num_vertex = scene->mMeshes[i]->mNumVertices;
			ourMesh.vertex = new float[ourMesh.num_vertex * 3];
			memcpy(ourMesh.vertex, scene->mMeshes[i]->mVertices, sizeof(float) * ourMesh.num_vertex * 3);
			LOG("New mesh with %d vertices", ourMesh.num_vertex);

			if (scene->mMeshes[i]->HasFaces())
			{
				ourMesh.num_index = scene->mMeshes[i]->mNumFaces * 3;
				ourMesh.index = new uint[ourMesh.num_index]; // assume each face is a triangle
				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
				{
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						for (int x = 0; x < 3; x++)
						{
							ourMesh.index[j * 3 + x] = scene->mMeshes[i]->mFaces[j].mIndices[x];
						}
						//memcpy(&ourMesh.index[i * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}
			}
		}
	
	
	ourMesh.id_index = 0;
	ourMesh.id_vertex = 0;
	glGenBuffers(1, (GLuint*)&(ourMesh.id_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, ourMesh.id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ourMesh.num_vertex * 3, ourMesh.vertex, GL_STATIC_DRAW);
	

	glGenBuffers(1, (GLuint*)&(ourMesh.id_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ourMesh.id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * ourMesh.num_index, ourMesh.index, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", file_path.c_str());


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
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, ourMesh.id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ourMesh.id_index);

	glDrawElements(GL_TRIANGLES, ourMesh.num_index, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return UPDATE_CONTINUE;
}
