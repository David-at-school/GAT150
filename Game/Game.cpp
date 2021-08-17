#include "Game.h"
#include "Actors/Player.h"
#include "Actors/Enemy.h"
#include "Actors/Projectile.h"

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

	engine->Get<ds::AudioSystem>()->AddAudio("explosion", "Audio/explosion.wav");
	engine->Get<ds::AudioSystem>()->AddAudio("music", "Audio/music.mp3");
	musicChannel = engine->Get<ds::AudioSystem>()->PlayAudio("music", 1.0f, 0.75f, true);

	explosion = engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("particle02.png", engine->Get<ds::Renderer>());

	/*texture = engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("duck.gif", engine->Get<ds::Renderer>());
	for (size_t i = 0; i < 1; i++)
	{
		ds::Transform transform{ {ds::RandomRange(0,(float)dm.w), ds::RandomRange(0,(float)dm.h)}, ds::RandomRange(0,359), 0.1f };
		std::unique_ptr<ds::Actor> actor = std::make_unique<ds::Actor>(transform, texture);
		scene->AddActor(std::move(actor));
	}*/

	// get font from resource system
	int size = 16;
	font = engine->Get<ds::ResourceSystem>()->Get<ds::Font>("fonts/spqr.ttf", &size);

	// create font texture
	textTexture = std::make_shared<ds::Texture>(engine->Get<ds::Renderer>());
	gameTextTexture = std::make_shared<ds::Texture>(engine->Get<ds::Renderer>());
	// set font texture with font surface
	textTexture->Create(font->CreateSurface("Press Space to begin.\nLeft Click to fire bullets.\nRight Click to fire missiles.\nEscape to exit.", ds::Color{ 1, 1, 1 }));
	// add font texture to resource system
	engine->Get<ds::ResourceSystem>()->Add("textTexture", textTexture);

	// game
	engine->Get<ds::AudioSystem>()->AddAudio("enemyFire", "Audio/enemyFire.wav");
	engine->Get<ds::AudioSystem>()->AddAudio("playerFire", "Audio/playerFire.wav");

	engine->Get<ds::EventSystem>()->Subscribe("AddPoints", std::bind(&Game::OnAddPoints, this, std::placeholders::_1));
	engine->Get<ds::EventSystem>()->Subscribe("PlayerDead", std::bind(&Game::OnPlayerDead, this, std::placeholders::_1));

	state = eState::Title;
}

void Game::Shutdown()
{
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update()
{
	engine->Update();

	float dt = engine->time.deltaTime;
	stateTimer += dt;

	switch (state)
	{
	case Game::eState::Title:
		if (engine->Get<ds::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == ds::InputSystem::eKeyState::Pressed)
		{
			state = eState::StartGame;
		}
		break;
	case Game::eState::StartGame:
		score = 0;
		lives = 3;
		state = eState::StartLevel;
		break;
	case Game::eState::StartLevel:
	{
		UpdateStartLevel();
		state = eState::Game;
	}
		break;
	case Game::eState::Game:
		if (scene->GetActors<Enemy>().size() == 0)
		{
			enemyCount += 1;
			int countRemaining = enemyCount;
			for (size_t i = 0; i < ds::RandomRangeInt(0, enemyCount); i++)
			{
				countRemaining -= 1;
				scene->AddActor(std::make_unique<Enemy>(ds::Transform{ ds::Vector2{ds::RandomRange(0,dm.w), ds::RandomRange(0,dm.h)}, ds::RandomRange(0, ds::TwoPi), 0.25f }, engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("Images/hunter.png", scene->engine->Get<ds::Renderer>()), 150.0f));
			}
			if (countRemaining != 0)
			{
				for (size_t i = 0; i < countRemaining; i++)
				{
					scene->AddActor(std::make_unique<Enemy>(ds::Transform{ ds::Vector2{ds::RandomRange(0,dm.w), ds::RandomRange(0,dm.h)}, ds::RandomRange(0, ds::TwoPi), 0.25f }, engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("Images/tracker.png", scene->engine->Get<ds::Renderer>()), 100.0f));
				}
			}
		}
		break;
	case Game::eState::GameOver:
		break;
	default:
		break;
	}

	quit = engine->Get<ds::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == ds::InputSystem::eKeyState::Pressed;

	engine->time.timeScale = 1.0f;

	scene->Update(engine->time.deltaTime);

	ds::Vector2 position = engine->Get<ds::InputSystem>()->GetMousePosition();

	/*if (engine->Get<ds::InputSystem>()->GetButtonState((int)ds::InputSystem::eMouseButton::Left) == ds::InputSystem::eKeyState::Held)
	{
		ds::Transform transform{ {position.x, position.y}, ds::RandomRange(0,359), 0.1f };
		//engine->Get<ds::ParticleSystem>()->Create(transform.position, 100, 25, explosion, 10);
		float pitch = ds::RandomRange(0.0f, 0.33f);
		float musicPitch = ds::RandomRange(0.5f, 1.0f);
		engine->Get<ds::AudioSystem>()->PlayAudio("explosion", 1, pitch);
		musicChannel.SetPitch(musicPitch);
		std::cout << pitch << std::endl;
		std::cout << musicPitch << std::endl;
	}*/
}

void Game::Draw()
{
	ds::Transform title;
	ds::Transform gameText;
	title.position = { dm.w / 2, dm.h / 2 };
	gameText.position = { dm.w / 2, 50 };
	switch (state)
	{
	case Game::eState::Title:
		lives = 3;
		score = 0;
		//graphics.SetColor(ds::Color::blue);
		//graphics.DrawString(400 + 400 * std::cos(stateTimer), 300 + 300 * std::sin(stateTimer), "Better Assassin's Creed");

		//graphics.DrawString(410, 450, "Press Space to Start");
		break;
	case Game::eState::StartGame:
		break;
	case Game::eState::StartLevel:
		break;
	case Game::eState::Game:
		gameTextTexture->Create(font->CreateSurface(("Lives: " + std::to_string(lives) + " Score: " + std::to_string(score)), ds::Color{ 1, 1, 1 }));
		engine->Get<ds::ResourceSystem>()->Add("gameTextTexture", gameTextTexture);

		break;
	case Game::eState::GameOver:
		//graphics.SetColor(ds::Color::red);
		//graphics.DrawString(410, 450, "GAME OVER");
		scene->RemoveAllActors();
		state = eState::Title;
		break;
	default:
		break;
	}

	engine->Get<ds::Renderer>()->BeginFrame();
	
	if (state == eState::Title) {
		engine->Get<ds::Renderer>()->Draw(textTexture, title);
	}

	if (state == eState::Game) {
		engine->Get<ds::Renderer>()->Draw(gameTextTexture, gameText);
	}

	//graphics.SetColor(ds::Color::white);
	//graphics.DrawString(10, 10, std::to_string(score).c_str());
	//graphics.DrawString(10, 20, std::to_string(lives).c_str());

	engine->Draw(engine->Get<ds::Renderer>());
	scene->Draw(engine->Get<ds::Renderer>());

	//ds::Vector2 position{ 0, 0 };
	//engine.Get<ds::Renderer>()->Draw(texture, position, 0, ds::Vector2{ 0.5f, 0.5f });

	engine->Get<ds::Renderer>()->EndFrame();
}

void Game::UpdateTitle()
{
	if (engine->Get<ds::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == ds::InputSystem::eKeyState::Pressed)
	{
		UpdateStartLevel();
		//state = eState::StartGame;
	}
}

void Game::UpdateStartLevel()
{
	scene->AddActor(std::make_unique<Player>(ds::Transform{ ds::Vector2{dm.w /2, dm.h /2}, 0, 0.25f  }, engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("Images/player.png", scene->engine->Get<ds::Renderer>()), 500.0f));
	for (size_t i = 0; i < 10; i++)
	{
		scene->AddActor(std::make_unique<Enemy>(ds::Transform{ ds::Vector2{ds::RandomRange(0,dm.w), ds::RandomRange(0,dm.h)}, ds::RandomRange(0, ds::TwoPi), 0.25f }, engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("Images/hunter.png", scene->engine->Get<ds::Renderer>()), 100.0f));
	}
}

void Game::OnAddPoints(const ds::Event& event)
{
	score += std::get<int>(event.data);
}

void Game::OnPlayerDead(const ds::Event& event)
{
	lives -= 1;
	std::cout << std::get<std::string>(event.data) << std::endl;
	if (lives == 0)
	{ 
		state = eState::GameOver;
	}
}
