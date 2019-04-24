#pragma once

#include <afxwin.h> 
#include <afxext.h>

// CHierarchy
class CHierarchy : public CWnd
{
	DECLARE_DYNAMIC(CHierarchy)

public:
	CHierarchy();
	virtual ~CHierarchy();

	void Setup();

protected:
	CListBox m_list;

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	DECLARE_MESSAGE_MAP()
};


