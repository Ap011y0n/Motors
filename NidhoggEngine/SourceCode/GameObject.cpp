#include "GameObject.h"
#include "Primitive.h"
#include "Application.h"

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#include "MathGeoLib/include/MathGeoLib.h"

//*************************		GameObject
GameObject::GameObject()
{
	active = true;
	Name = "NewGameObject";
	father = nullptr;
	isSelected = false;
}

GameObject::GameObject(const char* name, GameObject* node)
{
	active = true;
	Name = name;
	father = node;
	isSelected = false;
	if (father != nullptr)
	{
		father->childs.push_back(this);
	}
}

GameObject::~GameObject()
{}

bool GameObject::Update(float dt)
{
	bool ret = true;

	for (int i = 0; i < Components.size(); i++)
	{
		Components[i]->Update(dt);
	}

	return ret;
}

Component* GameObject::CreateComponent(ComponentType type)
{
	Component* newComponent = nullptr;
	switch (type)
	{
	case ComponentType::NONE: break;
	case ComponentType::MESH: { ComponentMesh* mesh = new ComponentMesh(this); newComponent = mesh; break; }
	case ComponentType::MATERIAL: { ComponentMaterial* material = new ComponentMaterial(this); newComponent = material; break; }
	case ComponentType::TRANSFORM: { ComponentTransform* transform = new ComponentTransform(this); newComponent = transform; break; }

	}
	Components.push_back(newComponent);

	return newComponent;
}

//*************************		Component
Component::Component()
{
	owner = nullptr;
	active = false;
	type = ComponentType::NONE;
}

Component::~Component()
{
}

void Component::Enable()
{
	if (!active)
		active = true;
}

bool Component::Update(float dt)
{
	bool ret = true;
	return ret;
}

void Component::Disable()
{
	if (active)
		active = false;
}

//*************************		ComponentMesh

ComponentMesh::ComponentMesh(GameObject* ObjectOwner) : Component()
{
	id_index = 0;
	num_index = 0;
	index = nullptr;

	id_normals = 0;
	num_normals = 0;
	normals = nullptr;

	id_vertex = 0;
	num_vertex = 0;
	vertex = nullptr;

	id_tex = 0;
	num_tex = 0;
	texCoords = nullptr;

	type = ComponentType::MESH;
	active = true;
	owner = ObjectOwner;

	triggerNormals = false;
}

ComponentMesh::~ComponentMesh()
{
}

bool ComponentMesh::Update(float dt)
{
	bool ret = true;
	ComponentMaterial* material = nullptr; 
	ComponentTransform* transform = nullptr;

	for (int i = 0; i < owner->Components.size(); i++)
	{
		if (owner->Components[i]->type == ComponentType::MATERIAL)
		{
			material = (ComponentMaterial*)owner->Components[i];
		}
		if (owner->Components[i]->type == ComponentType::TRANSFORM)
		{
			transform = (ComponentTransform*)owner->Components[i];
		}
	}

		glPushMatrix();
		if (transform != nullptr)
		{
			glMultMatrixf(transform->transform.ptr());
		}
		else
		{
			mat4x4 mat;
			glMultMatrixf(mat.M);
		}
	
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, id_normals);
		glNormalPointer(GL_FLOAT, 0, NULL);


		glBindBuffer(GL_ARRAY_BUFFER, id_tex);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	
				if (material != nullptr && material->hastexture)
				{
					glEnable(GL_TEXTURE_2D);
					glEnable(GL_CULL_FACE);

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, material->texbuffer);
				}
			
		
		

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);

		glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisable(GL_TEXTURE_2D);

		glPopMatrix();


	return ret;
}

void ComponentMesh::DisplayNormals()
{
	for (int i = 0; i < num_vertex; i++)
	{
		vec3 origin(vertex[i * 3], vertex[i * 3 + 1], vertex[i * 3 + 2]);
		vec3 destination(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]);
		destination *= 1;
		destination += origin;

		App->PrimManager->CreateLine(origin, destination);
	}
	LOG("DisplayNormals");
}
//*************************		ComponentMaterial

ComponentMaterial::ComponentMaterial(GameObject* ObjectOwner) : Component()
{
	type = ComponentType::MATERIAL;
	active = true;
	owner = ObjectOwner;
	texbuffer = 0;
	hastexture = false;
}

ComponentMaterial::~ComponentMaterial()
{
}

bool ComponentMaterial::Update(float dt)
{
	bool ret = true;

	return ret;
}


//*************************		ComponentTransform

ComponentTransform::ComponentTransform(GameObject* ObjectOwner) : Component()
{
	type = ComponentType::TRANSFORM;
	active = true;
	owner = ObjectOwner;

	pos.Set(0, 0, 0);
	scale.Set(0, 0, 0);
	rot.Set(0, 0, 0, 0);
	transform = transform.identity;
}

ComponentTransform::~ComponentTransform()
{
}

bool ComponentTransform::Update(float dt)
{
	bool ret = true;
	//UpdatePos(pos.x, pos.y, pos.z);
	//vec3 axis(1, 0, 0);

	transform = float4x4::FromTRS(pos, rot, scale);

	//UpdateScale(scale.x, scale.y, scale.z);
	return ret;
}
//void ComponentTransform::UpdatePos(float x, float y, float z)
//{
//	transform.Translate(x, y, z);
//
//}

void ComponentTransform::UpdateRotation(Quat quat)
{
	transform = transform * quat;
}

//void ComponentTransform::UpdateScale(float x, float y, float z)
//{
//	transform.Scale(x, y, z);
//}


void ComponentTransform::SetPos(float x, float y, float z)
{
	pos.Set(pos.x + x, pos.y + y, pos.z + z);
	for (int i = 0; i < owner->childs.size(); i++)
	{
		for (int j = 0; j < owner->childs[i]->Components.size(); j++)
		{
			if (owner->childs[i]->Components[j]->type == ComponentType::TRANSFORM)
			{
				ComponentTransform* transform = (ComponentTransform*)owner->childs[i]->Components[j];
				transform->SetPos(x, y, z);
			}
		}
	}
}

void ComponentTransform::SetRotation(float x, float y, float z)
{
	//rot.Set(x, y, z);
}

void ComponentTransform::Scale(float x, float y, float z)
{
	scale.Set(x, y, z);
	for (int i = 0; i < owner->childs.size(); i++)
	{
		for (int j = 0; j < owner->childs[i]->Components.size(); j++)
		{
			if (owner->childs[i]->Components[j]->type == ComponentType::TRANSFORM)
			{
				ComponentTransform* transform = (ComponentTransform*)owner->childs[i]->Components[j];
				transform->Scale(x, y, z);
			}
		}
	}
}

