#include "Engine.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

int main(int, char**)
{
	ds::Engine engine;
	engine.Startup();
	engine.Get<ds::Renderer>()->Create("Gat1Fiddy", 800, 600);

	ds::Scene scene;
	scene.engine = &engine;

	std::cout << ds::GetFilePath() << std::endl;
	ds::SetFilePath("../Resources");
	std::cout << ds::GetFilePath() << std::endl;

	std::shared_ptr<ds::Texture> texture = engine.Get<ds::ResourceSystem>()->Get<ds::Texture>("duck.gif", engine.Get<ds::Renderer>());

	for (size_t i = 0; i < 50; i++)
	{
		ds::Transform transform{ {ds::RandomRange(0,800), ds::RandomRange(0,600)}, ds::RandomRange(0,359), 0.1f };
		std::unique_ptr<ds::Actor> actor = std::make_unique<ds::Actor>(transform, texture);
		scene.AddActor(std::move(actor));
	}

	// wait for keyboard enter to exit
	
	bool quit = false;
	SDL_Event event;
	float quitTime = engine.time.time + 10.0f;
	while (!quit)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		engine.Update();
		quit = (engine.Get<ds::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == ds::InputSystem::eKeyState::Pressed);
		scene.Update(engine.time.deltaTime);

		//std::cout << engine.time.time << std::endl;
		if (engine.time.time >= quitTime) quit = true;

		engine.time.timeScale = 20.0f;

		engine.Get<ds::Renderer>()->BeginFrame();

		scene.Draw(engine.Get<ds::Renderer>());
		//ds::Vector2 position{ 0, 0 };
		//engine.Get<ds::Renderer>()->Draw(texture, position, 0, ds::Vector2{ 0.5f, 0.5f });

		engine.Get<ds::Renderer>()->EndFrame();

		/*for (size_t i = 0; i < 50; i++)
		{
			SDL_Rect src{ 360, 460, 360, 460 };
			SDL_Rect dest{ ds::RandomRangeInt(0,800),ds::RandomRangeInt(0,600),64,96 };
			SDL_RenderCopy(renderer, texture, &src, &dest);
		}*/
	}

	SDL_Quit();

	return 0;
}