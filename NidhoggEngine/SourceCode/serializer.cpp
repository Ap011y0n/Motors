#include "Globals.h"
#include "Application.h"
#include "serializer.h"
#include "parson/parson.h"
#include "FileSystem.h"




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

	arrayExample();
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
	main_array = json_object_get_array(main_object, "array");


	for (int i = 0; i < json_array_get_count(main_array); i++) {
		JSON_Object* obj_in_array = json_array_get_object(main_array, i);
		JSON_Array* array_in_obj = json_object_get_array(obj_in_array, "array");
	
		LOG("%f", json_object_get_number(obj_in_array, "name1"));
		LOG("%f", json_object_get_number(obj_in_array, "name2"));
		LOG("%f", json_object_get_number(obj_in_array, "name3"));

		for (int j = 0; j < json_array_get_count(array_in_obj); j++)
		{
			JSON_Object* obj_in_array_in_obj = json_array_get_object(array_in_obj, j);
			LOG("%s", json_object_get_string(obj_in_array_in_obj, "component1"));
			LOG("%s", json_object_get_string(obj_in_array_in_obj, "component2"));

		}
	}

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

void Serializer::arrayExample2()
{

}