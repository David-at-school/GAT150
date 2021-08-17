#pragma once
#include "Object/Actor.h"
#include <string>

class Projectile : public ds::Actor
{
public:
	Projectile(const ds::Transform& transform, std::shared_ptr<ds::Texture> texture, float speed, float lifetime) : ds::Actor{ transform, texture }, speed{ speed }, lifetime{ lifetime } {}

	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

	std::string type;
	float lifetime;
private:
	float speed;
};