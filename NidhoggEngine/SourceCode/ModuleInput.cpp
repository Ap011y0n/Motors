#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "Glew/include/glew.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "FBXloader.h"
#include "FileSystem.h"

#include <string.h>

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	//glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, App->renderer3D->frameBuffer); 
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;

	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);

		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			
				/*if(e.window.event == SDL_WINDOWEVENT_RESIZED)

				//App->window->windowSize = Vec2(e.window.data1, e.window.data2);
				
				//App->UI->OnResize(e.window.data1, e.window.data2);
				break;
					App->renderer3D->OnResize(e.window.data1, e.window.data2);*/
				break;
			
			case SDL_DROPFILE: 
			     // In case if dropped file
				std::string file_path = e.drop.file;
				// Shows directory of dropped file
				LOG("%s", file_path.c_str());
				char* buffer = nullptr;
				uint fileSize = 0;
				std::string fileStr, extensionStr;
				App->file_system->SplitFilePath(file_path.c_str(), &fileStr, &extensionStr);
				std::string relativePath = "";
				FileType type = App->file_system->SetFileType(extensionStr);
				uint UID;
				switch (type)
				{
				case FileType::UNKNOWN:
					break;
				case FileType::FBX:
					relativePath.append("Assets").append("/").append(fileStr).append(extensionStr);
					UID = App->ResManager->FindInAssets(relativePath.c_str());
					if (UID == 0)
					{
						UID = App->ResManager->ImportFile(relativePath.c_str());
					}
					if (UID != 0)
					{
						Resource* NewResource = App->ResManager->RequestResource(UID);
						if (NewResource != nullptr)
						{
							LOG("Resource Found");
							App->serializer->LoadModel(NewResource);
						}
					}
					break;
				case  FileType::IMAGE:
					relativePath.append("Assets").append("/").append(fileStr).append(extensionStr);
					App->FBX->ChangeTexture(relativePath.c_str());
					break;
				case  FileType::MESH:
					relativePath.append("Assets/library/").append(fileStr).append(extensionStr);

					App->FBX->ChangeMesh(relativePath.c_str());
					break;
				}
				

				break;
			
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}