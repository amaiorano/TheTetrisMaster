#ifndef _GAME_CLOCK_H_
#define _GAME_CLOCK_H_

#include "Singleton.h"
#include "HighResTimer.h"

class GameApp; // Forward declaration for friendship

// Singleton game clock that maintains the current game time.
// This game time is updated once per frame by the GameApp only,
// but accessible everywhere else.
class GameClock : public Singleton<GameClock>
{
protected:
	// Protected constructor for singleton
	GameClock()
	{ 
		Update(); // Initialize game time
	}

public:
	// Returns total elapsed time in milliseconds
	double TimeMS() const		{ return m_gameTime; }

private:
	// Only GameApp is allowed to update the game clock
	friend class GameApp;
	void Update()				{ m_gameTime = m_timer.GetSysElapsedMS(); }

	// TODO: implement
	void Pause();
	void Resume();

private:
	HighResTimer m_timer;
	double m_gameTime;
};


// Helper class for objects that require synchronization against
// the GameClock. It can save the game time in one frame and compare
// it against the current game time in following frames.
class GameClockClient
{
public:
	GameClockClient() { Reset(); }

	// Saves current game time internally so that DeltaTimeMS() returns 0
	void Reset() { m_dblLastTimeMS = GameClock::Instance().TimeMS(); }

	// Returns the difference between current game time and saved time
	// (amount of time elapsed since last call to Reset())
	double DeltaTimeMS() const { return GameClock::Instance().TimeMS() - m_dblLastTimeMS; }

	// Returns true if input time has elapsed since last Reset() call
	bool HasElapsedMS(double dblTimeMS) const { return DeltaTimeMS() >= dblTimeMS; }

private:
	double m_dblLastTimeMS;
};

#endif // _GAME_CLOCK_H_
