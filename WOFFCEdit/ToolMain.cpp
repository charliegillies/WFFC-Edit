#include "ToolMain.h"
#include "resource.h"
#include <vector>

#include "EditorCommands.h"
#include "History.h"

//
//ToolMain Class
ToolMain::ToolMain()
{
	m_currentChunk = 0;		//default value
	m_selectedObject = 0;	//initial selection ID
	m_sceneGraph.clear();	//clear the vector for the scenegraph
	m_doRebuildDisplay = false;

	ZeroMemory(&m_inputCommands, sizeof(InputCommands));
}


ToolMain::~ToolMain()
{
	m_database.closeConnection();
}


int ToolMain::getCurrentSelectionID()
{
	return m_selectedObject;
}

void ToolMain::onActionInitialise(HWND handle, int width, int height)
{
	//window size, handle etc for directX
	m_width		= width;
	m_height	= height;
	// Initialize our directX 3d renderer
	m_d3dRenderer.Initialize(handle, m_width, m_height);

	bool opened = m_database.tryOpenConnection();
	assert(("Failed to open database connection", opened));

	onActionLoad();
}

void ToolMain::onActionLoad()
{
	// Load the current chunk and scene objects into our data
	if (!m_sceneGraph.empty()) {
		m_sceneGraph.clear();
	}

	m_database.read(&m_sceneGraph, &m_chunk);

	//Process results into renderable
	m_d3dRenderer.BuildDisplayList(&m_sceneGraph);
	//build the renderable chunk 
	m_d3dRenderer.BuildDisplayChunk(&m_chunk);
}

void ToolMain::onActionSave()
{
	m_database.writeGraph(&m_sceneGraph);
	MessageBox(NULL, L"Objects Saved", L"Notification", MB_OK);
}

void ToolMain::onActionSaveTerrain()
{
	m_d3dRenderer.SaveDisplayChunk(&m_chunk);
}

void ToolMain::Tick(MSG *msg, History* history)
{
	// Process input, retrieves the current input commands
	m_inputCommands = m_input.tick();

	// If we click, then we need to perform a raycast to see
	// if we have selected an entity from the cameras perspective
	if (m_inputCommands.leftMouse == ClickState::DOWN) {
		std::vector<int> picked = m_d3dRenderer.FindMouseRayTargets();
		
		int new_pick = -1;
		if (picked.size() > 0) {
			if (picked.size() == 1) {
				new_pick = picked[0];
			}
			else {
				// TODO: sort by distance to find the guys we want!
			}
		}

		if (new_pick != -1 && new_pick != m_selectedObject) {
			// log the selection change - so the user can undo it
			history->log(new ChangeSelectionCommand(new_pick, m_selectedObject, this));
		}
	}

	//do we have a selection
	//do we have a mode
	//are we clicking / dragging /releasing
	//has something changed
		//update Scenegraph
		//add to scenegraph
		//resend scenegraph to Direct X renderer



	// Dirty flag that indicates if the display list requires
	// to be rebuilt or not.
	if (m_doRebuildDisplay) {
		m_d3dRenderer.BuildDisplayList(&m_sceneGraph);
		m_doRebuildDisplay = false;
	}

	//Renderer Update Call
	m_d3dRenderer.Tick(&m_inputCommands);
}

void ToolMain::UpdateInput(MSG * msg)
{
	m_input.process_msg(msg);

}

Command* ToolMain::createAddNewSceneObjectCommand()
{
	return new AddNewSceneObjectCommand(this);
}

SceneObject& ToolMain::createNewSceneObject()
{
	// Push an empty scene object into the graph, then get a reference to it
	SceneObject& new_obj = insertSceneObject(SceneObject::CreatePrimitive());
	return new_obj;
}

SceneObject & ToolMain::insertSceneObject(SceneObject && obj)
{
	m_doRebuildDisplay = true;
	m_sceneGraph.push_back(obj);
	SceneObject& new_obj = m_sceneGraph.back();
	new_obj.ID = createNewSceneObjectID();
	return new_obj;
}

int ToolMain::createNewSceneObjectID()
{
	// We want to generate an ID for our scene object
	// this can be done multiple ways, including just allowing the database 
	// to do it with auto-increment, but since we have 
	// local records of the entities, we should be
	// able to just get a new id from our local records
	int new_id = -1;
	for (SceneObject& sObj : m_sceneGraph)
		new_id = max(new_id, sObj.ID);
	return new_id + 1;
}

bool ToolMain::removeSceneObject(SceneObject & target)
{
	// The target ref is probably out of date
	// and so we will need to get the up to date one with the equal id
	for (auto it = m_sceneGraph.begin(); it != m_sceneGraph.end(); it++) {
		SceneObject& obj = *it;
		if (obj.ID == target.ID) {
			// update reference
			target = obj;
			m_sceneGraph.erase(it);
			m_doRebuildDisplay = true;
			return true;
		}
	}
	return false;
}

InputCommands& ToolMain::getInputCommands()
{
	return m_inputCommands;
}

void ToolMain::forceSetSelectionID(int id)
{
	m_selectedObject = id;

	for (auto& obj : m_sceneGraph) {
		if (obj.ID == id) {
			obj.tex_diffuse_path = "database/data/rock.dds";
			m_doRebuildDisplay = true;
			break;
		}
	}
}

bool ToolMain::onToggleWireframe()
{
	return m_d3dRenderer.toggleWireframe();
}

void ToolMain::editorModeChanged(const EditorMode mode)
{
	m_mode = mode;
}
