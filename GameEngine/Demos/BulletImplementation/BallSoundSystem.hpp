#pragma once

#include <Systems/System.h>
#include <Entities/EntityData.hh>
#include <Components/AudioListener.hpp>
#include <Components/AudioEmitter.hpp>
#include <Audio/AudioManager.hh>
#include <Core/Engine.hh>
#include <Utils/MatrixConversion.hpp>
#include <Components/Collision.hpp>

#include <MyTags.hpp>

class BallSoundSystem : public System
{
public:
	BallSoundSystem(std::weak_ptr<AScene> &&scene)
		: System(std::move(scene))
		, _balls(std::move(scene))
		, _heros(std::move(scene))
	{
		_name = "ball_sound_system";
	}
	virtual ~BallSoundSystem(){}
protected:
	EntityFilter _balls;
	EntityFilter _heros;

	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		for (auto e : _balls.getCollection())
		{
			auto ae = e->getComponent<Component::AudioEmitter>();
			auto force = e->getComponent<Component::Collision>()->force;
//			std::cout << force << std::endl;
			if (force > 1.0f)
			{
				ae->play("collision", false);
				ae->getAudio("collision")->channel->setVolume(force / 10.0f);
			}
		}

		for (auto e : _heros.getCollection())
		{
			auto ae = e->getComponent<Component::AudioEmitter>();
			auto c = e->getComponent<Component::FPController>();
			if (c->justArriveOnFloor)
				ae->play("arriveOnFloor", true);
			if (c->justJump)
				ae->play("jump", true);
		}
	}

	virtual bool initialize()
	{
		_balls.requireTag(BALL_TAG);
//		_balls.requireComponent<Component::AudioEmitter>();
		_balls.requireComponent<Component::Collision>();
		_heros.requireComponent<Component::FPController>();
		_heros.requireComponent<Component::AudioEmitter>();
		return true;
	}
};