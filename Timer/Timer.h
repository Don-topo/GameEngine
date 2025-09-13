#pragma once

#include <chrono>
#include "../Logger/Logger.h"

using namespace std::chrono;

class Timer
{
public:
	void Start();
	void Stop();

private:
	time_point<steady_clock> tim = {};
	bool alreadyStarted = false;
};

