#pragma once

#include<Windows.h>

class Timer
{
public:
	Timer();

	float	TotalTime()const;	//in secs
	float	DeltaTime()const;	//in secs

	void	Reset();			//called before message loop
	void	Start();			//called when un-paused
	void	Stop();				//called when paused
	void	Tick();				//called every frame

private:
	double	mSecondsPerCount;
	double	mDeltaTime;

	__int64	mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;
};


