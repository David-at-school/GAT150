#include "Game.h"

void Game::Initialize()
{
	// create engine
	engine = std::make_unique<ds::Engine>();
	engine->Startup();

	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
	}

	engine->Get<ds::Renderer>()->Create("Gat1Fiddy", dm.w, dm.h);

	// create scene
	scene = std::make_unique<ds::Scene>();
	scene->engine = engine.get();

	ds::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	ds::SetFilePath("../Resources");


	std::unique_ptr<ds::Actor> actor = std::make_unique <ds::Actor>(ds::Transform{ ds::Vector2{800, 400}, 0, 1 });
	{
		ds::SpriteAnimationComponent* component = actor->AddComponent<ds::SpriteAnimationComponent>();
		component->texture = engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("Images/duck.png", engine->Get<ds::Renderer>());
		component->fps = 30;
		component->numFramesX = 5;
		component->numFramesY = 3;
	}

	/*std::unique_ptr<ds::Actor> actor = std::make_unique<ds::Actor>(ds::Transform({700,550}, 0.0f, 0.5f));
	{
 		ds::SpriteComponent* component = actor->AddComponent<ds::SpriteComponent>();

		component->texture = engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("Images/character.png", engine->Get<ds::Renderer>());
	}
	{
		ds::PhysicsComponent* component = actor->AddComponent<ds::PhysicsComponent>();

		component->ApplyForce(ds::Vector2::right * 200);
	}*/
	scene->AddActor(std::move(actor));
}

void Game::Shutdown()
{
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update()
{
	engine->Update();

	quit = engine->Get<ds::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == ds::InputSystem::eKeyState::Pressed;

	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	
	engine->Get<ds::Renderer>()->BeginFrame();

	engine->Draw(engine->Get<ds::Renderer>());
	scene->Draw(engine->Get<ds::Renderer>());

	engine->Get<ds::Renderer>()->EndFrame();
}