#include "AudioStreamMgr.h"

#include <stdexcept>
#include "SmartAssert.h"

// Include Audiere library header
#include "../external/audiere/audiere.h"

// Use std namespace
using namespace std;

// Define function to return single AudioDevicePtr instance
audiere::AudioDevicePtr& GetAudioDevicePtr()
{
	// Declare a single audiere AudioStreamMgrPtr
	static audiere::AudioDevicePtr s_pAudioDevicePtr;
	return s_pAudioDevicePtr;
}

void AudioStreamMgr::Initialize()
{
	audiere::AudioDevicePtr& rpAudioDevicePtr = GetAudioDevicePtr();
	SMART_ASSERT(!rpAudioDevicePtr).msg("AudioStreamMgr should only be initialized once");

	rpAudioDevicePtr = audiere::AudioDevicePtr(audiere::OpenDevice());

	if ( !rpAudioDevicePtr )
		throw std::runtime_error("Failed to open Audiere audio device");
}


bool AudioStreamMgr::IsInitialized() const
{
	// Does smart pointer contain a value?
	return ( GetAudioDevicePtr().get() != NULL );
}

AudioStreamShPtr AudioStreamMgr::LoadAudioFile(std::string strFileName, bool bStreaming, int iMaxConcurrentStreams)
{
	// TODO:
	// if ( !IsInitialized() )
	//		return CreateDummyStreamPtr();

	AudioStreamShPtr ptr( new AudioStream(strFileName, bStreaming, iMaxConcurrentStreams) );
	m_audioStreamWkPtrList.push_back( ptr ); // Add to weak pointer list
	return ptr;
}

void AudioStreamMgr::PauseAll()
{
	// Cycle through weak pointers - if we can't acquire one, then we can
	// safely remove it from the list
	AudioStreamWkPtrList::iterator it;
	for (it = m_audioStreamWkPtrList.begin(); it != m_audioStreamWkPtrList.end(); ++it)
	{
		AudioStreamShPtr pAudioStream = it->lock();
		if ( pAudioStream )
		{
			pAudioStream->Pause(); // ***
		}
		else
		{
			m_audioStreamWkPtrList.erase(it);
		}
	}
}

void AudioStreamMgr::ResumeAll()
{
	// Cycle through weak pointers - if we can't acquire one, then we can
	// safely remove it from the list
	AudioStreamWkPtrList::iterator it;
	for (it = m_audioStreamWkPtrList.begin(); it != m_audioStreamWkPtrList.end(); ++it)
	{
		AudioStreamShPtr pAudioStream = it->lock();
		if ( pAudioStream )
		{
			pAudioStream->Resume(); // ***
		}
		else
		{
			m_audioStreamWkPtrList.erase(it);
		}
	}
}
