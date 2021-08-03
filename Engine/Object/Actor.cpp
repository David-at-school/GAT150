#include "Actor.h"
#include <algorithm>

namespace ds
{
	void Actor::Update(float dt)
	{
		transform.Update();
		std::for_each(children.begin(), children.end(), [](auto& child) { child->transform.Update(child->parent->transform.matrix); });
	}

	void Actor::Draw()
	{

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
		return 0;
	}
}