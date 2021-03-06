#pragma once

#include <afxwin.h> 
#include <afxext.h>
#include "MFCRenderFrame.h"
#include "ToolMain.h"



class CMyFrame : public CFrameWnd
{
protected:
//	DECLARE_DYNAMIC(CMainFrame)

public:
	CMenu			m_menu1;	//menu bar
	CStatusBar		m_wndStatusBar;
	CToolBar		m_toolBar;
	CToolBar		m_worldEditToolbar;
	CChildRender	m_DirXView;

	void EditorModeChanged(const EditorMode mode);
	void ChangeWireframeButtonState(const bool enabled);
	void ChangeGridButtonState(const bool enabled);
public:
	CMyFrame();
	void SetCurrentSelectionID(int ID);
	afx_msg void OnUpdatePage(CCmdUI *pCmdUI);


private:	//overrides
	int		m_selectionID;	//

	//note the afx_message keyword is linking this method to message map access.
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()	// required macro for message map functionality  One per class
};