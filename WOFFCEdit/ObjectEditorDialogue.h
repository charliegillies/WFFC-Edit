#pragma once

#include <afxwin.h>
#include <vector>

class SceneObject;
class SceneGraph;
class History;
class ResourceHandler;

// A dialogue window that acts as the primary way
// for the users to edit scene objects data.
class ObjectEditorDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(ObjectEditorDialogue)

public:
	ObjectEditorDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ObjectEditorDialogue();

#ifdef AFX_DESIGN_TIME
	enum { IDD = ID_OBJECT_EDITOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

private:
	CButton m_checkVisible;
	CEdit m_nameControl;
	CEdit m_position_x_ctrl, m_position_y_ctrl, m_position_z_ctrl;
	CEdit m_rotation_x_ctrl, m_rotation_y_ctrl, m_rotation_z_ctrl;
	CEdit m_scale_x_ctrl, m_scale_y_ctrl, m_scale_z_ctrl;
	CComboBox m_meshCombo;
	CComboBox m_textureCombo;

	SceneObject* m_target;
	SceneGraph* m_graph;
	History* m_history;
	ResourceHandler* m_resources;

	void applyChanges(SceneObject* target);
	void reloadBox(CComboBox& box, const std::vector<std::string>& names);

public:
	void setData(SceneObject* target, SceneGraph* graph, History* history, ResourceHandler* resources);
	afx_msg void Button_Delete();
	afx_msg void Button_Duplicate();
	afx_msg void Button_OK();
};
