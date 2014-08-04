#ifndef _HIGH_RES_TIMER_H_
#define _HIGH_RES_TIMER_H_


// High-resolution timer class, can be used to measure very small
// time intervals

class HighResTimer
{
public:
	HighResTimer();
	~HighResTimer();

	// Call to retrieve total elapsed time in milliseconds since system startup
	double GetSysElapsedMS() const;

	// Call to start the timer
	void StartTimer();

	// Call to stop the timer
	void StopTimer();

	// Call to retrieve time interval (in milliseconds) between
	// StartTimer() and StopTimer() calls
	double GetTimerIntervalMS() const { return m_dblStopMS - m_dblStartMS; }

	// Call to retrieve time interval (in milliseconds) since last
	// call to StartTimer
	double GetElapsedTimeMS() const;

	// After calling StartTimer(), call this to wait
	// until the input time has elapsed
	void WaitUntilTimerIntervalMS(double fIntervalMS) const;

private:
	double m_dblTicksPerMS;
	double m_dblStartMS;
	double m_dblStopMS;
};


#endif // _HIGH_RES_TIMER_H_
