// ObjectEditorDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditorDialogue.h"
#include "afxdialogex.h"
#include "resource.h"

// ObjectEditorDialogue dialog

IMPLEMENT_DYNAMIC(ObjectEditorDialogue, CDialogEx)

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
}


BEGIN_MESSAGE_MAP(ObjectEditorDialogue, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_OBJECT_NAME, &ObjectEditorDialogue::OnEnChangeEditObjectName)
	ON_EN_CHANGE(IDC_EDIT_TRANSFORM_X, &ObjectEditorDialogue::OnEnChangeEditTransformX)
	ON_EN_CHANGE(IDC_EDIT_TRANSFORM_Y, &ObjectEditorDialogue::OnEnChangeEditTransformY)
	ON_EN_CHANGE(IDC_EDIT_TRANSFORM_Z, &ObjectEditorDialogue::OnEnChangeEditTransformZ)
END_MESSAGE_MAP()


// ObjectEditorDialogue message handlers


void ObjectEditorDialogue::OnEnChangeEditTransformX()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void ObjectEditorDialogue::OnEnChangeEditTransformY()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void ObjectEditorDialogue::OnEnChangeEditObjectName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void ObjectEditorDialogue::OnEnChangeEditTransformZ()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
