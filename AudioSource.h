#ifndef __AUDIO_SOURCE_H__
#define __AUDIO_SOURCE_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct AudioSource AudioSource;

typedef bool MixSamplesFunction(AudioSource *self,int32_t *samples,int numsamples,int leftvolume,int rightvolume);

struct AudioSource
{
	MixSamplesFunction *mixfunc;
};

void InitializeAudioSource(AudioSource *self,MixSamplesFunction *mixfunc);

void ProduceInterleavedSamplesWithVolumes(AudioSource *self,int16_t *samples,int numsamples,int leftvolume,int rightvolume);
void ProduceMonoSamplesWithVolume(AudioSource *self,int16_t *samples,int numsamples,int volume);
void ProduceSeparatedSamplesWithVolumes(AudioSource *self,int16_t *leftsamples,int16_t *rightsamples,int numsamples,int leftvolume,int rightvolume);

static inline bool MixSamples(AudioSource *self,int32_t *samples,int numsamples,int leftvolume,int rightvolume)
{
	if(!self->mixfunc) return false;
	else return self->mixfunc(self,samples,numsamples,leftvolume,rightvolume);
}

static inline int CombineVolumes(int volume1,int volume2)
{
	return volume1*volume2>>8;
}

static inline void ProduceInterleavedSamples(AudioSource *self,int16_t *samples,int numsamples)
{
	ProduceInterleavedSamplesWithVolumes(self,samples,numsamples,0x100,0x100);
}

static inline void ProduceInterleavedSamplesWithVolume(AudioSource *self,int16_t *samples,int numsamples,int volume)
{
	ProduceInterleavedSamplesWithVolumes(self,samples,numsamples,volume,volume);
}

static inline void ProduceMonoSamples(AudioSource *self,int16_t *samples,int numsamples)
{
	ProduceMonoSamplesWithVolume(self,samples,numsamples,0x100);
}

static inline void ProduceSeparatedSamples(AudioSource *self,int16_t *leftsamples,int16_t *rightsamples,int numsamples)
{
	ProduceSeparatedSamplesWithVolumes(self,leftsamples,rightsamples,numsamples,0x100,0x100);
}

static inline void ProduceSeparatedSamplesWithVolume(AudioSource *self,int16_t *leftsamples,int16_t *rightsamples,int numsamples,int volume)
{
	ProduceSeparatedSamplesWithVolumes(self,leftsamples,rightsamples,numsamples,volume,volume);
}



extern uint32_t _GlobalSampleRate;

static inline uint32_t AudioGraphSampleRate() { return _GlobalSampleRate; }

static inline void SetAudioGraphSampleRate(uint32_t samplerate) { _GlobalSampleRate=samplerate; }

#endif
