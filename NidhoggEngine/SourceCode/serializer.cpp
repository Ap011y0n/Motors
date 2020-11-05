#include "Globals.h"
#include "Application.h"
#include "serializer.h"
#include "parson/parson.h"





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