#pragma once

#include "ScreenImage.h"
// CScreenSelectionDialog 대화 상자

class CScreenSelectionDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CScreenSelectionDialog)
private:
	CRect & m_rect;
	CRectTracker m_tracker;
	CScreenImage m_image;

public:
	//CScreenSelectionDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CScreenSelectionDialog(CRect& rect);
	virtual ~CScreenSelectionDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCREEN_SELECTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
