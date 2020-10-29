#pragma once
#include "Globals.h"
#include "glmath.h"

#include "MathGeoLib/include/MathGeoLib.h"
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

//Base component class, need to use childs to actually do something
class Component
{
public:
	Component();
	~Component();
	virtual void Enable();
	virtual bool Update(float dt);
	virtual void Disable(); 
public:
	ComponentType type = ComponentType::NONE; 	//Type of component

	bool active = false;
	GameObject* owner; 	//Gameobject which is parent to this component



};

//Mesh, child to component
class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* ObjectOwner);
	~ComponentMesh();
	//void Enable();
	bool Update(float dt);
	void DisplayNormals();
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

	uint id_tex; //textures coords in VRAM
	uint num_tex;
	float* texCoords;

	bool triggerNormals;

};

//Material, child to component
class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* ObjectOwner);
	~ComponentMaterial();
//	void Enable();
	bool Update(float dt);
//	void Disable();
public:
	std::string texture_path; 	//path to this texture image
	uint texbuffer;				//texture loaded in VRAM
	bool hastexture;
};

//Transform, child to component
class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* ObjectOwner);
	~ComponentTransform();
//	void Enable();
	bool Update(float dt);
//	void Disable();

	void			SetPos(float x, float y, float z);	//Call this method to change position of transform component
	void			Translate(float x, float y, float z);	//Call this method to add position to transform component
	void			SetRotation(float x, float y, float z);	//Call this method to change rotation of transform component
	void			Scale(float x, float y, float z);	//Call this method to change scale of transform component
	void			UpdateRotation(float angle, const vec3& u);

private:
	void			UpdatePos(float x, float y, float z);
	void			UpdateScale(float x, float y, float z);

public:
	float3 pos;
	float3 scale;
	float3 rot;
	mat4x4 transform;

};

class GameObject 
{
public:
	GameObject();
	GameObject(const char* name, GameObject* father = nullptr);
	~GameObject();
	bool Update(float dt);
	Component* CreateComponent(ComponentType type); //Create a new component for this game object, needs a Component type
public:
	bool active = false;
	std::string Name;
	std::vector<Component*> Components;
	std::vector<GameObject*> childs;
	GameObject* father;
	bool isSelected = false;

};

