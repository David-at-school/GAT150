#pragma once
#include "PhysicsComponent.h"
#include "Physics/PhysicsSystem.h"

namespace ds
{
	class RBPhysicsComponent : public PhysicsComponent
	{
		void Update() override;
		void ApplyForce(const Vector2& force) override;

		// Inherited via ISerializable
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public:
		PhysicsSystem::RigidBodyData data;
		b2Body* body{ nullptr };
	};
}