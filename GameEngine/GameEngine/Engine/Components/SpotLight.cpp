#include <Components/SpotLight.hh>
#include <Core/AScene.hh>
#include <Utils/MathematicTools.hh>
#include <glm/glm.hpp>
#include <AssetManagement/AssetManager.hh>

#include "Render/Properties/AutoProperty.hpp"
#include "Render/ProgramResources/Types/Uniform/Vec4.hh"
#include "Render/ProgramResources/Types/Uniform/Vec3.hh"
#include "Render/ProgramResources/Types/Uniform/Vec1.hh"

#include "Graphic/DRBLightElementManager.hpp"
#include "Graphic/DRBData.hpp"

#ifdef EDITOR_ENABLED
#	include <imgui\imgui.h>
#	include <glm/gtc/type_ptr.hpp>
#endif

namespace AGE
{
	SpotLightData::SpotLightData(glm::vec3 const &color, glm::vec3 const &range, float exponent, float cutOff, std::shared_ptr<ITexture> const &map)
		: color(color),
		range(range),
		exponent(exponent),
		cutOff(cutOff),
		map(map)
	{

	}

	SpotLightComponent::SpotLightComponent(SpotLightComponent const &o)
	{
		postUnserialization();
	}

	void SpotLightComponent::_copyFrom(const ComponentBase *model)
	{
		auto o = static_cast<const SpotLightComponent*>(model);
		postUnserialization();
	}

	void SpotLightComponent::reset()
	{
		_propShadowMatrix = nullptr;
		_propSpotCutOff = nullptr;
		_propPosition = nullptr;
		_propExponentLight = nullptr;
		_propDirection = nullptr;
		_propColorLight = nullptr;
		_propAttenuation = nullptr;
		if (_graphicHandle.invalid() == false)
		{
			auto manager = entity->getScene()->getInstance<DRBLightElementManager>();
			manager->removeSpotLight(_graphicHandle);
		}
	}

	void SpotLightComponent::init()
	{
		color = glm::vec4(1.0f);
		range = glm::vec3(1.0f, 0.1f, 0.01f);
		exponent = 5.0f;
		cutOff = 0.5f;

		AGE_ASSERT(_graphicHandle.invalid());

		auto manager = entity->getScene()->getInstance<DRBLightElementManager>();
		_graphicHandle = manager->addSpotLight();

		//_propShadowMap = std::make_shared<Sampler2D>("shadow_map");

		// TODO DORIAN -> fix spotlight texture
		//auto shadowMapTexture = entity->getScene()->getInstance<AssetsManager>()->getSpotLightTexture();
		//_propShadowMap->set(shadowMapTexture);

		_propShadowMatrix = std::make_shared<AutoProperty<glm::vec4, Vec4>>("light_matrix");
		_propPosition = std::make_shared<AutoProperty<glm::vec3, Vec3>>("position_light");
		_propAttenuation = std::make_shared<AutoProperty<glm::vec3, Vec3>>("attenuation_light");
		_propAttenuation->autoSet(range);
		_propDirection = std::make_shared<AutoProperty<glm::vec3, Vec3>>("direction_light");
		_propSpotCutOff = std::make_shared<AutoProperty<float, Vec1>>("spot_cuf_off");
		_propSpotCutOff->autoSet(cutOff);
		_propExponentLight = std::make_shared<AutoProperty<float, Vec1>>("exponent_light");
		_propExponentLight->autoSet(exponent);
		_propColorLight = std::make_shared<AutoProperty<glm::vec4, Vec4>>("color_light");
		_propColorLight->autoSet(color);

		auto &properties = _graphicHandle.getPtr()->getDatas()->globalProperties;
		
		properties.add_property(_propShadowMatrix);
		properties.add_property(_propPosition);
		properties.add_property(_propAttenuation);
		properties.add_property(_propDirection);
		properties.add_property(_propSpotCutOff);
		properties.add_property(_propExponentLight);
		properties.add_property(_propColorLight);
	}

	void SpotLightComponent::postUnserialization()
	{
		init();
	}

#ifdef EDITOR_ENABLED
	void SpotLightComponent::editorCreate()
	{}

	void SpotLightComponent::editorDelete()
	{
	}

	bool SpotLightComponent::editorUpdate()
	{
		bool modified = false;
		if (ImGui::ColorEdit3("Color", glm::value_ptr(color)))
		{
			_propColorLight->autoSet(color);
			modified = true;
		}
		if (ImGui::SliderFloat3("Range", glm::value_ptr(range), 0.0f, 1.0f))
		{
			_propAttenuation->autoSet(range);
			modified = true;
		}
		if (ImGui::InputFloat("Exponent", &exponent))
		{
			_propExponentLight->autoSet(exponent);
			modified = true;
		}
		if (ImGui::InputFloat("cut off", &cutOff))
		{
			_propSpotCutOff->autoSet(cutOff);
			modified = true;
		}
		return modified;
	}
#endif
}