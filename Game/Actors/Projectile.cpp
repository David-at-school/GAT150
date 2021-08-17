#include "Projectile.h"
#include "Engine.h"

void Projectile::Update(float dt)
{
	lifetime -= dt;
	std::cout << dt << std::endl;
	destroy = (lifetime <= 0);

	transform.position += ds::Vector2::Rotate(ds::Vector2::right, transform.rotation) * speed * dt;
	transform.position.x = ds::Wrap(transform.position.x, 0.0f, 1536.0f);
	transform.position.y = ds::Wrap(transform.position.y, 0.0f, 864.0f);

	//scene->engine->Get<ds::ParticleSystem>()->Create(transform.position, 1, 1, scene->engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("Images/particle01.png", scene->engine->Get<ds::Renderer>()), 1);

	Actor::Update(dt);
}

void Projectile::OnCollision(Actor* actor)
{
	
}
