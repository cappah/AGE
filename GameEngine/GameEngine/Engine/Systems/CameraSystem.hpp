#ifndef   __CAMERA_SYSTEM_HPP__
# define  __CAMERA_SYSTEM_HPP__

#include <Utils/MatrixConversion.hpp>
#include "System.h"
#include <Components/MeshRenderer.hh>
#include <Components/CameraComponent.hpp>
#include <Entities/EntityData.hh>
#include <Core/SceneManager.hh>
#include <Core/Renderer.hh>
#include <Systems/MeshRenderSystem.h>
#include <Utils/ScreenPosToWorldRay.hpp>
#include <Context/IRenderContext.hh>
#include <Utils/MatrixConversion.hpp>

class CameraSystem : public System
{
public:
	CameraSystem(AScene *scene)
		: System(scene)
		, _filter(scene)
		, _renderDebugMethod(false)
		, _totalTime(0)
	{}

	virtual ~CameraSystem(){}

	void setRenderDebugMode(bool t)
	{
		_renderDebugMethod = t;
	}

	bool getRenderDebugMode() const
	{
		return _renderDebugMethod;
	}

	void getRayFromMousePosOnScreen(glm::vec3 &from, glm::vec3 &to)
	{
		if (_filter.getCollection().size() == 0)
			return;
		auto mousePos = _scene->getEngine().getInstance<Input>()->getMousePosition();
		auto screenSize = _scene->getEngine().getInstance<IRenderContext>()->getScreenSize();
		auto cameraCpt = _filter.getCollection().begin()->get()->getComponent<Component::CameraComponent>();
		screenPosToWorldRay(mousePos.x, mousePos.y, screenSize.x, screenSize.y, cameraCpt->lookAtTransform, cameraCpt->projection, from, to);
	}

	void getRayFromCenterOfScreen(glm::vec3 &from, glm::vec3 &to)
	{
		if (_filter.getCollection().size() == 0)
			return;
		auto screenSize = _scene->getEngine().getInstance<IRenderContext>()->getScreenSize();
		auto centerPos = glm::vec2(screenSize) * glm::vec2(0.5f);
		auto cameraCpt = _filter.getCollection().begin()->get()->getComponent<Component::CameraComponent>();
		screenPosToWorldRay(centerPos.x, centerPos.y, screenSize.x, screenSize.y, cameraCpt->lookAtTransform , cameraCpt->projection, from, to);
	}

	// Returns the number of seconds since the component creation
	double		getLifeTime() const
	{
		return (_totalTime);
	}

protected:
	EntityFilter _filter;

	bool _renderDebugMethod;
	double	_totalTime;

	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		unsigned int textureOffset = 0;
		auto &renderer = _scene->getEngine().getInstance<Renderer>();
		OpenGLTools::UniformBuffer *perFrameBuffer = _scene->getEngine().getInstance<Renderer>()->getUniform("PerFrame");

		for (auto e : _filter.getCollection())
		{
			auto camera = e->getComponent<Component::CameraComponent>();
			auto skybox = camera->getSkybox();

			auto cameraPosition = camera->getLookAtTransform();
			OpenGLTools::Framebuffer &camFbo = e->getComponent<Component::CameraComponent>()->frameBuffer;

			if (camFbo.isInit() == true)
			{
				camFbo.bind();
				glEnable(GL_DEPTH_TEST);
				glDrawBuffer(GL_COLOR_ATTACHMENT0);
				glClearColor(1, 0, 0, 0);
				glClear(GL_COLOR_BUFFER_BIT);
			}
			if (skybox != nullptr)
			{
				OpenGLTools::Shader *s = _scene->getEngine().getInstance<Renderer>()->getShader(camera->getSkyboxShader());
				assert(s != NULL && "Skybox does not have a shader associated");

				_scene->getEngine().getInstance<Renderer>()->getUniform("cameraUniform")->setUniform("projection", camera->getProjection());

				glm::mat4 t = cameraPosition;
				t[3][0] = 0;
				t[3][1] = 0;
				t[3][2] = 0;
				t[3][3] = 1;

				_scene->getEngine().getInstance<Renderer>()->getUniform("cameraUniform")->setUniform("view", t);
				_scene->getEngine().getInstance<Renderer>()->getUniform("cameraUniform")->flushChanges();

				s->use();

				glDepthMask(GL_FALSE);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getId());
				skybox->draw();
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
				glDepthMask(GL_TRUE);
			}
		}
		_totalTime += time;
	}

	virtual void initialize()
	{
		_filter.requireComponent<Component::CameraComponent>();
	}
};

#endif    //__CAMERA_SYSTEM_HPP__