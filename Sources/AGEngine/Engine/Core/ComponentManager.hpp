#pragma once

#include <Utils/ObjectPool.hpp>
#include <memory>
#include "Components/Component.hh"

namespace AGE
{
	class ComponentManager
	{
	private:
		std::vector<std::shared_ptr<BufferPool>> _pools;
	public:
		template <typename T>
		void createComponentPool()
		{
			auto id = Component<T>::getTypeId();

			if (_pools.size() <= id)
			{
				_pools.resize(id + 1);
				_pools[id] = std::make_shared<ObjectPool<T>>();
			}
			else if (_pools[id] == nullptr)
			{
				_pools[id] = std::make_shared<ObjectPool<T>>();
			}
		}

		template <typename T, typename... Args>
		T *createComponent(const Entity &entity, Args &&...args)
		{
			auto id = Component<T>::getTypeId();

			if (_pools.size() <= id)
			{
				_pools.resize(id + 1);
				_pools[id] = std::make_shared<ObjectPool<T>>();
			}
			else if (_pools[id] == nullptr)
			{
				_pools[id] = std::make_shared<ObjectPool<T>>();
			}
			auto pool = (ObjectPool<T>*)_pools[id].get();
			auto ptr = pool->create();
			ptr->_typeId = id;
			ptr->entity = entity;
			return ptr;
		}

		void *allocateComponent(ComponentType id);
		void deleteComponent(ComponentBase *component);
	};
}