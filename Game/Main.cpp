#include "Engine.h"
#include <SDL.h>
#include <iostream>

int main(int, char**)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("GAT150", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	std::cout << ds::GetFilePath() << std::endl;
	ds::SetFilePath("../Resources");
	std::cout << ds::GetFilePath() << std::endl;

	//load surface
	//create texture

	// wait for keyboard enter to exit
	
	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		SDL_RenderPresent(renderer);
	}

	SDL_Quit();

	return 0;
}