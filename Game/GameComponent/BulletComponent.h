#pragma once
#include "Component/Component.h"
#include "Framework/EventSystem.h"

class BulletComponent : public ds::Component
{
	void Create() override;

	// Inherited via Component
	virtual void Update() override;
	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;

	virtual void OnCollisionEnter(const ds::Event& event);
	virtual void OnCollisionExit(const ds::Event& event);

public:
	virtual ~BulletComponent();
	std::unique_ptr<Object> Clone() const { return std::make_unique<BulletComponent>(*this); }

public:
	float speed{ 0 };
	bool init{ false };
};