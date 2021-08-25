#pragma once
#include "Engine.h"

class Game
{
public:

public:
	void Initialize();
	void Shutdown();

	void Update();
	void Draw();

	bool IsQuit() { return quit; };

private:

public:
	std::unique_ptr<ds::Engine> engine;
	std::unique_ptr<ds::Scene> scene;
	SDL_DisplayMode dm;

private:
	bool quit = false;
};

