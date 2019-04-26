#include "ToolMain.h"
#include "resource.h"
#include <vector>
#include <sstream>

#include "EditorCommands.h"
#include "History.h"

//
//ToolMain Class
ToolMain::ToolMain()
{
	m_currentChunk = 0;		//default value
	m_selectedObject = 0;	//initial selection ID
	m_sceneGraph.clear();	//clear the vector for the scenegraph
	m_databaseConnection = NULL;
	m_doRebuildDisplay = false;

	ZeroMemory(&m_inputCommands, sizeof(InputCommands));
}


ToolMain::~ToolMain()
{
	sqlite3_close(m_databaseConnection);		//close the database connection
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
	
	m_d3dRenderer.Initialize(handle, m_width, m_height);

	//database connection establish
	int rc;
	rc = sqlite3_open_v2("database/test.db",&m_databaseConnection, SQLITE_OPEN_READWRITE, NULL);

	if (rc) 
	{
		TRACE("Can't open database");
		//if the database cant open. Perhaps a more catastrophic error would be better here
	}
	else 
	{
		TRACE("Opened database successfully");
	}

	onActionLoad();
}

void ToolMain::onActionLoad()
{
	//load current chunk and objects into lists
	if (!m_sceneGraph.empty())		//is the vector empty
	{
		m_sceneGraph.clear();		//if not, empty it
	}

	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query
	sqlite3_stmt *pResultsChunk;

	//OBJECTS IN THE WORLD
	//prepare SQL Text
	sqlCommand = "SELECT * from Objects";				//sql command which will return all records from the objects table.
	//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0 );
	
	//loop for each row in results until there are no more rows.  ie for every row in the results. We create and object
	while (sqlite3_step(pResults) == SQLITE_ROW)
	{	
		SceneObject newSceneObject;
		newSceneObject.ID = sqlite3_column_int(pResults, 0);
		newSceneObject.chunk_ID = sqlite3_column_int(pResults, 1);
		newSceneObject.model_path		= reinterpret_cast<const char*>(sqlite3_column_text(pResults, 2));
		newSceneObject.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 3));
		newSceneObject.posX = sqlite3_column_double(pResults, 4);
		newSceneObject.posY = sqlite3_column_double(pResults, 5);
		newSceneObject.posZ = sqlite3_column_double(pResults, 6);
		newSceneObject.rotX = sqlite3_column_double(pResults, 7);
		newSceneObject.rotY = sqlite3_column_double(pResults, 8);
		newSceneObject.rotZ = sqlite3_column_double(pResults, 9);
		newSceneObject.scaX = sqlite3_column_double(pResults, 10);
		newSceneObject.scaY = sqlite3_column_double(pResults, 11);
		newSceneObject.scaZ = sqlite3_column_double(pResults, 12);
		newSceneObject.render = sqlite3_column_int(pResults, 13);
		newSceneObject.collision = sqlite3_column_int(pResults, 14);
		newSceneObject.collision_mesh = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 15));
		newSceneObject.collectable = sqlite3_column_int(pResults, 16);
		newSceneObject.destructable = sqlite3_column_int(pResults, 17);
		newSceneObject.health_amount = sqlite3_column_int(pResults, 18);
		newSceneObject.editor_render = sqlite3_column_int(pResults, 19);
		newSceneObject.editor_texture_vis = sqlite3_column_int(pResults, 20);
		newSceneObject.editor_normals_vis = sqlite3_column_int(pResults, 21);
		newSceneObject.editor_collision_vis = sqlite3_column_int(pResults, 22);
		newSceneObject.editor_pivot_vis = sqlite3_column_int(pResults, 23);
		newSceneObject.pivotX = sqlite3_column_double(pResults, 24);
		newSceneObject.pivotY = sqlite3_column_double(pResults, 25);
		newSceneObject.pivotZ = sqlite3_column_double(pResults, 26);
		newSceneObject.snapToGround = sqlite3_column_int(pResults, 27);
		newSceneObject.AINode = sqlite3_column_int(pResults, 28);
		newSceneObject.audio_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 29));
		newSceneObject.volume = sqlite3_column_double(pResults, 30);
		newSceneObject.pitch = sqlite3_column_double(pResults, 31);
		newSceneObject.pan = sqlite3_column_int(pResults, 32);
		newSceneObject.one_shot = sqlite3_column_int(pResults, 33);
		newSceneObject.play_on_init = sqlite3_column_int(pResults, 34);
		newSceneObject.play_in_editor = sqlite3_column_int(pResults, 35);
		newSceneObject.min_dist = sqlite3_column_double(pResults, 36);
		newSceneObject.max_dist = sqlite3_column_double(pResults, 37);
		newSceneObject.camera = sqlite3_column_int(pResults, 38);
		newSceneObject.path_node = sqlite3_column_int(pResults, 39);
		newSceneObject.path_node_start = sqlite3_column_int(pResults, 40);
		newSceneObject.path_node_end = sqlite3_column_int(pResults, 41);
		newSceneObject.parent_id = sqlite3_column_int(pResults, 42);
		newSceneObject.editor_wireframe = sqlite3_column_int(pResults, 43);
		newSceneObject.name = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 44));

		newSceneObject.light_type = sqlite3_column_int(pResults, 45);
		newSceneObject.light_diffuse_r = sqlite3_column_double(pResults, 46);
		newSceneObject.light_diffuse_g = sqlite3_column_double(pResults, 47);
		newSceneObject.light_diffuse_b = sqlite3_column_double(pResults, 48);
		newSceneObject.light_specular_r = sqlite3_column_double(pResults, 49);
		newSceneObject.light_specular_g = sqlite3_column_double(pResults, 50);
		newSceneObject.light_specular_b = sqlite3_column_double(pResults, 51);
		newSceneObject.light_spot_cutoff = sqlite3_column_double(pResults, 52);
		newSceneObject.light_constant = sqlite3_column_double(pResults, 53);
		newSceneObject.light_linear = sqlite3_column_double(pResults, 54);
		newSceneObject.light_quadratic = sqlite3_column_double(pResults, 55);
	
		//send completed object to scenegraph
		m_sceneGraph.push_back(newSceneObject);
	}

	//THE WORLD CHUNK
	//prepare SQL Text
	sqlCommand = "SELECT * from Chunks";				//sql command which will return all records from  chunks table. There is only one tho.
														//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResultsChunk, 0);


	sqlite3_step(pResultsChunk);
	m_chunk.ID = sqlite3_column_int(pResultsChunk, 0);
	m_chunk.name = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 1));
	m_chunk.chunk_x_size_metres = sqlite3_column_int(pResultsChunk, 2);
	m_chunk.chunk_y_size_metres = sqlite3_column_int(pResultsChunk, 3);
	m_chunk.chunk_base_resolution = sqlite3_column_int(pResultsChunk, 4);
	m_chunk.heightmap_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 5));
	m_chunk.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 6));
	m_chunk.tex_splat_alpha_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 7));
	m_chunk.tex_splat_1_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 8));
	m_chunk.tex_splat_2_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 9));
	m_chunk.tex_splat_3_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 10));
	m_chunk.tex_splat_4_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 11));
	m_chunk.render_wireframe = sqlite3_column_int(pResultsChunk, 12);
	m_chunk.render_normals = sqlite3_column_int(pResultsChunk, 13);
	m_chunk.tex_diffuse_tiling = sqlite3_column_int(pResultsChunk, 14);
	m_chunk.tex_splat_1_tiling = sqlite3_column_int(pResultsChunk, 15);
	m_chunk.tex_splat_2_tiling = sqlite3_column_int(pResultsChunk, 16);
	m_chunk.tex_splat_3_tiling = sqlite3_column_int(pResultsChunk, 17);
	m_chunk.tex_splat_4_tiling = sqlite3_column_int(pResultsChunk, 18);


	//Process results into renderable
	m_d3dRenderer.BuildDisplayList(&m_sceneGraph);
	//build the renderable chunk 
	m_d3dRenderer.BuildDisplayChunk(&m_chunk);

}

void ToolMain::onActionSave()
{
	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query
	

	//OBJECTS IN THE WORLD Delete them all
	//prepare SQL Text
	sqlCommand = "DELETE FROM Objects";	 //will delete the whole object table.   Slightly risky but hey.
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0);
	sqlite3_step(pResults);

	//Populate with our new objects
	std::wstring sqlCommand2;
	int numObjects = m_sceneGraph.size();	//Loop thru the scengraph.

	for (int i = 0; i < numObjects; i++)
	{
		SceneObject& obj = m_sceneGraph.at(i);

		std::stringstream command;
		command << "INSERT INTO Objects "
			<< "VALUES(" << obj.ID << ","
			<< obj.chunk_ID << ","
			<< "'" << obj.model_path << "'" << ","
			<< "'" << obj.tex_diffuse_path << "'" << ","
			<< obj.posX << ","
			<< obj.posY << ","
			<< obj.posZ << ","
			<< obj.rotX << ","
			<< obj.rotY << ","
			<< obj.rotZ << ","
			<< obj.scaX << ","
			<< obj.scaY << ","
			<< obj.scaZ << ","
			<< obj.render << ","
			<< obj.collision << ","
			<< "'" << obj.collision_mesh << "'" << ","
			<< obj.collectable << ","
			<< obj.destructable << ","
			<< obj.health_amount << ","
			<< obj.editor_render << ","
			<< obj.editor_texture_vis << ","
			<< obj.editor_normals_vis << ","
			<< obj.editor_collision_vis << ","
			<< obj.editor_pivot_vis << ","
			<< obj.pivotX << ","
			<< obj.pivotY << ","
			<< obj.pivotZ << ","
			<< obj.snapToGround << ","
			<< obj.AINode << ","
			<< "'" << obj.audio_path << "'" << ","
			<< obj.volume << ","
			<< obj.pitch << ","
			<< obj.pan << ","
			<< obj.one_shot << ","
			<< obj.play_on_init << ","
			<< obj.play_in_editor << ","
			<< obj.min_dist << ","
			<< obj.max_dist << ","
			<< obj.camera << ","
			<< obj.path_node << ","
			<< obj.path_node_start << ","
			<< obj.path_node_end << ","
			<< obj.parent_id << ","
			<< obj.editor_wireframe << ","
			<< "'" << obj.name << "'" << ","

			<< obj.light_type << ","
			<< obj.light_diffuse_r << ","
			<< obj.light_diffuse_g << ","
			<< obj.light_diffuse_b << ","
			<< obj.light_specular_r << ","
			<< obj.light_specular_g << ","
			<< obj.light_specular_b << ","
			<< obj.light_spot_cutoff << ","
			<< obj.light_constant << ","
			<< obj.light_linear << ","
			<< obj.light_quadratic

			<< ")";
		std::string sqlCommand2 = command.str();
		rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand2.c_str(), -1, &pResults, 0);
		sqlite3_step(pResults);	
	}
	MessageBox(NULL, L"Objects Saved", L"Notification", MB_OK);
}

void ToolMain::onActionSaveTerrain()
{
	m_d3dRenderer.SaveDisplayChunk(&m_chunk);
}

void ToolMain::Tick(MSG *msg, History* history)
{
	// Copy the keys from the this input frame
	// to the last input frame, so we can compare
	for (int i = 0; i < NUM_KEYS; i++)
		m_lastKeyArray[i] = m_keyArray[i];

	// calculate the mouse velocity from current mouse and last mouse position
	m_inputCommands.mouseVelocityX = (float)m_inputCommands.mouseX - m_lastMouseX;
	m_inputCommands.mouseVelocityY = (float)m_inputCommands.mouseY - m_lastMouseY;
	m_lastMouseX = m_inputCommands.mouseX;
	m_lastMouseY = m_inputCommands.mouseY;

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
	switch (msg->message) {
	case WM_KEYDOWN:
		m_keyArray[msg->wParam] = true;
		break;

	case WM_KEYUP:
		m_keyArray[msg->wParam] = false;
		break;

	case WM_MOUSEMOVE:
		m_inputCommands.mouseX = GET_X_LPARAM(msg->lParam);
		m_inputCommands.mouseY = GET_Y_LPARAM(msg->lParam);
		break;

	case WM_LBUTTONDOWN:
		m_inputCommands.leftMouse = ClickState::DOWN;
		break;
	case WM_LBUTTONUP:
		m_inputCommands.leftMouse = ClickState::UP;
		break;

	case WM_RBUTTONDOWN:
		m_inputCommands.rightMouse = ClickState::DOWN;
		break;
	case WM_RBUTTONUP:
		m_inputCommands.rightMouse = ClickState::UP;
		break;
	}

	m_inputCommands.ctrl = m_keyArray[VK_CONTROL];
	m_inputCommands.shift = m_keyArray[VK_SHIFT];
	if (!m_inputCommands.ctrl) {
		// Map actions if their appropriate keybinds are down
		// and the control modifier is NOT down
		m_inputCommands.forward = m_keyArray['W'];
		m_inputCommands.back = m_keyArray['S'];
		m_inputCommands.left = m_keyArray['A'];
		m_inputCommands.right = m_keyArray['D'];
		m_inputCommands.rotateLeft = m_keyArray['Q'];
		m_inputCommands.rotateRight = m_keyArray['E'];
		m_inputCommands.moveDown = m_keyArray['Z'];
		m_inputCommands.moveUp = m_keyArray['X'];
	}
	else {
		// Ctrl modifier is down, so check shortcuts!
		m_inputCommands.undo = m_keyArray['Z'] && !m_lastKeyArray['Z'];
		m_inputCommands.redo = m_keyArray['Y'] && !m_lastKeyArray['Y'];
		m_inputCommands.save = m_keyArray['S'] && !m_lastKeyArray['S'];
	}
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
