#include "VorbisPlayer.h"

#include "stb_vorbis.c"

static bool MixMonoVorbisSamples(AudioSource *source,int32_t *samples,int numsamples,int leftvolume,int rightvolume);
static bool MixStereoVorbisSamples(AudioSource *source,int32_t *samples,int numsamples,int leftvolume,int rightvolume);
static bool MixHalfRateMonoVorbisSamples(AudioSource *source,int32_t *samples,int numsamples,int leftvolume,int rightvolume);
static bool MixHalfRateStereoVorbisSamples(AudioSource *source,int32_t *samples,int numsamples,int leftvolume,int rightvolume);

bool InitializeVorbisPlayer(VorbisPlayer *self,const void *bytes,size_t length,bool loop)
{
	self->loop=loop;
	self->currentframe=NULL;
	self->framepos=0;
	self->framelength=0;

	int error;
	self->vorbis=stb_vorbis_open_memory((void *)bytes,(int)length,&error,NULL);
	if(!self->vorbis)
	{
		InitializeAudioSource(&self->source,NULL);
		return false;
	}

	stb_vorbis_info info=stb_vorbis_get_info(self->vorbis);
	uint32_t globalrate=AudioGraphSampleRate();

	if(info.sample_rate==globalrate)
	{
		switch(info.channels)
		{
			case 1: InitializeAudioSource(&self->source,MixMonoVorbisSamples); return true;
			case 2: InitializeAudioSource(&self->source,MixStereoVorbisSamples); return true;
			default: InitializeAudioSource(&self->source,NULL); return false;
		}
	}
	else if(info.sample_rate/2==globalrate)
	{
		switch(info.channels)
		{
			case 1: InitializeAudioSource(&self->source,MixHalfRateMonoVorbisSamples); return true;
			case 2: InitializeAudioSource(&self->source,MixHalfRateStereoVorbisSamples); return true;
			default: InitializeAudioSource(&self->source,NULL); return false;
		}
	}
	else
	{
		InitializeAudioSource(&self->source,NULL);
		return false;
	}
}

void CleanupVorbisPlayer(VorbisPlayer *self)
{
	stb_vorbis_close(self->vorbis);
}

void RestartVorbisPlayer(VorbisPlayer *self)
{
	stb_vorbis_seek_start(self->vorbis);
	self->currentframe=NULL;
	self->framepos=0;
	self->framelength=0;
}

static inline void LoadNextFrameIfNeeded(VorbisPlayer *self)
{
	if(self->framepos>=self->framelength)
	{
		self->framepos=0;
		self->framelength=stb_vorbis_get_frame_float(self->vorbis,NULL,&self->currentframe);
		if(!self->framelength)
		{
			stb_vorbis_seek_start(self->vorbis);
			self->framelength=stb_vorbis_get_frame_float(self->vorbis,NULL,&self->currentframe);
		}
	}
}

static bool MixMonoVorbisSamples(AudioSource *source,int32_t *samples,int numsamples,int leftvolume,int rightvolume)
{
	VorbisPlayer *self=(VorbisPlayer *)source;

	float leftscale=(float)leftvolume*32767;
	float rightscale=(float)rightvolume*32767;

	for(int i=0;i<numsamples;i++)
	{
		LoadNextFrameIfNeeded(self);
		samples[2*i+0]+=self->currentframe[0][self->framepos]*leftscale;
		samples[2*i+1]+=self->currentframe[0][self->framepos]*rightscale;
		self->framepos++;
	}
	return true;
}

static bool MixStereoVorbisSamples(AudioSource *source,int32_t *samples,int numsamples,int leftvolume,int rightvolume)
{
	VorbisPlayer *self=(VorbisPlayer *)source;

	float leftscale=(float)leftvolume*32767;
	float rightscale=(float)rightvolume*32767;

	for(int i=0;i<numsamples;i++)
	{
		LoadNextFrameIfNeeded(self);
		samples[2*i+0]+=self->currentframe[0][self->framepos]*leftscale;
		samples[2*i+1]+=self->currentframe[1][self->framepos]*rightscale;
		self->framepos++;
	}
	return true;
}

static bool MixHalfRateMonoVorbisSamples(AudioSource *source,int32_t *samples,int numsamples,int leftvolume,int rightvolume)
{
	VorbisPlayer *self=(VorbisPlayer *)source;

	float leftscale=(float)leftvolume*32767;
	float rightscale=(float)rightvolume*32767;

	for(int i=0;i<numsamples;i++)
	{
		LoadNextFrameIfNeeded(self);
		samples[2*i+0]+=self->currentframe[0][self->framepos]*leftscale;
		samples[2*i+1]+=self->currentframe[0][self->framepos]*rightscale;
		self->framepos+=2;
	}
	return true;
}

static bool MixHalfRateStereoVorbisSamples(AudioSource *source,int32_t *samples,int numsamples,int leftvolume,int rightvolume)
{
	VorbisPlayer *self=(VorbisPlayer *)source;

	float leftscale=(float)leftvolume*32767;
	float rightscale=(float)rightvolume*32767;

	for(int i=0;i<numsamples;i++)
	{
		LoadNextFrameIfNeeded(self);
		samples[2*i+0]+=self->currentframe[0][self->framepos]*leftscale;
		samples[2*i+1]+=self->currentframe[1][self->framepos]*rightscale;
		self->framepos+=2;
	}
	return true;
}
