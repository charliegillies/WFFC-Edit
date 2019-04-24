// CHierarchy.cpp : implementation file
//

#include "stdafx.h"
#include "CHierarchy.h"
#include <string>
#include "resource.h"

// CHierarchy
IMPLEMENT_DYNAMIC(CHierarchy, CWnd)

// Declare message map 
BEGIN_MESSAGE_MAP(CHierarchy, CWnd)
	ON_LBN_SELCHANGE(IDC_LIST1, &CHierarchy::Select)
END_MESSAGE_MAP()


CHierarchy::CHierarchy() : m_list()
{
	m_graph = nullptr;
}

CHierarchy::~CHierarchy()
{
}

void CHierarchy::Setup()
{
	CRect size;
	GetWindowRect(&size);

	bool created = m_list.Create(WS_CHILD | WS_VISIBLE | LBS_STANDARD | WS_HSCROLL,
		CRect(0, 0, size.right - size.left, size.bottom - size.top), this, IDC_LIST1);
}

void CHierarchy::SetObjectData(std::vector<SceneObject>* sceneGraph)
{
	m_graph = sceneGraph;

	for (int i = 0, size = m_graph->size(); i < size; i++) {
		SceneObject& obj = m_graph->at(i);


		std::wstring name(obj.name.begin(), obj.name.end());
		m_list.AddString(name.c_str());
		//std::wstring label = std::to_wstring(obj.name);
		//m_list.AddString(label.c_str());
	}
}

void CHierarchy::Select()
{
	int index = m_list.GetCurSel();
	CString currentSelectionValue;

	m_list.GetText(index, currentSelectionValue);
	//*m_currentSelection = _ttoi(currentSelectionValue);
}

void CHierarchy::DoDataExchange(CDataExchange * pDX)
{
	CWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BOOL CHierarchy::PreCreateWindow(CREATESTRUCT & cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return TRUE;
}


// CHierarchy message handlers
