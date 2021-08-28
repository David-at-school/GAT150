#pragma once
#include "Component/Component.h"
#include "Framework/EventSystem.h"
#include <list>

class PlayerComponent : public ds::Component
{
	void Create() override;

	// Inherited via Component
	virtual void Update() override;

	virtual void OnCollisionEnter(const ds::Event& event);
	virtual void OnCollisionExit(const ds::Event& event);

	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;

public:
	float speed{ 0 };

private:
	std::list<ds::Actor*> contacts;
};