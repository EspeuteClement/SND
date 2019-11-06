#include "snd_gui.h"


static void draw_string(snd_gui_context* ctxt, uint8_t pos_x, uint8_t pos_y, const char* str)
{
	if (pos_y >= TILEMAP_HEIGHT)
		return;

	for (uint8_t x = pos_x; x < TILEMAP_HEIGHT && str[x-pos_x] != '\0'; ++x)
	{
		ctxt->tilemap[pos_y][x] = str[x - pos_x];
	}
}

void snd_gui_init(snd_gui_context* ctxt)
{
	memset(ctxt, 0, sizeof(snd_gui_context));
}

void snd_gui_render(snd_gui_context* ctxt)
{
	draw_string(ctxt, 0, 0, "Hello World!");

	{
		char buffer[4];
		for (int i = 0; i < 16; ++i)
		{
			uint8_t note = ctxt->audio_context.song.notes[i];
			if (note == SND_NOTE_OFF)
			{
				strcpy_s(buffer, 4, "---");
			}
			else
			{
				snd_note_to_english(note, buffer);
			}

			draw_string(ctxt, 1, 1 + i, buffer);
		}

		if (ctxt->audio_context.song_current_note < 16)
		{
			draw_string(ctxt, 0, ctxt->audio_context.song_current_note, ">");
		}
	}

}
