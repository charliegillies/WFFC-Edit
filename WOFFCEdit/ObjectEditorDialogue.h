#pragma once



class ObjectEditorDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(ObjectEditorDialogue)

public:
	ObjectEditorDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ObjectEditorDialogue();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = ID_OBJECT_EDITOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditTransformX();
	afx_msg void OnEnChangeEditTransformY();
	afx_msg void OnEnChangeEditObjectName();
	afx_msg void OnEnChangeEditTransformZ();
};
