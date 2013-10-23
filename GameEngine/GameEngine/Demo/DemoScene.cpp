#include "Core/Engine.hh"
#include "DemoScene.hh"
#include "BasicCam.hh"

#include "ResourceManager/SharedMesh.hh"
#include "Components/EmptyComponent.hh"

#include <SDL\SDL.h>

DemoScene::DemoScene(void)
{
}

DemoScene::~DemoScene(void)
{
}

bool 			DemoScene::userStart()
{
	SmartPointer<Entity>		e = new Entity;
	GameEngine::instance()->resources().addResource("model:ball", new Resources::SharedMesh(), "../Assets/ball.obj");

	SmartPointer<Components::MeshRenderer>	r = new Components::MeshRenderer("renderer", "model:ball");
//		SmartPointer<Components::EmptyComponent> r = new Components::EmptyComponent();

	GameEngine::instance()->renderer().addUniform("PerFrame")
		.registerUniform("vProjection", 0, 16 * sizeof(float))
		.registerUniform("vView", 16 * sizeof(float), 16 * sizeof(float))
		.registerUniform("fLightSpot", 2 * (16 * sizeof(float)), 3 * sizeof(float));

	GameEngine::instance()->renderer().addUniform("PerModel")
		.registerUniform("vModel", 0, 16 * sizeof(float));

	GameEngine::instance()->renderer().addShader("basicLight", "shaders/light.vp", "shaders/light.fp");
	GameEngine::instance()->renderer().bindShaderToUniform("basicLight", "PerFrame", "PerFrame");
	GameEngine::instance()->renderer().bindShaderToUniform("basicLight", "PerModel", "PerModel");
	r->setShader("basicLight");
	e->addComponent(r);
	getRoot()->addSon(e);

	setCamera(new BasicCam);

	return (true);
}

bool 			DemoScene::userUpdate()
{
	Engine		&engine = *GameEngine::instance();

	if (engine.inputs().getInput(SDLK_ESCAPE) ||
		engine.inputs().getInput(SDL_QUIT))
		return (false);
	return (true);
}
