#include "Audio.h"

#include <SDL/SDL.h>

static AudioCallback *CallbackFunction;
static void *CallbackContext;
static int SampleRate;
static int NumberOfChannels;

static void Callback(void *context,uint8_t *stream,int len);

void InitializeAudio(int samplerate,int channels,int numsamples,bool exactformat,
AudioCallback *callback,void *context)
{
	SDL_AudioSpec spec=
	{
		.freq=samplerate, .format=AUDIO_S16, .channels=channels,
		.samples=numsamples, .callback=Callback, .userdata=NULL,
	};

	if(exactformat)
	{
		if(SDL_OpenAudio(&spec,NULL)<0)
		{
			fprintf(stderr,"Failed to open audio device: %s\n",SDL_GetError());
		}
		SampleRate=samplerate;
		NumberOfChannels=channels;
	}
	else
	{
		SDL_AudioSpec actualspec;
		if(SDL_OpenAudio(&spec,&actualspec)<0)
		{
			fprintf(stderr,"Failed to open audio device: %s\n",SDL_GetError());
		}
		SampleRate=actualspec.freq;
		NumberOfChannels=actualspec.channels;
	}

	CallbackFunction=callback;
	CallbackContext=context;
}

static void Callback(void *context,uint8_t *stream,int len)
{
	CallbackFunction(CallbackContext,(int16_t *)stream,
	len/(2*NumberOfChannels));
}

int ActualAudioSampleRate() { return SampleRate; }

int ActualNumberOfAudioChannels() { return NumberOfChannels; }

int MasterVolume() { return 0x100; }

void StartAudio()
{
    SDL_PauseAudio(0);
}

void StopAudio()
{
    SDL_PauseAudio(1);
}
