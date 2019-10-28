#include "snd_audio.h"

#define TICKS_PER_BPM 1440

void snd_compute_tick_length(snd_context* ctxt)
{
	ctxt->tick_lenght = ctxt->song.BPM / TICKS_PER_BPM;
}
