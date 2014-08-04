#include "AudioState.h"

///////////////////////////////////////////////////////////////////////////////
// AudioState implementation
void AudioState::Play(AudioStateContext& rAudioStateContext)
{
	// Update state in case stream stopped playing
	UpdateAudioState(rAudioStateContext).DoPlay(rAudioStateContext);
}

void AudioState::Stop(AudioStateContext& rAudioStateContext)
{	
	DoStop(rAudioStateContext);
}

void AudioState::Pause(AudioStateContext& rAudioStateContext)
{
	// Update state in case stream stopped playing
	UpdateAudioState(rAudioStateContext).DoPause(rAudioStateContext);
}

void AudioState::Resume(AudioStateContext& rAudioStateContext)
{
	DoResume(rAudioStateContext);
}

bool AudioState::IsPlaying(AudioStateContext& rAudioStateContext)
{
	// Update state in case stream stopped playing
	return UpdateAudioState(rAudioStateContext).IsPlayingClass();
}

bool AudioState::IsStopped(AudioStateContext& rAudioStateContext)
{	
	return IsStoppedClass();
}

bool AudioState::IsPaused(AudioStateContext& rAudioStateContext)
{	
	return IsPausedClass();
}

AudioState& AudioState::UpdateAudioState(AudioStateContext& rAudioStateContext)
{ 
	return rAudioStateContext.GetAudioState();
}

///////////////////////////////////////////////////////////////////////////////
// AudioStatePlaying implementation
void AudioStatePlaying::DoStop(AudioStateContext& rAudioStateContext)
{	
	rAudioStateContext.GetOutputStreamPtr()->stop(); // This acutally pauses
	rAudioStateContext.GetOutputStreamPtr()->reset(); // This brings us back to position 0, effectively "stopping" the audio
	rAudioStateContext.ChangeAudioState( rAudioStateContext.Stopped );
}

void AudioStatePlaying::DoPause(AudioStateContext& rAudioStateContext)
{
	rAudioStateContext.GetOutputStreamPtr()->stop(); // Pauses
	rAudioStateContext.ChangeAudioState( rAudioStateContext.Paused );
}

AudioState& AudioStatePlaying::UpdateAudioState(AudioStateContext& rAudioStateContext)
{
	// If internal stream stopped playing, then we must change state to Stopped
	if ( !rAudioStateContext.GetOutputStreamPtr()->isPlaying() )
		rAudioStateContext.ChangeAudioState( rAudioStateContext.Stopped );
	
	return rAudioStateContext.GetAudioState();
}

///////////////////////////////////////////////////////////////////////////////
// AudioStateStopped implementation
void AudioStateStopped::DoPlay(AudioStateContext& rAudioStateContext)
{
	rAudioStateContext.GetOutputStreamPtr()->play(); // Start playing
	rAudioStateContext.ChangeAudioState( rAudioStateContext.Playing );
}


///////////////////////////////////////////////////////////////////////////////
// AudioStatePaused implementation
void AudioStatePaused::DoPlay(AudioStateContext& rAudioStateContext)
{
	rAudioStateContext.GetOutputStreamPtr()->play(); // Resumes
	rAudioStateContext.ChangeAudioState( rAudioStateContext.Playing );
}

void AudioStatePaused::DoStop(AudioStateContext& rAudioStateContext)
{
	rAudioStateContext.GetOutputStreamPtr()->reset(); // Already paused, so just reset to position 0
	rAudioStateContext.ChangeAudioState( rAudioStateContext.Stopped );
}

///////////////////////////////////////////////////////////////////////////////
// AudioStateContext implementation
AudioStateContext::AudioStateContext() : m_pAudioState(&Stopped)
{
}

// Declare static instances of each state
AudioStatePlaying AudioStateContext::Playing;
AudioStateStopped AudioStateContext::Stopped;
AudioStatePaused AudioStateContext::Paused;
