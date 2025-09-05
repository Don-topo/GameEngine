#include <iostream>
#include <memory>
#include <string>

#include "Engine/GameEngine.h"

unsigned const width = 1280;
unsigned const height = 720;
std::string title = "GameEngine";

int main(int argc, char* argv[])
{
    std::cout << "Hello" << std::endl;
    std::unique_ptr<GameEngine> gameEngine = std::make_unique<GameEngine>();

    gameEngine->Initialization(width, height, title);
    gameEngine->Loop();
    gameEngine->Cleanup();
    return 0;
}