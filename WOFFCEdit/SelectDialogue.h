#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include <vector>

class ToolMain;

class SelectDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(SelectDialogue)

public:
	SelectDialogue(CWnd* pParent, std::vector<SceneObject>* SceneGraph);   // modal // takes in out scenegraph in the constructor
	SelectDialogue(CWnd* pParent = NULL);
	virtual ~SelectDialogue();
	
	void SetObjectData(ToolMain* tool);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void End();
	afx_msg void Select(NMHDR* pNMHDR, LRESULT* pResult);

	std::vector<SceneObject>* m_sceneGraph;
	ToolMain* m_tool;

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_list;

	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();
};
