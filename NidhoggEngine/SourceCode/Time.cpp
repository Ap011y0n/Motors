#include "Time.h"

bool Time::Engine_Active = false;
bool Time::Game_Paused = false;
Timer Time::Game_Timer;
float Time::delta_time;
float Time::time_Since_Game_Start;

void Time::Start() 
{
	time_Since_Game_Start = 0;
	Engine_Active = true;
	Game_Timer.Start();
}

void Time::PreUpdate(float dt)
{
	delta_time = dt;

	if (Engine_Active == true) 
	{
		time_Since_Game_Start = Game_Timer.ReadSec();
	}

}

void Time::Update(float dt)
{
	
}

void Time::Play()
{
	Engine_Active = true;
}

void Time::Stop()
{
	Engine_Active = false;
	Game_Timer.Stop();
}

void Time::Pause()
{
	Game_Paused = true;
	Game_Timer.Stop();
}

void Time::Resume()
{
	Game_Paused = false;
	Game_Timer.Continue();
}