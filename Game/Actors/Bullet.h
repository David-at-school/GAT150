#pragma once
#include "Projectile.h"

class Bullet : public Projectile
{
public:
	Bullet(const ds::Transform& transform, std::shared_ptr<ds::Texture> texture, float speed, float lifetime) : Projectile{ transform, texture, speed, lifetime } {}

	float lifetime{ 30 };
	std::string type;
private:
	float speed{ 500 };
};