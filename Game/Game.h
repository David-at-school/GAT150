#pragma once
#include "Engine.h"

class Game
{
public:
	enum class eState
	{
		Title,
		StartGame,
		StartLevel,
		Game,
		GameOver
	};

public:
	void Initialize();
	void Shutdown();

	void Update();
	void Draw();

	bool IsQuit() { return quit; };

private:
	void UpdateTitle();
	void UpdateStartLevel();
	void OnAddPoints(const ds::Event& event);
	void OnPlayerDead(const ds::Event& event);

public:
	std::unique_ptr<ds::Engine> engine;
	std::unique_ptr<ds::Scene> scene;
	SDL_DisplayMode dm;

private:
	bool quit = false;
	eState state = eState::Title;
	float stateTimer = 0.0f;
	unsigned int enemyCount = 10;

	size_t score = 0;
	size_t lives = 0;

	ds::AudioChannel musicChannel;
	std::shared_ptr<ds::Texture> textTexture;
	std::shared_ptr<ds::Texture> gameTextTexture;
	std::shared_ptr<ds::Font> font;
	std::shared_ptr<ds::Texture> texture;
	std::shared_ptr<ds::Texture> explosion;
};