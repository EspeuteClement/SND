#define SDL_DISABLE_IMMINTRIN_H
#define SDL_MAIN_HANDLED


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "snd_audio.h"
#include "snd_gui.h"

#include <SDL.h>

snd_context ctxt;
snd_gui_context gui_ctxt;

static double PlayTime = 0.0f;

typedef struct Params
{
	float freq;
} Params;

static Params params;

void MyAudioCallback(void *userdata, Uint8* stream, int len)
{
	snd_generate_frames(&ctxt, stream, len);
}

SDL_AudioDeviceID sdl_audio_device;


int init_audio()
{
	SDL_AudioSpec want, have;

	SDL_zero(want);
	want.freq = FREC;
	want.format = AUDIO_F32;
	want.channels = 2;
	want.samples = 4096;
	want.callback = MyAudioCallback;

	params.freq = 220.0f;

	sdl_audio_device = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);

	if (sdl_audio_device == 0)
	{
		fprintf(stderr, "Couldn't open audio device : %s\n", SDL_GetError());
		return 0;
	}
	else
	{
		printf("Audio device opened\n");
		SDL_PauseAudioDevice(sdl_audio_device, 0); // Start audio device
	}
}


SDL_Texture* tileset_texture = NULL;
SDL_Renderer* sdl_renderer;
SDL_Window* pWindow = NULL;

#define TILE_WIDTH 8
#define TILE_HEIGHT 8

#define ZOOM_LEVEL 3

#define TILESET_WIDTH 16


int init_graph()
{
	sdl_renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);

	if (!sdl_renderer)
		return 0;

	{
		SDL_Surface* tileset = NULL;
		tileset = SDL_LoadBMP("res/tileset.bmp");

		if (!tileset)
			return 0;

		tileset_texture = SDL_CreateTextureFromSurface(sdl_renderer, tileset);
		SDL_FreeSurface(tileset);
		if (!tileset_texture)
			return 0;
	}

	return 1;
}

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0 )
    {
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }

    {
        pWindow = SDL_CreateWindow("SND",SDL_WINDOWPOS_UNDEFINED,
										SDL_WINDOWPOS_UNDEFINED,
										ZOOM_LEVEL * TILEMAP_WIDTH * TILE_WIDTH,
										ZOOM_LEVEL * TILEMAP_HEIGHT * TILE_HEIGHT,
                                                                  SDL_WINDOW_SHOWN);

        snd_init(&ctxt);
		snd_gui_init(&gui_ctxt);

		// Init audio thread
		init_audio();

		if (!init_graph())
		{
			fprintf(stderr, "Couldn't open tileset");
		}

        if( pWindow )
        {
        	int quit = 0;
        	while(!quit)
        	{
	        	SDL_Event event;
			    while (SDL_PollEvent(&event) != 0) {
			        if (event.type == SDL_QUIT )
			        {
			        	quit = 1;
			        }
			    }

				SDL_LockAudioDevice(sdl_audio_device);
				memcpy(&gui_ctxt.audio_context, &ctxt, sizeof(snd_context));
				SDL_UnlockAudioDevice(sdl_audio_device);

				snd_gui_render(&gui_ctxt);

				// Render the screen
				SDL_Rect Origin = { 0, 0, TILE_WIDTH, TILE_HEIGHT };
				SDL_Rect Destination = { 0, 0, TILE_WIDTH, TILE_HEIGHT };

				SDL_RenderSetScale(sdl_renderer, (float)ZOOM_LEVEL, (float)ZOOM_LEVEL);

				for (uint8_t y = 0; y < TILEMAP_HEIGHT; ++y)
				{
					for (uint8_t x = 0; x < TILEMAP_WIDTH; ++x)
					{
						uint8_t c = gui_ctxt.tilemap[y][x];

						Origin.x = (c % TILESET_WIDTH) * TILE_WIDTH;
						Origin.y = (c / TILESET_WIDTH) * TILE_HEIGHT;
						SDL_RenderCopy(sdl_renderer, tileset_texture, &Origin, &Destination);
						Destination.x += Destination.w;

					}
					Destination.y += TILE_HEIGHT;
					Destination.x = 0;
				}



				//SDL_Delay(10);
				SDL_RenderPresent(sdl_renderer);
        	}

	    }

        SDL_DestroyWindow(pWindow);
    }

    SDL_Quit();

    return 0;
}