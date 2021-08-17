#include "Missile.h"
#include "Engine.h"
#include "Enemy.h"

void Missile::Update(float dt)
{
	Projectile::Update(dt);

	ds::Vector2 direction;
	Enemy* target{ nullptr };

	for (Enemy* actor : scene->GetActors<Enemy>())
	{
		float shortestLength = 10000;
		if (actor)
		{
			if ((actor->transform.position - transform.position).Length() < shortestLength)
			{
				shortestLength = direction.Length();
				target = actor;
				direction = target->transform.position - transform.position;
			}
		}
	}

	if (target)
	{
		ds::Vector2 forward = ds::Vector2::Rotate(ds::Vector2::right, transform.rotation);

		float turnAngle = ds::Vector2::SignedAngle(forward, direction.Normalized());
		transform.rotation = transform.rotation + turnAngle * (3 * dt);
	}
}

void Missile::OnCollision(Actor* actor)
{

}
