#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "Module.h"





enum class FileType
{
	UNKNOWN,
	FBX,
	IMAGE
};
class FileSystem : public Module
{
public:

	FileSystem(Application* app, bool start_enabled = true);
	~FileSystem();

	void SplitFilePath(const char* full_path, std::string* file, std::string* extension) const;
	FileType SetFileType(std::string file);
	const char* GetWriteDir();
	// Called before render is available
	bool Init();

	// Called before quitting
	bool CleanUp();

	// Return the bytes of a PhysFS filehandle
	uint Load(const char* path, char** buffer) const;
	bool CheckFile(const char* path) const;

	unsigned int Save(const char* file, const char* buffer, unsigned int size, bool append) const;
};

#endif // __ASSETSMANAGER_H__


