// picpic.cpp: 구현 파일
//

#include "pch.h"
#include "RFID.h"
#include "afxdialogex.h"
#include "picpic.h"


// picpic 대화 상자

IMPLEMENT_DYNAMIC(picpic, CDialogEx)

picpic::picpic(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RFID_DIALOG, pParent)
{

}

picpic::~picpic()
{
}

void picpic::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(picpic, CDialogEx)
END_MESSAGE_MAP()


// picpic 메시지 처리기
