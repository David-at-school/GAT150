#include "RBPhysicsComponent.h"
#include "Engine.h"

namespace ds
{
	RBPhysicsComponent::RBPhysicsComponent(const RBPhysicsComponent& other)
	{
		data = other.data;
	}

	RBPhysicsComponent::~RBPhysicsComponent()
	{
		if (body)
		{
			owner->scene->engine->Get<PhysicsSystem>()->DestroyBody(body);
		}
	}

	void RBPhysicsComponent::Update()
	{
		if (!body)
		{
			if (isCircle)
			{
				body = owner->scene->engine->Get<PhysicsSystem>()->CreateBallBody(owner->transform.position, owner->transform.rotation, data, owner);
			}
			else
			{
				body = owner->scene->engine->Get<PhysicsSystem>()->CreateBody(owner->transform.position, owner->transform.rotation, data, owner);
			}
			body->SetGravityScale(data.gravityScale);
			body->SetLinearDamping(0.5f);
		}

		owner->transform.position = PhysicsSystem::WorldToScreen(body->GetPosition());
		owner->transform.rotation = ds::RadToDeg(body->GetAngle());
		velocity = body->GetLinearVelocity();
	}

	void RBPhysicsComponent::ApplyForce(const Vector2& force)
	{
		if (body)
		{
			body->ApplyForceToCenter(force, true);
		}
	}

	bool RBPhysicsComponent::Write(const rapidjson::Value& value) const
	{
		return false;
	}

	bool RBPhysicsComponent::Read(const rapidjson::Value& value)
	{
		JSON_READ(value, data.isDynamic);
		JSON_READ(value, data.isSensor);
		JSON_READ(value, data.lockAngle);
		JSON_READ(value, data.size);
		JSON_READ(value, data.density);
		JSON_READ(value, data.friction);
		JSON_READ(value, data.restitution);
		JSON_READ(value, data.gravityScale);

		JSON_READ(value, isCircle);

		return true;
	}
}