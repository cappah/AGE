#include "PhysicsRoom.hh"
#include <MediaFiles/AssetsManager.hpp>
#include <Components/MeshRenderer.hh>
#include <glm/gtc/matrix_transform.hpp>
#include <Components/RigidBody.hpp>
#include <Sprite/SpriteManager.hh>
#include <Components/SpriteComponent.hh>
#include <Systems/EntityPlacingSystem.hpp>
#include <Systems/TransformationRegisterSystem.hpp>
#include <Systems/HotZoneSystem.hpp>
#include <Components/CollisionBody.hpp>
#include <Components/SpotLight.hh>

	PhysicsRoom::PhysicsRoom(std::weak_ptr<AScene> scene)
		: Room(scene)
	{
	}

	PhysicsRoom::~PhysicsRoom()
	{
		auto scene = _scene.lock();
		if (!scene)
			return;
	}

	bool PhysicsRoom::init()
	{
		// load
		auto s = _scene.lock();
		hotZonePhysicsProjection = createHotZone("Physics->Projection", "HZ-projection-physics");
		return true;
	}

	bool PhysicsRoom::_enable()
	{
		auto scene = _scene.lock();
		{
			glm::mat4 t(1);
			t = glm::translate(t, glm::vec3(-14.5, -0.1f, -34.0f));
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__cube");
			auto w = 10;
			auto h = 6;
			for (auto i = 0; i < h; i++)
			{
				for (auto j = 0; j < w; ++j)
				{
						auto e = scene->createEntity();
						e->setLocalTransform(t);
						e->setLocalTransform(glm::scale(t, glm::vec3(0.6f)));
						e->setLocalTransform(glm::translate(e->getLocalTransform(), glm::vec3(0, h - i + 0.001f, ((float)(w)-(float)(j)) + 4.0f)));
						e->addComponent<Component::MeshRenderer>(meshObj)->setShader("MaterialBasic");
						e->addComponent<Component::RigidBody>(1.0f)->setCollisionShape(Component::RigidBody::BOX);
						cubes.push_back(e);
				}
			}
		}
		{
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__cube");

			for (auto i = 0; i < 3; ++i)
			{
				auto light = scene->createEntity();
				//light->addComponent<Component::MeshRenderer>(meshObj)->setShader("MaterialBasic");
				auto l = light->addComponent<Component::SpotLight>();
				l->lightData.colorRange = glm::vec4(0.4f, 0.6f, 1.0f, 100.0f);
				l->lightData.positionPower.w = 10.0f;
				l->projection = glm::perspective(40.0f, 1.0f, 0.1f, 100.0f);
				l->lightData.shadowId = 1;
				light->addComponent<Component::EntityPlacable>("physics-room-spotlight-" + std::to_string(i));
				light->addComponent<Component::TransformationRegister>("physics-room-spotlight-" + std::to_string(i));
				lights.push_back(light);
			}
			for (auto i = 3; i < 5; ++i)
			{
				auto light = scene->createEntity();
				auto l = light->addComponent<Component::PointLight>();
				l->lightData.colorRange = glm::vec4(0.4f, 0.6f, 1.0f, 6.0f); // distance
				l->lightData.positionPower.w = 3.0f; // intensite
				light->addComponent<Component::TransformationRegister>("physics-room-pointlight-" + std::to_string(i));
				light->addComponent<Component::EntityPlacable>("physics-room-pointlight-" + std::to_string(i));
				lights.push_back(light);
			}
			//lights[0]->getComponent<Component::SpotLight>()->lightData.colorRange = glm::vec4(0.4f, 0.6f, 1.0f, 10.0f);
			//lights[1]->getComponent<Component::SpotLight>()->lightData.colorRange = glm::vec4(0.9f, 0.4f, 0.3f, 10.0f);
			//lights[2]->getComponent<Component::SpotLight>()->lightData.colorRange = glm::vec4(0.1f, 0.2f, 0.6f, 10.0f);
			lights[3]->getComponent<Component::PointLight>()->lightData.colorRange = glm::vec4(0.6f, 0.6f, 0.2f, 16.0f);
			lights[4]->getComponent<Component::PointLight>()->lightData.colorRange = glm::vec4(0.6f, 0.9f, 0.2f, 5.0f);

		}
		return true;
	}

	bool PhysicsRoom::_disable()
	{
		auto scene = _scene.lock();
		for (auto e : cubes)
		{
			scene->destroy(e);
		}
		for (auto e : lights)
		{
			scene->destroy(e);
		}
		cubes.clear();
		lights.clear();
		return true;
	}