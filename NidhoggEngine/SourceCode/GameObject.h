#pragma once
#include "Globals.h"
#include "glmath.h"
#include "Primitive.h"

#include "MathGeoLib/include/MathGeoLib.h"
#include <iostream>
#include <vector>


enum class ComponentType
{
	NONE,
	MESH,
	MATERIAL,
	TRANSFORM,
	CAMERA,
};


class GameObject;

//Base component class, need to use childs to actually do something
class Component
{
public:
	Component();
	virtual ~Component();
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
	void HideNormals();
	void SetAABB();
	AABB GetAABB();
	//void Disable();
private:
	PrimNormals* GraphicNormals;
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

	AABB bbox;
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
	bool checkers;
	int texture_w;
	int texture_h;
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

	void			SetPos(float x, float y, float z);	//Call this method to add position to transform component
	void			SetRotation(Quat quat);	//Call this method to change rotation of transform component
	void			Scale(float x, float y, float z);	//Call this method to change scale of transform component
	void			UpdateRotation(Quat quat);
	float4x4			AcumulateparentTransform();

private:
//	void			UpdatePos(float x, float y, float z);
//	void			UpdateScale(float x, float y, float z);

public:
	float3 pos;
	float3 scale;
	Quat rot;
	float4x4 transform;
	bool should_update;

};

class ComponentCamera : public Component {

public:
	ComponentCamera(GameObject* ObjectOwner);
	~ComponentCamera();
	bool Update(float dt);
	void PrintFrustrum();
	void CreateFrustrum(float3*corners);
	void updateFrustrum();
public:

	Frustum frustrum;
	Plane planes[6];

};

class GameObject 
{
public:
	GameObject();
	GameObject(const char* name, GameObject* father = nullptr);
	~GameObject();
	bool Update(float dt);
	Component* CreateComponent(ComponentType type); //Create a new component for this game object, needs a Component type
	Component* GetComponent(ComponentType type); //Create a new component for this game object, needs a Component type
	void DisplayAABB();
	void HideAABB();
public:
	bool to_delete = false;
	bool active = false;
	std::string Name;
	std::vector<Component*> Components;
	std::vector<GameObject*> childs;
	GameObject* parent;
	bool isSelected = false;
	int UID;
	int parentUID;
	OBB obb;
	AABB aabb;
	bool displayAABB;
private:
	PrimAABB* currentAABB;
};


