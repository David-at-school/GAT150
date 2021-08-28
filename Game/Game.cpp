#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"

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
	//1536x864

	// create scene
	scene = std::make_unique<ds::Scene>();
	scene->engine = engine.get();

	ds::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	ds::SetFilePath("../Resources");

	REGISTER_CLASS(PlayerComponent);
	REGISTER_CLASS(EnemyComponent);

	rapidjson::Document document;
	bool success = ds::json::Load("scene.txt", document);
	assert(success);

	scene->Read(document);

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