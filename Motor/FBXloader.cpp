#include "Globals.h"
#include "Application.h"
#include "FBXloader.h"
#include "PrimitiveManager.h"

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

			const aiMesh* mesh = scene->mMeshes[i];
			ourMesh.num_vertex = mesh->mNumVertices;
			ourMesh.vertex = new float[ourMesh.num_vertex * 3];
			memcpy(ourMesh.vertex, mesh->mVertices, sizeof(float) * ourMesh.num_vertex * 3);
			LOG("New mesh with %d vertices", ourMesh.num_vertex);

			ourMesh.num_normals = mesh->mNumVertices;
			ourMesh.normals = new float[ourMesh.num_vertex * 3];
		/*	for (int p = 0; p < ourMesh.num_vertex; p++)
			{
				LOG("%f", mesh->mNormals[p].x);
				LOG("%f", mesh->mNormals[p].y);
				LOG("%f", mesh->mNormals[p].z);
			}*/
			

			memcpy(ourMesh.normals, mesh->mNormals, sizeof(float) * ourMesh.num_vertex * 3);

			if (mesh->HasTextureCoords(0)) {  // Assuming only one texture is attached to this mesh

				ourMesh.texCoords = (float*)malloc(sizeof(float) * 2 * mesh->mNumVertices);
				ourMesh.num_tex = mesh->mNumVertices * 2;
				for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
					// Go through the array of texture coordinates and copy them to a one-dimensional array. 
					// Note that in Assimp the texture coordinates are not stored in a "flat" array. 
					// The mTextureCoords variable in aiMesh is a 2D array. 
					// The first dimension is the number of textures associated with this mesh. 
					// The second dimension is the number of vertices. 
					// Assimp uses aiVector3D to store texture coordinates, but a texture coordinate only uses the first two components. 
					ourMesh.texCoords[k * 2] = mesh->mTextureCoords[0][k].x;
					ourMesh.texCoords[k * 2 + 1] = mesh->mTextureCoords[0][k].y;

				}
			}
			if (mesh->HasFaces())
			{
				ourMesh.num_index = mesh->mNumFaces * 3;
				ourMesh.index = new uint[ourMesh.num_index]; // assume each face is a triangle
				for (uint j = 0; j < mesh->mNumFaces; j++)
				{
					if (mesh->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						for (int x = 0; x < 3; x++)
						{
							ourMesh.index[j * 3 + x] = mesh->mFaces[j].mIndices[x];
						}
						//memcpy(&ourMesh.index[i * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}
			}
		}
		for (int i = 0; i < ourMesh.num_vertex; i ++)
		{
			vec3 origin(ourMesh.vertex[i*3], ourMesh.vertex[i*3+1], ourMesh.vertex[i * 3 + 2]);
			vec3 destination(ourMesh.normals[i], ourMesh.normals[i + 1], ourMesh.normals[i + 2]);
			destination += origin;
			
			App->PrimManager->CreateLine(origin, destination);
		}
	
	ourMesh.id_index = 0;
	ourMesh.id_vertex = 0;
	ourMesh.id_normals = 0;
	ourMesh.id_tex = 0;

	glGenBuffers(1, (GLuint*)&(ourMesh.id_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, ourMesh.id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ourMesh.num_vertex * 3, ourMesh.vertex, GL_STATIC_DRAW);
	
	glGenBuffers(1, (GLuint*)&(ourMesh.id_tex));
	glBindBuffer(GL_ARRAY_BUFFER, ourMesh.id_tex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ourMesh.num_tex, ourMesh.texCoords, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&(ourMesh.id_normals));
	glBindBuffer(GL_ARRAY_BUFFER, ourMesh.id_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ourMesh.num_normals * 3, ourMesh.normals, GL_STATIC_DRAW);


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

	/*glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, ourMesh.id_normals);
	glNormalPointer(GL_FLOAT, 0, NULL);*/

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ourMesh.id_index);

	glDrawElements(GL_TRIANGLES, ourMesh.num_index, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return UPDATE_CONTINUE;
}
