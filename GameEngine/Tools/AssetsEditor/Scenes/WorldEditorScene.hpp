#pragma once

#include <Core/AScene.hh>
#include <Core/Engine.hh>
#include <memory>
#include "IMenuInheritrance.hpp"

namespace AGE
{
	class WorldEditorScene : public AScene, public IMenuInheritance
	{
	public:
		static const std::string Name;

		WorldEditorScene(Engine *engine);

		virtual ~WorldEditorScene(void);
		virtual bool _userStart();
		virtual bool _userUpdateBegin(float time);
		virtual bool _userUpdateEnd(float time);
		virtual void updateMenu();
	};
}