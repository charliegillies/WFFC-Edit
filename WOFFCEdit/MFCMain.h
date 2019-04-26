#pragma once

#include <afxwin.h> 
#include <afxext.h>
#include <afx.h>
#include "pch.h"
#include "Game.h"
#include "ToolMain.h"
#include "resource.h"
#include "MFCFrame.h"
#include "SelectDialogue.h"
#include "History.h"

enum class EditorMode {
	CAMERA, MOVE, ROTATE, SCALE
};

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
	ToolMain m_ToolSystem;	//Instance of Tool System that we interface to. 
	CRect WindowRECT;	//Window area rectangle. 
	SelectDialogue m_ToolSelectDialogue;			//for modeless dialogue, declare it here
	
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

	// Editor controls
	afx_msg void Button_CameraToggle();
	afx_msg void Button_TranslateToggle();
	afx_msg void Button_RotateToggle();
	afx_msg void Button_ScaleToggle();

	DECLARE_MESSAGE_MAP()	// required macro for message map functionality  One per class
};
