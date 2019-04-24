#pragma once

#include <afxwin.h> 
#include <afxext.h>

#include "SceneObject.h"
#include <vector>

// CHierarchy
class CHierarchy : public CWnd
{
	DECLARE_DYNAMIC(CHierarchy)

public:
	CHierarchy();
	virtual ~CHierarchy();

	void Setup();
	void SetObjectData(std::vector<SceneObject>* sceneGraph);

	afx_msg void Select();

protected:
	CListBox m_list;
	std::vector<SceneObject>* m_graph;

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	DECLARE_MESSAGE_MAP()
};


