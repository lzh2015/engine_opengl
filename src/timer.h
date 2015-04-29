#ifndef TIMER_H_
#define TIMER_H_

class Timer
{
public:
	static Timer& GetInstance();
	void Start();
	void Tick();
	float GetTime() const;
	float GetElapsedTime() const;
private:
	float getTime();
	Timer();
	~Timer() {};
	Timer(Timer const&);  // Don't Implement
	void operator=(Timer const&); // Don't implement
	float	m_fStartTime;
	float 	m_fCurrentTime;
	float 	m_fElapsedTime;
	int		m_nFPS;
};



#endif /* TIMER_H_ */
