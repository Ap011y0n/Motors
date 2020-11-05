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
	serialization_example();
	int num = get_Number("Assets/library/TEST.json", "age");
	LOG("%d", num);

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