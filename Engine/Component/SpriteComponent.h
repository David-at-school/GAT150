#pragma once
#include "GraphicsComponent.h"
#include "Graphics/Texture.h"
#include <memory>
#include <vector>

namespace ds
{
	class SpriteComponent : public GraphicsComponent
	{
	public:
		void Update() override;
		void Draw(Renderer* renderer) override;

	public:
		std::shared_ptr<Texture> texture;
	};
}