#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "Module.h"

#include "PhysFS/include/physfs.h"
#include "SDL/include/SDL.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

class FileSystem : public Module
{
public:

	FileSystem(Application* app, bool start_enabled = true);
	~FileSystem();

	std::string NormalizePath(const char* path) const;
	void FileSystem::SplitFilePath(const char* full_path, std::string* path, std::string* file, std::string* extension) const;


	// Called before render is available
	bool Init();

	// Called before quitting
	bool CleanUp();

	// Return the bytes of a PhysFS filehandle
	uint Load(const char* path, char** buffer) const;

	// Allows you to use pointers to memory instead of files or things such as images or samples
	SDL_RWops* Load(const char* path) const;

	unsigned int Save(const char* file, const char* buffer, unsigned int size) const;
};

#endif // __ASSETSMANAGER_H__
