#ifndef _MULTI_OUTPUT_STREAM_H_
#define _MULTI_OUTPUT_STREAM_H_

// *****************************************
// *** INTERNAL! USED BY AudioStream.cpp ***
// *****************************************


#include <vector>
#include <algorithm>

// Include Audiere library header
#include "../3rdParty/audiere/audiere.h"

// Function defined in AudioStreamMgr.cpp
audiere::AudioDevicePtr& GetAudioDevicePtr();

// We're adding a class to the audiere lib, so make it part of the
// namespace
namespace audiere
{

// Our own derived OutputStream that provides a way
// to play a given stream multiple times at the same
// time (concurrently)
class MultiOutputStream : public RefImplementation<OutputStream>
{
public:
	// Most constructor params copied from audiere::OpenSound()
	MultiOutputStream(
		int iMaxConcurrentStreams,
		const AudioDevicePtr& device,
		const char* filename,
		bool streaming = false,
		FileFormat file_format = FF_AUTODETECT) : m_iCurr(0), m_iMax(iMaxConcurrentStreams)
	{
		if ( iMaxConcurrentStreams <= 0 )
			throw std::invalid_argument("iMaxConcurrentStreams must be positive");

		// Grab audiere device pointer
		AudioDevicePtr& rpAudioDevicePtr = GetAudioDevicePtr();
		SMART_ASSERT(rpAudioDevicePtr).msg("AudioStreamMgr has not been initialized");

		for (int i=0; i<iMaxConcurrentStreams; ++i)
		{
			// Create the audiere audio stream and add to list
			OutputStreamPtr ptr = OpenSound(rpAudioDevicePtr, filename, streaming);
			m_outputStreamPtrList.push_back(ptr);
		}
	}

	~MultiOutputStream()
	{
	}

	virtual void ADR_CALL play()
	{
		// Play current and move to next one
		m_outputStreamPtrList[m_iCurr]->play();
		m_iCurr = ++m_iCurr % m_iMax;
	}

	virtual void ADR_CALL stop()
	{
		m_outputStreamPtrList[m_iCurr]->stop();
	}

	virtual bool ADR_CALL isPlaying()
	{
		return m_outputStreamPtrList[m_iCurr]->isPlaying();
	}

	virtual void ADR_CALL reset()
	{
		m_outputStreamPtrList[m_iCurr]->reset();
	}

	virtual void ADR_CALL setRepeat(bool repeat)
	{
		OutputStreamPtrList::iterator it;
		for (it = m_outputStreamPtrList.begin(); it != m_outputStreamPtrList.end(); ++it)
			(*it)->setRepeat(repeat);
	}

	virtual bool ADR_CALL getRepeat()
	{
		return m_outputStreamPtrList[m_iCurr]->getRepeat();
	}

	virtual void ADR_CALL setVolume(float volume)
	{
		OutputStreamPtrList::iterator it;
		for (it = m_outputStreamPtrList.begin(); it != m_outputStreamPtrList.end(); ++it)
			(*it)->setVolume(volume);
	}

	virtual float ADR_CALL getVolume()
	{
		return m_outputStreamPtrList[m_iCurr]->getVolume();
	}

	virtual void ADR_CALL setPan(float pan)
	{
		OutputStreamPtrList::iterator it;
		for (it = m_outputStreamPtrList.begin(); it != m_outputStreamPtrList.end(); ++it)
			(*it)->setPan(pan);
	}

	virtual float ADR_CALL getPan()
	{
		return m_outputStreamPtrList[m_iCurr]->getPan();
	}

	virtual void ADR_CALL setPitchShift(float shift)
	{
		OutputStreamPtrList::iterator it;
		for (it = m_outputStreamPtrList.begin(); it != m_outputStreamPtrList.end(); ++it)
			(*it)->setPitchShift(shift);
	}

	virtual float ADR_CALL getPitchShift()
	{
		return m_outputStreamPtrList[m_iCurr]->getPitchShift();
	}

	virtual bool ADR_CALL isSeekable()
	{
		return m_outputStreamPtrList[m_iCurr]->isSeekable();
	}

	virtual int ADR_CALL getLength()
	{
		return m_outputStreamPtrList[m_iCurr]->getLength();
	}

	virtual void ADR_CALL setPosition(int position)
	{
		return m_outputStreamPtrList[m_iCurr]->setPosition(position);
	}

	virtual int ADR_CALL getPosition()
	{
		return m_outputStreamPtrList[m_iCurr]->getPosition();
	}

private:
	typedef std::vector<OutputStreamPtr> OutputStreamPtrList;
	OutputStreamPtrList m_outputStreamPtrList;
	int m_iCurr, m_iMax;
};

} // namespace Audiere

#endif // _MULTI_OUTPUT_STREAM_H_
