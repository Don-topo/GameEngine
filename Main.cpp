#include <iostream>
#include <memory>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "Engine/GameEngine.h"

unsigned const width = 1280;
unsigned const height = 720;
std::string title = "GameEngine";

int main(int argc, char* argv[])
{
    
    std::unique_ptr<GameEngine> gameEngine = std::make_unique<GameEngine>();

    gameEngine->Initialization(width, height, title);
    gameEngine->Loop();
    gameEngine->Cleanup();

    return 0;
}