#include "ResourceHandler.h"

#include <sstream>

void ResourceHandler::addTexResource(const int id, const std::string & name, const std::string & filepath)
{
	Resource resource;
	resource.id = id;
	resource.name = name;
	resource.filepath = filepath;
	m_textures[name] = resource;
}

void ResourceHandler::addMeshResource(const int id, const std::string & name, const std::string & filepath)
{
	Resource resource;
	resource.id = id;
	resource.name = name;
	resource.filepath = filepath;
	m_meshes[name] = resource;
}

const std::vector<std::string> ResourceHandler::getTextureNames() const
{
	std::vector<std::string> names;
	for (auto& pair : m_textures) names.push_back(pair.first);
	return names;
}

const std::vector<std::string> ResourceHandler::getMeshNames() const
{
	std::vector<std::string> names;
	for (auto& pair : m_meshes) names.push_back(pair.first);
	return names;
}

const std::string ResourceHandler::getTextureNameByFilepath(const std::string & path) const
{
	for (auto& pair : m_textures) {
		if (pair.second.filepath == path) {
			return pair.first;
		}
	}
	return "";
}

const std::string ResourceHandler::getMeshNameByFilepath(const std::string & path) const
{
	for (auto& pair : m_meshes) {
		if (pair.second.filepath == path) {
			return pair.first;
		}
	}
	return "";
}

const std::string ResourceHandler::getModelPathFromName(const std::string & name) const
{
	if (m_meshes.find(name) != m_meshes.end()) {
		return m_meshes.at(name).filepath;
	}
	return "";
}

const std::string ResourceHandler::getTexturePathFromName(const std::string & tex) const
{
	if (m_textures.find(tex) != m_textures.end()) {
		return m_textures.at(tex).filepath;
	}
	return "";
}
