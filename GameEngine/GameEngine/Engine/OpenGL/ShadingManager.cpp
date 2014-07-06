#include <OpenGL/ShadingManager.hh>
#include <OpenGL/Shader.hh>
#include <iostream>
#include <string>

namespace gl
{
	ShadingManager::ShadingManager()
	{
	}

	ShadingManager::~ShadingManager()
	{
	}

	Key<Shader> ShadingManager::addShader(std::string const &compute)
	{
		Key<Shader> key;
		Shader shader(compute);

		if (!shader.isValid())
			DEBUG_MESSAGE("Warning", "ShadingManager-addShader(string const &compute)", "compute invalid", Key<Shader>(KEY_DESTROY))
		_shaders[key] = shader;
		return (key);
	}

	Key<Shader> ShadingManager::addShader(std::string const &vertex, std::string const &frag)
	{
		Key<Shader> key;
		Shader shader(vertex, frag);

		if (!shader.isValid())
			DEBUG_MESSAGE("Warning", "ShadingManager-addShader(string const &frag, string const &vertex)", "frag and vertex invalid", Key<Shader>(KEY_DESTROY))
		_shaders[key] = shader;
		return (key);
	}

	Key<Shader> ShadingManager::addShader(std::string const &geo, std::string const &vertex, std::string const &frag)
	{
		Key<Shader> key;
		Shader shader(vertex, frag, geo);

		if (!shader.isValid())
			DEBUG_MESSAGE("Warning", "ShadingManager-addShader(string const &frag, string const &vertex, string const &geo)", "frag, vertex or geo invalid", Key<Shader>(KEY_DESTROY))
		_shaders[key] = shader;
		return (key);
	}

	ShadingManager &ShadingManager::rmShader(Key<Shader> &shader)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - rmShader", "the key is destroy", *this)
		auto element = _shaders.find(shader);
		if (element == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - rmShader", "the shader ask can't be find", *this)
		_shaders.erase(shader);
		shader.destroy();
		return (*this);
	}

	Key<Shader> ShadingManager::getShader(size_t target) const
	{
		if (target >= _shaders.size())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp-getShader(size_t target)", "the target is out of range", Key<Shader>(KEY_DESTROY));
		auto &element = _shaders.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	ShadingManager &ShadingManager::useShader(Key<Shader> const &shader)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - useShader", "key already destroy", *this)
		auto &element = _shaders.find(shader);
		if (element == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - useShader", "shader not find", *this)
		auto &s = element->second;
		s.use();
		return (*this);
	}

	Key<Uniform> ShadingManager::addShaderUniform(Key<Shader> const &shader, std::string const &flag)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - addShaderUniform", "key already destroy", Key<Uniform>(KEY_DESTROY))
		auto &element = _shaders.find(shader);
		if (element == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - addShaderUniform", "shader not find", Key<Uniform>(KEY_DESTROY))
		auto &s = element->second;
		return (s.addUniform(flag));
	}

	ShadingManager &ShadingManager::rmShaderUniform(Key<Shader> const &shader, Key<Uniform> &uniform)
	{
		if (!shader && !uniform)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - rmShaderUniform", "key already destroy", *this)
		auto &element = _shaders.find(shader);
		if (element == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - rmShaderUniform", "shader not find", *this)
		auto &s = element->second;
		s.rmUniform(uniform);
		return (*this);
	}

	Key<Uniform> ShadingManager::getShaderUniform(Key<Shader> const &shader, size_t target)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - getShaderUniform", "key destroy", Key<Uniform>(KEY_DESTROY))
		auto &element = _shaders.find(shader);
		if (element == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - getShaderUniform", "shader not find", Key<Uniform>(KEY_DESTROY))
		auto &s = element->second; 
		return (s.getUniform(target));
	}

	Key<Uniform> ShadingManager::addShaderUniform(Key<Shader> const &shader, std::string const &flag, glm::mat4 const &value)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - addShaderUniform", "key aldreadt destroy", Key<Uniform>(KEY_DESTROY))
		auto &element = _shaders.find(shader);
		if (element == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - addShaderUniform", "shader not find", Key<Uniform>(KEY_DESTROY))
		auto &s = element->second;
		return (s.addUniform(flag, value));
	}

	ShadingManager &ShadingManager::setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::mat4 const &mat4)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - setShaderUniform(mat4)", "key destroy", *this)
			auto &element = _shaders.find(shader);
		if (element == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - setShaderUniform(mat4)", "shader not find", *this)
		auto &s = element->second;
		s.setUniform(key, mat4);
		return (*this);
	}

	ShadingManager &ShadingManager::setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::vec4 const &vec4)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - setShaderUniform(vec4)", "key destroy", *this)
			auto &element = _shaders.find(shader);
		if (element == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - setShaderUniform(vec4)", "shader not find", *this)
			auto &s = element->second;
		s.setUniform(key, vec4);
		return (*this);
	}

	ShadingManager &ShadingManager::setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, float v)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - setShaderUniform(float)", "key destroy", *this)
			auto &element = _shaders.find(shader);
		if (element == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - setShaderUniform(float)", "shader not find", *this)
			auto &s = element->second;
		s.setUniform(key, v);
		return (*this);
	}

	ShadingManager &ShadingManager::setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::mat3 const &mat3)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - setShaderUniform(mat3)", "key destroy", *this)
			auto &element = _shaders.find(shader);
		if (element == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - setShaderUniform(mat3)", "shader not find", *this)
			auto &s = element->second;
		s.setUniform(key, mat3);
		return (*this);
	}

	Key<Sampler> ShadingManager::addShaderSampler(Key<Shader> const &shader, std::string const &flag)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - addShaderSampler", "key already destroy", Key<Sampler>(KEY_DESTROY))
		auto &element = _shaders.find(shader);
		if (element == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - addShaderSampler", "shader not find", Key<Sampler>(KEY_DESTROY))
			auto &s = element->second;
		return (s.addSampler(flag));
	}

	ShadingManager &ShadingManager::rmShaderSampler(Key<Shader> const &shader, Key<Sampler> &uniform)
	{
		if (!shader && !uniform)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - rmShaderSampler", "key already destroy", *this)
			auto &element = _shaders.find(shader);
		if (element == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - rmShaderSampler", "shader not find", *this)
		auto &s = element->second;
		s.rmSampler(uniform);
		return (*this);
	}

	Key<Sampler> ShadingManager::getShaderSampler(Key<Shader> const &shader, size_t target)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - getShaderSampler", "key destroy", Key<Sampler>(KEY_DESTROY))
			auto &element = _shaders.find(shader);
		if (element == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - getShaderSampler", "shader not find", Key<Sampler>(KEY_DESTROY))
		auto &s = element->second;
		return (s.getSampler(target));
	}

	ShadingManager &ShadingManager::setShaderSampler(Key<Shader> const &shader, Key<Sampler> const &key, Texture const &texture)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - setShaderSampler()", "key destroy", *this)
			auto &element = _shaders.find(shader);
		if (element == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - setShaderSampler", "shader not find", *this)
		auto &s = element->second;
		s.setSampler(key, texture);
		return (*this);
	}

	Key<UniformBlock> ShadingManager::addUniformBlock(size_t nbrElement, size_t *sizeElement)
	{
		Key<UniformBlock> key;

		_uniformBlock[key] = UniformBlock(nbrElement, sizeElement);
		return (key);
	}

	ShadingManager &ShadingManager::rmUniformBlock(Key<UniformBlock> &key)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - rmUniformBlock", "the key is destroy", *this)
		_uniformBlock.erase(key);
		key.destroy();
		return (*this);
	}

	Key<UniformBlock> ShadingManager::getUniformBlock(size_t target)
	{
		if (target >= _uniformBlock.size())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp-getUniformBlock(size_t target)", "the target is out of range", Key<UniformBlock>(KEY_DESTROY));
		auto &element = _uniformBlock.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	Key<InterfaceBlock> ShadingManager::addShaderInterfaceBlock(Key<Shader> const &shader, std::string const &flag)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - addShaderInterfaceBlock(UniformBlock)", "key destroy", Key<InterfaceBlock>(KEY_DESTROY))
			auto &element = _shaders.find(shader);
		if (element == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - addShaderInterfaceBlock(UniformBlock)", "shader not find", Key<InterfaceBlock>(KEY_DESTROY))
			auto &s = element->second;
		return (s.addInterfaceBlock(flag));
	}

	Key<InterfaceBlock> ShadingManager::addShaderInterfaceBlock(Key<Shader> const &shader, std::string const &flag, Key<UniformBlock> const &keyUniformBlock)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - addShaderInterfaceBlock(UniformBlock)", "key destroy", Key<InterfaceBlock>(KEY_DESTROY))
			auto &element = _shaders.find(shader);
		if (element == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - addShaderInterfaceBlock(UniformBlock)", "shader not find", Key<InterfaceBlock>(KEY_DESTROY))
			auto &s = element->second;
		if (!keyUniformBlock)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - addShaderInterfaceBlock(uniformBlock)", "uniformBuffer key destroy", Key<InterfaceBlock>(KEY_DESTROY))
			auto &ubElement = _uniformBlock.find(keyUniformBlock);
		if (ubElement == _uniformBlock.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - addShaderInterfaceBlock(uniformBlock)", "uniformBuffer key is not found", Key<InterfaceBlock>(KEY_DESTROY))
		auto &u = ubElement->second;
		return (s.addInterfaceBlock(flag, u));
	}

	ShadingManager &ShadingManager::rmShaderInterfaceBlock(Key<Shader> const &shader, Key<InterfaceBlock> &interfaceblock)
	{
		if (!shader && !interfaceblock)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - rmShaderInterfaceBlock(uniformBlock)", "key already destroy", *this)
			auto &element = _shaders.find(shader);
		if (element == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - rmShaderInterfaceBlock(uniformBlock)", "shader not find", *this)
			auto &s = element->second;
		s.rmInterfaceBlock(interfaceblock);
		return (*this);
	}

	Key<InterfaceBlock> ShadingManager::getShaderInterfaceBlock(Key<Shader> const &shader, size_t target)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - getShaderInterfaceBlock(uniformBlock)", "key destroy", Key<InterfaceBlock>(KEY_DESTROY))
			auto &element = _shaders.find(shader);
		if (element == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - getShaderInterfaceBlock(uniformBlock)", "shader not find", Key<InterfaceBlock>(KEY_DESTROY))
			auto &s = element->second;
		return (s.getInterfaceBlock(target));
	}

	ShadingManager &ShadingManager::setShaderInterfaceBlock(Key<Shader> const &shader, Key<InterfaceBlock> const &key, Key<UniformBlock> const &keyUniformBlock)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - setShaderInterfaceBlock(uniformBlock)", "key destroy", *this)
			auto &element = _shaders.find(shader);
		if (element == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - setShaderInterfaceBlock(uniformBlock)", "shader not find", *this)
		auto &s = element->second;
		if (!keyUniformBlock)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - setShaderInterfaceBlock(uniformBlock)", "uniformBuffer key destroy", *this)
			auto &ubElement = _uniformBlock.find(keyUniformBlock);
		if (ubElement == _uniformBlock.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - setShaderInterfaceBlock(uniformBlock)", "uniformBuffer key is not found", *this)
		auto &u = ubElement->second;
		s.setInterfaceBlock(key, u);
		return (*this);
	}
}