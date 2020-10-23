#include "Application.h"
#include "FileSystem.h"

#include "Assimp/include/cfileio.h"
#include "Assimp/include/types.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )
#include "PhysFS/include/physfs.h"

FileSystem::FileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{

	// TODO 0 (Solved): Open the following link and check it: https://icculus.org/physfs/docs/html/physfs_8h.html
	// TODO 1 (Solved): You must initialize and de-initialize the PhysFS API.

	// Initialize the PhysicsFS library
	// This must be called before any other PhysicsFS function
	// This should be called prior to any attempts to change your process's current working directory
	PHYSFS_init(nullptr);

	// We only need this when compiling in debug. In Release we don't need it.
	PHYSFS_mount(".", nullptr, 1);
}


FileSystem::~FileSystem()
{
	// Deinitialize the PhysicsFS library.
	// This closes any files opened via PhysicsFS, blanks the search/write paths, frees memory, and invalidates all of your file handles.
	// NOTE: This call can FAIL if there's a file open for writing that refuses to close
	PHYSFS_deinit();
}

bool FileSystem::Init()
{
	// Determine if the PhysicsFS library is initialized, we can check it for avoid errors.
	if (PHYSFS_isInit())
	{
		LOG("Asset Manager is succefully loaded");
	}
	else
		LOG("Failed loading Asset Manager");

	// TODO 2 (Solved): Mount the desired ZIP with PhysFS.

	// Add an archive or directory to the search path.
	// If this is a duplicate, the entry is not added again, even though the function succeeds.
	// When you mount an archive, it is added to a virtual file system...
	// all files in all of the archives are interpolated into a single hierachical file tree.
	if (PHYSFS_mount("Assets", nullptr, 1) == 0)
	{
		LOG("File System error while adding a path or zip: %s\n", PHYSFS_getLastError());
	}

	//PHYSFS_setWriteDir("data");
	return true;
}


bool FileSystem::CleanUp()
{
	return false;
}


void FileSystem::SplitFilePath(const char* full_path, std::string* file, std::string* extension) const
{
	if (full_path != nullptr)
	{
		std::string full(full_path);
		size_t pos_separator = full.find_last_of("\\/");
		size_t pos_dot = full.find_last_of(".");


		if (file != nullptr)
		{
			if (pos_separator < full.length())
				*file = full.substr(pos_separator + 1, pos_dot - pos_separator);
			else
				*file = full.substr(0, pos_dot);
		}

		if (extension != nullptr)
		{
			if (pos_dot < full.length())
				*extension = full.substr(pos_dot + 1);
			else
				extension->clear();
		}
	}
}

FileType FileSystem::SetFileType(std::string extension)
{
	FileType ret = FileType::UNKNOWN;
	transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

	if (extension == "fbx")
	{
		ret = FileType::FBX;
	}

	if (extension == "png" || extension == "dds")
	{
		ret = FileType::IMAGE;
	}

	return ret;
}

SDL_RWops* FileSystem::Load(const char* path) const
{
	char* buffer;
	uint bytes = Load(path, &buffer);


	// Read-only memory buffer for use with RWops, retruns a pointer to a new SDL_RWops structure
	SDL_RWops* ret = SDL_RWFromConstMem(buffer, bytes);

	return ret;
}


uint FileSystem::Load(const char* path, char** buffer) const
{
	uint ret = 0;

	// TODO 3 (Solved): You want to return the number of bytes it has read from the file that we passed to this function. 
	// Maybe you want to search readBytes in the documentation, and investigate from there how to build the function.

	// The reading offset is set to the first byte of the file.
	// Returns a filehandle on success that we will need for the PHYSFS_fileLength
	PHYSFS_file* file = PHYSFS_openRead(path);

	// Check for end-of-file state on a PhysicsFS filehandle.
	if (file != nullptr)
	{
		// Get total length of a file in bytes
		uint lenght = PHYSFS_fileLength(file); 
		*buffer = new char[lenght]; 

		// Read data from a PhysicsFS firehandle. Returns a number of bytes read.
		uint bytes = PHYSFS_readBytes(file, *buffer, lenght);

		if (bytes != lenght) 
		{
			LOG("%s" , path, "ERROR: %s" , PHYSFS_getLastError());
			{                              
				if (buffer != NULL)              
				{                            
					delete[] buffer;                
					buffer = NULL;                    
				}                            
					
			}
		}
		else
			ret = bytes; 
	}
	else
		LOG("ERROR: %s", PHYSFS_getLastError());


	// Close a PhysicsFS firehandle
	PHYSFS_close(file);

	return ret;
}

// Save a whole buffer to disk
unsigned int FileSystem::Save(const char* file, const char* buffer, unsigned int size) const
{
	unsigned int ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openWrite(file);

	if (fs_file != NULL)
	{
		PHYSFS_sint64 written = PHYSFS_write(fs_file, (const void*)buffer, 1, size);
		if (written != size)
		{
			LOG("File System error while writing to file %s: %s", file, PHYSFS_getLastError());

		}
		else
			ret = (uint)written;

		if (PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s", file, PHYSFS_getLastError());

	return ret;
}
