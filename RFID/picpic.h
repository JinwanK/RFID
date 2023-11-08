#pragma once
#include "afxdialogex.h"


// picpic 대화 상자

class picpic : public CDialogEx
{
	DECLARE_DYNAMIC(picpic)

public:
	picpic(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~picpic();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RFID_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
