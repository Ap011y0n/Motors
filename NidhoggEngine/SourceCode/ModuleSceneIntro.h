#pragma once
#include "Module.h"
#include "Globals.h"


class GameObject;
class ComponentCamera;

typedef unsigned char GLubyte;
typedef unsigned int GLuint;

#define checkImageWidth 64
#define checkImageHeight 64

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	GameObject* CreateGameObject(const char* name, GameObject* father = nullptr);
	void firstCube();
	void secondCube();
	void UpdateGameObject(GameObject* father, float dt);
	void SetDelete(GameObject* father);
	bool DeleteGameObject(GameObject* father);

public:

	uint my_indices = 0;
	uint my_vertex = 0;
	uint my_indices2 = 0;
	uint my_vertex2 = 0;
	int num_vertices;
	int num_indices;
	int num_vertices2;
	int num_indices2;
	float vert[24];
	uint index[36];
	float vert2[24];
	uint index2[36];

	//vector<GameObject*> gameObjects;
	GameObject* scene;
	GameObject* selectedObj;

	ComponentCamera* culling;
	GLubyte checkImage[checkImageHeight][checkImageWidth][4];
	uint texName;

};
