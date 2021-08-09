#include "Engine.h"

namespace ds
{
	void Engine::Startup()
	{
		systems.push_back(std::make_unique<Renderer>());
		systems.push_back(std::make_unique<EventSystem>());
		systems.push_back(std::make_unique<ResourceSystem>());
		systems.push_back(std::make_unique<InputSystem>());

		std::for_each(systems.begin(), systems.end(), [](auto& system) {system->Startup(); });
	}

	void Engine::Shutdown()
	{
		std::for_each(systems.begin(), systems.end(), [](auto& system) {system->Shutdown(); });
	}

	void Engine::Update()
	{
		time.Tick();
		std::for_each(systems.begin(), systems.end(), [](auto& system) {system->Update(0); });
	}

	void Engine::Draw()
	{

	}
}