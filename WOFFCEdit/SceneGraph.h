#pragma once

#include "SceneObject.h"
#include <vector>

// A wrapper around the scene graph with utilities
// for helping the operations.
class SceneGraph {
private:
	std::vector<SceneObject> m_objects;
	bool m_dirty;

public:
	SceneGraph();
	
	void setDirty(const bool dirty);
	const bool isDirty() const;

	std::vector<SceneObject>* getObjects();
	SceneObject* getObjectById(const int id);

	void clear();
	const bool empty() const;
	const size_t size() const;

	SceneObject& createNewSceneObject();
	SceneObject& insertSceneObject(SceneObject&& obj);
	int createNewSceneObjectID() const;
	bool removeSceneObject(SceneObject& target);

};
