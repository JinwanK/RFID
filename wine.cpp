
// RFIDDlg.cpp: 구현 파일
//
#include "is_d2xx.h"
#include "pch.h"
#include "framework.h"
#include "RFID.h"
#include "RFIDDlg.h"
#include "afxdialogex.h"
#include <mysql.h>

#include <MMSystem.h>
#pragma comment(lib, "winmm") 

#include <iostream>
using namespace std;


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


// CRFIDDlg 대화 상자



CRFIDDlg::CRFIDDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RFID_DIALOG, pParent)
	, m_strRfid(_T(""))
	, m_Name10(_T(""))
	, m_Price10(_T(""))
	, m_Text10(_T(""))
	, m_Place10(_T(""))
	, m_Stock10(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CRFIDDlg::~CRFIDDlg()
{
	OnDisconnect();
}

void CRFIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strRfid);
	DDX_Control(pDX, IDC_PIC, m_picture_control);
	//  DDX_Control(pDX, IDC_EDIT_Price, m_Price);
	DDX_Control(pDX, IDC_EDIT_Text, m_Text);
	//  DDX_Control(pDX, IDC_EDIT_Name, m_Name);
	//  DDX_Control(pDX, IDC_EDIT_Place, m_Place);
	DDX_Text(pDX, IDC_EDIT2, m_Name10);
	DDX_Text(pDX, IDC_EDIT3, m_Price10);
	DDX_Text(pDX, IDC_EDIT4, m_Text10);
	DDX_Text(pDX, IDC_EDIT5, m_Place10);
	DDX_Text(pDX, IDC_EDIT6, m_Stock10);
}

BEGIN_MESSAGE_MAP(CRFIDDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CRFIDDlg::OnConnect)
	ON_BN_CLICKED(IDC_BUTTON3, &CRFIDDlg::OnReadOnce)
	ON_BN_CLICKED(IDC_BUTTON4, &CRFIDDlg::OnReadContinue)
	ON_BN_CLICKED(IDC_BUTTON2, &CRFIDDlg::OnDisconnect)
	//ON_EN_CHANGE(IDC_EDIT2, &CRFIDDlg::OnEnChangeEdit2)
	//ON_EN_CHANGE(IDC_EDIT_Price, &CRFIDDlg::OnEnChangeEditPrice)
END_MESSAGE_MAP()


// CRFIDDlg 메시지 처리기

BOOL CRFIDDlg::OnInitDialog()
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

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CRFIDDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CRFIDDlg::OnPaint()
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
HCURSOR CRFIDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRFIDDlg::OnConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		//열린 포트번호 찾기
	if (is_GetDeviceNumber(&usbnumber) == IS_OK)
	{
		printf("FTDI USB To Serial 연결된 개수 : %d\n", usbnumber);
		if (is_GetSerialNumber(0, readSerialNumber) == IS_OK)
		{
			if (memcmp(changeSerialNumber, readSerialNumber, sizeof(changeSerialNumber) != 0))
			{
				if (is_SetSerialNumber(0, changeSerialNumber) == IS_OK)
				{
					printf(" USB To Serial Number 를 변경 하였습니다.\n");
					printf(" FTDI SerialNumber :  %s \n", changeSerialNumber);
				}
			}
			else
				printf(" FTDI SerialNumber :  %s \n", changeSerialNumber);
		}
	}

	//열린 포트번호로 연결
	unsigned long portNumber;
	if (is_GetCOMPort_NoConnect(0, &portNumber) == IS_OK)
	{
		printf("COM Port : %d\n", portNumber);
	}
	if (is_OpenSerialNumber(&ftHandle, readSerialNumber, 115200) != IS_OK)
	{
		printf("USB To Serial과 통신 연결 실패\n");
		//return -1;
	}
	else {
		printf("Serial포트 %d와 통신 연결성공!! \n", portNumber);
	}
	Sleep(100);
	flag_r = 0;

}


void CRFIDDlg::OnReadOnce()
{
	// 여기 DB연결 시작
	CString pic = _T("");
	CString name = _T("");
	CString price = _T("");
	CString text = _T("");
	CString place = _T("");    // 버튼을 눌렀을 때 초기화.
	CString stock = _T("");
	int buff1 = 0;
	CString buff2 = _T("");


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MYSQL* conn;
	MYSQL* conn_result;
	unsigned int timeout_sec = 1;

	conn = mysql_init(NULL);
	//	mysql_query(conn, "SET Names euckr");

	mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, &timeout_sec);
	conn_result = mysql_real_connect(conn, "127.0.0.1", "root", "123456789", "mysql", 3306, NULL, 0);   // DB 접속



	if (NULL == conn_result)  //  conn_result가 없으면
	{
		cout << "DB Connection Fail" << endl;   // 연결 실패
	}
	else
	{
		cout << "DB Connection Success" << endl;  // 연결 성공
		char query[1024];
		MYSQL_RES* result;
		MYSQL_ROW row;

		// RFID 시작
		CString temp, temp1 = _T(""); // CString 객체에는 + 연산자가 재정의 되어 있다. 그래서 그곳에 넣기만 해도 옆에 붙는다.
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		//ISO15693모드로 읽기( 싱글모드 읽기 )
		if (flag_r == 0)
		{// 아래 CM2_ISO15693_ACTIVE+ BUZZER_ON
			if (is_WriteReadCommand(ftHandle, CM1_ISO15693, CM2_ISO15693_ACTIVE,
				writeLength, wirteData, &readLength, readData) == IS_OK)
			{
				int i;
				printf("ISO 15693 UID : ");
				for (i = 0; i < readLength; i++)
				{
					temp.Format(_T("%02x "), readData[i]);        // readData[i]의 정수값을 헥사값으로 변환해서(Format) temp에 밀어넣는다. temp는 CString의 객체라서 + 연산자가 재정의 되어 있다.
					temp1 += temp;
					printf("%02x ", readData[i]);  // cmd 창에 출력
				}
				m_strRfid = temp1;  // m_strRfid도 CString 
				UpdateData(FALSE);



				if (temp1 == "82 99 68 cb 50 01 04 e0 ") // 뒤에 한 칸 띄어준다. "82 99 68 cb 50 01 04 e0"    
				{
					sndPlaySound(_T("dingdong.wav"), SND_ASYNC);  // 소리

					sprintf_s(query, 1024, "UPDATE test SET Stock = Stock - 1 WHERE Number=1");   // test 테이블에 있는 Number가 1인 행의 Stock을 - 1 해서 Stock에 SET 저장해라.
					mysql_query(conn, query); // DB로 보내준다.

					///////1번 경우 시작
					sprintf_s(query, 1024, "SELECT * FROM test WHERE Number=1"); // WHERE Number=1만 불러온다. 
					// Send Query
					if (mysql_query(conn, query))
					{
						cout << "SELECT Fail" << endl;
					}
					// Get Response
					result = mysql_store_result(conn);

					int fields = mysql_num_fields(result);		// 필드 갯수 구함

					row = mysql_fetch_row(result); // row에 1의 정보 저장됨.
					pic = row[1];                 // row배열 정보를 변수에 저장한다.
					name = row[2];
					price = row[3];
					text = row[4];
					place = row[5];	
					stock = row[6];

					m_Text.SetWindowTextW(TEXT("마트에서 쉽게 찾아볼 수 있는 스파클링\r\n중에서도 만족스러운 퀄리티를 갖춘 와인.\r\n미국 콜롬비아 밸리에서 수확한 샤르도네,\r\n피노 누아, 피노 그리로 생산했는데,\r\n이곳은 프랑스의 샴페인 생산지와 비슷한\r\n위도에 자리한다."));
					CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
					m_picture_control.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
					CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
					dc = m_picture_control.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
					CImage image;//불러오고 싶은 이미지를 로드할 CImage 
					image.Load(pic);//이미지 로드

					image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정 
					ReleaseDC(dc);//DC 해제
					m_Name10 = name;				// row배열 정보가 든 변수를 edit control의 값 변수로 저장하면 출력된다.
					m_Price10 = price;
					m_Text10 = text;
					m_Place10 = place;
					m_Stock10 = stock;

					if (stock == "0")
					{
						AfxMessageBox(_T("추가 주문이 필요합니다."));
					}

					UpdateData(FALSE);

					mysql_free_result(result);
					mysql_close(conn);

					////////1번 경우 끝
				}

				if (temp1 == "f3 37 68 cb 50 01 04 e0 ")  // temp1[0] == 'f'
				{
					sndPlaySound(_T("dingdong.wav"), SND_ASYNC);  // 소리

					///////2번 경우 시작
					sprintf_s(query, 1024, "UPDATE test SET Stock = Stock - 1 WHERE NUMBER=2");
					mysql_query(conn, query);
					sprintf_s(query, 1024, "SELECT * FROM test WHERE Number=2"); // WHERE Number=2만 불러온다. 
					// Send Query
					if (mysql_query(conn, query))
					{
						cout << "SELECT Fail" << endl;
					}
					// Get Response
					result = mysql_store_result(conn);

					int fields = mysql_num_fields(result);		// 필드 갯수 구함

					row = mysql_fetch_row(result); // row에 1의 정보 저장됨.
					pic = row[1];                 // row배열 정보를 변수에 저장한다.
					name = row[2];
					price = row[3];
					text = row[4];
					place = row[5];					
					stock = row[6];
					m_Text.SetWindowTextW(TEXT("이미 애호가들 사이에서는 가성비가 뛰어난\r\n것으로 잘 알려진 샴페인.\r\n앙드레 끌루에는 샹파뉴 지방에서도\r\n피노 누아 품종을 주로 생산하는\r\n부지(Bouzy) 마을에 자리한다."));

					CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
					m_picture_control.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
					CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
					dc = m_picture_control.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
					CImage image;//불러오고 싶은 이미지를 로드할 CImage 
					image.Load(pic);//이미지 로드

					image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정 
					ReleaseDC(dc);//DC 해제
					m_Name10 = name;				// row배열 정보가 든 변수를 edit control의 값 변수로 저장하면 출력된다.
					m_Price10 = price;
					m_Text10 = text;
					m_Place10 = place;
					m_Stock10 = stock;
					if (stock == "0")
					{
						AfxMessageBox(_T("추가 주문이 필요합니다."));
					}
					UpdateData(FALSE);

					mysql_free_result(result);
					mysql_close(conn);
					////////2번 경우 끝
				}
		// RFID 끝
	

	} 
	// DB연결 끝
			printf("\n");
		}
		// 014
		CString temp2, temp3 = _T("");
		//ISO14443A모드로 읽기
		if (is_WriteReadCommand(ftHandle, CM1_ISO14443AB, CM2_ISO14443A_ACTIVE,
			writeLength, wirteData, &readLength, readData) == IS_OK)
		{//위에 CM2_ISO14443A_ACTIVE+ BUZZER_ON
			int i;
			printf("ISO 14443AB UID : ");
			for (i = 0; i < readLength; i++)
			{
				temp2.Format(_T("%02x "), readData[i]);
				temp3 += temp2;
				printf("%02x ", readData[i]);  // cmd 창에 출력
			}
			m_strRfid = temp3;
			UpdateData(FALSE);



			if (temp3 == "21 a8 b2 5b ")  // temp3[0] == '2'
			{
				sndPlaySound(_T("dingdong.wav"), SND_ASYNC);  // 소리
				///////3번 경우 시작
				sprintf_s(query, 1024, "UPDATE test SET Stock = Stock - 1 WHERE NUMBER=3");
				mysql_query(conn, query);
				sprintf_s(query, 1024, "SELECT * FROM test WHERE Number=3"); // WHERE Number=3만 불러온다. 
				// Send Query
				if (mysql_query(conn, query))
				{
					cout << "SELECT Fail" << endl;
				}
				// Get Response
				result = mysql_store_result(conn);

				int fields = mysql_num_fields(result);		// 필드 갯수 구함

				row = mysql_fetch_row(result); // row에 1의 정보 저장됨.
				pic = row[1];                 // row배열 정보를 변수에 저장한다.
				name = row[2];
				price = row[3];
				text = row[4];
				place = row[5];				
				stock = row[6];
				m_Text.SetWindowTextW(TEXT("식사 자리에서 다양한 음식과 잘 어울리는\r\n화이트 와인을 찾는다면 이 와인이 적당하다.\r\n까살 디 쎄라는 이탈리아의 우마니론끼\r\n와이너리가아브루쪼 지역에서 베르디끼오\r\n품종으로 생산하는 와인이다."));

				CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
				m_picture_control.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
				CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
				dc = m_picture_control.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
				CImage image;//불러오고 싶은 이미지를 로드할 CImage 
				image.Load(pic);//이미지 로드

				image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정 
				ReleaseDC(dc);//DC 해제
				m_Name10 = name;				// row배열 정보가 든 변수를 edit control의 값 변수로 저장하면 출력된다.
				m_Price10 = price;
				m_Text10 = text;
				m_Place10 = place;
				m_Stock10 = stock;
				if (stock == "0")
				{
					AfxMessageBox(_T("추가 주문이 필요합니다."));
				}
				UpdateData(FALSE);

				mysql_free_result(result);
				mysql_close(conn);

				////////3번 경우 끝

			}

			if (temp3 == "31 b9 d0 5b ") // temp3[0] == '3'
			{
				sndPlaySound(_T("dingdong.wav"), SND_ASYNC);  // 소리
				///////4번 경우 시작
				sprintf_s(query, 1024, "UPDATE test SET Stock = Stock - 1 WHERE NUMBER=4");
				mysql_query(conn, query);
				sprintf_s(query, 1024, "SELECT * FROM test WHERE Number=4"); // WHERE Number=4만 불러온다. 
				// Send Query
				if (mysql_query(conn, query))
				{
					cout << "SELECT Fail" << endl;
				}
				// Get Response
				result = mysql_store_result(conn);

				int fields = mysql_num_fields(result);		// 필드 갯수 구함

				row = mysql_fetch_row(result); // row에 1의 정보 저장됨.
				pic = row[1];                 // row배열 정보를 변수에 저장한다.
				name = row[2];
				price = row[3];
				text = row[4];
				place = row[5];
				stock = row[6];
				m_Text.SetWindowTextW(TEXT("모스카토 다스티는 마트의 와인 코너에서\r\n가장 큰 인기를 누리는 와인들.\r\n미켈리 키아를로 와이너리는 모스카토\r\n다스티가 가장 위대한 와인이란 믿음으로\r\n최상급 모스카토 다스티를 생산하고 있다."));

				CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
				m_picture_control.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
				CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
				dc = m_picture_control.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
				CImage image;//불러오고 싶은 이미지를 로드할 CImage 
				image.Load(pic);//이미지 로드

				image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정 
				ReleaseDC(dc);//DC 해제
				m_Name10 = name;				// row배열 정보가 든 변수를 edit control의 값 변수로 저장하면 출력된다.
				m_Price10 = price;
				m_Text10 = text;
				m_Place10 = place;
				m_Stock10 = stock;
				if (stock == "0")
				{
					AfxMessageBox(_T("추가 주문이 필요합니다."));
				}
				UpdateData(FALSE);

				mysql_free_result(result);
				mysql_close(conn);
				////////4번 경우 끝

			}
			printf("\n");
		}


	}

}


void CRFIDDlg::OnReadContinue()
{
		// 계속 읽기

}


void CRFIDDlg::OnDisconnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		// 무선파워를 끊어요.
	is_RfOff(ftHandle);
	//USB 포트를 Close
	if (is_Close(ftHandle) == IS_OK)
	{
		printf("연결을 닫습니다. ");
	}

}
