#include "GameEngine.h"

void GameEngine::Initialization(unsigned int width, unsigned int height, std::string title)
{
	DEV_ASSERT(SDL_Init(SDL_INIT_VIDEO) != 0, "GameEngine", "Error initializating SLD3!");
	sdlWindow = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_VULKAN);
	DEV_ASSERT(sdlWindow, "GameEngine", "Error creating the SDL Window!");

	renderManager = std::make_unique<RenderManager>();
	renderManager->Initialization(sdlWindow);

	audioManager = std::make_unique<AudioManager>();
	audioManager->Initialization();
	audioManager->SetAudioListener(glm::vec3{}, glm::vec3{}, glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	// TEST
	audioManager->LoadAudio("C:\\Users\\ruben\\Desktop\\GameEngine\\GameEngine\\Assets\\AudioClips\\TestClip.mp3");
	audioManager->PlayAudio("C:\\Users\\ruben\\Desktop\\GameEngine\\GameEngine\\Assets\\AudioClips\\TestClip.mp3");
	audioManager->PauseOrResumeAudio("C:\\Users\\ruben\\Desktop\\GameEngine\\GameEngine\\Assets\\AudioClips\\TestClip.mp3");
}

void GameEngine::Loop()
{
	bool running = true;
	while (running)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			renderManager->Update();
			audioManager->Update();
			if (e.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}		
	}
}

void GameEngine::Cleanup()
{
	renderManager->Cleanup();
	audioManager->Cleanup();
	SDL_DestroyWindow(sdlWindow);
	SDL_Quit();
	DEV_LOG(TE_INFO, "GameEngine", "SDL Closed");
}
