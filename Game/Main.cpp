#include "Engine.h"
#include "Graphics/Font.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

int main(int, char**)
{
	std::string str1 = "HELLO";
	std::string str2 = "hello";

	std::transform(str1.begin(), str1.end(), str1.begin(), [](char c)
		{
			return std::tolower(c);
		});

	if (str1 == str2) std::cout << "match\n";
	system("pause");

	ds::Engine engine;
	engine.Startup();
	engine.Get<ds::Renderer>()->Create("Gat1Fiddy", 800, 600);

	ds::Scene scene;
	scene.engine = &engine;

	ds::SetFilePath("../Resources");

	engine.Get<ds::AudioSystem>()->AddAudio("explosion", "Audio/explosion.wav");
	engine.Get<ds::AudioSystem>()->AddAudio("music", "Audio/music.mp3");
	ds::AudioChannel channel = engine.Get<ds::AudioSystem>()->PlayAudio("music", 1.0f, 0.25f, true);

	std::shared_ptr<ds::Texture> texture = engine.Get<ds::ResourceSystem>()->Get<ds::Texture>("duck.gif", engine.Get<ds::Renderer>());
	std::shared_ptr<ds::Texture> explosion = engine.Get<ds::ResourceSystem>()->Get<ds::Texture>("particle02.png", engine.Get<ds::Renderer>());

	for (size_t i = 0; i < 1; i++)
	{
		ds::Transform transform{ {ds::RandomRange(0,800), ds::RandomRange(0,600)}, ds::RandomRange(0,359), 0.1f };
		std::unique_ptr<ds::Actor> actor = std::make_unique<ds::Actor>(transform, texture);
		scene.AddActor(std::move(actor));
	}

	// get font from resource system
	int size = 16;
	std::shared_ptr<ds::Font> font = engine.Get<ds::ResourceSystem>()->Get<ds::Font>("fonts/spqr.ttf", &size);

	// create font texture
	std::shared_ptr<ds::Texture> textTexture = std::make_shared<ds::Texture>(engine.Get<ds::Renderer>());
	// set font texture with font surface
	textTexture->Create(font->CreateSurface("hello world", ds::Color{ 1, 1, 1 }));
	// add font texture to resource system
	engine.Get<ds::ResourceSystem>()->Add("textTexture", textTexture);


	// wait for keyboard enter to exit
	bool quit = false;
	SDL_Event event;
	float quitTime = engine.time.time + 10.0f;
	while (!quit)
	{
		quit = (engine.Get<ds::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == ds::InputSystem::eKeyState::Pressed);
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		ds::Vector2 position = engine.Get<ds::InputSystem>()->GetMousePosition();

		if (engine.Get<ds::InputSystem>()->GetButtonState((int)ds::InputSystem::eMouseButton::Left) == ds::InputSystem::eKeyState::Pressed)
		{
			ds::Transform transform{ {position.x, position.y}, ds::RandomRange(0,359), 0.1f };
			engine.Get<ds::ParticleSystem>()->Create(transform.position, 10, 25, explosion, 10);
			float pitch = ds::RandomRange(0.0f, 0.5f);
			float musicPitch = ds::RandomRange(-10.0f, 10.0f);
			engine.Get<ds::AudioSystem>()->PlayAudio("explosion", 1, pitch);
			channel.SetPitch(musicPitch);
			std::cout << pitch << std::endl;
			std::cout << musicPitch << std::endl;
		}

		engine.Update();
		scene.Update(engine.time.deltaTime);


		//std::cout << engine.time.time << std::endl;
		//if (engine.time.time >= quitTime) quit = true;

		engine.time.timeScale = 20.0f;

		engine.Get<ds::Renderer>()->BeginFrame();

		scene.Draw(engine.Get<ds::Renderer>());
		engine.Draw(engine.Get<ds::Renderer>());

		ds::Transform t;
		t.position = { 30, 30 };
		engine.Get<ds::Renderer>()->Draw(textTexture, t);

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