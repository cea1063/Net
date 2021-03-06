
// CaptureDlg.h: 헤더 파일
//

#pragma once

#include "ScreenImage.h"
#include "picturectrl.h"
#include "dataType.h"
#include "TextFile.h"
#include "auth.h"

// CCaptureDlg 대화 상자
class CCaptureDlg : public CDialogEx
{
// 생성입니다.
public:
	CCaptureDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAPTURE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CScreenImage m_image;

	//Directory & File name variable
	TCHAR currentDir[MAX_PATH];
	CString m_capture_filename;
public:
	afx_msg void OnBnClickedButtonCapture();
	CPictureCtrl m_picCtrl;
	afx_msg void OnBnClickedButtonClick();
	afx_msg void OnBnClickedButtonExecute();

	//Command List con
	CListCtrl m_cmd_list;
	void InitListCtrl();
	void Script_to_List_ctrl();


	afx_msg void OnBnClickedButtonAdd();
	BOOL CheckCmdCondition();

	//Block ESC Key
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//Capture의 Data
	CString m_capture_dec;
	int m_clk_x;
	int m_clk_y;
	int m_clk_delay;

	//Script의 Data
	int m_script_x;
	int m_script_y;
	int m_script_delay;
	CPictureCtrl m_script_img;	//Picture for ListCtrl

	Script_List* m_current_script;	//Load 되기 전에 기본 100개 정도의 List를 만든다.
	int m_script_cmd_cnt;
	int m_script_cmd_index;
	Script_List m_current_cmd;
	void print_script();


	//Variable for save
	CString m_script_dir;
	CString m_script_dir_path;
	CString m_script_file_name;

	//Variable for load
	CString m_load_script_file_name;

	//check command and click
	BOOL check_command(int index);
	void check_script();

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//Warning Pop up
	void WarningPopup();
	CString m_warning_msg;

	//Initialize data
	void InitAllData();
		
	afx_msg void OnBnClickedButtonStop();

	//List Ctrl Move & Del
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedButtonCmdDel();
	void MoveListControl(int bUp);
	
	afx_msg void OnNMClickListCommands(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonScrLoad();

	TextFile m_text_ctrl;
	TextFile m_load_ctrl;
	afx_msg void OnBnClickedButtonSrcSave();
	afx_msg void OnBnClickedButtonScrDel();
	
	//Combo box control
	CComboBox m_script_list;
	void initComboBox();
	CButton m_check_priority;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonScrNew();

	BOOL DeleteDirectoryFile(LPCTSTR RootDir);
	int m_main_timer;
	
	afx_msg void OnBnClickedButtonAuth();

	auth m_auth_ctrl;
	BOOL auth_check();
	void button_disable();
	void button_enable();
	CString m_output_str;
	CString m_auth_pw;

};
