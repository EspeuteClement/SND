#include "snd_audio.h"

#include <math.h>
#include <string.h>

#define TICKS_PER_BPM (1440.0*6.0)
#define _TWELTH_ROOT_OF_TWO 1.059463094359

void snd_compute_tick_length(snd_context* ctxt)
{
	ctxt->tick_lenght = ctxt->song.BPM / TICKS_PER_BPM;

	SND_LOG("tick_lenght : %20.18d", ctxt->tick_lenght);
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
	memset(ctxt, 0, sizeof(snd_context));

	ctxt->song.BPM = 140;

	// Debug LICK

	for (int _i = 0; _i < 32; ++_i)
	{
		ctxt->song.notes[_i] = 127;
	}
	ctxt->song.notes[0] = 53;
	ctxt->song.notes[2] = 55;
	ctxt->song.notes[4] = 56;
	ctxt->song.notes[6] = 58;
	ctxt->song.notes[8] = 55;
	ctxt->song.notes[12] = 51;
	ctxt->song.notes[14] = 53;

	ctxt->current_note = ctxt->song.notes[0];

	snd_init_pitch_table(ctxt);
	snd_compute_tick_length(ctxt);
}

void snd_generate_frames(snd_context* ctxt, void* out_buffer, int nb_bytes)
{
	float (*_stream)[2] = (float**) out_buffer;

	for (int _i = 0; _i < nb_bytes / 2 / sizeof(float); ++_i)
	{
		float _sample = 0;
		int _current_note = ctxt->current_note;

		if (ctxt->current_note != SND_NOTE_OFF)
		{
			double _freq = 2.0 * PI * ctxt->pitch_data[_current_note].pitch;
			_sample = 0.10*sin(ctxt->generator_phase * _freq + 0.25*sin(ctxt->generator_phase * _freq *0.5) + 0.25 * sin(ctxt->generator_phase * _freq * 2));

			ctxt->generator_phase += SND_FRAME_TIME;
		}

		_stream[_i][0] = _sample;
		_stream[_i][1] = _sample;

		ctxt->song_frame_pointer += SND_FRAME_TIME;

		while (ctxt->song_frame_pointer > ctxt->tick_lenght)
		{
			ctxt->song_current_tick ++;
			ctxt->song_frame_pointer -= ctxt->tick_lenght;

			if (ctxt->song_current_tick > SND_TICK_PER_NOTE)
			{
				ctxt->song_current_tick %= SND_TICK_PER_NOTE;
				ctxt->song_current_note = (ctxt->song_current_note + 1) % 32;

				int _next_note = ctxt->song.notes[ctxt->song_current_note];
				if (_next_note != SND_NOTE_OFF)
				{
					ctxt->current_note = _next_note;
					ctxt->generator_phase = 0;

#if SND_DEBUG
					char notename[4];
					snd_note_to_english(ctxt->current_note, notename);
					SND_LOG("Playing note %d (%s)\n", ctxt->current_note, notename);
#endif
				}


			}
		}
	}
}
