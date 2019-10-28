#include "snd_audio.h"

#include <math.h>

#define TICKS_PER_BPM 1440
#define _TWELTH_ROOT_OF_TWO 1.059463094359

void snd_compute_tick_length(snd_context* ctxt)
{
	ctxt->tick_lenght = ctxt->song.BPM / TICKS_PER_BPM;
}

void snd_note_to_english(uint8_t note, char* out)
{
	// Quick hack, might do a simpler version later
	const char* _conversion_table[12] = 
	{
		"A ",
		"A#",
		"B ",
		"C ",
		"C#",
		"D ",
		"D#",
		"E ",
		"F ",
		"F#",
		"G ",
		"G#"
	};
	
	int _distance_to_A = ((note-SND_A4_INDEX)% 12 + 12) % 12;
	int _octave = (note - SND_A4_INDEX - _distance_to_A) / 12 + 4;

	out[0] = _conversion_table[_distance_to_A][0];
	out[1] = _conversion_table[_distance_to_A][1];
	if (_octave <= 9)
	{
		out[2] = '0' + _octave;
	}
	else
	{
		out[2] = 'A' + _octave-10;
	}
	out[3] = '\0';

}

void snd_init_pitch_table(snd_context* ctxt)
{
	SND_LOG("== Generating pitch table ==\n");

	for (int _pitch = 0; _pitch < SND_PITCH_COUNT; ++_pitch)
	{
		ctxt->pitch_data[_pitch ].pitch = 440.0 * pow(_TWELTH_ROOT_OF_TWO, _pitch - SND_A4_INDEX);
		
#if SND_DEBUG
		char notename[4];
		snd_note_to_english(_pitch, notename); 
		SND_LOG("pitch %d (%s) : %f\n", _pitch, notename, ctxt->pitch_data[_pitch ].pitch);
#endif
	}
}


void snd_init(snd_context* ctxt)
{
	snd_init_pitch_table(ctxt);
	snd_compute_tick_length(ctxt);
}
