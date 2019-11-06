#pragma once

#include <stdint.h>
#include <string.h>

#include "snd_audio.h"

#define TILEMAP_WIDTH 20
#define TILEMAP_HEIGHT 18

typedef struct snd_gui_context
{
	snd_context audio_context;	// Copy of the snd_context used by the audio thread

	uint8_t tilemap[TILEMAP_HEIGHT][TILEMAP_WIDTH];
} snd_gui_context;

void snd_gui_init(snd_gui_context* ctxt);

void snd_gui_render(snd_gui_context* ctxt);