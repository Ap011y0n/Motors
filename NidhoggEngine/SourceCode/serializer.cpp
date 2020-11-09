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
	JSON_Object* obj = AddObjectToArray(leaves);
	AddFloat(obj, "UID", 1642009359);
	AddFloat(obj, "ParentUID", 1619219037);
	AddString(obj, "Name", "RootNode");
	JSON_Array* array = AddArray(obj, "Components");

	AddComponent(array, ComponentType::MESH, "Library/Meshes/mesh_0.mesh");
	AddComponent(array, ComponentType::MATERIAL, "Library/Textures/texture_0.dds");
	SaveScene();

	JSON_Object* obj2 = AddObjectToArray(leaves);
	AddFloat(obj2, "UID", 1642009359);
	AddFloat(obj2, "ParentUID", 1619219037);
	AddString(obj2, "Name", "RootNode");
	JSON_Array* array2 = AddArray(obj2, "Components");

	AddComponent(array2, ComponentType::MESH, "Library/Meshes/mesh_0.mesh");
	AddComponent(array2, ComponentType::MATERIAL, "Library/Textures/texture_0.dds");
	SaveScene();

//	arrayExample();
	get_Array("Assets/library/TEST.json");
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

void Serializer::SaveScene()
{
	char* serialized_string = NULL;
	serialized_string = json_serialize_to_string_pretty(root_value);
	size_t size = sprintf(serialized_string, "%s", serialized_string);
	App->file_system->Save("TEST.json", serialized_string, size, false);
	json_free_serialized_string(serialized_string);
}

void Serializer::AddFloat(JSON_Object* obj, const char* name, float value)
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
			AddString(leaf_object, "Type", "transform");
			AddString(leaf_object, "Path", path);

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
	App->file_system->Save("TEST.json", serialized_string, size, false);
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

	App->file_system->Save("TEST.json", serialized_string, size, false);
	json_free_serialized_string(serialized_string);
	json_value_free(root_value);
}

void Serializer::arrayExample2()
{

}