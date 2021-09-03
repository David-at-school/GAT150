#include "BulletComponent.h"
#include "Engine.h"

using namespace ds;

BulletComponent::~BulletComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_exit", owner);
}

void BulletComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&BulletComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit", std::bind(&BulletComponent::OnCollisionExit, this, std::placeholders::_1), owner);

	owner->scene->engine->Get<AudioSystem>()->AddAudio("bounce", "audio/bounce.wav");
	owner->scene->engine->Get<AudioSystem>()->AddAudio("pop", "audio/pop.wav");
}

void BulletComponent::Update()
{
	if (!init)
	{
		init = true;
		Vector2 force = Vector2::zero;
		RBPhysicsComponent* physicsComponent = owner->GetComponent<RBPhysicsComponent>();
		assert(physicsComponent);

		physicsComponent->Update();
		force = Vector2::Rotate(Vector2::up, owner->transform.rotation) * 20;

		physicsComponent->ApplyForce(force);
	}
		std::cout << ds::RadToDeg(owner->transform.rotation) << std::endl;
}

bool BulletComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool BulletComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed);

	return true;
}

void BulletComponent::OnCollisionEnter(const Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "tile"))
	{
		actor->destroy = true;
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("pop");


		Event event;
		event.name = "add_score";
		event.data = 10;
		owner->scene->engine->Get<EventSystem>()->Notify(event);
	}
	else
	{

	}
}

void BulletComponent::OnCollisionExit(const Event& event)
{
	
}