#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"
#include "GameComponent/PickupComponent.h"

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

	std::cout << dm.w << ", " << dm.h << std::endl;
	//1536x864

	// create scene
	scene = std::make_unique<ds::Scene>();
	scene->engine = engine.get();

	ds::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	ds::SetFilePath("../Resources");

	engine->Get<ds::EventSystem>()->Subscribe("add_score", std::bind(&Game::OnAddScore, this, std::placeholders::_1));

	REGISTER_CLASS(PlayerComponent);
	REGISTER_CLASS(EnemyComponent);
	REGISTER_CLASS(PickupComponent);

	/*for (size_t j = 0; j <= 18; j++)
	{
		for (size_t i = 0; i <= 46; i++)
		{
			auto actor = ds::ObjectFactory::Instance().Create<ds::Actor>("brick");
			actor->transform.position.x += 32 * (i);
			actor->transform.position.y += 32 * (j);
			scene->AddActor(std::move(actor));
		}
	}*/
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

	switch (state)
	{
	case Game::eState::Reset:
		Reset();
		break;
	case Game::eState::Title:
		Title();
		break;
	case Game::eState::StartGame:
		StartGame();
		break;
	case Game::eState::StartLevel:
		StartLevel();
		break;
	case Game::eState::Level:
		Level();
		break;
	case Game::eState::PlayerDead:
		PlayerDead();
		break;
	case Game::eState::GameOver:
		GameOver();
		break;
	default:
		break;
	}

	// update score
	auto scoreActor = scene->FindActor("Score");
	if (scoreActor)
	{
		scoreActor->GetComponent<ds::TextComponent>()->SetText(std::to_string(score));
	}

	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	
	engine->Get<ds::Renderer>()->BeginFrame();

	engine->Draw(engine->Get<ds::Renderer>());
	scene->Draw(engine->Get<ds::Renderer>());

	engine->Get<ds::Renderer>()->EndFrame();
}

void Game::Reset()
{
	scene->RemoveAllActors();

	rapidjson::Document document;
	bool success = ds::json::Load("title.txt", document);
	assert(success);

	scene->Read(document);

	state = eState::Title;
}

void Game::Title()
{
	if (engine->Get<ds::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == ds::InputSystem::eKeyState::Pressed)
	{
		auto title = scene->FindActor("Title");
		assert(title);
		title->active = false;

		state = eState::StartGame;
	}
}

void Game::StartGame()
{
	rapidjson::Document document;
	bool success = ds::json::Load("scene.txt", document);
	assert(success);

	scene->Read(document);

	ds::Tilemap tilemap;
	tilemap.scene = scene.get();
	success = ds::json::Load("map.txt", document);
	assert(success);
	tilemap.Read(document);
	tilemap.Create();

	state = eState::StartLevel;
}

void Game::StartLevel()
{
	stateTimer += engine->time.deltaTime;
	if (stateTimer >= 1)
	{
		auto player = ds::ObjectFactory::Instance().Create<ds::Actor>("Player");
		player->transform.position = { 768, 800 };
		scene->AddActor(std::move(player));

		spawnTimer = 2;
		state = eState::Level;
	}
}

void Game::Level()
{
	spawnTimer -= engine->time.deltaTime;
	if (spawnTimer <= 0)
	{
		spawnTimer = ds::RandomRange(2, 4);

		auto coin = ds::ObjectFactory::Instance().Create<ds::Actor>("coin");
		coin->transform.position = ds::Vector2{ ds::RandomRange(50, 1486), ds::RandomRange(750, 800) };
		scene->AddActor(std::move(coin));
	}
}

void Game::PlayerDead()
{

}

void Game::GameOver()
{

}

void Game::OnAddScore(const ds::Event& event)
{
	score += std::get<int>(event.data);
}
