#pragma once
#include "Globals.h"
#include <iostream>
#include <vector>

enum class ComponentType
{
	NONE,
	MESH,
	MATERIAL,
	TRANSFORM,
};

class GameObject;

class Component
{
public:
	Component();
	~Component();
	virtual void Enable();
	virtual bool Update(float dt);
	virtual void Disable();
public:
	ComponentType type = ComponentType::NONE;
	bool active = false;
	GameObject* owner;


};

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* ObjectOwner);
	~ComponentMesh();
	//void Enable();
	bool Update(float dt);
	//void Disable();
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

	uint id_tex;
	uint num_tex;
	float* texCoords;

	uint texbuffer;
	bool hastexture;
};
class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* ObjectOwner);
	~ComponentMaterial();
//	void Enable();
	bool Update(float dt);
//	void Disable();
public:

};

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* ObjectOwner);
	~ComponentTransform();
//	void Enable();
	bool Update(float dt);
//	void Disable();
public:

};

class GameObject 
{
public:
	GameObject();
	GameObject(const char* name);
	~GameObject();
	bool Update(float dt);
	Component* CreateComponent(ComponentType type);
	
public:
	bool active = false;
	std::string Name;
	std::vector<Component*> Components;

};

