#include <Systems/CameraSystem.hh>
#include <Utils/MatrixConversion.hpp>
#include <Context/IRenderContext.hh>
#include <Utils/ScreenPosToWorldRay.hpp>
#include <Core/Renderer.hh>
#include <Components/MeshRenderer.hh>
#include <Components/CameraComponent.hpp>
#include <Core/SceneManager.hh>
#include <OpenGL/ShadingManager.hh>

# define NEW_SHADER 1
# define VERTEX_SHADER "../../test_pipeline_1.vp"
# define FRAG_SHADER "../../test_pipeline_1.fp"

CameraSystem::CameraSystem(std::weak_ptr<AScene> &&scene)
	: System(std::move(scene)),
	_manager(NULL),
	_renderDebugMethod(false),
	_totalTime(0),
	_camera(std::move(scene)),
	_drawable(std::move(scene))
{
	_name = "camera_system";
}

void CameraSystem::setRenderDebugMode(bool t)
{
	_renderDebugMethod = t;
}

bool CameraSystem::getRenderDebugMode() const
{
	return _renderDebugMethod;
}

void CameraSystem::getRayFromMousePosOnScreen(glm::vec3 &from, glm::vec3 &to)
{
	if (_camera.getCollection().size() == 0)
		return;
	auto scene = _scene.lock();
	auto mousePos = scene->getInstance<Input>()->getMousePosition();
	auto screenSize = scene->getInstance<IRenderContext>()->getScreenSize();
	auto cameraCpt = scene->getComponent<Component::CameraComponent>(*(_camera.getCollection().begin()));
	screenPosToWorldRay(mousePos.x, mousePos.y, screenSize.x, screenSize.y, cameraCpt->lookAtTransform, cameraCpt->projection, from, to);
}

void CameraSystem::getRayFromCenterOfScreen(glm::vec3 &from, glm::vec3 &to)
{
	if (_camera.getCollection().size() == 0)
		return;
	auto scene = _scene.lock();
	auto screenSize = scene->getInstance<IRenderContext>()->getScreenSize();
	auto centerPos = glm::vec2(screenSize) * glm::vec2(0.5f);
	auto cameraCpt = scene->getComponent<Component::CameraComponent>(*(_camera.getCollection().begin()));
	screenPosToWorldRay(
		static_cast<int>(centerPos.x),
		static_cast<int>(centerPos.y),
		static_cast<int>(screenSize.x),
		static_cast<int>(screenSize.y),
		cameraCpt->lookAtTransform,
		cameraCpt->projection,
		from,
		to);
}

void CameraSystem::setManager(gl::ShadingManager &m)
{
	_manager = &m;
	if (_manager == NULL)
		std::cerr << "Warning: No manager set for the camerasystem" << std::endl;
	_shader = _manager->addShader(VERTEX_SHADER, FRAG_SHADER);
	size_t sizeElement = sizeof(glm::mat4);
	_global_state = _manager->addUniformBlock(1, &sizeElement);
	_manager->addShaderInterfaceBlock(_shader, "global_state", _global_state);
	_modelview_matrix = _manager->addShaderUniform(_shader, "modelview_matrix");
}

// Returns the number of seconds since the component creation
double CameraSystem::getLifeTime() const
{
	return (_totalTime);
}

void CameraSystem::updateBegin(double time)
{
}

void CameraSystem::updateEnd(double time)
{
}

void CameraSystem::mainUpdate(double time)
{
#if !NEW_SHADER
	unsigned int textureOffset = 0;
	auto scene = _scene.lock();
	auto &renderer = scene->getInstance<Renderer>();
	std::shared_ptr<OpenGLTools::UniformBuffer> perFrameBuffer = scene->getInstance<Renderer>()->getUniform("PerFrame");

	for (auto e : _filter.getCollection())
	{
		auto camera = scene->getComponent<Component::CameraComponent>(e);
		auto skybox = camera->getSkybox();

		auto cameraPosition = camera->lookAtTransform;
		OpenGLTools::Framebuffer &camFbo = scene->getComponent<Component::CameraComponent>(e)->frameBuffer;

		if (skybox != nullptr && camFbo.isInit() == true)
		{
			std::shared_ptr<gl::Shader> s = scene->getInstance<Renderer>()->getShader(camera->getSkyboxShader());
			assert(s != nullptr && "Skybox does not have a shader associated");

			scene->getInstance<Renderer>()->getUniform("cameraUniform")->setUniform("projection", camera->projection);

			glm::mat4 t = cameraPosition;
			t[3][0] = 0;
			t[3][1] = 0;
			t[3][2] = 0;
			t[3][3] = 1;

			scene->getInstance<Renderer>()->getUniform("cameraUniform")->setUniform("view", t);
			scene->getInstance<Renderer>()->getUniform("cameraUniform")->flushChanges();

			camFbo.bind();
			glViewport(0, 0, camFbo.getSize().x, camFbo.getSize().y);

			glEnable(GL_DEPTH_TEST);
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glDepthMask(GL_TRUE);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClearDepth(1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
#endif
}

bool CameraSystem::initialize()
{
	_camera.requireComponent<Component::CameraComponent>();
	_drawable.requireComponent<Component::MeshRenderer>();
	return true;
}