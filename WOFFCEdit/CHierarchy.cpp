// CHierarchy.cpp : implementation file
//

#include "stdafx.h"
#include "CHierarchy.h"


// CHierarchy

IMPLEMENT_DYNAMIC(CHierarchy, CWnd)


BEGIN_MESSAGE_MAP(CHierarchy, CWnd)
END_MESSAGE_MAP()


CHierarchy::CHierarchy() : m_list()
{
}

CHierarchy::~CHierarchy()
{
}

void CHierarchy::Setup()
{
	CRect size;
	GetWindowRect(&size);

	m_list.Create(WS_CHILD | WS_VISIBLE | LBS_STANDARD | WS_HSCROLL, 
		CRect(0, 0, size.right, size.bottom), this, NULL);
	
	// append some test data!
	for(int i = 0; i < 5; i++)
		m_list.AddString(L"This is some text data.");
}

BOOL CHierarchy::PreCreateWindow(CREATESTRUCT & cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	//cs.dwExStyle |= WS_EX_CLIENTEDGE;
	//cs.style &~ WS_BORDER;
	//cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
	//	::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return TRUE;
}


// CHierarchy message handlers
