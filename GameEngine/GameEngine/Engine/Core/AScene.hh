#pragma once

#include <Entities/EntityData.hh>
//#include <Systems/System.h>
#include <Utils/DependenciesInjector.hpp>
#include <memory>
#include <Components/ComponentRegistrar.hpp>
#include <Core/EntityIdRegistrar.hh>

#include <list>
#include <queue>
#include <map>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <Components/ComponentRegistrar.hpp>

class System;
class Engine;
class EntityFilter;

class AScene : public DependenciesInjector, public ComponentRegistrar, public EntityIdRegistrar
{
private:
	std::multimap<std::size_t, std::shared_ptr<System> >_systems;
	std::vector<EntityData>                             _pool;
	std::queue<std::size_t>                             _free;
	std::size_t                                         _entityNumber;
	std::map<unsigned short, std::list<EntityFilter*>> _filters;
public:
	AScene(std::weak_ptr<Engine> &&engine);
	virtual ~AScene();
	inline std::size_t getNumberOfEntities() { return _entityNumber; }
	virtual bool 			userStart() = 0;
	virtual bool 			userUpdate(double time) = 0;
	void 					update(double time);
	bool                    start();
	void filterSubscribe(unsigned short, EntityFilter* filter);
	void filterUnsubscribe(unsigned short, EntityFilter* filter);
	void informFilters(bool added, unsigned short id, Entity &&entity);

	Entity &createEntity();
	void destroy(const Entity &h);
	EntityData *get(const Entity &h);

	template <typename T>
	std::shared_ptr<T> addSystem(std::size_t priority)
	{
		auto tmp = std::make_shared<T>(std::static_pointer_cast<AScene>(shared_from_this()));
		if (!tmp->init())
			return nullptr;
		_systems.insert(std::make_pair(priority, tmp));
		return tmp;
	}

	template <typename T>
	std::shared_ptr<T> getSystem()
	{
		for (auto &e : _systems)
		{
			if (typeid(*e.second.get()).name() == typeid(T).name())
				return std::static_pointer_cast<T>(e.second);
		}
		return nullptr;
	}

	template <typename T>
	void deleteSystem()
	{
		for (auto &e : _systems)
		{
			if (typeid(*e.second.get()).name() == typeid(T).name())
			{
				delete e.second;
				_systems.erase(e);
				return;
			}
		}
	}


	template <typename T>
	bool activateSystem()
	{
		for (auto &e : _systems)
		{
			if (typeid(*e.second.get()).name() == typeid(T).name())
				return e.second->setActivation(true);
		}
		return false;
	}

	template <typename T>
	bool deactivateSystem()
	{
		for (auto &e : _systems)
		{
			if (typeid(*e.second.get()).name() == typeid(T).name())
				return e.second->setActivation(false);
		}
		return false;
	}

	template <typename Archive>
	void save(std::ofstream &s)
	{
		Archive ar(s);
		unsigned int size = 0;
		for (auto &e : _pool)
		{
			if (e.getFlags() & EntityData::ACTIVE)
			{
				++size;
			}
		}
		ar(cereal::make_nvp("Number_of_serialized_entities", size));
		for (auto &e : _pool)
		{
			if (e.getFlags() & EntityData::ACTIVE)
			{
				ar(cereal::make_nvp("Entity_" + std::to_string(e.getHandle().getId()), e));
			}
		}
	}

	template <typename Archive>
	void load(std::ifstream &s)
	{
		Archive ar(s);
		unsigned int size = 0;
		ar(size);
		for (unsigned int i = 0; i < size; ++i)
		{
			auto e = createEntity();
			ar(*e.get());
		}
		updateEntityHandles();
	}

	////////////////////////
	//////
	// Component operation


	std::vector<std::vector<Component::Base>> _components;
	std::vector<std::vector<std::uint32_t>> _componentsRefs;


	template <typename T, typename... Args>
	T *addComponent(Entity &&entity, Args &&...args)
	{
		// get the component type ID
		unsigned short id = T::getTypeId();

		// if entity already have component, return it
		if (entity->hasComponent<T>())
		{
			// TODO -> Get component
			return  nullptr;
		}
		// else if entity components array is to small, resize it
		else if (_components.size() <= id)
		{
			_components.resize(id + 1);
		}

		_componentsRef[id].push_back(entity.getId());
		_components[id].push_back(T);

		//init component
		auto &component = static_cast<T>(_components[id].back());
		component.init(std::forward<Args>(args)...);
		entity->getCode().add(id + MAX_TAG_NUMBER);
		informFilters(true, id + MAX_TAG_NUMBER, std::move(entity));
		return &component;
	}

};
