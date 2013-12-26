#include "CameraComponent.hh"
#include <Core/Engine.hh>
#include <ResourceManager/ResourceManager.hh>

namespace Component
{
	CameraComponent::CameraComponent(Engine &engine, Handle &entity)
		: ComponentBase<CameraComponent>(engine, entity, "CameraComponent"),
		_projection(glm::perspective(55.0f, 16.0f / 9.0f, 0.1f, 2000.0f)),
		_lookAtTransform(1)
	{}

	CameraComponent::~CameraComponent(void)
	{}

	void CameraComponent::attachSkybox(const std::string &name, const std::string &cubeMapShader)
	{
		_skybox = _engine.getInstance<Resources::ResourceManager>().getResource(name);
		_cubeMapShader = cubeMapShader;
	}

	void CameraComponent::dettachSkybox()
	{
		_skybox = nullptr;
	}

	glm::mat4         &CameraComponent::setProjection()
	{
		_entity->getFlags() & Entity::HAS_MOVED;
		return (_projection);
}

	glm::mat4 		  &CameraComponent::getProjection()
	{
		return (_projection);
	}

	SmartPointer<Resources::CubeMap> CameraComponent::getSkybox()
	{
		return _skybox;
	}

	const std::string &CameraComponent::getSkyboxShader() const
	{
		return _cubeMapShader;
	}
};
