#pragma once

class Timer
{
private:
	unsigned int m_start;
	unsigned int m_stop;
	unsigned int m_pausedTime;
	bool m_isPaused;
public:
	Timer();
	Timer(const Timer& obj);
	Timer& operator=(const Timer& obj);
	void Start();
	void Stop();
	void Resume();
	void Pause();
	unsigned int GetSec();
	unsigned int GetMili();
	unsigned int GetDurationMili();
	unsigned int GetDurationSec();
};
