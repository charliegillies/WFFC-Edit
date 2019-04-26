#include "SceneGraph.h"
#include <algorithm> // std::max

SceneGraph::SceneGraph()
{
	setDirty(true);
}

void SceneGraph::setDirty(const bool dirty)
{
	m_dirty = dirty;
}

const bool SceneGraph::isDirty() const
{
	return m_dirty;
}

std::vector<SceneObject>* SceneGraph::getObjects()
{
	return &m_objects;
}

SceneObject* SceneGraph::getObjectById(const int id)
{
	// TODO: Move this to a map for no linear lookup time.
	for (SceneObject& so : m_objects) {
		if (so.ID == id) return &so;
	}

	return nullptr;
}

void SceneGraph::clear()
{
	m_objects.clear();
}

const bool SceneGraph::empty() const
{
	return m_objects.empty();
}

const size_t SceneGraph::size() const
{
	return m_objects.size();
}

SceneObject & SceneGraph::createNewSceneObject()
{
	// Push an empty scene object into the graph, then get a reference to it
	SceneObject& new_obj = insertSceneObject(SceneObject::CreatePrimitive());
	return new_obj;
}

SceneObject & SceneGraph::insertSceneObject(SceneObject && obj)
{
	setDirty(true);
	// moves the object into the list
	m_objects.push_back(obj);
	// then we get the ref, assign it a new id and return it
	SceneObject& new_obj = m_objects.back();
	new_obj.ID = createNewSceneObjectID();
	return new_obj;
}

int SceneGraph::createNewSceneObjectID() const
{
	// We want to generate an ID for our scene object
	// this can be done multiple ways, including just allowing the database 
	// to do it with auto-increment, but since we have 
	// local records of the entities, we should be
	// able to just get a new id from our local records
	int new_id = -1;
	for (const SceneObject& sObj : m_objects)
		new_id = std::max(new_id, sObj.ID);
	return new_id + 1;
}

bool SceneGraph::removeSceneObject(SceneObject & target)
{
	// The target ref is probably out of date
	// and so we will need to get the up to date one with the equal id
	for (auto it = m_objects.begin(); it != m_objects.end(); it++) {
		SceneObject& obj = *it;
		if (obj.ID == target.ID) {
			// update reference
			target = obj;
			m_objects.erase(it);
			setDirty(true);
			return true;
		}
	}
	return false;
}
