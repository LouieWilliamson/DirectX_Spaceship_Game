#include "Timer.h"

Timer::Timer()
	: mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0),
	mPausedTime(0), mPrevTime(0), mStopTime(0), mCurrTime(0), mStopped(false)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)& countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;
}

float Timer::TotalTime() const
{
	//If we are stopped, do not count the time that has passed since stopping. Also, if there has been a pause, subtract paused time from stop time

	//								previous paused time
	//									|<---------->|
	// ---*------------*----------------*------------*--------------*--------------*--------------*------------>time
	//mBaseTime													mStopTime						mCurrTime

	if (mStopped)
	{
		return (float)(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}

	//mCurrTime - mBaseTime includes the paused time. to remove it subtract paused time from current time
	//(mCurrTime - mPausedTime) - mBaseTime

	//									|<--paused time-->|
	// ---*------------*----------------*-----------------*--------------*--------------*--------------*------------>time
	//mBaseTime						StopTime			StartTime											mCurrTime
	else
	{
		return (float)(((mCurrTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
}

float Timer::DeltaTime() const
{
	return (float)mDeltaTime;
}

void Timer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)& currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped = false;
}

void Timer::Start()
{

	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)& startTime);

	//increment time between stop and start

	//									|<-----elapsed----->|
	//---------------*------------------*-------------------*-------------------->time
	//								Stop time			Start time

	//if continuing from stopped state
	if (mStopped)
	{
		//increment paused time
		mPausedTime += (startTime - mStopTime);

		//reset previous time to the current time
		mPrevTime = startTime;

		//change state back to not stopped
		mStopTime = 0;
		mStopped = false;
	}
}

void Timer::Stop()
{

	//only execute if not yet stopped
	if (!mStopped)
	{
		__int64	currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)& currTime);

		mStopTime = currTime;
		mStopped = true;
	}

}

void Timer::Tick()
{

	if (mStopped)
	{
		mDeltaTime = 0.0f;
		return;
	}

	//current frame time
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)& currTime);
	mCurrTime = currTime;
	//time diff between last and this frame
	mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;

	//next frame ready
	mPrevTime = mCurrTime;

	//prevent negative values
	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}

}

