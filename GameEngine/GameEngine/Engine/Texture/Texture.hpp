#pragma once

#include <Utils/GlmSerialization.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

namespace AGE
{
	struct TextureData
	{
		std::string rawPath;
		std::uint32_t width;
		std::uint32_t height;
		std::uint8_t bpp;
		AGE::Vector<unsigned char> data;

		template <class Archive>
		void serialize(Archive &ar)
		{
			ar(rawPath, width, height, bpp, data);
		}
	};
}