#pragma once
//audio
//#include "Audio/AudioSystem.h"

//core
#include "Core/FileSystem.h"

//math
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Math/MathUtils.h"
#include "Math/Random.h"
#include "Math/Transform.h"

// framework
#include "Framework/EventSystem.h"

//resource
#include "Resource/ResourceSystem.h"

//object
#include "Object/Actor.h"
#include "Object/Scene.h"

#define NOMINMAX
#include <vector>
#include <memory>
#include <algorithm>

namespace ds
{
	class Engine
	{
	public:
		void Startup();
		void Shutdown();

		void Update(float dt);
		void Draw();

		template<typename T>
		T* Get();

	private:
		std::vector<std::unique_ptr<System>> systems;

	};

	template<typename T>
	inline T* Engine::Get()
	{
		for (auto& system : systems)
		{
			if (dynamic_cast<T*>(system.get())) return dynamic_cast<T*>(system.get());
		}

		return nullptr;
	}
}