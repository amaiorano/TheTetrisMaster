#ifndef _FRAME_RATE_TIMER_H_
#define _FRAME_RATE_TIMER_H_

#define DEF_FPS	60
#define MS_PER_FRAME	(1.0f/(DEF_FPS*0.001f))
#define PER_FRAME_TO_PER_MS(x) (x/MS_PER_FRAME)

class FrameRateTimer
{
public:
	FrameRateTimer();

	// Enables/disables frame synchronization to a desired FPS
	void EnableSyncFPS(bool bEnable=true, int iDesiredFPS=60);

	// Call at start and end of frame code
	void FrameBegin();
	void FrameEnd();
	
	// NOTE: Functions below must not be called between FrameBegin()
	// and FrameEnd() (usually call immediately after FrameEnd())

	// Retrieves frames per second
	int GetFPS() const { return m_iFPS; }

	// Retrieves elasped time (in milliseconds) between
	// current and last frame (delta T)
	float GetFrameIntervalMS() { return static_cast<float>(m_dblFrameIntervalMS); }

private:
	void CalculateFPS();		// Calculates FPS

private:
	// Frame-synchronization variables
	float m_fDesiredMSPerFrame;
	bool m_bEnableSyncFPS;

	// FPS variables
	int m_iFPS;					// Current frames per second
	double m_dblLastTimeMS;		// Holds time from last frame (in milliseconds)
	int m_iFPSCounter;			// Used to count frames	

	double m_dblFrameIntervalMS;	// Time interval between frames
};

#endif // _FRAME_RATE_TIMER_H_
