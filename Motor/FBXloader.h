#pragma once
#include "Module.h"
#include "Globals.h"



class FBXloader : public Module
{
public:
	FBXloader(Application* app, bool start_enabled = true);
	~FBXloader();

	bool Start();
	update_status PostUpdate(float dt);
	bool CleanUp();

private:

public:

};

