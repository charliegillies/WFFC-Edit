#include "MFCFrame.h"
#include "resource.h"



BEGIN_MESSAGE_MAP(CMyFrame, CFrameWnd)
	
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TOOL, &CMyFrame::OnUpdatePage)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_TOOL
};


CRect SimplerRect(int x, int y, int xSize, int ySize) {
	return CRect(x, y, x + xSize, y + ySize);
}

//frame initialiser
CMyFrame::CMyFrame()
{
	m_selectionID = 999; //an obviously wrong selection ID,  to verify its working
}

void CMyFrame::SetCurrentSelectionID(int ID)
{
	m_selectionID = ID;
}

void CMyFrame::OnUpdatePage(CCmdUI * pCmdUI)
{
	pCmdUI->Enable();
	CString strPage;
	strPage.Format(_T("%d"), m_selectionID);
	pCmdUI->SetText(strPage);
}

//oncretae, called after init but before window is shown. 
int CMyFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1) return -1;
	
	const CRect viewSize = SimplerRect(0, 0, 1024, 576);
	if (!m_viewportWindow.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, viewSize, this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create hierarchy window\n");
		return -1;
	}
	// create a view to occupy the client area of the frame. this is where DirectX is rendered
	if (!m_DirXView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, viewSize, &m_viewportWindow, AFX_IDW_PANE_FIRST + 1, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	if (!m_hierarchy.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, SimplerRect(1024, 40, 440, 200), this, AFX_IDW_PANE_FIRST + 2, NULL)) 
	{
		TRACE0("Failed to create hierarchy window\n");
		return -1;
	}
	m_hierarchy.Setup();

	m_menu1.LoadMenuW(IDR_MENU1);
	SetMenu(&m_menu1);
	
	if (!m_toolBar.CreateEx(this, TBSTYLE_TRANSPARENT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_toolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CRect rect;
	GetClientRect(&rect);
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));
	m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_TOOL, SBPS_NORMAL, rect.Width() - 500);//set width of status bar panel

	return 0;
}