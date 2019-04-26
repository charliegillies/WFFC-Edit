#include "ToolMain.h"
#include "resource.h"
#include <vector>

#include "EditorCommands.h"
#include "History.h"

const int NO_SELECTION_ID = -1;

//
//ToolMain Class
ToolMain::ToolMain() : m_manipulator()
{
	m_currentChunk = 0;		//default value
	m_selectedObject = NO_SELECTION_ID;	//initial selection ID
	m_graph.clear();	//clear the scenegraph
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
	if (!m_graph.empty()) {
		m_graph.clear();
	}

	// read our scene graph & chunk data from the database IO
	m_database.read(&m_graph, &m_chunk);
	//Process results into renderable
	m_d3dRenderer.BuildDisplayList(m_graph.getObjects());
	//build the renderable chunk 
	m_d3dRenderer.BuildDisplayChunk(&m_chunk);
}

void ToolMain::onActionSave()
{
	// Write the scene graph to the database
	m_database.writeGraph(&m_graph);
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

	SceneObject* selected = getSelectedObject();
	if (m_mode == EditorMode::CAMERA) {
		m_d3dRenderer.setCameraLock(false);
	}
	else {
		// lock camera if we are not in camera mode 
		m_d3dRenderer.setCameraLock(true);

		// if we have the selection, use the manipulator tool
		// and if the tool is used - change the flag to rebuild our display
		if (selected != nullptr) {
			if (m_mode == EditorMode::MOVE) {
				m_doRebuildDisplay |= m_manipulator.translate(&m_inputCommands, selected);
			}
			else if (m_mode == EditorMode::ROTATE) {
				m_doRebuildDisplay |= m_manipulator.rotate(&m_inputCommands, selected);
			}
			else if (m_mode == EditorMode::SCALE) {
				m_doRebuildDisplay |= m_manipulator.scale(&m_inputCommands, selected);
			}
		}
	}
	
	// Dirty flag that indicates if the display list requires
	// to be rebuilt or not.
	if (m_doRebuildDisplay || m_graph.isDirty()) {
		m_d3dRenderer.BuildDisplayList(m_graph.getObjects());
		m_doRebuildDisplay = false;
		m_graph.setDirty(false);
	}

	//Renderer Update Call
	m_d3dRenderer.Tick(&m_inputCommands);
}

void ToolMain::UpdateInput(MSG * msg)
{
	// forward onto our input processor to process it
	m_input.process_msg(msg);
}

Command* ToolMain::createAddNewSceneObjectCommand()
{
	return new AddNewSceneObjectCommand(this);
}

InputCommands& ToolMain::getInputCommands()
{
	return m_inputCommands;
}

void ToolMain::setSelectionID(int id)
{
	m_selectedObject = id;
}

bool ToolMain::onToggleWireframe()
{
	return m_d3dRenderer.toggleWireframe();
}

void ToolMain::editorModeChanged(const EditorMode mode)
{
	m_mode = mode;
}

SceneGraph * ToolMain::getGraph()
{
	return &m_graph;
}

SceneObject * ToolMain::getSelectedObject()
{
	// No selection? nullptr!
	if (m_selectedObject == NO_SELECTION_ID) return nullptr;
	// otherwise, search through the graph for our selection!
	return m_graph.getObjectById(m_selectedObject);
}
