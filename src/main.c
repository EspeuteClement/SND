#define SDL_DISABLE_IMMINTRIN_H
#define SDL_MAIN_HANDLED


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "snd_audio.h"

#include <SDL.h>

snd_context ctxt;

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

void init_audio()
{
	SDL_AudioSpec want, have;
	SDL_AudioDeviceID dev;

	SDL_zero(want);
	want.freq = FREC;
	want.format = AUDIO_F32;
	want.channels = 2;
	want.samples = 4096;
	want.callback = MyAudioCallback;

	params.freq = 220.0f;

	dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);

	if (dev == 0)
	{
		fprintf(stderr, "Couldn't open audio device : %s\n", SDL_GetError());
	}
	else
	{
		printf("Audio device opened\n");
		SDL_PauseAudioDevice(dev, 0); // Start audio device
	}
}

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0 )
    {
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }

    {
        SDL_Window* pWindow = NULL;
        pWindow = SDL_CreateWindow("SND",SDL_WINDOWPOS_UNDEFINED,
                                                                  SDL_WINDOWPOS_UNDEFINED,
                                                                  640,
                                                                  480,
                                                                  SDL_WINDOW_SHOWN);

        snd_init(&ctxt);

		// Init audio thread
		init_audio();

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
			    SDL_Delay(10);
        	}

	    }

        SDL_DestroyWindow(pWindow);
    }

    SDL_Quit();

    return 0;
}