#pragma once

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace AGE
{
	namespace WE
	{
		//return true if entity has been modified
		static bool displayEntity(Entity &entity, AScene *scene)
		{
			bool modified = false;

			auto cpt = entity.getComponent<AGE::WE::EntityRepresentation>();

			ImGui::InputText("Name", cpt->name, ENTITY_NAME_LENGTH);
			cpt->position = entity.getLink().getPosition();
			if (ImGui::InputFloat3("Position", glm::value_ptr(cpt->position)))
			{
				modified = true;
				entity.getLink().setPosition(cpt->position);
			}

			cpt->rotation = glm::eulerAngles(entity.getLink().getOrientation());
			if (ImGui::InputFloat3("Rotation", glm::value_ptr(cpt->rotation)))
			{
				modified = true;
				entity.getLink().setOrientation(glm::quat(cpt->rotation));
			}

			cpt->scale = entity.getLink().getScale();
			if (ImGui::InputFloat3("Scale", glm::value_ptr(cpt->scale)))
			{
				modified = true;
				entity.getLink().setScale(cpt->scale);
			}

			ImGui::Separator();

			if (cpt->isLinkedToArchetype())
			{
				ImGui::TextColored(ImVec4(0.3, 0.4, 0.5, 1.0), "Entity is Archetype, edit the proper archetype.");
				return modified;
			}

			auto &components = entity.getComponentList();
			for (ComponentType i = 0; i < components.size(); ++i)
			{
				if (entity.haveComponent(i))
				{
					auto ptr = entity.getComponent(i);
					if (ptr->exposedInEditor)
					{
						bool opened = ImGui::TreeNode(ComponentRegistrationManager::getInstance().getComponentName(ptr->getType()).c_str());
						bool deleted = false;
						if (ptr->deletableInEditor)
						{
							ImGui::SameLine();
							ImGui::PushID(i);
							deleted = ImGui::SmallButton("Delete");
							if (deleted)
							{
								modified = true;
								ptr->editorDelete();
								entity.removeComponent(i);
							}
							ImGui::PopID();
						}
						if (opened && !deleted)
						{
							if (ptr->editorUpdate())
							{
								modified = true;
							}
							ImGui::TreePop();
						}
					}
				}
			}
			return modified;
		}

		//return true if entity has been modified
		static bool recursiveDisplayList(Entity &entity, Entity *selectedEntity, bool &selectParent)
		{
			bool modified = false;

			auto cpt = entity.getComponent<AGE::WE::EntityRepresentation>();
			bool opened = false;
			opened = ImGui::TreeNode(cpt->name);
			ImGui::PushID(entity.getPtr());
			if (selectedEntity != entity.getPtr())
			{
				if (!selectParent)
				{
					ImGui::SameLine();
					if (ImGui::SmallButton("Select"))
					{
						selectedEntity = entity.getPtr();
					}
				}
				else
				{
					ImGui::SameLine();
					if (ImGui::SmallButton("Set as parent"))
					{
						modified = true;
						selectedEntity->getLink().attachParent(entity.getLinkPtr());
						selectParent = false;
					}
				}
			}
			else
			{
				if (!selectParent)
				{
					ImGui::SameLine();
					if (ImGui::SmallButton("Set parent"))
					{
						selectParent = true;
					}
				}
				else
				{
					ImGui::SameLine();
					if (ImGui::SmallButton("Root"))
					{
						modified = true;
						selectedEntity->getLink().detachParent();
						selectParent = false;
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("Cancel"))
					{
						selectParent = false;
					}
				}
			}
			ImGui::PopID();
			if (opened)
			{
				if (entity.getLink().hasChildren())
				{
					auto children = entity.getLink().getChildren();
					for (auto &e : children)
					{
						if (recursiveDisplayList(e->getEntity()->getEntity(), selectedEntity, selectParent))
						{
							modified = true;
						}
					}
				}
				ImGui::TreePop();
			}
			return modified;
		}
	}
}