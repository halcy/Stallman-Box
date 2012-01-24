#ifndef __VORBIS_PLAYER_H__
#define __VORBIS_PLAYER_H__

#include "AudioSource.h"

//#define STB_VORBIS_NO_PULLDATA_API
#define STB_VORBIS_NO_PUSHDATA_API
#define STB_VORBIS_NO_STDIO
#define STB_VORBIS_NO_INTEGER_CONVERSION
#define STB_VORBIS_MAX_CHANNELS 2

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"
#undef STB_VORBIS_HEADER_ONLY

#include <stdlib.h>

typedef struct VorbisPlayer
{
	AudioSource source;
	stb_vorbis *vorbis;
	bool loop;

	float **currentframe;
	int framepos,framelength;
} VorbisPlayer;

bool InitializeVorbisPlayer(VorbisPlayer *self,const void *bytes,size_t length,bool loop);
void CleanupVorbisPlayer(VorbisPlayer *self);
void RestartVorbisPlayer(VorbisPlayer *self);

#endif
