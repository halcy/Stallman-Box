#include "AudioSource.h"

#include <string.h>

uint32_t _GlobalSampleRate;

void InitializeAudioSource(AudioSource *self,MixSamplesFunction *mixfunc)
{
	self->mixfunc=mixfunc;
}

static inline int16_t ScaleAndClamp(int32_t val)
{
	if(val>0x7fffff) return 0x7fff;
	else if(val<-0x800000) return -0x8000;
	else return val>>8;
}

void ProduceInterleavedSamplesWithVolumes(AudioSource *self,int16_t *samples,int numsamples,int leftvolume,int rightvolume)
{
	int32_t mixbuffer[numsamples*2];

	memset(mixbuffer,0,sizeof(mixbuffer));
	MixSamples(self,mixbuffer,numsamples,leftvolume,rightvolume);

	for(int i=0;i<numsamples;i++)
	{
		samples[2*i+0]=ScaleAndClamp(mixbuffer[2*i+0]);
		samples[2*i+1]=ScaleAndClamp(mixbuffer[2*i+1]);
	}
}

void ProduceMonoSamplesWithVolume(AudioSource *self,int16_t *samples,int numsamples,int volume)
{
	int32_t mixbuffer[numsamples*2];

	memset(mixbuffer,0,sizeof(mixbuffer));
	MixSamples(self,mixbuffer,numsamples,volume,volume);

	for(int i=0;i<numsamples;i++)
	{
		samples[i]=((int32_t)ScaleAndClamp(mixbuffer[2*i+0])+
		(int32_t)ScaleAndClamp(mixbuffer[2*i+1]))>>1;
	}
}

void ProduceSeparatedSamplesWithVolumes(AudioSource *self,int16_t *leftsamples,int16_t *rightsamples,int numsamples,int leftvolume,int rightvolume)
{
	int32_t mixbuffer[numsamples*2];

	memset(mixbuffer,0,sizeof(mixbuffer));
	MixSamples(self,mixbuffer,numsamples,leftvolume,rightvolume);

	for(int i=0;i<numsamples;i++)
	{
		leftsamples[i]=ScaleAndClamp(mixbuffer[2*i+0]);
		rightsamples[i]=ScaleAndClamp(mixbuffer[2*i+1]);
	}
}

