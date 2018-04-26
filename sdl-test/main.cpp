
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
		std::cout << "SDL_Init Error: " << SDL_GetError() << '\n';

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

	SDL_Window *mainWindow = SDL_CreateWindow("Hello, World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);

	if (!mainWindow) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << '\n';
		
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
		
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << '\n';
		
		SDL_Quit();
		return EXIT_FAILURE;
	}

	/** Function: SDL_LoadBMP
	 *
	 *  Description:
	 *  To render a bitmap (BMP) image, we'll need to load it into memory and then onto the rendering
	 *  platform we're using (in this case the GPU). We can load the image with SDL_LoadBMP, which gives
	 *  us back a SDL_Surface pointer (*) that we can then take and upload to a SDL_Texture that the
	 *  renderer is able to use.
	 *
	 *  Parameters:
	 *    1. File path of image
	 *
	 *  Return: SDL_Surface*
	 *
	 */

	std::string imagePath = "C:/Users/jflop/Downloads/swirl_effect.bmp";
	SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());

	if (!bmp) {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(mainWindow);
		
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << '\n';
		
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

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, bmp);
	SDL_FreeSurface(bmp);

	if (!texture) {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(mainWindow);

		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << '\n';

		SDL_Quit();
		return EXIT_FAILURE;
	}

	/** To draw the texture to the screen, first we'll clear the renderer, render our texture, and
	 *  then present the update screen to show the result. Since we want to render the whole image
	 *  and have it stretch to fill the screen, we'll pass NULL as the source and destination
	 *  rectangles for SDL_RenderCopy. We'll also want to keep the window open for a bit so we can
	 *  see the result before the program exits, so we'll add in a call to SDL_Delay.
	 *
	 *  We'll place all this rendering code within the main loop of our program, which for now will
	 *  be a simple for loop. Each iteration through our loop, we will sleep for a second, so we can
	 *  increase or decrease the counter to make our program run for a longer or shorter period. When
	 *  we get to event handling we'll instead track a boolean that indicates if the user wants to
	 *  quit our program (e.g... if they clicked 'x' on the window), and exit our loop in that case.
	 *
	 */

	for (auto i = 0; i < 3; ++i) {
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
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

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(mainWindow);

	SDL_Quit();
	return EXIT_SUCCESS;
}