#pragma once
#include <afxwin.h> 
#include <afxext.h>
#include "MFCRenderFrame.h"

#include "ToolMain.h"
#include "CHierarchy.h"
#include "SelectDialogue.h"



class CMyFrame : public CFrameWnd
{
protected:
//	DECLARE_DYNAMIC(CMainFrame)

public:
	CMenu			m_menu1;
	CStatusBar		m_wndStatusBar;
	CToolBar		m_toolBar;
	CChildRender	m_DirXView;
	CWnd m_hierarchyWindow;
	//CHierarchy m_hierarchy;

public:
	CMyFrame();
	void SetCurrentSelectionID(int ID);
	afx_msg void OnUpdatePage(CCmdUI *pCmdUI);


private:
	int	m_selectionID;

	//note the afx_message keyword is linking this method to message map access.
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()	// required macro for message map functionality  One per class
};