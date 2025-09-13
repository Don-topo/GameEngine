#include "Timer.h"

void Timer::Start()
{
	if (!alreadyStarted)
	{
		alreadyStarted = true;
		tim = steady_clock::now();
		return;
	}
	DEV_LOG(TE_WARNING, "Timer", "Warning the timer already started!");
}

void Timer::Stop()
{
	if (!alreadyStarted)
	{
		DEV_LOG(TE_WARNING, "Timer", "Warning the timer is not started!");
		return;
	}
	alreadyStarted = false;
	// TODO Get value en milis	
}