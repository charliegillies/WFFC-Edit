#pragma once

#include <afxext.h>
#include "pch.h"
#include "Game.h"
#include "SceneObject.h"
#include "InputCommands.h"
#include "Command.h"
#include "InputProcessor.h"
#include "DatabaseIO.h"
#include "ManipulationTool.h"
#include "SceneGraph.h"
#include "ResourceHandler.h"
#include "TerrainManipulator.h"

#include <vector>

enum class EditorMode {
	CAMERA, MOVE, ROTATE, SCALE, TERRAIN_SCULPT
};

class ToolMain
{
public:
	ToolMain();
	~ToolMain();

	//onAction - These are the interface to MFC
	int		getCurrentSelectionID();										//returns the selection number of currently selected object so that It can be displayed.
	void	onActionInitialise(HWND handle, int width, int height);			//Passes through handle and hieght and width and initialises DirectX renderer and SQL LITE
	void	onActionLoad();													//load the current chunk
	afx_msg	void	onActionSave();											//save the current chunk
	afx_msg void	onActionSaveTerrain();									//save chunk geometry

	void	Tick(MSG *msg, class History* history);
	void	UpdateInput(MSG *msg);

	Command* createAddNewSceneObjectCommand();

	InputCommands* getInputCommands();

	void setSelectionID(int id);
	bool onToggleWireframe();
	bool onToggleGrid();
	void editorModeChanged(const EditorMode mode);

	ResourceHandler* getResourceHandler();
	SceneObject* getSelectedObject();

	void onFlattenTerrain();

public:
	ChunkObject					m_chunk;		//our landscape chunk
	int m_selectedId;						//ID of current Selection

	SceneGraph* getGraph();
	void setDirty(bool dirty);

private:
	EditorMode m_mode;

	ResourceHandler m_resources;
	SceneGraph m_graph;
	DatabaseIO m_database;
	InputProcessor m_input;
	ManipulationTool m_manipulator;
	TerrainManipulator m_terrain;

	bool m_doRebuildDisplay;
	HWND	m_toolHandle;		//Handle to the  window
	Game	m_d3dRenderer;		//Instance of D3D rendering system for our tool
	CRect	WindowRECT;		//Window area rectangle. 

	int m_width;		//dimensions passed to directX
	int m_height;
	int m_currentChunk;			//the current chunk of thedatabase that we are operating on.  Dictates loading and saving. 
};
