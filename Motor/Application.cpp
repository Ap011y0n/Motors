#include "Application.h"
#include "Timer.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	UI = new ModuleUI(this);
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	
	// Scenes
	AddModule(scene_intro);
	AddModule(UI);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	// release modules
	list<Module*>::reverse_iterator item;
	item = list_modules.rbegin();

	while (item != list_modules.rend())
	{
		delete* item;
		item++;
	}
	list_modules.clear();

}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules

	list<Module*>::iterator item_list;
	Module* it;

	for (item_list = list_modules.begin(); item_list != list_modules.end() && ret == true; ++item_list) {

			it = *item_list;
			ret = it->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (item_list = list_modules.begin(); item_list != list_modules.end() && ret == true; ++item_list) {

			it = *item_list;
			ret = it->Start();
	}

	last_sec_frame_time.Start();
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	ms_timer.ReadSec();
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		last_sec_frame_count = frame_count;
		frame_count = 0;
	}

	uint32_t last_frame_ms = ms_timer.Read();
	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		SDL_Delay(capped_ms - last_frame_ms);
	}
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	list<Module*>::iterator item_list;
	Module* it;

	for (item_list = list_modules.begin(); item_list != list_modules.end() && ret == UPDATE_CONTINUE; ++item_list) {

			it = *item_list;
			ret = it->PreUpdate(dt);
	}

	for (item_list = list_modules.begin(); item_list != list_modules.end() && ret == UPDATE_CONTINUE; ++item_list) {

			it = *item_list;
			ret = it->Update(dt);
	}

	for (item_list = list_modules.begin(); item_list != list_modules.end() && ret == UPDATE_CONTINUE; ++item_list) {

			it = *item_list;
			ret = it->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	list<Module*>::reverse_iterator item;
	item = list_modules.rbegin();

	
	while (item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		item++;
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}
int Application::GetFPS()
{
	return last_sec_frame_count;
}

void Application::SetMaxFPS(int max_fps)
{
	fps = max_fps;
	if (fps == 0) fps = -1;
	capped_ms = 1000 / fps;
}
