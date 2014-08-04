#ifndef _AUDIO_STATE_H_
#define _AUDIO_STATE_H_

// *****************************************
// *** INTERNAL! USED BY AudioStream.cpp ***
// *****************************************
//
// Use the GOF "State" pattern to represent current audio state
// and manage the FSM for it.
//
// Notes on this pattern:
// 1) States should not contain any data members; context must be passed in.
// 2) To change states, a State cannot change 'this', but rather must
//    change the state of the context object passed in.
//
// Audio State Machine:
//
// Playing --> Stopped or Paused
// Stopped --> Playing
// Paused --> Playing or Stopped

// Include Audiere library header
#include "../3rdParty/audiere/audiere.h"

// Useful typedef
typedef audiere::OutputStreamPtr& OutputStreamPtrRef;

// Forward declarations
class AudioStateContext;

// Base class
class AudioState
{
public:
	void Play(AudioStateContext& rAudioStateContext);
	void Stop(AudioStateContext& rAudioStateContext);
	void Pause(AudioStateContext& rAudioStateContext);	
	void Resume(AudioStateContext& rAudioStateContext); // Note: no "Resumed" state

	bool IsPlaying(AudioStateContext& rAudioStateContext);
	bool IsStopped(AudioStateContext& rAudioStateContext);
	bool IsPaused(AudioStateContext& rAudioStateContext);

protected:
	virtual void DoPlay(AudioStateContext& rAudioStateContext)		{ }
	virtual void DoStop(AudioStateContext& rAudioStateContext)		{ }
	virtual void DoPause(AudioStateContext& rAudioStateContext)		{ }
	virtual void DoResume(AudioStateContext& rAudioStateContext)	{ }

	// RTTI-like members
	virtual bool IsPlayingClass()	{ return false; }
	virtual bool IsStoppedClass()	{ return false; }
	virtual bool IsPausedClass()	{ return false;	}

	// Unfortunately, when the internal state of Audiere changes, the AudioState will not be
	// automatically updated (i.e. music stops playing, it will still be in Playing state).
	// This function is called by the public functions whenever such a change may occur.
	virtual AudioState& UpdateAudioState(AudioStateContext& rAudioStateContext);
};

// Playing state
class AudioStatePlaying : public AudioState
{
public:
	// From Playing, can go to Stopped or Paused
	virtual void DoStop(AudioStateContext& rAudioStateContext);
	virtual void DoPause(AudioStateContext& rAudioStateContext);

	virtual bool IsPlayingClass() { return true; }
	virtual AudioState& UpdateAudioState(AudioStateContext& rAudioStateContext);
};

// Stopped state
class AudioStateStopped : public AudioState
{
public:
	// From Stopped, can only go to Playing
	virtual void DoPlay(AudioStateContext& rAudioStateContext);

	virtual bool IsStoppedClass() { return true; }
};

// Paused state
class AudioStatePaused : public AudioState
{
public:
	// From Paused, can go to Playing or Stopped
	virtual void DoPlay(AudioStateContext& rAudioStateContext);
	virtual void DoStop(AudioStateContext& rAudioStateContext);

	// Same as Play() but only works from Paused state
	virtual void DoResume(AudioStateContext& rAudioStateContext)
	{
		DoPlay(rAudioStateContext);
	}
	
	virtual bool IsPausedClass() { return true;	}
};

// Context clas for AudioStates - initial state is set to Stopped.
// All state functions take this context in order to change the current state
// through it and access context-specific data.
class AudioStateContext
{
public:
	AudioStateContext();

	// Changes current audio state
	void ChangeAudioState(AudioState& rAudioState) { m_pAudioState = &rAudioState; }

	// Returns current audio state
	AudioState& GetAudioState() { return *m_pAudioState; }
	
	// Returns reference to Audiere output stream ptr
	OutputStreamPtrRef GetOutputStreamPtr() { return m_pOutputStream; }

	// Public instances of each state
	static AudioStatePlaying	Playing;
	static AudioStateStopped	Stopped; // Initial state
	static AudioStatePaused		Paused;

private:
	AudioState* m_pAudioState;					// Current audio state pointer
	audiere::OutputStreamPtr m_pOutputStream;	// The output stream
};

#endif // _AUDIO_STATE_H_
