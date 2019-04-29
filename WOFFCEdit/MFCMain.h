#pragma once

#include <afxwin.h> 
#include <afxext.h>
#include <afx.h>
#include "pch.h"
#include "Game.h"
#include "ToolMain.h"
#include "resource.h"
#include "MFCFrame.h"
#include "History.h"

#include "SelectDialogue.h"
#include "ObjectEditorDialogue.h"

class MFCMain;

class ChangeEditorModeCommand : public Command {
private:
	EditorMode m_next, m_last;
	MFCMain* m_main;
public:
	ChangeEditorModeCommand(EditorMode next, EditorMode last, MFCMain* main);
	// Inherited via Command
	virtual void execute(ToolMain * tool, bool asRedo) override;
	virtual void undo(ToolMain * tool) override;
	virtual std::wstring get_label() override;
};

class MFCMain : public CWinApp 
{
public:
	MFCMain();
	~MFCMain();
	BOOL InitInstance();
	int  Run();

	void ProcessInput(InputCommands * input);
	void ChangeEditorMode(const EditorMode mode);

private:
	CMyFrame * m_frame;	//handle to the frame where all our UI is
	HWND m_toolHandle;	//Handle to the MFC window
	ToolMain m_toolSystem;	//Instance of Tool System that we interface to. 
	CRect WindowRECT;	//Window area rectangle. 
	SelectDialogue m_selectDialogue;			//for modeless dialogue, declare it here
	ObjectEditorDialogue m_objectEditDialogue;

	bool m_editorCreated;

	EditorMode m_mode;
	History m_history;

	int m_width;		
	int m_height;
	
	//Interface funtions for menu and toolbar etc requires
	afx_msg void MenuFileQuit();
	afx_msg void MenuFileSaveTerrain();
	afx_msg void MenuEditSelect();
	afx_msg	void Button_SaveScene();
	afx_msg void Button_NewSceneObject();
	afx_msg void Button_ToggleWireframe();
	afx_msg void Button_ToggleGrid();

	// Editor controls
	afx_msg void Button_CameraToggle();
	afx_msg void Button_TranslateToggle();
	afx_msg void Button_RotateToggle();
	afx_msg void Button_ScaleToggle();

	afx_msg void Button_BrowseHierarchy();
	afx_msg void Button_EditObject();

	// Terrain edit controls
	afx_msg void Button_FlattenTerrain();

	DECLARE_MESSAGE_MAP()	// required macro for message map functionality  One per class
};
