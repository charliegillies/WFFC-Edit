#pragma once

#include <afxext.h>
#include "pch.h"
#include "Game.h"
#include "sqlite3.h"
#include "SceneObject.h"
#include "InputCommands.h"
#include "Command.h"
#include "InputProcessor.h"

#include <vector>

enum class EditorMode {
	CAMERA, MOVE, ROTATE, SCALE
};

class ToolMain
{
public:
	ToolMain();
	~ToolMain();

	//onAction - These are the interface to MFC
	int		getCurrentSelectionID();										//returns the selection number of currently selected object so that It can be displayed.
	void	onActionInitialise(HWND handle, int width, int height);			//Passes through handle and hieght and width and initialises DirectX renderer and SQL LITE
	void	onActionFocusCamera();
	void	onActionLoad();													//load the current chunk
	afx_msg	void	onActionSave();											//save the current chunk
	afx_msg void	onActionSaveTerrain();									//save chunk geometry

	void	Tick(MSG *msg, class History* history);
	void	UpdateInput(MSG *msg);

	Command* createAddNewSceneObjectCommand();
	SceneObject& createNewSceneObject();
	SceneObject& insertSceneObject(SceneObject&& obj);
	int createNewSceneObjectID();

	bool removeSceneObject(SceneObject& target);
	InputCommands& getInputCommands();

	void forceSetSelectionID(int id);

	bool onToggleWireframe();

	void editorModeChanged(const EditorMode mode);

public:
	std::vector<SceneObject>    m_sceneGraph;	//our scenegraph storing all the objects in the current chunk
	ChunkObject					m_chunk;		//our landscape chunk
	int m_selectedObject;						//ID of current Selection

private:	//methods
	void	onContentAdded();

private:
	EditorMode m_mode;
	InputProcessor m_input;

	bool m_doRebuildDisplay;
	HWND	m_toolHandle;		//Handle to the  window
	Game	m_d3dRenderer;		//Instance of D3D rendering system for our tool
	InputCommands m_inputCommands;		//input commands that we want to use and possibly pass over to the renderer
	CRect	WindowRECT;		//Window area rectangle. 
	sqlite3 *m_databaseConnection;	//sqldatabase handle

	int m_width;		//dimensions passed to directX
	int m_height;
	int m_currentChunk;			//the current chunk of thedatabase that we are operating on.  Dictates loading and saving. 
	
};
