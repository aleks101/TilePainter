#pragma once

#include <SDL.h>

class Animation {
private:
	bool m_loop;
	bool m_stop;
	bool m_finished;

	int m_frames;
	int m_currFrame;
	float* m_frameDuration;
	float m_time;

	int m_row;
	int m_startX, m_width;
	int m_startY, m_height;
	SDL_Rect* m_srcRect;
public:
	Animation();
	Animation(const Animation& ani);
	virtual ~Animation();
	void Init(SDL_Rect* sourceRect, int startX, int startY, int width, int height, int frames);

	void SetFrameDuration(int frame, float duration);
	void SetLooping(bool isLooping);
	bool IsFinished();
	void SkipToFrame(int frame);

	void Start();
	void Stop();
	void Resume();
	void Reset();
	void Clean();
	void Animate();
protected:

};
