#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "External\SDL\include\SDL.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();
	void Resume();
	void Pause();
	void Restart();

	float ReadSec() const;

	Uint32 Read();


	bool	running;
	Uint32	started_at;
	Uint32	stopped_at;
	Uint32	paused_at;
	Uint32	resumed_at;
	Uint32	time;

private:

};

#endif //__TIMER_H__