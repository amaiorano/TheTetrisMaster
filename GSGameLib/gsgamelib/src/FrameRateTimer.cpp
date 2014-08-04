#include "FrameRateTimer.h"
#include "HighResTimer.h"

// Helpers for this CPP only
double GetTickCountMS()
{
	static HighResTimer sTimer;
	return sTimer.GetSysElapsedMS();
}

// Globals for this CPP only
HighResTimer m_gHighResTimerForSync;


// Implementation of class FrameRateTimer
FrameRateTimer::FrameRateTimer() 
: m_fDesiredMSPerFrame(0.0f), m_bEnableSyncFPS(false),
  m_iFPS(0), m_dblLastTimeMS(0.0f), m_iFPSCounter(0), m_dblFrameIntervalMS(0.0f)
{
}

void FrameRateTimer::EnableSyncFPS(bool bEnable/*=true*/, int iDesiredFPS/*=60*/)
{
	m_fDesiredMSPerFrame = 1000.0f / (float)iDesiredFPS;
	m_bEnableSyncFPS = bEnable;
}


void FrameRateTimer::FrameBegin()
{
	if ( m_bEnableSyncFPS )
		m_gHighResTimerForSync.StartTimer();
}
	

void FrameRateTimer::FrameEnd()
{ 
	if ( m_bEnableSyncFPS )
		m_gHighResTimerForSync.WaitUntilTimerIntervalMS(m_fDesiredMSPerFrame);

	CalculateFPS();
}

void FrameRateTimer::CalculateFPS()
{
	// Increment frame counter
	++m_iFPSCounter;

	double fCurrTimeMS = GetTickCountMS();

	// Store time between last frame and current frame
	static double fFrameTimeMS = 0.0f;
	m_dblFrameIntervalMS = fCurrTimeMS - fFrameTimeMS;
	fFrameTimeMS = fCurrTimeMS;

	// Now we want to subtract the current time by the last time that was
	// stored. If it is greater than 1 that means a second has passed and 
	// we need to display the new frame rate.  Of course, the first time
	// will always be greater than 1 because lastTime = 0.  The first second
	// will NOT be true, but the remaining ones will.  The 1.0 represents 1 
	// second.  Let's say we got 12031 (12.031) from GetTickCount for the 
	// currentTime, and the lastTime had 11230 (11.230).  Well, 
	// 12.031 - 11.230 = 0.801, which is NOT a full second.  So we try again
	// the next frame.  Once the currentTime - lastTime comes out to be greater
	// than a second (> 1), we calculate the frames for this last second.

	if ( (fCurrTimeMS - m_dblLastTimeMS) > 1000.0f ) // Has 1 second passed?
	{
		// Here we set the lastTime to the currentTime.  This will be used as the 
		// starting point for the next second. This is because GetTickCount() counts
		// up, so we need to create a delta that subtract the current time from.
		m_dblLastTimeMS = fCurrTimeMS;

		// Store updated frames per second
		m_iFPS = m_iFPSCounter;

		// Reset counter
		m_iFPSCounter = 0;
	}
}
