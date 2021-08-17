#pragma once
#include "Projectile.h"

class Missile : public Projectile
{
public:
	Missile(const ds::Transform& transform, std::shared_ptr<ds::Texture> texture, float speed, float lifetime) : Projectile{ transform, texture, speed, lifetime } {}

	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

	float lifetime{ 40 };
	std::string type;
private:
	float speed{ 500 };
};