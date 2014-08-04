#include "KeyboardMgr.h"

bool KeyState::Down(float fMinTimeMS, float fDeltaTimeMS, bool& rbDownTimeElapsed) const
{
	// We always return true on first hit down, so if it was
	// just pressed, max out the down time
	if ( Pressed() )
	{
		m_fDownTimeMS = fMinTimeMS;
	}
	else
	{
		// If it's not down right now, then just return false
		if ( !m_bCurrDown )
		{
			m_fDownTimeMS = 0; // Make sure it's reset
			rbDownTimeElapsed = false;
			return false;
		}
	}

	// Increment current down time and see if it meets the input min time
	m_fDownTimeMS += fDeltaTimeMS;
	if ( m_fDownTimeMS >= fMinTimeMS )
	{			
		m_fDownTimeMS = 0; // Reset
		rbDownTimeElapsed = true;
	}
	else
	{
		rbDownTimeElapsed = false;
	}

	return true; // The key is down
}
