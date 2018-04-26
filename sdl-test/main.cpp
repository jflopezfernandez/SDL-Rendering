
#include <iostream>
#include <string>

#include <SDL/SDL.h>

const auto SCREEN_WIDTH = 640;
const auto SCREEN_HEIGHT = 480;

void LogSDLError(std::ostream& os, const std::string& errorMessage) {
	os << errorMessage << " error: " << SDL_GetError() << '\n';
}

SDL_Texture* loadTexture(const std::string& filename, SDL_Renderer *renderer) {
	// Initialize to nullptr to avoid dangling pointer issues
	SDL_Texture *texture = nullptr;

	// Load the image
	SDL_Surface *loadedImage = SDL_LoadBMP(filename.c_str());

	// If the loading went ok, convert to texture and return the texture
	if (loadedImage != nullptr) {
		texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
		SDL_FreeSurface(loadedImage);

		// Make sure everything went ok, too
		if (texture == nullptr) {
			LogSDLError(std::cerr, "LoadBMP");
		}
	} else {
		LogSDLError(std::cerr, "LoadBMP");
	}

	return texture;
}

void RenderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y) {
	// Setup the destination rectangle to be at the position we want
	SDL_Rect destination;
	destination.x = x;
	destination.y = y;

	// Query the texture to get its width and height to use
	SDL_QueryTexture(texture, NULL, NULL, &destination.w, &destination.x);
	SDL_RenderCopy(renderer, texture, NULL, &destination);
}



int main(int argc, char *argv[])
{
	/** Function: SDL_Init
	 *
	 *  Description:
	 *  To use SDL we first need to initialize the various SDL subsystems we want to use.
	 *  This is done through SDL_Init which takes a set of flags OR'd together specifying
	 *  the subsystems we'd like to initialize. Note that the event handling subsystem is
	 *  initialized automatically when the video system is if not explicitly requested by
	 *  itself while the file I/O and threading systems are initialized by default. If
	 *  everything goes ok, SDL_Init will return 0; if not, we'll want to print out the
	 *  error and exit.
	 *
	 */

	if (SDL_Init(SDL_INIT_VIDEO)) {
		LogSDLError(std::cerr, "SDL_Init");

		return EXIT_FAILURE;
	}

	/** Function: SDl_CreateWindow
	 *
	 *  Description:
	 *  We'll need a window to display our render in. We can create one with SDL_CreateWindow,
	 *  which takes a title for the window, the x and y position to create it at, the window width
	 *  and height, and some flags to set properties of the window, and then returns an
	 *  SDL_Window pointer (*). This pointer will be NULL if anything went wrong when creating the
	 *  window. If an error does occur, we need to clean up SDL before exiting the program.
	 *
	 *  Parameters: 
	 *              Window Title
	 *              x position
	 *              y position
	 *              width
	 *              height
	 *              properties
	 *
	 *  Return: SDL_Window*
	 *
	 */

	SDL_Window *mainWindow = SDL_CreateWindow("Hello, World!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (!mainWindow) {
		LogSDLError(std::cerr, "CreateWindow");
		
		SDL_Quit();
		return EXIT_FAILURE;
	}

	/** Function: SDL_Renderer
	 *
	 *  Description: 
	 *  Now we can create a renderer to draw to the window using SDL_CreateRenderer. This function 
	 *  takes the window to associate the renderer with, the index of the redendering driver to be
	 *  used (or -1 to select the first that meets our requirements), and various flags used to 
	 *  specify what sort of renderer we want. Here we're requesting a hardware accelerated renderer
	 *  with vsync enabled. We'll get back an SDL_Renderer pointer (*) which will be NULL if something
	 *  went wrong. If an error does occur, we need to clean up anything we've previously created and
	 *  quit SDL before exiting the program.
	 *
	 *  Parameters: 
	 *              window to associate renderer with
	 *              index of the rendering driver to be used (or -1 to select first one that meets our req's)
	 *              renderer properties flags
	 *
	 *  Return: SDL_Renderer*
	 *
	 */

	SDL_Renderer *renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!renderer) {
		SDL_DestroyWindow(mainWindow);
		
		LogSDLError(std::cerr, "CreateRenderer");

		SDL_Quit();
		return EXIT_FAILURE;
	}

	/** Function: SDL_CreateTextureFromSurface
	 *
	 *  Description:
	 *  With the image loaded into an SDL_Surface, we can now upload it to the renderer using
	 *  SDL_CreateTextureFromSurface. We pass in the rendering context to upload to, as well as the
	 *  image in memory (the SDL_Surface), and get back the loaded texture. We're done with the original
	 *  surface at this point so we'll free it now.
	 *
	 *  Parameters:
	 *    1. Renderer
	 *    2. Image
	 *
	 *  Return: SDL_Texture*
	 *
	 */

	SDL_Texture *background = loadTexture("C:\\Users\\jflop\\source\\repos\\sdl-test\\sdl-test\\img\\background.bmp", renderer);
	SDL_Texture *foreground = loadTexture("C:\\Users\\jflop\\source\\repos\\sdl-test\\sdl-test\\img\\foreground.bmp", renderer);

	if ((!background) || (!foreground)) {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(mainWindow);

		LogSDLError(std::cerr, "LoadTexture");

		SDL_Quit();
		return EXIT_FAILURE;
	}

	for (auto i = 0; i < 3; i++) {
		SDL_RenderClear(renderer);

		int bW = 0;
		int bH = 0;

		SDL_QueryTexture(background, NULL, NULL, &bW, &bW);

		RenderTexture(background, renderer, 0, 0);
		RenderTexture(background, renderer, bW, 0);
		RenderTexture(background, renderer, 0, bH);
		RenderTexture(background, renderer, bW, bH);

		int iW = 0;
		int iH = 0;

		SDL_QueryTexture(foreground, NULL, NULL, &iW, &iH);

		int x = SCREEN_WIDTH / 2 - iW / 2;
		int y = SCREEN_HEIGHT / 2 - iH / 2;

		RenderTexture(foreground, renderer, x, y);

		SDL_RenderPresent(renderer);
		SDL_Delay(1000);
	}

	/** Before we exit, we've got to destroy all the objects we created through the various SDL_DestroyX
	 *  functions, and quit SDL. Error Handling Note: previously in the program we may have encountered
	 *  an error and exited early, in which case we'd have to destroy any SDL objects we had created and
	 *  quit SDL to properly clean up before exiting. This part of the error handling is omitted from the
	 *  lessons since they're such small examples and it helps keep the code a bit shorter, but in a real
	 *  world program, proper error handling and clean up is absolutely required.
	 *
	 */

	SDL_DestroyTexture(background);
	SDL_DestroyTexture(foreground);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(mainWindow);

	SDL_Quit();
	return EXIT_SUCCESS;
}