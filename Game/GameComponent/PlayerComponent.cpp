#include "PlayerComponent.h"
#include "Engine.h"

using namespace ds;

PlayerComponent::~PlayerComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_exit", owner);
}

void PlayerComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&PlayerComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit", std::bind(&PlayerComponent::OnCollisionExit, this, std::placeholders::_1), owner);

	owner->scene->engine->Get<AudioSystem>()->AddAudio("hurt", "audio/hurt.wav");
	owner->scene->engine->Get<AudioSystem>()->AddAudio("coin", "audio/coin.wav");
}

void PlayerComponent::Update()
{
 	Vector2 mouseLocation = owner->scene->engine->Get<InputSystem>()->GetMousePosition();
	Vector2 direction = mouseLocation - owner->transform.position;
	owner->transform.rotation = direction.Angle() + ds::DegToRad(90);

	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == InputSystem::eKeyState::Pressed)
	{
		auto bullet = ds::ObjectFactory::Instance().Create<ds::Actor>("Bullet");
		bullet->transform = owner->transform;
		bullet->transform.scale = 1;
		owner->scene->AddActor(std::move(bullet));
	}
}

void PlayerComponent::OnCollisionEnter(const Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "ground"))
	{
		contacts.push_back(actor);
	}

	if (istring_compare(actor->tag, "enemy"))
	{
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("hurt");
	}

	if (istring_compare(actor->tag, "pickup"))
	{
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("coin");
		actor->destroy = true;

		Event event;
		event.name = "add_score";
		event.data = 10;
		owner->scene->engine->Get<EventSystem>()->Notify(event);
	}

	//std::cout << actor->tag << std::endl;
}

void PlayerComponent::OnCollisionExit(const Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "ground"))
	{
		contacts.remove(actor);
	}
}

bool PlayerComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool PlayerComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed);
	JSON_READ(value, jump);

	return true;
}
