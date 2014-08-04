#include "HighResTimer.h"
#include <windows.h> // TODO: Remove! HighResTimer should use IWindowPort functions

HighResTimer::HighResTimer()
{
	// First store the timer frequency in counts/millsecond
	LARGE_INTEGER timerFrequency;
	QueryPerformanceFrequency(&timerFrequency);
	m_dblTicksPerMS = static_cast<double>(timerFrequency.QuadPart) * 0.001f;
}

HighResTimer::~HighResTimer()
{
}

double HighResTimer::GetSysElapsedMS() const
{
	static LARGE_INTEGER li64Count;
	QueryPerformanceCounter(&li64Count);
	return static_cast<double>(li64Count.QuadPart) / m_dblTicksPerMS;
}

void HighResTimer::StartTimer()
{
	m_dblStartMS = GetSysElapsedMS();
}

void HighResTimer::StopTimer()
{
	m_dblStopMS = GetSysElapsedMS();
}

double HighResTimer::GetElapsedTimeMS() const
{
	// Cast away constness on this to call StopTimer()
	const_cast<HighResTimer*>(this)->StopTimer();
	return GetTimerIntervalMS();
}

void HighResTimer::WaitUntilTimerIntervalMS(double fIntervalMS) const
{
	do
	{
		// Cast away constness on this to call StopTimer()
		const_cast<HighResTimer*>(this)->StopTimer();

	} while (fIntervalMS - GetTimerIntervalMS() > 0);
}

