// for leak detection
#define _CRTDBG_MAP_ALLOC
#define GLM_FORCE_INLINE 
#define GLM_FORCE_SSE2 
#define GLM_FORCE_AVX

#include <stdlib.h>
#include <crtdbg.h>


#include <stdlib.h>
#include <Core/Engine.hh>

// SCENES
#include "Scenes/MainScene.hh"
#include "Scenes/SpaceGame.hh"
#include "Scenes/SolarSystemDemoScene.hh"
#include "Scenes/SponzaScene.hh"

// DEPENDENCIES
#include <Context/SdlContext.hh>
#include <Core/SceneManager.hh>
#include <Core/Renderer.hh>
#include <Physic/BulletDynamicManager.hpp>
#include <MediaFiles/AssetsManager.hpp>
#include <Audio/AudioManager.hh>
#include <OpenGL/VertexManager.hh>
#include <Text/FontManager.hh>
#include <Sprite/SpriteManager.hh>

int			main(int ac, char **av)
{
	std::shared_ptr<Engine>	e = std::make_shared<Engine>();

	e->setInstance<PubSub::Manager>();
	e->setInstance<SdlContext, IRenderContext>();
	e->setInstance<Input>();
	e->setInstance<Timer>();
	e->setInstance<AssetsManager>()->init();
	e->setInstance<Renderer>();
	e->setInstance<SceneManager>();
	e->setInstance<AudioManager>()->init();


	// init engine
	if (e->init(0, 1920, 1080, "PFA Présentation") == false)
		return (EXIT_FAILURE);

	std::array<Attribute, 4> param = //-V112
	{
		Attribute(GL_FLOAT, sizeof(float), 4), //-V112
		Attribute(GL_FLOAT, sizeof(float), 4), //-V112
		Attribute(GL_FLOAT, sizeof(float), 4), //-V112
		Attribute(GL_FLOAT, sizeof(float), 2),
	};

	e->setInstance<VertexManager<4>>(param)->init();
	e->setInstance<SpriteManager>()->init();

	// add main scene
	e->getInstance<SceneManager>()->addScene(std::make_shared<MainScene>(e), "MainScene");
	e->getInstance<SceneManager>()->addScene(std::make_shared<SpaceGame>(e), "Asteroid");
	e->getInstance<SceneManager>()->addScene(std::make_shared<SponzaScene>(e), "Sponza");

	// bind scene
	if (!e->getInstance<SceneManager>()->initScene("MainScene"))
		return (EXIT_FAILURE);
	if (!e->getInstance<SceneManager>()->initScene("Asteroid"))
		return (EXIT_FAILURE);
	if (!e->getInstance<SceneManager>()->initScene("Sponza"))
		return (EXIT_FAILURE);

	e->getInstance<SceneManager>()->enableScene("MainScene", 100);


	// launch engine
	if (e->start() == false)
		return (EXIT_FAILURE);
	while (e->update())
		;
	//e.stop();
	return (EXIT_SUCCESS);
}
