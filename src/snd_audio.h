#ifndef _SND_AUDIO_H_
#define _SND_AUDIO_H_

#include <stdint.h>

typedef struct snd_song
{
	int BPM;


} snd_song;

typedef struct snd_context
{
	double song_frame_pointer;	// Position in seconds in the current song

	double tick_lenght;

	struct {
		float pitch;
	} pitch_data;

	struct {

	}

	snd_song song;

} snd_context;

// Init the SND engine
// return the newly created snd_context
void snd_init(snd_context* ctxt);

void snd_compute_tick_length(snd_context* ctxt);

// fills buffer with num samples using data present in the current context
// buffer size should be at least num*2*sizeof(uint8_t)
// context get updated
void snd_get_samples(snd_context* ctxt, uint8_t (*buffer)[2], int num);

#endif //_SND_AUDIO_H_