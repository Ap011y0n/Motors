#ifndef __SERIALIZER_H__
#define __SERIALIZER_H__

#include "Module.h"
#include "Globals.h"

typedef struct json_object_t JSON_Object;
typedef struct json_array_t  JSON_Array;
typedef struct json_value_t  JSON_Value;

enum class ComponentType;

class Serializer : public Module
{
public:
	Serializer(Application* app, bool start_enabled = true);

	~Serializer();

	bool Start();
	update_status PostUpdate(float dt);
	bool CleanUp();

	void CreateNewScene();
	JSON_Object* AddObjectToArray(JSON_Array* obj);
	void SaveScene();

	void AddFloat(JSON_Object* obj, const char* name, float value);
	void AddString(JSON_Object* obj, const char* name, const char* string);
	void AddComponent(JSON_Array* obj, ComponentType type, const char* path);
	JSON_Array* AddArray(JSON_Object* obj, const char* name);

	double get_Number(const char* file, const char* name);
	void get_Array(const char* file);

	void serialization_example();
	void arrayExample();
	void arrayExample2();

public:
	JSON_Value* root_value;
	JSON_Object* root_object;
	JSON_Value* branch;
	JSON_Array* leaves;
};
#endif