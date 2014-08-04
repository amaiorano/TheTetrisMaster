#ifndef _AUDIO_STREAM_MGR_H_
#define _AUDIO_STREAM_MGR_H_

#include <string>
#include <list>
#include <memory>
#include "Singleton.h"
#include "AudioStream.h"

// To make life easy, we use the std shared pointer class
// so you can pass around the AudioStream objects, and never
// worry about destroying them - as soon as the last reference
// is destroyed, the object is deleted automatically.
typedef std::shared_ptr<AudioStream> AudioStreamShPtr;
typedef std::weak_ptr<AudioStream> AudioStreamWkPtr;

// Singleton class that is an AudioStream factory and
// manager for all streams during runtime.
class AudioStreamMgr : public Singleton<AudioStreamMgr>
{
public:
	
	// Must call to Initialize() the audio component. If not called,
	// then everything is a no-op, returning dummy AudioStream objects
	void Initialize();

	// Returns whether manager has been initialized
	bool IsInitialized() const;

	////////////////////////////////////////////////////////
	// Factory functions
	////////////////////////////////////////////////////////

	// Use to load any audio file, specifiying whether it should stream it
	// or load it all into memory, and the max number of times the audio stream
	// should be able to play concurrently (with multiple calls to Play()).
	AudioStreamShPtr LoadAudioFile(std::string strFileName, bool bStreaming, int iMaxConcurrentStreams=1);

	// Same as LoadAudioFile(strFileName, false, iMaxConcurrentStreams)
	AudioStreamShPtr LoadSoundFile(std::string strFileName, int iMaxConcurrentStreams)
	{
		return LoadAudioFile(strFileName, false, iMaxConcurrentStreams);
	}

	// Same as LoadAudioFile(strFileName, true, 1)
	AudioStreamShPtr LoadMusicFile(std::string strFileName)
	{
		return LoadAudioFile(strFileName, true);
	}

	////////////////////////////////////////////////////////
	// Runtime stream management functions
	////////////////////////////////////////////////////////

	// Pauses all playing AudioStreams
	void PauseAll();

	// Resumes all paused AudioStreams
	void ResumeAll();

protected:
	// Make singleton class
	AudioStreamMgr() {}

private:	
	typedef std::list<AudioStreamWkPtr> AudioStreamWkPtrList;
	AudioStreamWkPtrList m_audioStreamWkPtrList;	
};


#endif // _AUDIO_STREAM_MGR_H_
