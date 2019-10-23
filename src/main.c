#define SDL_DISABLE_IMMINTRIN_H

#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>


int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }

    {
        SDL_Window* pWindow = NULL;
        pWindow = SDL_CreateWindow("Ma première application SDL2",SDL_WINDOWPOS_UNDEFINED,
                                                                  SDL_WINDOWPOS_UNDEFINED,
                                                                  640,
                                                                  480,
                                                                  SDL_WINDOW_SHOWN);

	        if( pWindow )
	        {
	        	while(1)
	        	{
		        	SDL_Event event;
				    while (SDL_PollEvent(&event) != 0) {
				        if (event.type == SDL_QUIT )
				        {
				        	break;
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