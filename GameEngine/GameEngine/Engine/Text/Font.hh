#pragma once

#include "FontGlyph.hh"
#include <Utils/OpenGL.hh>
#include <vector>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>

class FontConvertor;

class Font
{
public:
	Font();
	~Font();
	bool load(std::unique_ptr<VertexManager<2>> &vm);
	bool isLoaded();

	template <class Archive>
	void serialize(Archive &ar)
	{
		ar(_name, _sizes);
	}
private:
	struct FontSize
	{
		std::size_t _size;
		std::size_t _glyphSize;
		std::vector<unsigned char> _textureDatas;
		GLuint _textureId;
		std::size_t _texW;
		std::size_t _texH;
		std::array<FontGlyph, ASCII_END - ASCII_BEGIN> _map;

		FontSize();
		bool load(std::unique_ptr<VertexManager<2>> &vm);
		bool isLoaded();

		template <class Archive>
		void serialize(Archive &ar)
		{
			ar(_size, _glyphSize, _textureDatas, _texW, _texH, _map);
		}

	};
	std::string _name;
	bool _isLoaded;
	std::map<std::size_t, FontSize> _sizes;
	friend class FontManager;
	friend class FontConvertor;
};