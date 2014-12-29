#include "Link.hpp"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Core/RenderScene.hpp>

using namespace AGE;

void Link::registerOctreeObject(const PrepareKey &key)
{
	assert(_lastOctreeObjectIndex < MAX_CPT_NUMBER);
	auto &b = _octreeObjects[_lastOctreeObjectIndex];
	assert(b.invalid());

	b = key;
	_octree->setPosition(_position, key);
	_octree->setScale(_scale, key);
	_octree->setOrientation(_orientation, key);
	++_lastOctreeObjectIndex;
}

void Link::unregisterOctreeObject(const PrepareKey &key)
{
	for (std::size_t i = 0; i < _lastOctreeObjectIndex; ++i)
	{
		auto &b = _octreeObjects[i];
		if (b == key)
		{
			_octree->removeElement(b);
			b = PrepareKey();
			if (_lastOctreeObjectIndex - 1 != i)
			{
				std::swap(b, _octreeObjects[_lastOctreeObjectIndex - 1]);
			}
			--_lastOctreeObjectIndex;
			return;
		}
	}
}

void Link::setPosition(const glm::vec3 &v)
{
	_userModification = true;
	internalSetPosition(v);
}

void Link::setScale(const glm::vec3 &v) {
	_userModification = true;
	internalSetScale(v);
}

void Link::setOrientation(const glm::quat &v) {
	_userModification = true;
	internalSetOrientation(v);
}

void Link::setForward(const glm::vec3 &v)
{
	_userModification = true;
	internalSetForward(v);
}

void Link::internalSetPosition(const glm::vec3 &v)
{
	_computeTrans = true;
	_position = v;
	auto ot = static_cast<RenderScene*>(_octree);
	for (std::size_t i = 0; i < _lastOctreeObjectIndex; ++i)
	{
		auto &e = _octreeObjects[i];
		assert(!e.invalid());
		ot->setPosition(_position, e);
	}
}

void Link::internalSetForward(const glm::vec3 &v)
{
	_computeTrans = true;
	glm::vec4 get = glm::mat4(glm::toMat4(_orientation) * glm::translate(glm::mat4(1), v))[3];
	_position.x = _position.x + get.x;
	_position.y = _position.y + get.y;
	_position.z = _position.z + get.z;
	auto ot = static_cast<RenderScene*>(_octree);
	for (std::size_t i = 0; i < _lastOctreeObjectIndex; ++i)
	{
		auto &e = _octreeObjects[i];
		assert(!e.invalid());
		ot->setPosition(_position, e);
	}
}

void Link::internalSetScale(const glm::vec3 &v) {
	_computeTrans = true;
	_scale = v;
	auto ot = static_cast<RenderScene*>(_octree);
	for (std::size_t i = 0; i < _lastOctreeObjectIndex; ++i)
	{
		auto &e = _octreeObjects[i];
		assert(!e.invalid());
		ot->setScale(_scale, e);
	}
}

void Link::internalSetOrientation(const glm::quat &v) {
	_computeTrans = true;
	_orientation = v;
	auto ot = static_cast<RenderScene*>(_octree);
	for (std::size_t i = 0; i < _lastOctreeObjectIndex; ++i)
	{
		auto &e = _octreeObjects[i];
		assert(!e.invalid());
		ot->setOrientation(_orientation, e);
	}
}

const glm::mat4 &Link::getTransform()
{
	if (_computeTrans)
	{
		_trans = glm::mat4(1);
		_trans = glm::translate(_trans, _position);
		_trans = _trans * glm::toMat4(_orientation);
		_trans = glm::scale(_trans, _scale);
		_computeTrans = false;
	}
	return _trans;
}

Link::Link()
{
	reset();
}

void Link::reset()
{
	_position = glm::vec3(0);
	_scale = glm::vec3(1);
	_orientation = glm::quat(glm::mat4(1));
	_trans = glm::mat4(1);
	_computeTrans = true;
	_octreeObjects.fill(PrepareKey());
	_parent = nullptr;
	_children.fill(nullptr);
	_lastOctreeObjectIndex = 0;
	_lastChildrenIndex = 0;
}

void Link::_setChild(Link *ptr)
{
	_children[_lastChildrenIndex++] = ptr;
}

void Link::_setParent(Link *ptr)
{
	_parent = ptr;
}

void Link::_removeChild(Link *ptr)
{
	assert(_lastChildrenIndex != 0);
	for (std::size_t i = 0; i < _lastOctreeObjectIndex; ++i)
	{
		auto &b = _children[i];
		if (b == ptr)
		{
			if (_lastOctreeObjectIndex - 1 != i)
			{
				std::swap(b, _children[_lastChildrenIndex - 1]);
			}
			--_lastChildrenIndex;
			return;
		}
	}
}

void Link::_removeParent()
{
	_parent = nullptr;
}
