#include "SpriteComponent.h"
#include "Graphics/Renderer.h"
#include "Object/Actor.h"

namespace ds
{
	void SpriteComponent::Update()
	{

	}

	void SpriteComponent::Draw(Renderer* renderer)
	{
		if (texture)
		{
			renderer->Draw(texture, owner->transform);
		}
	}
}