#include "Globals.h"
#include "Application.h"
#include "serializer.h"
#include "parson/parson.h"
#include "FileSystem.h"
#include "GameObject.h"




Serializer::Serializer(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}
Serializer::~Serializer()
{

}

bool Serializer::Start()
{

	bool ret = true;
	LOG("starting serializer");
	//serialization_example();
	//	int num = get_Number("Assets/library/TEST.json", "age");
	CreateNewScene();


//	arrayExample();
	//get_Array("Assets/library/TEST.json");
//	LOG("%d", num);

	return ret;
}

update_status Serializer::PostUpdate(float dt) 
{
	return UPDATE_CONTINUE;

}
bool Serializer::CleanUp() 
{
	return true;
}

double Serializer::get_Number(const char* file, const char* name)
{
	JSON_Value* root_value;
	JSON_Object* object;
	root_value = json_parse_file(file);
	object = json_value_get_object(root_value);
	
	double number = 0;

	if (json_object_has_value_of_type(object, name, JSONNumber))
		number = json_object_get_number(object, name);

	json_value_free(root_value);
	return number;
}
const char* Serializer::get_String(const char* file, const char* name)
{
	JSON_Value* root_value;
	JSON_Object* object;
	root_value = json_parse_file(file);
	object = json_value_get_object(root_value);

	std::string newstring;

	if (json_object_has_value_of_type(object, name, JSONString))
		newstring = json_object_get_string(object, name);

	json_value_free(root_value);
	return newstring.c_str();
}
void Serializer::get_Array(const char* file)
{
	JSON_Value* root_value;
	JSON_Object* main_object;
	JSON_Array* main_array;

	root_value = json_parse_file(file);

	main_object = json_value_get_object(root_value);
	main_array = json_object_get_array(main_object, "Game Objects");


	for (int i = 0; i < json_array_get_count(main_array); i++) {
		JSON_Object* obj_in_array = json_array_get_object(main_array, i);
		JSON_Array* array_in_obj = json_object_get_array(obj_in_array, "Components");
	
		LOG("%f", json_object_get_number(obj_in_array, "UID"));
		LOG("%f", json_object_get_number(obj_in_array, "ParentUID"));
		LOG("%s", json_object_get_string(obj_in_array, "Name"));

		for (int j = 0; j < json_array_get_count(array_in_obj); j++)
		{
			JSON_Object* obj_in_array_in_obj = json_array_get_object(array_in_obj, j);
			LOG("%s", json_object_get_string(obj_in_array_in_obj, "Type"));
			LOG("%s", json_object_get_string(obj_in_array_in_obj, "Path"));

		}
	}

}


void Serializer::CreateNewScene()
{
	//creating a Json_Array
	 root_value = json_value_init_object();
	 root_object = json_value_get_object(root_value);
	 branch = json_value_init_array();
	 leaves = json_value_get_array(branch);

	 json_object_set_value(root_object, "Game Objects", branch);

}
JSON_Object* Serializer::AddObjectToArray(JSON_Array* obj)
{
	JSON_Value* ObjectValue = json_value_init_object();
	JSON_Object* Object = json_value_get_object(ObjectValue);
	
	json_array_append_value(obj, ObjectValue);
	return Object;

}

void Serializer::AddVec3(JSON_Array* obj, float x, float y, float z)
{
	json_array_append_number(obj, x);
	json_array_append_number(obj, y);
	json_array_append_number(obj, z);
}
void Serializer::AddVec4(JSON_Array* obj, float x, float y, float z, float w)
{
	json_array_append_number(obj, x);
	json_array_append_number(obj, y);
	json_array_append_number(obj, z);
	json_array_append_number(obj, w);

}

void Serializer::SaveScene()
{
	char* serialized_string = NULL;
	serialized_string = json_serialize_to_string_pretty(root_value);
	size_t size = sprintf(serialized_string, "%s", serialized_string);

	App->file_system->Save("library/TEST.json", serialized_string, size, false);
	json_free_serialized_string(serialized_string);
}

void Serializer::SaveValueAsFile(JSON_Value* root, const char* name, std::string directory)
{
	char* serialized_string = NULL;
	serialized_string = json_serialize_to_string_pretty(root);
	size_t size = sprintf(serialized_string, "%s", serialized_string);
	directory = directory + name;
	App->file_system->Save(directory.c_str(), serialized_string, size, false);
	json_free_serialized_string(serialized_string);
}

void Serializer::LoadScene(const char* path)
{
	JSON_Value* root_value;
	JSON_Object* main_object;
	JSON_Array* main_array;

	root_value = json_parse_file(path);

	main_object = json_value_get_object(root_value);
	main_array = json_object_get_array(main_object, "Game Objects");
	

	for (int i = 0; i < json_array_get_count(main_array); i++) {
		JSON_Object* obj_in_array = json_array_get_object(main_array, i);
		JSON_Array* component_array = json_object_get_array(obj_in_array, "Components");
		JSON_Array* JsonTrans = json_object_get_array(obj_in_array, "Translation");
		JSON_Array* JsonScale = json_object_get_array(obj_in_array, "Scale");
		JSON_Array* JsonRot = json_object_get_array(obj_in_array, "Rotation");
	
		int UID = json_object_get_number(obj_in_array, "UID");
		int parentUID = json_object_get_number(obj_in_array, "ParentUID");
		const char* name = json_object_get_string(obj_in_array, "Name");

		GameObject* object = App->scene_intro->CreateGameObject(name, nullptr);
		tempvector.push_back(object);
		object->UID = UID;
		object->parentUID = parentUID;


		ComponentTransform* NewTrans = (ComponentTransform*)object->CreateComponent(ComponentType::TRANSFORM);
		NewTrans->pos.x = json_array_get_number(JsonTrans, 0);
		NewTrans->pos.y = json_array_get_number(JsonTrans, 1);
		NewTrans->pos.z = json_array_get_number(JsonTrans, 2);

		NewTrans->scale.x = json_array_get_number(JsonScale, 0);
		NewTrans->scale.y = json_array_get_number(JsonScale, 1);
		NewTrans->scale.z = json_array_get_number(JsonScale, 2);

		NewTrans->rot.x = json_array_get_number(JsonRot, 0);
		NewTrans->rot.y = json_array_get_number(JsonRot, 1);
		NewTrans->rot.z = json_array_get_number(JsonRot, 2);
		NewTrans->rot.w = json_array_get_number(JsonRot, 3);

		NewTrans->transform = float4x4::FromTRS(NewTrans->pos, NewTrans->rot, NewTrans->scale);
		NewTrans->transform;

		for (int j = 0; j < json_array_get_count(component_array); j++)
		{
			JSON_Object* obj_in_array_in_obj = json_array_get_object(component_array, j);
			std::string type = json_object_get_string(obj_in_array_in_obj, "Type");
			const char* componentpath = json_object_get_string(obj_in_array_in_obj, "Path");
			char* buffer = nullptr;
			uint fileSize = 0;

			if (type == "Mesh")
			{
				ComponentMesh* NewMesh = (ComponentMesh*)object->CreateComponent(ComponentType::MESH);
				fileSize = App->file_system->Load(componentpath, &buffer);
				MeshImporter::Load(buffer, fileSize, NewMesh);

				NewMesh->id_vertex = App->FBX->FillArrayBuffer(NewMesh->num_vertex * 3, NewMesh->vertex);

				NewMesh->id_tex = App->FBX->FillArrayBuffer(NewMesh->num_tex, NewMesh->texCoords);

				NewMesh->id_normals = App->FBX->FillArrayBuffer(NewMesh->num_normals * 3, NewMesh->normals);

				NewMesh->id_index = App->FBX->FillElementArrayBuffer(NewMesh->num_index, NewMesh->index);

				NewMesh->SetAABB();
			}
			else if (type == "texture")
			{
				ComponentMaterial* NewTex = (ComponentMaterial*)object->CreateComponent(ComponentType::MATERIAL);
				fileSize = App->file_system->Load(componentpath, &buffer);
				MaterialImporter::Load(buffer, fileSize, NewTex);

				if (NewTex->texbuffer != 0)
					NewTex->hastexture = true;
			}
			

		}
	}
	sortScene();

	tempvector.clear();
}

void Serializer::sortScene() {
	for (int i = 0; i < tempvector.size(); i++)
	{
		for (int j = 0; j < tempvector.size(); j++)
		{
			if (tempvector[i]->parentUID == tempvector[j]->UID)
			{
				tempvector[i]->parent = tempvector[j];
				tempvector[j]->childs.push_back(tempvector[i]);
				
			}
				
		}
		if (tempvector[i]->parent == nullptr)
		{
			tempvector[i]->parent = App->scene_intro->scene;
			App->scene_intro->scene->childs.push_back(tempvector[i]);
		}
	}

}

void Serializer::LoadModel(Resource* model)
{
	JSON_Value* root_value;
	JSON_Object* main_object;
	JSON_Array* main_array;

	root_value = json_parse_file(model->GetLibraryFile());

	main_object = json_value_get_object(root_value);
	main_array = json_object_get_array(main_object, "Game Objects");


	for (int i = 0; i < json_array_get_count(main_array); i++) {
		JSON_Object* obj_in_array = json_array_get_object(main_array, i);
		JSON_Array* component_array = json_object_get_array(obj_in_array, "Components");
		JSON_Array* JsonTrans = json_object_get_array(obj_in_array, "Translation");
		JSON_Array* JsonScale = json_object_get_array(obj_in_array, "Scale");
		JSON_Array* JsonRot = json_object_get_array(obj_in_array, "Rotation");

		int UID = json_object_get_number(obj_in_array, "UID");
		int parentUID = json_object_get_number(obj_in_array, "ParentUID");
		const char* name = json_object_get_string(obj_in_array, "Name");

		GameObject* object = App->scene_intro->CreateGameObject(name, nullptr);
		tempvector.push_back(object);
		object->UID = UID;
		object->parentUID = parentUID;


		ComponentTransform* NewTrans = (ComponentTransform*)object->CreateComponent(ComponentType::TRANSFORM);
		NewTrans->pos.x = json_array_get_number(JsonTrans, 0);
		NewTrans->pos.y = json_array_get_number(JsonTrans, 1);
		NewTrans->pos.z = json_array_get_number(JsonTrans, 2);

		NewTrans->scale.x = json_array_get_number(JsonScale, 0);
		NewTrans->scale.y = json_array_get_number(JsonScale, 1);
		NewTrans->scale.z = json_array_get_number(JsonScale, 2);

		NewTrans->rot.x = json_array_get_number(JsonRot, 0);
		NewTrans->rot.y = json_array_get_number(JsonRot, 1);
		NewTrans->rot.z = json_array_get_number(JsonRot, 2);
		NewTrans->rot.w = json_array_get_number(JsonRot, 3);

		NewTrans->transform = float4x4::FromTRS(NewTrans->pos, NewTrans->rot, NewTrans->scale);
		NewTrans->transform;

		for (int j = 0; j < json_array_get_count(component_array); j++)
		{
			JSON_Object* obj_in_array_in_obj = json_array_get_object(component_array, j);
			std::string type = json_object_get_string(obj_in_array_in_obj, "Type");
			uint componentUID = json_object_get_number(obj_in_array_in_obj, "Resource UID");
			

			if (type == "Mesh")
			{
				ResourceMesh* NewMeshResource = (ResourceMesh*)App->ResManager->RequestResource(componentUID);
				if (NewMeshResource != nullptr)
				{
					ComponentMesh* NewMesh = (ComponentMesh*)object->CreateComponent(ComponentType::MESH);
					
					NewMesh->num_vertex = NewMeshResource->num_vertex;
					NewMesh->num_tex = NewMeshResource->num_tex;
					NewMesh->num_normals = NewMeshResource->num_normals;
					NewMesh->num_index = NewMeshResource->num_index;
					NewMesh->vertex = NewMeshResource->vertex;
					NewMesh->texCoords = NewMeshResource->texCoords;
					NewMesh->normals = NewMeshResource->normals;
					NewMesh->index = NewMeshResource->index;
					NewMesh->id_vertex = NewMeshResource->id_vertex;
					NewMesh->id_tex = NewMeshResource->id_tex;
					NewMesh->id_normals = NewMeshResource->id_normals;
					NewMesh->id_index = NewMeshResource->id_index;
					

					NewMesh->SetAABB();
				}
				else
					LOG("Error loading model resource meshes");
		
			}
			else if (type == "texture")
			{
				ResourceTexture* NewMeshResource = (ResourceTexture*)App->ResManager->RequestResource(componentUID);
				if (NewMeshResource != nullptr)
				{
					ComponentMaterial* NewTex = (ComponentMaterial*)object->CreateComponent(ComponentType::MATERIAL);
					NewTex->texbuffer = NewMeshResource->texbuffer;
					NewTex->texture_h = NewMeshResource->texture_h;
					NewTex->texture_w = NewMeshResource->texture_w;

					if (NewTex->texbuffer != 0)
						NewTex->hastexture = true;
				}
				else
				LOG("Error loading model resource texture");
				
			}


		}
	}
	sortScene();

	tempvector.clear();
}

bool Serializer::LoadMeta(const char* path, uint* uid, ResourceType* type, std::string* Assets, std::string* library)
{
	bool ret = false;
	JSON_Value* value;
	JSON_Object* object;

	value = json_parse_file(path);
	if (value == NULL)
	{
		return ret;
	}
	ret = true;
	object = json_value_get_object(value);


	if (json_object_has_value_of_type(object, "UID", JSONNumber))
		*uid = json_object_get_number(object, "UID");

	std::string Type;
	if (json_object_has_value_of_type(object, "Type", JSONString))
		Type = json_object_get_string(object, "Type");

	if (Type == "3D Model")
		*type = ResourceType::MODEL;
	else if (Type == "Texture")
		*type = ResourceType::TEXTURE;

	if (json_object_has_value_of_type(object, "Asset Path", JSONString))
		*Assets = json_object_get_string(object, "Asset Path");
	if (json_object_has_value_of_type(object, "Library path", JSONString))
		*library = json_object_get_string(object, "Library path");

	//json_value_free(root_value);
	return ret;
}
void Serializer::AddFloat(JSON_Object* obj, const char* name, double value)
{
	json_object_set_number(obj, name, value);
}
void Serializer::AddString(JSON_Object* obj, const char* name, const char* string)
{
	json_object_set_string(obj, name, string);
}

JSON_Array* Serializer::AddArray(JSON_Object* obj, const char* name)
{
	JSON_Value* components = json_value_init_array();
	JSON_Array* componentsArray = json_value_get_array(components);

	json_object_set_value(obj, name, components);
	return componentsArray;
}

void Serializer::AddComponent(JSON_Array* componentsArray, ComponentType type, const char* path)
{
	
	JSON_Value* leaf_value = json_value_init_object();
	JSON_Object* leaf_object = json_value_get_object(leaf_value);
	switch (type)
	{
		case ComponentType::NONE: 
			break;
		case ComponentType::MESH: 
		{ 
			AddString(leaf_object, "Type", "Mesh");
			AddString(leaf_object, "Path", path);
			break;
		}
		case ComponentType::MATERIAL: { 
			AddString(leaf_object, "Type", "texture");
			AddString(leaf_object, "Path", path);

			break; 
		}
		case ComponentType::TRANSFORM: { 

			break; 
		}

	}

	json_array_append_value(componentsArray, leaf_value);
	
}

void Serializer::AddResourceComponent(JSON_Array* componentsArray, ComponentType type, uint UID)
{

	JSON_Value* leaf_value = json_value_init_object();
	JSON_Object* leaf_object = json_value_get_object(leaf_value);
	switch (type)
	{
	case ComponentType::NONE:
		break;
	case ComponentType::MESH:
	{
		AddString(leaf_object, "Type", "Mesh");
		AddFloat(leaf_object, "Resource UID", UID);
		break;
	}
	case ComponentType::MATERIAL: {
		AddString(leaf_object, "Type", "texture");
		AddFloat(leaf_object, "Resource UID", UID);
		break;
	}
	case ComponentType::TRANSFORM: {

		break;
	}

	}

	json_array_append_value(componentsArray, leaf_value);

}



void Serializer::arrayExample()
{
	//creating a Json_Array
	JSON_Value* root_value = json_value_init_object();
	JSON_Object* root_object = json_value_get_object(root_value);
	{
		JSON_Value* branch = json_value_init_array();
		JSON_Array* leaves = json_value_get_array(branch);
		{
			JSON_Value* leaf_value = json_value_init_object();
			JSON_Object* leaf_object = json_value_get_object(leaf_value);

			JSON_Value* leaf_value2 = json_value_init_object();
			JSON_Object* leaf_object2 = json_value_get_object(leaf_value2);
			{
				json_object_set_number(leaf_object, "name1", 1);
				json_object_set_number(leaf_object, "name2", 2);
				json_object_set_number(leaf_object, "name3", 3);

				JSON_Value* insidearray = json_value_init_array();
				JSON_Array* insideleaves = json_value_get_array(insidearray);
				{
					JSON_Value* leaf_value3 = json_value_init_object();
					JSON_Object* leaf_object3 = json_value_get_object(leaf_value3);
					json_object_set_string(leaf_object3, "component1", "transform");
					json_object_set_string(leaf_object3, "component2", "mesh");

					JSON_Value* leaf_value4 = json_value_init_object();
					JSON_Object* leaf_object4 = json_value_get_object(leaf_value4);
					json_object_set_string(leaf_object4, "component1", "mesh1");
					json_object_set_string(leaf_object4, "component2", "texture");

					json_array_append_value(insideleaves, leaf_value3);
					json_array_append_value(insideleaves, leaf_value4);
				}

				json_object_set_number(leaf_object2, "name1", 11);
				json_object_set_number(leaf_object2, "name2", 12);
				json_object_set_number(leaf_object2, "name3", 13);
				json_object_set_value(leaf_object2, "array", insidearray);


			}




			json_array_append_value(leaves, leaf_value);

			json_array_append_value(leaves, leaf_value2);
		}

		//creating measurement Json
		json_object_set_value(root_object, "array", branch);

	}



	//size_t size = json_serialization_size(leaf_value);
	//json_serialize_to_file(leaf_value, "user_data.json");
	//char* buffer;
	//buffer = new char[size];
	//json_serialize_to_buffer(leaf_value, buffer, size);
	//App->file_system->Save("TEST.json", buffer, size, false);

	char* serialized_string = NULL;
	serialized_string = json_serialize_to_string_pretty(root_value);
	size_t size = sprintf(serialized_string, "%s", serialized_string);
	App->file_system->Save("library/TEST.json", serialized_string, size, false);
	json_free_serialized_string(serialized_string);


	//json_value_free(branch);

}

void Serializer::serialization_example()
{

	JSON_Value* root_value = json_value_init_object();
	JSON_Object* root_object = json_value_get_object(root_value);
	char* serialized_string = NULL;
	json_object_set_string(root_object, "name", "John Smith");
	json_object_set_number(root_object, "age", 25);
	json_object_dotset_string(root_object, "address.city", "Cupertino");
	json_object_dotset_value(root_object, "contact.emails", json_parse_string("[\"email@example.com\",\"email2@example.com\"]"));
	
	serialized_string = json_serialize_to_string_pretty(root_value);
	size_t size = sprintf(serialized_string, "%s", serialized_string);

	/*
	char* buffer;
	size_t size = json_serialization_size(root_value);
	buffer = new char[size];
	json_serialize_to_buffer(root_value, buffer, size);*/

	App->file_system->Save("library/TEST.json", serialized_string, size, false);
	json_free_serialized_string(serialized_string);
	json_value_free(root_value);
}

void Serializer::arrayExample2()
{

}