#include "Actor.h"
#include "Graphics/Renderer.h"
#include "Math/MathUtils.h"
#include "Math/Random.h"
#include <algorithm>

namespace ds
{
	void Actor::Update(float dt)
	{
		transform.rotation += 45.0f * dt;
		transform.position.x += 25.0f * dt;
		transform.position.x = Wrap(transform.position.x, 0.0f, 800.0f);
		transform.position.y = Wrap(transform.position.y, 0.0f, 600.0f);

		transform.Update();
		std::for_each(children.begin(), children.end(), [](auto& child) { child->transform.Update(child->parent->transform.matrix); });
	}

	void Actor::Draw(Renderer* renderer)
	{
		renderer->Draw(texture, transform);
	}

	void Actor::OnCollision(Actor* actor)
	{

	}

	void Actor::AddChild(std::unique_ptr<Actor> actor)
	{
		actor->parent = this;

		children.push_back(std::move(actor));
	}

	float Actor::GetRadius()
	{
		//return std::max(texture->GetSize().x/2, texture->GetSize().y/2);

		return (texture) ? texture->GetSize().Length() * 0.5f : 0;
	}
}