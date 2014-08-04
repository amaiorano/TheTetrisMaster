#ifndef _ANIM_SEQ_H_
#define _ANIM_SEQ_H_

// Base animation sequence class
class AnimSeq
{
public:
	virtual ~AnimSeq() {}

	// Resets frame index to 0
	void Reset() { m_iCurrFrame = 0; }

	// Returns current frame index
	int Curr() const { return m_iCurrFrame; }

	// Computes and returns next frame index
	virtual int Next() = 0;

protected:
	AnimSeq(int iNumFrames) : m_iNumFrames(iNumFrames), m_iCurrFrame(0) { }

	const int m_iNumFrames;
	int m_iCurrFrame;    
};

// Use when only one animation index (no animation)
class NoAnimSeq : public AnimSeq
{
public:
	NoAnimSeq() : AnimSeq(0) {}
	virtual int Next() { return Curr(); }
};

// Loop ex: 1,2,3,4,1,2,3,4,...
class LoopAnimSeq : public AnimSeq
{
public:
	LoopAnimSeq(int iNumFrames) : AnimSeq(iNumFrames) {}

	virtual int Next()
	{
		m_iCurrFrame = ++m_iCurrFrame % m_iNumFrames;
		return m_iCurrFrame;
	}
};

// Ping-Pong ex: 1,2,3,4,3,2,1,2,3,4,...
class PingPongAnimSeq : public AnimSeq
{
public:
	PingPongAnimSeq(int iNumFrames) : AnimSeq(iNumFrames), m_bRising(true) {}

	virtual int Next()
	{
		if ( m_bRising )
		{
			if ( ++m_iCurrFrame == (m_iNumFrames) )
			{
				m_bRising = false;
				m_iCurrFrame -= 2;
			}
		}
		else
		{
			if ( --m_iCurrFrame == -1 )
			{
				m_bRising = true;
				m_iCurrFrame = 1;
			}
		}
		return m_iCurrFrame;
	}

private:
	bool m_bRising;
};

#endif // _ANIM_SEQ_H_