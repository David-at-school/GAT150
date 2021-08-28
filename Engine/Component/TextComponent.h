#pragma once
#include "GraphicsComponent.h"

namespace ds
{
	class Texture;
	class Font;

	class TextComponent : public GraphicsComponent
	{
	public:
		virtual void Update() override;
		virtual void Draw(Renderer* renderer) override;

		void SetText(const std::string& text);

		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public:
		std::string fontName;
		int fontSize{ 12 };
		Color color;
		std::string text;

		std::shared_ptr<Font> font;
		std::shared_ptr<Texture> texture;
	};

}