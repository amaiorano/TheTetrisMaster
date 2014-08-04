#include "AudioStream.h"

#include <stdexcept>
#include "SmartAssert.h"
#include "Window.h"

#include "MultiOutputStream.h" // Include Audiere lib header - internal use only
#include "AudioState.h" // Used to manage current audio state - internal use only

// Use std namespace
using namespace std;

// Function defined in AudioStreamMgr.cpp
audiere::AudioDevicePtr& GetAudioDevicePtr();


// Private data of the class
struct AudioStream::PIMPL
{
	string strFileName;
	AudioStateContext audioStateContext;
};


AudioStream::AudioStream(std::string strFileName, bool bStreaming, int iMaxConcurrentStreams) 
	: m_pPrivateData(new AudioStream::PIMPL)
{
	// Set file name
	m_pPrivateData->strFileName = strFileName;

	// Grab audiere device pointer
	audiere::AudioDevicePtr& rpAudioDevicePtr = GetAudioDevicePtr();
	SMART_ASSERT(rpAudioDevicePtr).msg("AudioStreamMgr has not been initialized");

	// Create the audiere audio stream. If more than 1 concurrent stream, we use
	// our own class that supports multiple concurrent streams, otherwise just use
	// the default audiere class (more efficient)
	if ( iMaxConcurrentStreams == 1 )
	{
		m_pPrivateData->audioStateContext.GetOutputStreamPtr()
			= audiere::OpenSound(rpAudioDevicePtr, strFileName.c_str(), bStreaming);
	}
	else
	{
		m_pPrivateData->audioStateContext.GetOutputStreamPtr()
			= new audiere::MultiOutputStream(iMaxConcurrentStreams, rpAudioDevicePtr, strFileName.c_str(), bStreaming);
	}

	if ( !m_pPrivateData->audioStateContext.GetOutputStreamPtr() )
		throw std::invalid_argument("Failed to load audio file: " + strFileName);

	// Set default values - even if they currently match Audiere, if these values 
	// are ever changed in an updated version of Audiere, we won't be affected
	SetRepeat(false);
	SetVolume(0.5f);
}

AudioStream::~AudioStream()
{
}

std::string AudioStream::GetFileName() const
{
	return m_pPrivateData->strFileName;
}

void AudioStream::SetRepeat(bool bRepeat)
{	
	m_pPrivateData->audioStateContext.GetOutputStreamPtr()->setRepeat(bRepeat);
}

bool AudioStream::IsRepeating() const
{
	return m_pPrivateData->audioStateContext.GetOutputStreamPtr()->getRepeat();
}

void AudioStream::SetVolume(float fVolume)
{
	m_pPrivateData->audioStateContext.GetOutputStreamPtr()->setVolume(fVolume);
}

float AudioStream::GetVolume() const
{
	return m_pPrivateData->audioStateContext.GetOutputStreamPtr()->getVolume();
}

void AudioStream::Play(bool bSync/*=false*/)
{
	AudioState& rAudioState = m_pPrivateData->audioStateContext.GetAudioState();
	rAudioState.Play(m_pPrivateData->audioStateContext);

	while (bSync && IsPlaying())
		Window::Instance().Sleep(1);
}

void AudioStream::Stop()
{	
	AudioState& rAudioState = m_pPrivateData->audioStateContext.GetAudioState();
	rAudioState.Stop(m_pPrivateData->audioStateContext);
}

void AudioStream::Pause()
{
	AudioState& rAudioState = m_pPrivateData->audioStateContext.GetAudioState();
	rAudioState.Pause(m_pPrivateData->audioStateContext);
}

void AudioStream::Resume()
{	
	AudioState& rAudioState = m_pPrivateData->audioStateContext.GetAudioState();
	rAudioState.Resume(m_pPrivateData->audioStateContext);
}

bool AudioStream::IsPlaying() const
{
	AudioState& rAudioState = m_pPrivateData->audioStateContext.GetAudioState();
	return rAudioState.IsPlaying(m_pPrivateData->audioStateContext);
}

bool AudioStream::IsStopped() const
{
	AudioState& rAudioState = m_pPrivateData->audioStateContext.GetAudioState();
	return rAudioState.IsStopped(m_pPrivateData->audioStateContext);
}

bool AudioStream::IsPaused() const
{
	AudioState& rAudioState = m_pPrivateData->audioStateContext.GetAudioState();
	return rAudioState.IsPaused(m_pPrivateData->audioStateContext);
}
