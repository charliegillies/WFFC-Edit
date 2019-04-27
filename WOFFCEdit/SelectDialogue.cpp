// SelectDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "SelectDialogue.h"
#include "ToolMain.h"
#include "Utils.h"
#include "resource.h"

// SelectDialogue dialog
IMPLEMENT_DYNAMIC(SelectDialogue, CDialogEx)

//Message map.  Just like MFCMAIN.cpp.  This is where we catch button presses etc and point them to a handy dandy method.
BEGIN_MESSAGE_MAP(SelectDialogue, CDialogEx)
	ON_COMMAND(IDOK, &SelectDialogue::End)
	ON_BN_CLICKED(IDOK, &SelectDialogue::OnBnClickedOk)		
	ON_LBN_SELCHANGE(IDC_HIERARCHY_LIST, &SelectDialogue::Select)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_HIERARCHY_LIST, &SelectDialogue::Select)
END_MESSAGE_MAP()


SelectDialogue::SelectDialogue(CWnd* pParent, std::vector<SceneObject>* SceneGraph)		//constructor used in modal
	: CDialogEx(IDD_HIERARCHY_DIALOG, pParent)
{
	m_sceneGraph = SceneGraph;
}

SelectDialogue::SelectDialogue(CWnd * pParent)			//constructor used in modeless
	: CDialogEx(IDD_HIERARCHY_DIALOG, pParent)
{

}

SelectDialogue::~SelectDialogue()
{
}

///pass through pointers to the data in the tool we want to manipulate
void SelectDialogue::SetObjectData(ToolMain* tool)
{
	m_tool = tool;
	m_sceneGraph = tool->getGraph()->getObjects();

	// Allows full row selection instead of single-element
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	// Insert columns into the structure
	m_list.InsertColumn(0, L"ID", LVCFMT_LEFT, 100, 0);
	m_list.InsertColumn(1, L"Name", LVCFMT_LEFT, 100, 1);
	m_list.InsertColumn(2, L"Mesh", LVCFMT_LEFT, 200, 2);
	m_list.InsertColumn(3, L"Texture", LVCFMT_LEFT, 200, 3);

	// Show every scene object
	for (int i = 0, count = m_sceneGraph->size(); i < count; i++) {
		SceneObject& obj = m_sceneGraph->at(i);

		// Display the row. Format: ID/Name/Mesh/Texture.
		int row = m_list.InsertItem(obj.ID, std::to_wstring(obj.ID).c_str());
		m_list.SetItem(row, 1, LVIF_TEXT, Utils::StringToWCHART(obj.name).c_str(), 0, 0, 0, 0);
		m_list.SetItem(row, 2, LVIF_TEXT, Utils::StringToWCHART(obj.model_path).c_str(), 0, 0, 0, 0);
		m_list.SetItem(row, 3, LVIF_TEXT, Utils::StringToWCHART(obj.tex_diffuse_path).c_str(), 0, 0, 0, 0);
	}
}

void SelectDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HIERARCHY_LIST, m_list);
}

void SelectDialogue::End()
{
	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again. 
}

void SelectDialogue::Select()
{
	int column = m_list.GetSelectedColumn();
	if (column != -1) {
		CString id = m_list.GetItemText(column, 0);
		m_tool->setSelectionID(_ttoi(id));
	}
}

BOOL SelectDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void SelectDialogue::PostNcDestroy()
{
}

void SelectDialogue::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

