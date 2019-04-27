// ObjectEditorDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditorDialogue.h"
#include "afxdialogex.h"
#include "resource.h"

#include "SceneObject.h"
#include "SceneGraph.h"
#include "History.h"
#include "Utils.h"

// ObjectEditorDialogue dialog

IMPLEMENT_DYNAMIC(ObjectEditorDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(ObjectEditorDialogue, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &ObjectEditorDialogue::Button_Delete)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &ObjectEditorDialogue::Button_Duplicate)
END_MESSAGE_MAP()


ObjectEditorDialogue::ObjectEditorDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(ID_OBJECT_EDITOR, pParent)
{
}

ObjectEditorDialogue::~ObjectEditorDialogue()
{
}

void ObjectEditorDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK_VISIBLE, m_checkVisible);
	DDX_Control(pDX, IDC_EDIT_OBJECT_NAME, m_nameControl);
	DDX_Control(pDX, IDC_EDIT_TRANSFORM_X, m_position_x_ctrl);
	DDX_Control(pDX, IDC_EDIT_TRANSFORM_Y, m_position_y_ctrl);
	DDX_Control(pDX, IDC_EDIT_TRANSFORM_Z, m_position_z_ctrl);
	DDX_Control(pDX, IDC_EDIT_TRANSFORM_ROT_X, m_rotation_x_ctrl);
	DDX_Control(pDX, IDC_EDIT_TRANSFORM_ROT_Y, m_rotation_y_ctrl);
	DDX_Control(pDX, IDC_EDIT_TRANSFORM_ROT_Z, m_rotation_z_ctrl);
	DDX_Control(pDX, IDC_EDIT_TRANSFORM_SCALE_X, m_scale_x_ctrl);
	DDX_Control(pDX, IDC_EDIT_TRANSFORM_SCALE_Y, m_scale_y_ctrl);
	DDX_Control(pDX, IDC_EDIT_TRANSFORM_SCALE_Z, m_scale_z_ctrl);
	DDX_Control(pDX, IDC_CHECK_VISIBLE, m_checkVisible);
}

void ObjectEditorDialogue::setData(SceneObject* target, SceneGraph* graph, History* history)
{
	m_target = target; 
	m_graph = graph; 
	m_history = history;

	m_checkVisible.SetCheck(target->render ? 1 : 0);
	m_nameControl.SetWindowTextW(Utils::StringToWCHART(target->name).c_str());
	
	// Set transformation
	m_position_x_ctrl.SetWindowTextW(std::to_wstring(target->posX).c_str());
	m_position_y_ctrl.SetWindowTextW(std::to_wstring(target->posY).c_str());
	m_position_z_ctrl.SetWindowTextW(std::to_wstring(target->posZ).c_str());
	m_rotation_x_ctrl.SetWindowTextW(std::to_wstring(target->rotX).c_str());
	m_rotation_y_ctrl.SetWindowTextW(std::to_wstring(target->rotY).c_str());
	m_rotation_z_ctrl.SetWindowTextW(std::to_wstring(target->rotZ).c_str());
	m_scale_x_ctrl.SetWindowTextW(std::to_wstring(target->scaX).c_str());
	m_scale_y_ctrl.SetWindowTextW(std::to_wstring(target->scaY).c_str());
	m_scale_z_ctrl.SetWindowTextW(std::to_wstring(target->scaZ).c_str());

	// Now mesh & texture details.. 

}


void ObjectEditorDialogue::Button_Delete()
{
	// We want to delete an item from the scene graph
	// then close the popup
	m_graph->removeSceneObject(*m_target);
	CDialogEx::OnOK();
}


void ObjectEditorDialogue::Button_Duplicate()
{
	// We want to make a copy of the object, and insert it
	// into the scene graph.
	SceneObject obj = *m_target;
	m_graph->insertSceneObject(std::move(obj));
}
