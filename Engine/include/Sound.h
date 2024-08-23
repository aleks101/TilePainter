#pragma once

#include <SDL_mixer.h>

template<class T>
class Sound
{
private:
	T* audio;
	bool isPlaying;
	int volume;
	int channel;

	int temporaryChannel;
	int temporaryChannel1;
public:
	Sound(T* _audio, int _volume);
	void ChangeAudio(T* _audio);

	void Pause();
	void Resume();
	void ChangeVolume(int _volume);
	void Play(int loops);
	void Play();
	void PlayOnMultipleChannels();
	bool CheckIfAudioIsPlaying();

	static int channels;
protected:

};
template<class T>
int Sound<T>::channels = 1;

template<class T>
Sound<T>::Sound(T* _audio, int _volume) : audio(_audio), volume(_volume) {
	channel = channels;
	temporaryChannel = channel;
	temporaryChannel = 0;
	if (volume < 0)
		volume = 0;
	else if (volume > MIX_MAX_VOLUME)
		volume = MIX_MAX_VOLUME;
	isPlaying = false;
	channels++;
}
template<class T>
void Sound<T>::ChangeAudio(T* _audio) {
	audio = _audio;
}
template<class T>
bool Sound<T>::CheckIfAudioIsPlaying() {
	if (Mix_Playing(channel)) {
		return true;
	}
	return false;
}
template<class T>
void Sound<T>::Play(int loops) {
	if (audio == NULL) {
		LOG("AUDIO FILE IS NULL");
		return;
	}
	Mix_Volume(channel, volume);
	Mix_PlayChannel(channel, audio, loops);
}
template<class T>
void Sound<T>::Play() {
	if (audio == NULL) {
		LOG("AUDIO FILE IS NULL");
		return;
	}
	Mix_Volume(channel, volume);
	Mix_PlayChannel(channel, audio, 0);
}
template<class T>
void Sound<T>::PlayOnMultipleChannels() {
	if (Mix_Playing(temporaryChannel)) {
		temporaryChannel = channel + temporaryChannel;
		temporaryChannel++;
		Mix_Volume(temporaryChannel, volume);
		Mix_PlayChannel(temporaryChannel, audio, 0);
	}
	else {
		temporaryChannel = channel;
		Mix_Volume(temporaryChannel, volume);
		Mix_PlayChannel(temporaryChannel, audio, 0);
	}
}
template<class T>
void Sound<T>::Pause() {
	Mix_Pause(channel);
}
template<class T>
void Sound<T>::Resume() {
	Mix_Resume(channel);
}
template<class T>
void Sound<T>::ChangeVolume(int _volume) {
	volume = _volume;
	if (volume < 0)
		volume = 0;
	else if (volume > MIX_MAX_VOLUME)
		volume = MIX_MAX_VOLUME;
	Mix_Volume(channel, volume);
}