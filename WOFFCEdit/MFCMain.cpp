#include "MFCMain.h"
#include "resource.h"


BEGIN_MESSAGE_MAP(MFCMain, CWinApp)
	ON_COMMAND(ID_FILE_QUIT, &MFCMain::MenuFileQuit)
	ON_COMMAND(ID_FILE_SAVETERRAIN, &MFCMain::MenuFileSaveTerrain)
	ON_COMMAND(ID_EDIT_SELECT, &MFCMain::MenuEditSelect)
	ON_COMMAND(ID_BUTTON40001, &MFCMain::Button_SaveScene)
	ON_COMMAND(ID_BUTTON_ADD_OBJECT, &MFCMain::Button_NewSceneObject)
	ON_COMMAND(ID_BUTTON_WIREFRAME, &MFCMain::Button_ToggleWireframe)

	ON_COMMAND(ID_BUTTON_CAMERA, &MFCMain::Button_CameraToggle)
	ON_COMMAND(ID_BUTTON_ROTATE, &MFCMain::Button_RotateToggle)
	ON_COMMAND(ID_BUTTON_SCALE, &MFCMain::Button_ScaleToggle)
	ON_COMMAND(ID_BUTTON_MOVE, &MFCMain::Button_TranslateToggle)
	ON_COMMAND(ID_BUTTON_EDIT_OBJECT, &MFCMain::Button_EditObject)
	
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TOOL, &CMyFrame::OnUpdatePage)
END_MESSAGE_MAP()

const int DEFAULT_WINDOW_WIDTH = 1400;
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
	// Set the appropriate mode button down
	m_frame->EditorModeChanged(m_mode);

	//show and set the window to run and update. 
	m_frame->ShowWindow(SW_SHOW);
	m_frame->UpdateWindow();

	//get the rect from the MFC window so we can get its dimensions
	m_toolHandle = m_frame->m_DirXView.GetSafeHwnd();				//handle of directX child window
	m_frame->m_DirXView.GetClientRect(&WindowRECT);
	m_width = WindowRECT.Width();
	m_height = WindowRECT.Height();

	m_toolSystem.onActionInitialise(m_toolHandle, m_width, m_height);

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

			m_toolSystem.UpdateInput(&msg);
		}
		else
		{
			int numCommands = m_history.num_commands();
			m_toolSystem.Tick(&msg, &m_history);

			// Check if there has been a change in the command history
			// if there has, update the label.
			if (numCommands != m_history.num_commands()) {
				m_frame->m_wndStatusBar.SetPaneText(1, m_history.get_current_cmd_label().c_str(), 1);
			}

			InputCommands& input = m_toolSystem.getInputCommands();
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
			if (input.wireframe) {
				// Emulate the toggle button using the keybind
				Button_ToggleWireframe();
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
	m_toolSystem.onActionSaveTerrain();
}

void MFCMain::MenuEditSelect()
{
	m_selectDialogue.Create(IDD_DIALOG1);
	m_selectDialogue.ShowWindow(SW_SHOW);
	m_selectDialogue.SetObjectData(m_toolSystem.getGraph()->getObjects(), &m_toolSystem.m_selectedObject);
}

void MFCMain::Button_SaveScene()
{
	m_toolSystem.onActionSave();
}

void MFCMain::Button_NewSceneObject()
{
	// Log the command for creating a new blank scene object
	m_history.log(m_toolSystem.createAddNewSceneObjectCommand());
	// Change the bottom info to show the new history command label
	m_frame->m_wndStatusBar.SetPaneText(1, m_history.get_current_cmd_label().c_str(), 1);
}

void MFCMain::Button_ToggleWireframe()
{
	// Toggle wireframe on/off, and change the interface button mode appropriately
	bool toggled = m_toolSystem.onToggleWireframe();
	m_frame->ChangeWireframeButtonState(toggled);
}

void MFCMain::Button_CameraToggle()
{
	ChangeEditorMode(EditorMode::CAMERA);
}

void MFCMain::Button_TranslateToggle()
{
	ChangeEditorMode(EditorMode::MOVE);
}

void MFCMain::Button_RotateToggle()
{
	ChangeEditorMode(EditorMode::ROTATE);
}

void MFCMain::Button_ScaleToggle()
{
	ChangeEditorMode(EditorMode::SCALE);
}

void MFCMain::Button_EditObject()
{
	// First of all, check there is a valid object selected
	int id = m_toolSystem.getCurrentSelectionID();
	SceneObject* target = m_toolSystem.getGraph()->getObjectById(id);

	if (target != nullptr) {
		m_objectEditDialogue.Create(ID_OBJECT_EDITOR);
		m_objectEditDialogue.ShowWindow(SW_SHOW);
		m_objectEditDialogue.setData(target, m_toolSystem.getGraph(), &m_history);
	}
	else {
		// if not, let the user know..
		MessageBox(NULL, L"No selected object. Please select an object before editing.", L"Object Editor", MB_OK);
	}
}

void MFCMain::ChangeEditorMode(const EditorMode mode)
{
	m_mode = mode;
	if (m_frame != nullptr) {
		// informs the frame so the appropriate button states will change.
		m_frame->EditorModeChanged(mode);
	}
	m_toolSystem.editorModeChanged(mode);
}

MFCMain::MFCMain() : m_history(&m_toolSystem)
{
	ChangeEditorMode(EditorMode::CAMERA);
}


MFCMain::~MFCMain()
{
}
