
// CaptureDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Capture.h"
#include "CaptureDlg.h"
#include "afxdialogex.h"

#include "ScreenSelectionDialog.h"
#include "picturectrl.h"
#include "dataType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCaptureDlg 대화 상자



CCaptureDlg::CCaptureDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CAPTURE_DIALOG, pParent)
	, m_capture_dec(_T(""))
	, m_clk_x(0)
	, m_clk_y(0)
	, m_clk_delay(0)
	, m_script_x()
	, m_script_y()
	, m_script_delay()
	, m_script_dir(_T(""))
	, m_main_timer(100)
	, m_output_str(_T(""))
	, m_auth_pw(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_picCtrl);
	DDX_Control(pDX, IDC_STATIC_CMD_IMG, m_script_img);
	DDX_Control(pDX, IDC_LIST_COMMANDS, m_cmd_list);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_capture_dec);
	DDX_Text(pDX, IDC_EDIT_CLK_X, m_clk_x);
	DDX_Text(pDX, IDC_EDIT_CLK_Y, m_clk_y);
	DDX_Text(pDX, IDC_EDIT_CLK_DELAY, m_clk_delay);
	DDX_Text(pDX, IDC_EDIT_CMD_X, m_script_x);
	DDX_Text(pDX, IDC_EDIT_CMD_Y, m_script_y);
	DDX_Text(pDX, IDC_EDIT_CMD_DELAY, m_script_delay);
	DDX_Text(pDX, IDC_EDIT_SCRIPT_NAME, m_script_dir);
	DDX_Control(pDX, IDC_COMBO_SCRIPTS, m_script_list);
	DDX_Control(pDX, IDC_CHECK, m_check_priority);
	DDX_Text(pDX, IDC_EDIT_MAIN_TIMER, m_main_timer);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_output_str);
	DDX_Text(pDX, IDC_EDIT_AUTH_CODE, m_auth_pw);
}

BEGIN_MESSAGE_MAP(CCaptureDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE, &CCaptureDlg::OnBnClickedButtonCapture)
	ON_BN_CLICKED(IDC_BUTTON_CLICK, &CCaptureDlg::OnBnClickedButtonClick)
	ON_BN_CLICKED(IDC_BUTTON_EXECUTE, &CCaptureDlg::OnBnClickedButtonExecute)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CCaptureDlg::OnBnClickedButtonAdd)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CCaptureDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CCaptureDlg::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &CCaptureDlg::OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_CMD_DEL, &CCaptureDlg::OnBnClickedButtonCmdDel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_COMMANDS, &CCaptureDlg::OnNMClickListCommands)
	ON_BN_CLICKED(IDC_BUTTON_SCR_LOAD, &CCaptureDlg::OnBnClickedButtonScrLoad)
	ON_BN_CLICKED(IDC_BUTTON_SRC_SAVE, &CCaptureDlg::OnBnClickedButtonSrcSave)
	ON_BN_CLICKED(IDC_BUTTON_SCR_DEL, &CCaptureDlg::OnBnClickedButtonScrDel)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SCR_NEW, &CCaptureDlg::OnBnClickedButtonScrNew)
	ON_BN_CLICKED(IDC_BUTTON_AUTH, &CCaptureDlg::OnBnClickedButtonAuth)
END_MESSAGE_MAP()


// CCaptureDlg 메시지 처리기

BOOL CCaptureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	//Read Current Directory
	GetCurrentDirectory(MAX_PATH, currentDir);
	
	//Command List control 초기화
	if (auth_check())
	{
		if (m_auth_ctrl.m_remain_count < 6)
		{
			m_warning_msg.Format("인증을 업데이트 해주세요.\n");
			WarningPopup();
		}
	}
	else
	{
		button_disable();
		m_warning_msg.Format("인증이 되지 않았습니다.\n");
		WarningPopup();
	}
	InitListCtrl();
	InitAllData();
	initComboBox();
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CCaptureDlg::initComboBox()
{
	int i;
	m_text_ctrl.FindScriptDir(currentDir);
	for (i = 0; i < m_text_ctrl.m_dir_cnt; i++)
		m_script_list.AddString(m_text_ctrl.m_dir_list[i]);
	
}

void CCaptureDlg::InitAllData()
{
	m_current_cmd.capture_info.left = 0;
	m_current_cmd.capture_info.top = 0;
	m_current_cmd.click_info.left = 0;
	m_current_cmd.click_info.top = 0;
}

void CCaptureDlg::InitListCtrl()
{
	CRect rt;
	m_cmd_list.GetWindowRect(&rt);
	m_cmd_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_cmd_list.InsertColumn(0, TEXT("순서"), LVCFMT_CENTER, rt.Width()*0.2);
	m_cmd_list.InsertColumn(1,TEXT("동작"), LVCFMT_CENTER, rt.Width()*0.5);
	m_cmd_list.InsertColumn(2, TEXT("우선실행"), LVCFMT_CENTER, rt.Width()*0.3);
}

BOOL CCaptureDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			return TRUE;
		default:
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CCaptureDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CCaptureDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CCaptureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCaptureDlg::OnBnClickedButtonCapture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();
	m_capture_filename.Format("%s\\current.bmp", currentDir);
	if (m_image.CaptureRectSave(rect, m_capture_filename))
	{
		TRACE("Capture OK\n");
		m_current_cmd.capture_info = rect;

		TRACE("Left : %d Top : %d\n", m_current_cmd.capture_info.left, m_current_cmd.capture_info.top);
		TRACE("Right : %d Bottom : %d\n", m_current_cmd.capture_info.right, m_current_cmd.capture_info.bottom);
		m_picCtrl.Load(m_capture_filename);
	}
	else
		TRACE("Capture Failed\n");
}

// LeftClick function
void LeftClick()
{
	INPUT    Input = { 0 };
	// left down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));

	// left up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}

// MouseMove function
void MouseMove(int x, int y)
{
	double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN) - 1;
	double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN) - 1;
	double fx = x * (65535.0f / fScreenWidth);
	double fy = y * (65535.0f / fScreenHeight);
	INPUT  Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
	Input.mi.dx = fx;
	Input.mi.dy = fy;
	::SendInput(1, &Input, sizeof(INPUT));
}

void CCaptureDlg::OnBnClickedButtonClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

//아래 부분은 click된 좌표를 print 하는 code
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	m_current_cmd.click_info = rect;
	TRACE("Left : %d Top : %d\n", m_current_cmd.click_info.left, m_current_cmd.click_info.top);
	TRACE("Right : %d Bottom : %d\n", m_current_cmd.click_info.right, m_current_cmd.click_info.bottom);
	UpdateData(TRUE);
	m_clk_x = m_current_cmd.click_info.left;
	m_clk_y = m_current_cmd.click_info.top;
	UpdateData(FALSE);
}

void CCaptureDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case MAIN_TIMER:
			KillTimer(MAIN_TIMER);
			check_script();
			SetTimer(MAIN_TIMER, m_main_timer, NULL);
			break;
		default:
			TRACE("Not matched timer\n");
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CCaptureDlg::check_script()
{
	int i;
	
	for (i = 0; i < m_script_cmd_cnt; i++)
	{
		if (m_current_script[i].priority && check_command(i))
		{
			TRACE("Priority cmd %s command matched\n", m_current_script[i].comment);
			return;
		}
	}

	while (m_current_script[m_script_cmd_index].priority)
		m_script_cmd_index++;
	if (check_command(m_script_cmd_index))
		TRACE("%s command matched\n", m_current_script[m_script_cmd_index].comment);
	else
		TRACE("Not Matched Any command!!!\n");
	m_script_cmd_index++;
	if (m_script_cmd_index == m_script_cmd_cnt)
		m_script_cmd_index = 0;
		
}

BOOL CCaptureDlg::check_command(int index)
{
	BOOL ret;
	Script_List cmd;
	cmd = m_current_script[index];
	ret = m_image.CheckBitmapFile(cmd.filename, cmd.capture_info);
	TRACE("Compare Value is %s\n", ret ? "Same" : "Diff");
	if (ret)
	{
		MouseMove(cmd.click_info.left, cmd.click_info.top);
		LeftClick();
		return TRUE;
	}
	else
		return FALSE;
}


void CCaptureDlg::OnBnClickedButtonExecute()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_script_cmd_cnt <= 0)
	{
		m_warning_msg.Format("스크립트를 선택해주세요.\n");
		WarningPopup();
		return;
	}
	TRACE("Start Timer\n");
	m_script_cmd_index = 0;
	UpdateData(TRUE);
	TRACE("Main Timer : %d\n", m_main_timer);
	if (m_main_timer <= 0)
	{
		m_warning_msg.Format("처리 시간은 100보다 커야 합니다.\n");
		WarningPopup();
		return;
	}
		
	SetTimer(MAIN_TIMER, m_main_timer, NULL);
}


void CCaptureDlg::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KillTimer(MAIN_TIMER);
}

void CCaptureDlg::WarningPopup()
{
	MessageBox(m_warning_msg, NULL, MB_ICONHAND | MB_ICONSTOP | MB_ICONERROR);
}

BOOL CCaptureDlg::CheckCmdCondition()
{
	UpdateData(TRUE);
	m_current_cmd.comment = m_capture_dec;
	m_current_cmd.delay = m_clk_delay;

	if (m_check_priority.GetCheck() == BST_CHECKED)
		m_current_cmd.priority = TRUE;
	else
		m_current_cmd.priority = FALSE;

	if (m_current_cmd.capture_info.top == 0 && m_current_cmd.capture_info.left == 0)
		m_warning_msg.Format("캡쳐가 되지 않았습니다.");
	else if (m_current_cmd.click_info.top == 0 && m_current_cmd.click_info.left == 0)
		m_warning_msg.Format("클릭이 되지 않았습니다.");
	else if (m_current_cmd.comment == "")
		m_warning_msg.Format("동작설명을 넣어주세요.");
	else if (m_script_dir == "")
		m_warning_msg.Format("폴더 이름을 넣어주세요.");
	else
		return TRUE;
	WarningPopup();
	return FALSE;
}

void CCaptureDlg::OnBnClickedButtonAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!CheckCmdCondition())
		return;

	//현재 script가 없으면 생성
	if (m_current_script == NULL)
	{
		m_current_script = new Script_List[MAX_CMD_NUM];
		m_script_cmd_cnt = 0;
	}

	if (m_script_cmd_cnt == MAX_CMD_NUM)
	{
		m_warning_msg.Format("더 이상 추가할 수 없습니다.");
		WarningPopup();
		return;
	}

	m_script_dir_path.Format("%s\\%s", currentDir, m_script_dir);
	DWORD dwAttrib = GetFileAttributes(m_script_dir_path);
	// 존재하지 않거나 Folder가 아니면 Make
	if (dwAttrib == 0xffffffff || (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		::CreateDirectory(m_script_dir_path, NULL);


	m_current_cmd.filename.Format("%s\\%s\\%s.bmp", currentDir, m_script_dir, m_current_cmd.comment);
	CFile::Rename(m_capture_filename, m_current_cmd.filename);
	m_current_script[m_script_cmd_cnt] = m_current_cmd;
	
	//List control에 추가한다.
	CString string;
	
	//순서
	string.Format("%d", m_script_cmd_cnt+1);
	m_cmd_list.InsertItem(m_script_cmd_cnt, string);
	
	//설명
	string.Format("%s", m_current_script[m_script_cmd_cnt].comment);
	m_cmd_list.SetItemText(m_script_cmd_cnt, 1, string);

	m_cmd_list.SetItemText(m_script_cmd_cnt, 2, m_current_cmd.priority ? "O" : "X");
	
	//모든 Data 추가 후에 count++
	m_script_cmd_cnt++;
	
	//현재 Command Data들을 초기화 한다.
	m_current_cmd.capture_info.left = 0;
	m_current_cmd.capture_info.top = 0;
	m_current_cmd.click_info.left = 0;
	m_current_cmd.click_info.top = 0;
	m_current_cmd.priority = FALSE;
	
	m_check_priority.SetCheck(BST_UNCHECKED);
	m_capture_dec = "";
	m_clk_x = 0;
	m_clk_y = 0;
	m_clk_delay = 0;
	UpdateData(FALSE);

}

void CCaptureDlg::print_script()
{
	int i;
	for (i = 0; i < m_script_cmd_cnt; i++)
	{
		TRACE("[%d] Dec : %s X : %d Y : %d\n", i + 1, m_current_script[i].comment, m_current_script[i].click_info.left, m_current_script[i].click_info.top);
	}
}

void CCaptureDlg::MoveListControl(int bUp)
{
	// Single row must be selected
	if (1 != m_cmd_list.GetSelectedCount())
		return;
	int sel = m_cmd_list.GetNextItem(-1, LVNI_SELECTED);
	// Move up or down
	int next = bUp ? sel - 1 : sel + 1;
	if (next < 0 || next >= m_cmd_list.GetItemCount())
	{
		m_cmd_list.SetSelectionMark(sel);
		m_cmd_list.SetItemState(sel, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_cmd_list.SetFocus();
		return;
	}
	CString strSel, strNext;
	for (int i = 1; i < m_cmd_list.GetHeaderCtrl()->GetItemCount(); i++)
	{
		strSel = m_cmd_list.GetItemText(sel, i);
		strNext = m_cmd_list.GetItemText(next, i);
		m_cmd_list.SetItemText(sel, i, strNext);
		m_cmd_list.SetItemText(next, i, strSel);
	}
	//Swap Script list
	Script_List temp = m_current_script[sel];
	m_current_script[sel] = m_current_script[next];
	m_current_script[next] = temp;

	// Move selection
	m_cmd_list.SetSelectionMark(sel);
	m_cmd_list.SetItemState(sel, LVIS_FOCUSED, LVIS_SELECTED);
	m_cmd_list.SetSelectionMark(next);
	m_cmd_list.SetItemState(next, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_cmd_list.SetFocus();
}

void CCaptureDlg::OnBnClickedButtonUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MoveListControl(1);
	print_script();
}


void CCaptureDlg::OnBnClickedButtonDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MoveListControl(0);
	print_script();
}

void CCaptureDlg::Script_to_List_ctrl()
{
	int i;
	m_cmd_list.DeleteAllItems();
	for (i = 0; i < m_script_cmd_cnt; i++)
	{
		//List control에 추가한다.
		CString string;

		//순서
		string.Format("%d", i + 1);
		m_cmd_list.InsertItem(i, string);

		//설명
		string.Format("%s", m_current_script[i].comment);
		m_cmd_list.SetItemText(i, 1, string);

		//우선실행
		m_cmd_list.SetItemText(i, 2, m_current_script[i].priority ? "O" : "X");
	}
}

void CCaptureDlg::OnBnClickedButtonCmdDel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int sel = m_cmd_list.GetNextItem(-1, LVNI_SELECTED);
	int i;
	TRACE("Index to delete is %d\n", sel);
	if (sel < 0)
		return;
	//First update script list
	::DeleteFile(m_current_script[sel].filename);

	m_script_cmd_cnt--;
	for (i = sel; i < m_script_cmd_cnt; i++)
	{
		m_current_script[i] = m_current_script[i + 1];
	}

	print_script();

	Script_to_List_ctrl();

}


void CCaptureDlg::OnNMClickListCommands(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int index = pNMItemActivate->iItem;

	TRACE("Selected Index : %d\n", index);
	if (index < 0)
		return;
	Script_List cmd = m_current_script[index];
	m_script_x = cmd.click_info.left;
	m_script_y = cmd.click_info.top;
	m_script_delay = cmd.delay;
	m_script_img.Load(cmd.filename);
	UpdateData(FALSE);

	*pResult = 0;
}


void CCaptureDlg::OnBnClickedButtonScrLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//first check selected combo box
	int sel = m_script_list.GetCurSel();
	if (sel < 0)
	{
		m_warning_msg.Format("스크립트가 선택되지 않았습니다");
		WarningPopup();
		return;
	}

	CString Dir_name;
	m_script_list.GetLBText(sel, Dir_name);

	m_load_script_file_name.Format("%s\\%s\\script.txt", currentDir, Dir_name);
	TRACE("Selected Script is : %s\n", m_load_script_file_name);

	m_load_ctrl.m_file_name = m_load_script_file_name;
	m_load_ctrl.OpenFileRead();
	CString str;

	//Command Number
	m_load_ctrl.ReadString(str);
	m_script_cmd_cnt = _ttoi(str);
	TRACE("Loaded script command count : %d\n", m_script_cmd_cnt);


	//현재 존재하는 script 를 삭제한다.
	if (m_current_script != NULL)
	{
		delete[] m_current_script;
		m_current_script = NULL;
	}
	m_current_script = new Script_List[MAX_CMD_NUM];

	int i;
	for (i = 0; i < m_script_cmd_cnt; i++)
	{
		Script_List cmd;
		m_load_ctrl.ReadString(str);
		TRACE("Filename : %s\n", str);
		cmd.filename = str;
		m_load_ctrl.ReadString(str);
		TRACE("event infor : %s\n", str);
		sscanf_s(str, "%d %d %d %d %d %d %d %d", &cmd.capture_info.left, &cmd.capture_info.right, &cmd.capture_info.top, &cmd.capture_info.bottom, &cmd.click_info.left,
			&cmd.click_info.top, &cmd.delay, &cmd.priority);
		m_load_ctrl.ReadString(str);
		TRACE("Comment : %s\n", str);
		cmd.comment = str;
		m_current_script[i] = cmd;
	}
	m_script_dir = Dir_name;
	UpdateData(FALSE);
	m_load_ctrl.CloseFile();
	Script_to_List_ctrl();
}


void CCaptureDlg::OnBnClickedButtonSrcSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	m_script_file_name.Format("%s\\script.txt", m_script_dir_path);
	m_text_ctrl.m_file_name = m_script_file_name;
	m_text_ctrl.OpenFile();

	str.Format("%d\n", m_script_cmd_cnt);
	m_text_ctrl.WriteString(str);

	for (int i = 0; i < m_script_cmd_cnt; i++)
	{
		Script_List cmd = m_current_script[i];
		str.Format("%s\n", cmd.filename);
		m_text_ctrl.WriteString(str);
		str.Format("%d %d %d %d %d %d %d %d\n", cmd.capture_info.left, cmd.capture_info.right, cmd.capture_info.top, cmd.capture_info.bottom, cmd.click_info.left,
			cmd.click_info.top, cmd.delay, cmd.priority);
		m_text_ctrl.WriteString(str);
		str.Format("%s\n", cmd.comment);
		m_text_ctrl.WriteString(str);
	}
	m_text_ctrl.CloseFile();
	if (m_script_list.FindString(0, m_script_dir) >= 0)
		TRACE("Script Updated\n");
	else
	{
		TRACE("New Script Added\n");
		m_script_list.AddString(m_script_dir);
		m_script_list.SelectString(0, m_script_dir);
	}
	
}


void CCaptureDlg::OnBnClickedButtonScrDel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//first check selected combo box
	int sel = m_script_list.GetCurSel();
	if (sel < 0)
	{
		m_warning_msg.Format("스크립트가 선택되지 않았습니다");
		WarningPopup();
		return;
	}
	CString RootDir;
	TRACE("sel : %d\n", sel);
	m_script_list.GetLBText(sel, RootDir);
	RootDir.Format("%s\\%s", currentDir, RootDir);
	TRACE("Full Dir : %s\n", RootDir);
	if (DeleteDirectoryFile(RootDir))
	{
		TRACE("Delete Success\n");
		m_script_list.DeleteString(sel);
		m_script_list.Clear();
		OnBnClickedButtonScrNew();
	}
	else
		TRACE("Delete Failed\n");
}

BOOL CCaptureDlg::DeleteDirectoryFile(LPCTSTR RootDir)
{
	if (RootDir == NULL)
	{
		return FALSE;
	}

	BOOL bRval = FALSE;

	CString szNextDirPath = _T("");
	CString szRoot = _T("");


	// 해당 디렉토리의 모든 파일을 검사한다.
	szRoot.Format(_T("%s\\*.*"), RootDir);

	CFileFind find;

	bRval = find.FindFile(szRoot);

	if (bRval == FALSE)
	{
		return bRval;
	}

	while (bRval)
	{
		bRval = find.FindNextFile();

		// . or .. 인 경우 무시 한다.  
		if (find.IsDots() == FALSE)
		{
			// Directory 일 경우 재귀호출 한다.
			if (find.IsDirectory())
			{
				DeleteDirectoryFile(find.GetFilePath());
			}
			// file일 경우 삭제 
			else
			{
				bRval = DeleteFile(find.GetFilePath());
			}
		}
	}
	find.Close();
	bRval = RemoveDirectory(RootDir);
	return bRval;
}

void CCaptureDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	if (m_current_script != NULL)
	{
		delete[] m_current_script;
		m_current_script = NULL;
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CCaptureDlg::OnBnClickedButtonScrNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_current_script != NULL)
	{
		delete[] m_current_script;
		m_current_script = NULL;
	}
	m_cmd_list.DeleteAllItems();
	//현재 Command Data들을 초기화 한다.
	m_current_cmd.capture_info.left = 0;
	m_current_cmd.capture_info.top = 0;
	m_current_cmd.click_info.left = 0;
	m_current_cmd.click_info.top = 0;
	m_current_cmd.priority = FALSE;

	m_check_priority.SetCheck(BST_UNCHECKED);
	m_script_dir = "";
	m_capture_dec = "";
	m_clk_x = 0;
	m_clk_y = 0;
	m_clk_delay = 1000;
	UpdateData(FALSE);
}


void CCaptureDlg::OnBnClickedButtonAuth()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString pw;
	int pw_int;
	UpdateData(TRUE);
	if (m_auth_pw.GetLength() != 8)
	{
		m_warning_msg.Format("password is wrong!!!\n");
		WarningPopup();
		return;
	}

	pw = m_auth_pw.Left(6);
	TRACE("m_Auth_pw : %s real pw : %s\n", m_auth_pw, pw);
	pw_int = _ttoi(pw);
	if (m_auth_ctrl.do_auth(pw_int))
	{
		TRACE("Auth OK\n");
		button_enable();
	}
	else
	{
		TRACE("Auth Fail\n");
		m_warning_msg.Format("password is wrong!!!\n");
		WarningPopup();
	}

}

BOOL CCaptureDlg::auth_check()
{
	m_auth_ctrl.auth_init();
	if (m_auth_ctrl.open_auth_file())
		return TRUE;
	else
		return FALSE;
}

void CCaptureDlg::button_disable()
{
	GetDlgItem(IDC_BUTTON_EXECUTE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SCR_LOAD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SCR_NEW)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SRC_SAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SCR_DEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CAPTURE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLICK)->EnableWindow(FALSE);
}

void CCaptureDlg::button_enable()
{
	GetDlgItem(IDC_BUTTON_EXECUTE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SCR_LOAD)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SCR_NEW)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SRC_SAVE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SCR_DEL)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CAPTURE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLICK)->EnableWindow(TRUE);
}
