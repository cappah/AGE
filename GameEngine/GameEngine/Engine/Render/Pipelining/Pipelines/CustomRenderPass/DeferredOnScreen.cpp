#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredOnScreen.hh>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Culling/Output/RenderLight.hh>
#include <Culling/Output/RenderPipeline.hh>
#include <Culling/Output/RenderPainter.hh>
#include <Culling/Output/RenderCamera.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler2D.hh>
#include <Render/ProgramResources/Types/Uniform/Vec3.hh>
#include <Render/ProgramResources/Types/Uniform/Vec2.hh>
#include <Render/ProgramResources/Types/Uniform/Vec1.hh>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/ConfigurationManager.hpp>
#include <Core/Engine.hh>

#include "Render/GeometryManagement/SimpleGeometry.hh"

#define DEFERRED_SHADING_SCREEN_VERTEX "deferred_shading/deferred_shader_screen.vp"
#define DEFERRED_SHADING_SCREEN_FRAG "deferred_shading/deferred_shader_screen.fp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_SCREEN = 0,
		PROGRAM_NBR
	};

	DeferredOnScreen::DeferredOnScreen(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
		std::shared_ptr<Texture2D> diffuse) :
		ScreenRender(screenSize, painterManager)
	{
		_diffuseInput = diffuse;

		_programs.resize(PROGRAM_NBR);

		auto confManager = GetEngine()->getInstance<ConfigurationManager>();

		auto shaderPath = confManager->getConfiguration<std::string>("ShadersPath");

		// you have to set shader directory in configuration path
		AGE_ASSERT(shaderPath != nullptr);

		auto vertexShaderPath = shaderPath->getValue() + DEFERRED_SHADING_SCREEN_VERTEX;
		auto fragmentShaderPath = shaderPath->getValue() + DEFERRED_SHADING_SCREEN_FRAG;

		_programs[PROGRAM_SCREEN] = std::make_shared<Program>(Program(std::string("basic_3d_render"),
		{
			std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
		}));

		Key<Painter> quadPainterKey;

		GetRenderThread()->getQuadGeometry(_quadVertices, quadPainterKey);
		_quadPainter = _painterManager->get_painter(quadPainterKey);
	}

	void DeferredOnScreen::renderPass(const DRBCameraDrawableList &infos)
	{
		SCOPE_profile_gpu_i("DefferedOnScreen");
		SCOPE_profile_cpu_function("RenderTime", "DefferedOnScreen");
		_programs[PROGRAM_SCREEN]->use();
		_programs[PROGRAM_SCREEN]->get_resource<Sampler2D>("screen").set(_diffuseInput);

		OpenGLState::glDisable(GL_BLEND);
		OpenGLState::glDisable(GL_CULL_FACE);
		OpenGLState::glDisable(GL_DEPTH_TEST);
		OpenGLState::glDisable(GL_STENCIL_TEST);
		{
			SCOPE_profile_gpu_i("Overhead pipeline");
			SCOPE_profile_cpu_function("RenderTime", "Overhead pipeline");
			_programs[PROGRAM_SCREEN]->get_resource<Vec2>("resolution").set(glm::vec2(viewport.x, viewport.y));
			//@PROUT TODO -> re enable FXAA
			_programs[PROGRAM_SCREEN]->get_resource<Vec1>("activated").set(1.0f /*infos.data.activated == true ? 1.0f : 0.f*/);
		}

		_quadPainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_SCREEN], Properties(), _quadVertices);
	}

}
