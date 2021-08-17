#include "Player.h"
#include "Enemy.h"
#include "Math/MathUtils.h"
#include "Projectile.h"
#include "Missile.h"
#include "Bullet.h"
#include "Engine.h"
#include <memory>

Player::Player(const ds::Transform& transform, std::shared_ptr<ds::Texture> texture, float speed) : ds::Actor{ transform, texture }, speed{ speed } {}

void Player::Initialize()
{
	std::unique_ptr locator = std::make_unique<Actor>();
	locator->transform.localPosition = ds::Vector2{ 8,0 };
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = ds::Vector2{ 0,5 };
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = ds::Vector2{ 0,-5 };
	AddChild(std::move(locator));
	
	locator = std::make_unique<Actor>();
	locator->transform.localPosition = ds::Vector2{ -8,0 };
	locator->transform.localRotation = ds::Pi;
	AddChild(std::move(locator));

	std::unique_ptr turret = std::make_unique<Actor>(ds::Transform{}, scene->engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("Images/turret.png", scene->engine->Get<ds::Renderer>()));
	turret->transform.localPosition = ds::Vector2{ 8,0 };
	AddChild(std::move(turret));
}

void Player::Update(float dt)
{	
	//independant rotation
	children[4]->transform.localRotation += dt / 10;

	//controls
	float thrust = 0;
	ds::Vector2 acceleration;

	if (scene->engine->Get<ds::InputSystem>()->GetKeyState(SDL_SCANCODE_W) == ds::InputSystem::eKeyState::Held) transform.position.y -= speed * dt;
	if (scene->engine->Get<ds::InputSystem>()->GetKeyState(SDL_SCANCODE_A) == ds::InputSystem::eKeyState::Held) transform.position.x -= speed * dt;
	if (scene->engine->Get<ds::InputSystem>()->GetKeyState(SDL_SCANCODE_S) == ds::InputSystem::eKeyState::Held) transform.position.y += speed * dt;
	if (scene->engine->Get<ds::InputSystem>()->GetKeyState(SDL_SCANCODE_D) == ds::InputSystem::eKeyState::Held) transform.position.x += speed * dt;

	if (scene->engine->Get<ds::InputSystem>()->GetKeyState(SDL_SCANCODE_LSHIFT) == ds::InputSystem::eKeyState::Held) thrust *= 10;

	//rotation
	ds::Vector2 mouseLocation{ scene->engine->Get<ds::InputSystem>()->GetMousePosition() };

	transform.rotation = std::atan2(mouseLocation.y - transform.position.y, mouseLocation.x - transform.position.x);

	//wrapping
	transform.position.x = ds::Wrap(transform.position.x, 0.0f, 1536.0f);
	transform.position.y = ds::Wrap(transform.position.y, 0.0f, 864.0f);

	// fire
	fireTimer -= dt;
	secondaryTimer -= dt;
	if (fireTimer <= 0 && scene->engine->Get<ds::InputSystem>()->GetButtonState((int)ds::InputSystem::eMouseButton::Left) == ds::InputSystem::eKeyState::Held)
	{
		fireTimer = 0.25f;

		ds::Transform t = children[0]->transform;
		t.scale = 0.25f;

		std::unique_ptr<Bullet> projectile = std::make_unique<Bullet>(t, scene->engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("Images/playerBullet.png", scene->engine->Get<ds::Renderer>()), 600.0f, 3.0f);
		projectile->tag = "Player";
		projectile->type = "bullet";
		scene->AddActor(std::move(projectile));

		scene->engine->Get<ds::AudioSystem>()->PlayAudio("playerFire");
	}

	if (secondaryTimer <= 0 && scene->engine->Get<ds::InputSystem>()->GetButtonState((int)ds::InputSystem::eMouseButton::Right) == ds::InputSystem::eKeyState::Held)
	{
		secondaryTimer = 0.25f;

		ds::Transform t = children[0]->transform;
		t.scale = 0.25f;

		std::unique_ptr<Missile> projectile = std::make_unique<Missile>(t, scene->engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("Images/playerMissile.png", scene->engine->Get<ds::Renderer>()), 500.0f, 5.0f);
		projectile->type = "missile";
		projectile->tag = "Player";
		scene->AddActor(std::move(projectile));

		scene->engine->Get<ds::AudioSystem>()->PlayAudio("playerFire");
	}

	scene->engine->Get<ds::ParticleSystem>()->Create(children[3]->transform.position, 1, 1, scene->engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("Images/particle01.png", scene->engine->Get<ds::Renderer>()), 20, children[3]->transform.rotation, ds::DegToRad(15));

	Actor::Update(dt);
}

void Player::OnCollision(Actor* actor)
{
	if (dynamic_cast<Enemy*>(actor) || (dynamic_cast<Projectile*>(actor) && actor->tag == "Enemy"))
	{
		actor->destroy = true;
		ds::Event event;
		event.name = "PlayerDead";
		event.data = std::string("You... are... dead.");
		scene->engine->Get<ds::EventSystem>()->Notify(event);
		lives--;
		if (lives == 0)
		{
			destroy = true;

			std::vector<ds::Color> colors = { ds::Color::white, ds::Color::red, ds::Color::orange };
			scene->engine->Get<ds::ParticleSystem>()->Create(transform.position, 50, 5, scene->engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("Images/debrisStone_1.png", scene->engine->Get<ds::Renderer>()), 250);
			scene->engine->Get<ds::ParticleSystem>()->Create(transform.position, 25, 5, scene->engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("Images/particle01.png", scene->engine->Get<ds::Renderer>()), 250);
			scene->engine->Get<ds::ParticleSystem>()->Create(transform.position, 25, 5, scene->engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("Images/particle02.png", scene->engine->Get<ds::Renderer>()), 250);
			scene->engine->Get<ds::AudioSystem>()->PlayAudio("explosion");
		}
	}
}