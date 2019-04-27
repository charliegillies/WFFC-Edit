#pragma once

#include <string>
#include <map>
#include <vector>

// Holds a list of each resource that can be stored in the database
// and then used within the project.
class ResourceHandler {
public:
	// Mimics the table data layout of the meshes and texture tables
	struct Resource {
		int id;
		std::string name;
		std::string filepath;
	};

private:
	std::map<std::string, Resource> m_textures;
	std::map<std::string, Resource> m_meshes;

public:
	void addTexResource(const int id, const std::string& name, const std::string& filepath);
	void addMeshResource(const int id, const std::string& name, const std::string& filepath);

	const std::vector<std::string> getTextureNames() const;
	const std::vector<std::string> getMeshNames() const;

	const std::string getTextureNameByFilepath(const std::string& path) const;
	const std::string getMeshNameByFilepath(const std::string& path) const;
	
	const std::string getModelPathFromName(const std::string& name) const;
	const std::string getTexturePathFromName(const std::string& tex) const;

};
