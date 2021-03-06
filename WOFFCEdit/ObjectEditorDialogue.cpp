// ObjectEditorDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "resource.h"

#include "ObjectEditorDialogue.h"
#include "EditorCommands.h"
#include "ResourceHandler.h"
#include "SceneObject.h"
#include "SceneGraph.h"
#include "History.h"
#include "Utils.h"

// ObjectEditorDialogue dialog

IMPLEMENT_DYNAMIC(ObjectEditorDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(ObjectEditorDialogue, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &ObjectEditorDialogue::Button_Delete)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &ObjectEditorDialogue::Button_Duplicate)
	ON_BN_CLICKED(IDOK, &ObjectEditorDialogue::Button_OK)
END_MESSAGE_MAP()

// Converts type CString back to std::string
std::string CStringBack(const CString & cstr) {
	CT2CA pszConvertedAnsiString(cstr);
	return std::string(pszConvertedAnsiString);
}

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
	DDX_Control(pDX, IDC_MESH_COMBO, m_meshCombo);
	DDX_Control(pDX, IDC_TEXTURE_COMBO, m_textureCombo);
}

float read_float_from_cedit(const CEdit& edit) {
	CString str;
	edit.GetWindowTextW(str);
	return std::stof(std::wstring(str));
}

void ObjectEditorDialogue::applyChanges(SceneObject* target)
{
	CString str;
	m_nameControl.GetWindowTextW(str);
	target->name = CStringBack(str);

	target->render = m_checkVisible.GetCheck();
	target->posX = read_float_from_cedit(m_position_x_ctrl);
	target->posY = read_float_from_cedit(m_position_y_ctrl);
	target->posZ = read_float_from_cedit(m_position_z_ctrl);
	target->scaX = read_float_from_cedit(m_scale_x_ctrl);
	target->scaY = read_float_from_cedit(m_scale_y_ctrl);
	target->scaZ = read_float_from_cedit(m_scale_z_ctrl);
	target->rotX = read_float_from_cedit(m_rotation_x_ctrl);
	target->rotY = read_float_from_cedit(m_rotation_y_ctrl);
	target->rotZ = read_float_from_cedit(m_rotation_y_ctrl);

	// Now we want to lookup the correct texture & mesh
	m_meshCombo.GetLBText(m_meshCombo.GetCurSel(), str);
	std::string name = CStringBack(str);
	target->model_path = m_resources->getModelPathFromName(name);

	m_textureCombo.GetLBText(m_textureCombo.GetCurSel(), str);
	name = CStringBack(str);
	target->tex_diffuse_path = m_resources->getTexturePathFromName(name);
}

void ObjectEditorDialogue::reloadBox(CComboBox & box, const std::vector<std::string>& names)
{
	while (box.GetCount() > 0) box.DeleteString(0);
	if (names.size() > 0) {
		for (const std::string& name : names)
			box.AddString(Utils::StringToWCHART(name).c_str());
	}

	// Resize the dropdown box to be the correct size for all elements to show
	// Note: We may want to clamp this at some point, and enable the 
	// vertical scrollbar instead.
	CRect rctComboBox, rctDropDown;
	box.GetClientRect(&rctComboBox);
	box.GetDroppedControlRect(&rctDropDown);
	int itemHeight = box.GetItemHeight(-1);
	box.GetParent()->ScreenToClient(&rctDropDown);
	rctDropDown.bottom = rctDropDown.top + rctComboBox.Height() + itemHeight * names.size();
	box.MoveWindow(&rctDropDown);
}

void ObjectEditorDialogue::setData(SceneObject* target, SceneGraph* graph, History* history, ResourceHandler* resources)
{
	m_target = target; 
	m_graph = graph; 
	m_history = history;
	m_resources = resources;

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

	// Now add all available meshes & textures to the combo boxes
	// so the user can drop down select the texture and mesh they want
	reloadBox(m_meshCombo, resources->getMeshNames());
	reloadBox(m_textureCombo, resources->getTextureNames());

	// now we need to change cursor selection to the current selection
	// by getting the texture name (in the database) by the filepath in the
	// resource handler
	const std::string tex_name = resources->getTextureNameByFilepath(target->tex_diffuse_path);
	if (tex_name != "") {
		m_textureCombo.SelectString(-1, Utils::StringToWCHART(tex_name).c_str());
	}
	const std::string& mesh_name = resources->getMeshNameByFilepath(target->model_path);
	if (mesh_name != "") {
		m_meshCombo.SelectString(-1, Utils::StringToWCHART(mesh_name).c_str());
	}
}

void ObjectEditorDialogue::Button_Delete()
{
	// We want to delete an item from the scene graph, then close the popup
	m_history->log(new DeleteSceneObjectCommand(m_target));
	CDialogEx::OnOK();
}

void ObjectEditorDialogue::Button_Duplicate()
{
	// We want to make a copy of the object, and insert it
	// into the scene graph.
	SceneObject obj = *m_target;
	applyChanges(&obj);
	m_graph->insertSceneObject(std::move(obj));
}

void ObjectEditorDialogue::Button_OK()
{
	// Apply the changes to the target
	applyChanges(m_target);
	m_graph->setDirty(true);
	CDialogEx::OnOK();
}
