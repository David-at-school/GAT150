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

		// Inherited via ISerializable
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;
	};
}