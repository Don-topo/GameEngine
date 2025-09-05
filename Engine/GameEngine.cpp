#include "GameEngine.h"

void GameEngine::Initialization(unsigned int width, unsigned int height, std::string title)
{
	DEV_ASSERT(SDL_Init(SDL_INIT_VIDEO) != 0, "GameEngine", "Error initializating SLD3!");
	sdlWindow = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_RESIZABLE);
	DEV_ASSERT(sdlWindow, "GameEngine", "Error creating the SDL Window!");
}

void GameEngine::Loop()
{
	bool running = true;
	while (running)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}
	}
}

void GameEngine::Cleanup()
{
	SDL_DestroyWindow(sdlWindow);
	SDL_Quit();
	DEV_LOG(TE_INFO, "GameEngine", "SDL Closed");
}
