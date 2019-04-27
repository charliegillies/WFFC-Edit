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

class MFCMain : public CWinApp 
{
public:
	MFCMain();
	~MFCMain();
	BOOL InitInstance();
	int  Run();

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

	afx_msg void Button_BrowseHiearchy();
	afx_msg void Button_EditObject();

	void ChangeEditorMode(const EditorMode mode);

	DECLARE_MESSAGE_MAP()	// required macro for message map functionality  One per class
};
