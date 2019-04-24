#include "MFCMain.h"
#include "resource.h"


BEGIN_MESSAGE_MAP(MFCMain, CWinApp)
	ON_COMMAND(ID_FILE_QUIT, &MFCMain::MenuFileQuit)
	ON_COMMAND(ID_FILE_SAVETERRAIN, &MFCMain::MenuFileSaveTerrain)
	ON_COMMAND(ID_EDIT_SELECT, &MFCMain::MenuEditSelect)
	ON_COMMAND(ID_BUTTON40001, &MFCMain::Button_SaveScene)
	ON_COMMAND(ID_BUTTON_ADD_OBJECT, &MFCMain::Button_NewSceneObject)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TOOL, &CMyFrame::OnUpdatePage)
END_MESSAGE_MAP()

const int DEFAULT_WINDOW_WIDTH = 1600;
const int DEFAULT_WINDOW_HEIGHT = 900;

BOOL MFCMain::InitInstance()
{
	//instanciate the mfc frame
	m_frame = new CMyFrame();
	m_pMainWnd = m_frame;

	// Initialises the window that is displayed 
	m_frame->Create(
		NULL,
		_T("World Of Flim-Flam Craft Editor"),
		WS_OVERLAPPEDWINDOW,
		CRect(100, 100, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT),
		NULL,
		NULL,
		0,
		NULL
	);

	//show and set the window to run and update. 
	m_frame->ShowWindow(SW_SHOW);
	m_frame->UpdateWindow();

	//get the rect from the MFC window so we can get its dimensions
	m_toolHandle = m_frame->m_DirXView.GetSafeHwnd();				//handle of directX child window
	m_frame->m_DirXView.GetClientRect(&WindowRECT);
	m_width = WindowRECT.Width();
	m_height = WindowRECT.Height();

	m_ToolSystem.onActionInitialise(m_toolHandle, m_width, m_height);

	return TRUE;
}

int MFCMain::Run()
{
	MSG msg;
	BOOL bGotMsg;

	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		if (true)
		{
			bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);
		}
		else
		{
			bGotMsg = (GetMessage(&msg, NULL, 0U, 0U) != 0);
		}

		if (bGotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			m_ToolSystem.UpdateInput(&msg);
		}
		else
		{
			m_ToolSystem.Tick(&msg);

			InputCommands& input = m_ToolSystem.getInputCommands();
			if (input.undo) {
				// attempt to undo the history, change the label if we succeed
				if (m_history.undo()) {
					std::wstring label = L"Undo: " + m_history.get_current_cmd_label();
					m_frame->m_wndStatusBar.SetPaneText(1, label.c_str(), 1);
				}
			}
			if (input.redo) {
				// attempt to redo the history, change the label if we succeed
				if (m_history.redo()) {
					std::wstring label = L"Redo: " + m_history.get_current_cmd_label();
					m_frame->m_wndStatusBar.SetPaneText(1, label.c_str(), 1);
				}
			}
			if (input.save) {
				// Emulate the button save using the keybind
				Button_SaveScene();
			}
		}
	}

	return (int)msg.wParam;
}

void MFCMain::MenuFileQuit()
{
	//will post message to the message thread that will exit the application normally
	PostQuitMessage(0);
}

void MFCMain::MenuFileSaveTerrain()
{
	m_ToolSystem.onActionSaveTerrain();
}

void MFCMain::MenuEditSelect()
{
	m_ToolSelectDialogue.Create(IDD_DIALOG1);
	m_ToolSelectDialogue.ShowWindow(SW_SHOW);
	m_ToolSelectDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.m_selectedObject);
}

void MFCMain::Button_SaveScene()
{
	m_ToolSystem.onActionSave();
}

void MFCMain::Button_NewSceneObject()
{
	// Log the command for creating a new blank scene object
	m_history.log(m_ToolSystem.createAddNewSceneObjectCommand());
	// Change the bottom info to show the new history command label
	m_frame->m_wndStatusBar.SetPaneText(1, m_history.get_current_cmd_label().c_str(), 1);
}


MFCMain::MFCMain()
{
}


MFCMain::~MFCMain()
{
}
