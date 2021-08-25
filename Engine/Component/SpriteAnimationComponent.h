#pragma once
#include "SpriteComponent.h"


namespace ds
{
	class SpriteAnimationComponent : public SpriteComponent
	{
	public:
		void Update() override;
		void Draw(Renderer* renderer) override;

		// Inherited via ISerializable
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public:
		int frame{ 0 };
		int fps{ 0 };
		int numFramesX{ 0 };
		int numFramesY{ 0 };

		float frameTime{ 0.0f };
		float frameTimer{ 0.0f };

		SDL_Rect rect;
	};
}