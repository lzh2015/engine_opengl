#include "timer.h"
#include <iostream>
using namespace std;
#include <time.h>

Timer& Timer::GetInstance() {
	static Timer instance; // Guaranteed to be destroyed.
									 // Instantiated on first use.
	return instance;
}

Timer::Timer():
	m_fStartTime(0.0f),
	m_fCurrentTime(0.0f),
	m_fElapsedTime(0.0f),
	m_nFPS(0)
{
	//Start();
}

void Timer::Start()
{
	m_fStartTime = getTime();
	m_fCurrentTime = 0.0f;
	m_fElapsedTime = 0.0f;
	m_nFPS = 0;
}

void Timer::Tick()
{
	static int TempFPS = 0;
	static float fPreviousFPSTime = 0.0f;


	float fNewCurrentTime;

	fNewCurrentTime = getTime() - m_fStartTime;

	m_fElapsedTime = fNewCurrentTime - m_fCurrentTime;

	++TempFPS;

	if((fNewCurrentTime - fPreviousFPSTime) > 1.0f )
	{
		fPreviousFPSTime = fNewCurrentTime;
		m_nFPS = TempFPS;
	    TempFPS = 0;
	}

	m_fCurrentTime = fNewCurrentTime;

	//cout << m_fCurrentTime << endl;
	//cout << m_nFPS << endl;
}

float Timer::getTime()
{
	clock_t tCurrent = clock();
	return float(tCurrent)/1000;
}

float Timer::GetTime() const
{
	return m_fCurrentTime;
}

float Timer::GetElapsedTime() const
{
	return m_fElapsedTime;
}
