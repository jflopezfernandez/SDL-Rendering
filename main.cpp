
#include <iostream>
#include <string>

#include <SDL/SDL.h>


int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO)) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << '\n';

		return EXIT_FAILURE;
	}

	SDL_Window *mainWindow = SDL_CreateWindow("Hello, World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);

	if (!mainWindow) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << '\n';
		
		SDL_Quit();
		return EXIT_FAILURE;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!renderer) {
		SDL_DestroyWindow(mainWindow);
		
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << '\n';
		
		SDL_Quit();
		return EXIT_FAILURE;
	}

	std::string imagePath = "C:/Users/jflop/Downloads/swirl_effect.bmp";
	SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());

	if (!bmp) {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(mainWindow);
		
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << '\n';
		
		SDL_Quit();
		return EXIT_FAILURE;
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, bmp);
	SDL_FreeSurface(bmp);

	if (!texture) {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(mainWindow);

		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << '\n';

		SDL_Quit();
		return EXIT_FAILURE;
	}

	for (auto i = 0; i < 3; ++i) {
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		SDL_Delay(1000);
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(mainWindow);

	SDL_Quit();
	return EXIT_SUCCESS;
}