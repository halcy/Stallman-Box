#ifndef __PLATFORM_AUDIO_H__
#define __PLATFORM_AUDIO_H__

#include <stdint.h>
#include <stdbool.h>

typedef void AudioCallback(void *context,int16_t *samples,int numsamples);

void InitializeAudio(int samplerate,int channels,int numsamples,bool exactformat,
AudioCallback *callback,void *context);

int ActualAudioSampleRate();
int ActualNumberOfAudioChannels();
int MasterVolume();

void StartAudio();
void StopAudio();

#endif
