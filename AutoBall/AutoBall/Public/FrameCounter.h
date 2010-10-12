/** 
 *	 
 *
 */
#pragma  once 


#define TickCounter GetInstObj(FrameCounter)

class FrameCounter
{
public:

	FrameCounter():m_lCount(0), m_iFramesElapsed(0){}

	FrameCounter(const FrameCounter&);
	FrameCounter& operator=(const FrameCounter&);

public:

	void Update(){++m_lCount; ++m_iFramesElapsed;}

	long GetCurrentFrame(){return m_lCount;}

	void Reset(){m_lCount = 0;}

	void Start(){m_iFramesElapsed = 0;}
	int  FramesElapsedSinceStartCalled()const{return m_iFramesElapsed;}

private:

	long m_lCount;

	int  m_iFramesElapsed;
};