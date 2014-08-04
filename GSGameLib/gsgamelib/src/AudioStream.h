#ifndef _AUDIO_STREAM_H_
#define _AUDIO_STREAM_H_

#include <string>
#include <memory>

class AudioStreamMgr; // Forward declaration

// Each instance represents a single audio stream attached to a specific
// file. You must use the AudioStreamMgr singleton to create instances of
// AudioStream.
class AudioStream
{
public:	
	~AudioStream();

	// Returns the audio file name
	std::string GetFileName() const;

	// Sets and gets the repeating state (defaults to false)
	void SetRepeat(bool bRepeat);
	bool IsRepeating() const;

	// Sets and gets volume (defaults to 0.5)
	void SetVolume(float fVolume); // Range [0.0, 1.0] with silence=0.0
	float GetVolume() const;

	// Plays the audio stream if it is stopped or paused (same as resume if paused).
	// If bSync is true, the function will block until the audio stream is finished
	// (make sure repeat state is false, or it will never return)
	void Play(bool bSync=false);

	// Stops the audio stream if it is playing
	void Stop();

	// Pauses the audio stream if it is playing
	void Pause();

	// Resumes the audio stream if it is paused
	void Resume();

	// Functions to query current stream state
	bool IsPlaying() const;
	bool IsStopped() const;
	bool IsPaused() const;	

private:
	friend class AudioStreamMgr; // Only class that can create AudioStream objects
	AudioStream(std::string strFileName, bool bStreaming, int iMaxConcurrentStreams);

	// Don't allow copy construction or assignment (no implementation)
	AudioStream(const AudioStream& rhs);
	AudioStream& operator=(const AudioStream& rhs);

private:
	// Use PIMPL idiom (Private IMPLementation) to hide audio-lib specifics
	struct PIMPL;
	std::auto_ptr<PIMPL> m_pPrivateData;
};

#endif // _AUDIO_STREAM_H_
