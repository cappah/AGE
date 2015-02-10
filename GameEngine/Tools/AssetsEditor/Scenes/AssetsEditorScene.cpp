#include "AssetsEditorScene.hpp"

//CORE
#include <Threads/ThreadManager.hpp>
#include <Core/Engine.hh>
#include <Core/Timer.hh>
#include <AssetManagement/AssetManager.hh>
#include <Threads/RenderThread.hpp>
#include <Utils/Age_Imgui.hpp>
#include <Core/Tasks/Basics.hpp>
#include <Threads/TaskScheduler.hpp>
#include <Core/Input.hh>
#include <SDL/SDL.h>

#include <string>

//CONVERTOR
#include <Convertor/SkeletonLoader.hpp>
#include <Convertor/AnimationsLoader.hpp>
#include <Convertor/MeshLoader.hpp>
#include <Convertor/MaterialConvertor.hpp>
#include <Convertor/ImageLoader.hpp>
#include <Convertor/BulletLoader.hpp>
#include <Convertor/ConvertorStatusManager.hpp>

#include <AssetFiles/Folder.hpp>
#include <AssetFiles/RawFile.hpp>
#include <AssetFiles/AssetFileManager.hpp>
#include <AssetFiles/CookedFile.hpp>

#include <AssetFiles/AssetsTypes.hpp>

#include <Utils/FileSystem.hpp>


namespace AGE
{
	const std::string AssetsEditorScene::Name = "AssetsEditor";

	AssetsEditorScene::AssetsEditorScene(std::weak_ptr<AGE::Engine> engine)
		: AScene(engine)
		, _raw("../../Assets/AGE-Assets-For-Test/Raw")
		, _cook("../../Assets/AGE-Assets-For-Test/Serialized")
	{
		_raw.list();
		_cook.list();
		AE::AssetFileManager::LinkRawToCooked(&_raw, &_cook);
	}

	AssetsEditorScene::~AssetsEditorScene(void)
	{
	}

	bool AssetsEditorScene::userStart()
	{
		Singleton<AGE::AE::ConvertorStatusManager>::setInstance();
		return true;
	}

	bool AssetsEditorScene::userUpdateBegin(double time)
	{
		//check dirty for test
		{
			static double counter = 0;
			counter += time;
			if (counter > 1)
			{
				_raw.list();
				std::set<std::shared_ptr<AE::RawFile>> dirty;
				AE::AssetFileManager::CheckIfRawModified(&_raw, dirty);
				for (auto &e : dirty)
				{

				}
				counter = 0;
			}
		}

		ImGui::BeginChild("Assets browser", ImVec2(ImGui::GetWindowWidth() * 0.3333333f, 0), true);
		{
			{
				ImGui::BeginChild("Raw", ImVec2(0, 0), false);
				_raw.update(
				std::function<bool(AE::Folder*)>([](AE::Folder* folder) {
					bool opened = ImGui::TreeNode((void*)(folder), folder->_path.path().filename().c_str());
					if (opened)
					{
						ImGui::Separator();
					}
					return opened;
				}),
					std::function<bool(AE::Folder*)>([](AE::Folder* folder) {
					ImGui::Separator();
					ImGui::TreePop();
					return true;
				}),
					std::function<void(AE::RawFile*)>([&](AE::RawFile* file) {
					AE::AssetFileManager::PrintClickableRawAssetsFile(file, AE::AssetFileManager::PrintSection::Date | AE::AssetFileManager::PrintSection::Name | AE::AssetFileManager::PrintSection::Type, _selectedRaw);
				}));
				ImGui::EndChild();
			}
			//{
			//	ImGui::BeginChild("Cooked", ImVec2(0, ImGui::GetIO().DisplaySize.y / 2 - 10), false);
			//	//_cook.printImgUi(AE::AssetFile::PrintInfos(AE::AssetFile::Name | AE::AssetFile::Type | AE::AssetFile::Date));
			//	ImGui::EndChild();
			//}
		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Selected Raw", ImVec2(ImGui::GetWindowWidth() * 0.33333333f, 0), false);
		{
			if (_selectedRaw != nullptr)
			{
<<<<<<< HEAD
				slowTouch = true;
				// CATWOMAN
				AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
					std::shared_ptr<AGE::AssetDataSet> dataSet = std::make_shared<AGE::AssetDataSet>();
					dataSet->filePath = File("catwoman/catwoman.fbx");
					dataSet->skeletonName = "catwoman";
					dataSet->animationName = "catwoman-roulade";
					dataSet->skinName = "catwoman";
					dataSet->materialName = "catwoman";

					dataSet->serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
					dataSet->rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");

					AGE::AssimpLoader::Load(*dataSet.get());

					AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
						AGE::MaterialLoader::load(*dataSet.get());
						AGE::ImageLoader::load(*dataSet.get());
						AGE::SkeletonLoader::load(*dataSet.get());
						AGE::AnimationsLoader::load(*dataSet.get());
						AGE::MeshLoader::load(*dataSet.get());
						AGE::MaterialLoader::save(*dataSet.get());
						AGE::ImageLoader::save(*dataSet.get());
						AGE::MeshLoader::save(*dataSet.get());
						AGE::SkeletonLoader::save(*dataSet.get());
						AGE::AnimationsLoader::save(*dataSet.get());
					});
				});
				//SPONZA
				AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
					std::shared_ptr<AGE::AssetDataSet> dataSet = std::make_shared<AGE::AssetDataSet>();
					dataSet->filePath = File("sponza/sponza.obj");
					dataSet->skinName = "sponza";
					dataSet->materialName = "sponza";

					dataSet->serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
					dataSet->rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");

					AGE::AssimpLoader::Load(*dataSet.get());
					AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
						AGE::MaterialLoader::load(*dataSet.get());
						AGE::MaterialLoader::save(*dataSet.get());

						AGE::ImageLoader::load(*dataSet.get());
						AGE::ImageLoader::save(*dataSet.get());

						AGE::MeshLoader::load(*dataSet.get());
						AGE::MeshLoader::save(*dataSet.get());
					});
				});
				//CUBE
				AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
					std::shared_ptr<AGE::AssetDataSet> dataSet = std::make_shared<AGE::AssetDataSet>();
					dataSet->filePath = File("cube/cube.obj");
					dataSet->skinName = "cube";
					dataSet->materialName = "cube";

					dataSet->serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
					dataSet->rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");

					AGE::AssimpLoader::Load(*dataSet.get());

					AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
						AGE::MaterialLoader::load(*dataSet.get());
						AGE::MaterialLoader::save(*dataSet.get());

						AGE::ImageLoader::load(*dataSet.get());
						AGE::ImageLoader::save(*dataSet.get());

						AGE::MeshLoader::load(*dataSet.get());
						AGE::MeshLoader::save(*dataSet.get());
					});
				});
				//BALL
				AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
					std::shared_ptr<AGE::AssetDataSet> dataSet = std::make_shared<AGE::AssetDataSet>();
					dataSet->filePath = File("ball/ball.obj");
					dataSet->skinName = "ball";
					dataSet->materialName = "ball";

					dataSet->serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
					dataSet->rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");

					AGE::AssimpLoader::Load(*dataSet.get());

					AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
						AGE::MaterialLoader::load(*dataSet.get());
						AGE::MaterialLoader::save(*dataSet.get());

						AGE::ImageLoader::load(*dataSet.get());
						AGE::ImageLoader::save(*dataSet.get());

						AGE::MeshLoader::load(*dataSet.get());
						AGE::MeshLoader::save(*dataSet.get());
					});
				});
				//BROKEN TOWER
				//AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
				//	std::shared_ptr<AGE::AssetDataSet> dataSet = std::make_shared<AGE::AssetDataSet>();
				//	dataSet->filePath = File("Broken Tower/Broken Tower.fbx");
				//	dataSet->skinName = "tower";
				//	dataSet->materialName = "tower";
				//
				//	dataSet->serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
				//	dataSet->rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");
				//
				//	AGE::AssimpLoader::Load(*dataSet.get());
				//
				//	AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
				//		AGE::MaterialLoader::load(*dataSet.get());
				//		AGE::MaterialLoader::save(*dataSet.get());
				//
				//		AGE::ImageLoader::load(*dataSet.get());
				//		AGE::ImageLoader::save(*dataSet.get());
				//
				//		AGE::MeshLoader::load(*dataSet.get());
				//		AGE::MeshLoader::save(*dataSet.get());
				//	});
				//});
				//VENICE
				//AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
				//	std::shared_ptr<AGE::AssetDataSet> dataSet = std::make_shared<AGE::AssetDataSet>();
				//	dataSet->filePath = File("Venice/venice.obj");
				//	dataSet->skinName = "venice";
				//	dataSet->materialName = "venice";

				//	dataSet->serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
				//	dataSet->rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");

				//	AGE::AssimpLoader::Load(*dataSet.get());

				//	AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
				//		AGE::MaterialLoader::load(*dataSet.get());
				//		AGE::MaterialLoader::save(*dataSet.get());

				//		AGE::ImageLoader::load(*dataSet.get());
				//		AGE::ImageLoader::save(*dataSet.get());

				//		AGE::MeshLoader::load(*dataSet.get());
				//		AGE::MeshLoader::save(*dataSet.get());
				//	});
				//});
=======
				if (_selectedRaw->dataSet == nullptr)
				{
					auto path = _selectedRaw->getPath();
					path = path.erase(0, _raw._path.path().string().size());
					//_selectedRaw->dataSet->filePath = File(path);
					_selectedRaw->dataSet = std::make_shared<AssetDataSet>(path);

				}
				if (!_selectedRaw->dataSet->isConverting)
				{
					ImGui::Text("Selected asset : "); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.58, 0, 0.7, 1), _selectedRaw->getFileName().c_str());
					ImGui::Text("Last modifiction : %s", _selectedRaw->_lastWriteTimeStr.c_str());

					auto dataset = _selectedRaw->dataSet;

					ImGui::Checkbox("Mesh", &dataset->loadMesh);
					if (dataset->loadMesh)
					{
						ImGui::Checkbox("Normalize size", &dataset->normalize);
						if (dataset->normalize)
						{
							ImGui::SliderFloat("Max size length", &dataset->maxSideLength, 0.00001f, 1000.0f, "%.3f", 1.0f);
						}
						ImGui::Checkbox("Positions", &dataset->positions);
						ImGui::Checkbox("Normals", &dataset->normals);
						ImGui::Checkbox("Bones infos", &dataset->bonesInfos);
						ImGui::Checkbox("Texture coordinates", &dataset->uvs);
						ImGui::Checkbox("Tangents", &dataset->tangents);
						ImGui::Checkbox("BiTangents", &dataset->biTangents);
					}
					ImGui::Separator();

					ImGui::Checkbox("Physic", &dataset->loadPhysic);
					if (dataset->loadPhysic)
					{
						ImGui::Checkbox("Dynamic concave", &dataset->dynamicConcave);
						ImGui::Checkbox("Static concave", &dataset->staticConcave);
					}
					ImGui::Separator();

					ImGui::Checkbox("Material", &dataset->loadMaterials);
					if (dataset->loadMaterials)
					{
						ImGui::Checkbox("Generate normal map from bump", &dataset->bumpToNormal);
						if (dataset->bumpToNormal)
						{
							ImGui::SliderInt("Normal strength", &dataset->normalStrength, 1, 10);
						}
					}
					ImGui::Separator();

					ImGui::Checkbox("Animation", &dataset->loadAnimations);
					if (dataset->loadAnimations)
					{
					}
					ImGui::Separator();

					ImGui::Checkbox("Skeleton", &dataset->loadSkeleton);
					if (dataset->loadSkeleton || dataset->loadMesh || dataset->loadAnimations)
					{
					}
					ImGui::Separator();

					ImGui::Checkbox("Textures", &dataset->loadTextures);
					if (dataset->loadTextures)
					{
						ImGui::Checkbox("Compress textures", &dataset->compressTextures);
						ImGui::Checkbox("Generate mipmaps", &dataset->generateMipmap);
						if (dataset->generateMipmap)
						{
							ImGui::SliderInt("Mipmap levels", &dataset->mipmapLevels, 1, 16);
						}
					}
					ImGui::Separator();
					if (ImGui::Button("Cook"))
					{
						auto cookingTask = std::make_shared<CookingTask>(_selectedRaw->dataSet);
						AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=]()
						{
							cookingTask->serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
							cookingTask->rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");

							AGE::AssimpLoader::Load(cookingTask);

							AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
								AGE::MaterialLoader::load(cookingTask);
								AGE::MaterialLoader::save(cookingTask);
								AGE::ImageLoader::load(cookingTask);
								AGE::ImageLoader::save(cookingTask);
							});
							if ((cookingTask->dataSet->loadMesh || cookingTask->dataSet->loadAnimations) && cookingTask->dataSet->loadSkeleton)
							{
								AGE::SkeletonLoader::load(cookingTask);
								AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
									AGE::AnimationsLoader::load(cookingTask);
									AGE::AnimationsLoader::save(cookingTask);
								});
								AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
									AGE::MeshLoader::load(cookingTask);
									AGE::MeshLoader::save(cookingTask);
									AGE::BulletLoader::load(cookingTask);
									AGE::BulletLoader::save(cookingTask);
								});
								AGE::SkeletonLoader::save(cookingTask);
							}
							else
							{
								AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
									AGE::AnimationsLoader::load(cookingTask);
									AGE::AnimationsLoader::save(cookingTask);
								});
								AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
									AGE::MeshLoader::load(cookingTask);
									AGE::MeshLoader::save(cookingTask);
									AGE::BulletLoader::load(cookingTask);
									AGE::BulletLoader::save(cookingTask);
								});
								AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
									AGE::SkeletonLoader::load(cookingTask);
									AGE::SkeletonLoader::save(cookingTask);
								});
							}
						});
					}
				}
				else
				{
					ImGui::TextColored(ImVec4(1, 0, 0, 1), "Currently cooking : %s", _selectedRaw->getFileName().c_str());
				}
>>>>>>> prod_graphic
			}
		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Todo", ImVec2(ImGui::GetWindowWidth() * 0.33333333f, 0), false);
		Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->DisplayTasks();
		ImGui::EndChild();
		return true;
	}

	bool AssetsEditorScene::userUpdateEnd(double time)
	{
		ImGui::End();
		if (getInstance<Input>()->getInput(SDLK_ESCAPE))
			return (false);
		return true;
	}
}