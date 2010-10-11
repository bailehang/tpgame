
#pragma once 

class  CTimer
{
	
public:
	CTimer( long fps );

	void  Start();

	bool  ReadyForNextFrame()  ;

private:

	bool   m_bStarted;
	long   m_lStartTime;

	long   m_lFps;

};