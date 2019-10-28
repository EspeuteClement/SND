#ifndef _SND_AUDIO_H_
#define _SND_AUDIO_H_

#include <stdint.h>

#define SND_PITCH_COUNT 127
#define SND_OFFSET_FROM_MIDI 21
#define SND_A4_INDEX (69 - SND_OFFSET_FROM_MIDI) // Midi A4, also Nice

#define SND_DEBUG 1

#if SND_DEBUG
#include <stdio.h>
#	define SND_LOG(...) printf(__VA_ARGS__)
#else
#	define SND_LOG(...)
#endif

typedef struct snd_song
{
	int BPM;


} snd_song;

typedef struct snd_context
{
	double song_frame_pointer;	// Position in seconds in the current song

	double tick_lenght;

	struct {
		double pitch;
	} pitch_data[SND_PITCH_COUNT];

	snd_song song;

} snd_context;

// Init the SND engine
// return the newly created snd_context
void snd_init(snd_context* ctxt);

// Convert a note to it's english notation
// out must be at least 4 char wide
// only the first 4 byte will be cleared
void snd_note_to_english(uint8_t note, char* out);

void snd_compute_tick_length(snd_context* ctxt);

// fills buffer with num samples using data present in the current context
// buffer size should be at least num*2*sizeof(uint8_t)
// context get updated
void snd_get_samples(snd_context* ctxt, uint8_t (*buffer)[2], int num);

#endif //_SND_AUDIO_H_