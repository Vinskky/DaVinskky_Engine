#include "GameTime.h"

float GameTime::deltaTime;

float GameTime::scaleTime = 1;
float GameTime::time;

bool GameTime::running = false;
bool GameTime::paused = false;

Timer GameTime::gameTimer;

void GameTime::Start(int maxFPS)
{
	running = true;
	gameTimer.Start();
	time = 0;
}

void GameTime::PreUpdate(float dt)
{
	deltaTime =  dt;
	// TODO: Calculate game time
	if (running)
		time = gameTimer.ReadSec();
}

void GameTime::Update()
{

}

void GameTime::Play()
{
	running = true;
}

void GameTime::Pause()
{
	paused = true;
	gameTimer.Stop();
}

void GameTime::Resume()
{
	paused = false;
	gameTimer.Resume();
}

void GameTime::Stop()
{
	running = false;
	gameTimer.Stop();
}