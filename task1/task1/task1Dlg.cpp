
// task1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "task1.h"
#include "task1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define WM_AUTO_RANDOM (WM_APP + 1)

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


// Ctask1Dlg 대화 상자



Ctask1Dlg::Ctask1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TASK1_DIALOG, pParent)
    , m_clickCnt(0) // 클릭 횟수 
    , m_isClicked(false) // 클릭 여부
    , borderWidth(1)  // 가장자리 두께
    , m_dragIndex(-1)       // 드래그용 마커 인덱스
    , m_isDragging(false)     // 드래그 여부
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ctask1Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_BORDER_WIDTH, borderWidth);
}

BEGIN_MESSAGE_MAP(Ctask1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &Ctask1Dlg::OnBnClickedOk)
	ON_WM_LBUTTONDOWN()
 
    ON_BN_CLICKED(IDC_BTN_RESET, &Ctask1Dlg::OnBnClickedBtnReset)
    ON_BN_CLICKED(IDC_BTN_BORDER_WIDTH, &Ctask1Dlg::OnBnClickedBtnBorderWidth)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    
    ON_BN_CLICKED(IDC_BTN_RANDOM_MARKER_THREAD, &Ctask1Dlg::OnBnClickedBtnRandomMarkerThread)
    ON_MESSAGE(WM_AUTO_RANDOM, &Ctask1Dlg::OnAutoRandomMarkerUpdate)
END_MESSAGE_MAP()


// Ctask1Dlg 메시지 처리기

BOOL Ctask1Dlg::OnInitDialog()
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

void Ctask1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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


void Ctask1Dlg::OnPaint()
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

        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CPaintDC dc(this);
        CDialogEx::OnPaint();
        // 마커 찍기
        for (int i = 0; i < m_clickCnt; ++i)
        {
            DrawMarker(dc, m_onClickPoints[i]);
        }
        if (m_clickCnt == 3)
        {
            // 세 점을 이용하여 외접원 계산
            double x1 = m_onClickPoints[0].x, y1 = m_onClickPoints[0].y;
            double x2 = m_onClickPoints[1].x, y2 = m_onClickPoints[1].y;
            double x3 = m_onClickPoints[2].x, y3 = m_onClickPoints[2].y;
            double D = 2 * (x1 * (y2 - y3) +
                x2 * (y3 - y1) +
                x3 * (y1 - y2));

            // 마커가 직선이면 원 안 만들어짐.
            if (fabs(D) < 1e-9)
            {
                for (int i = 0; i < 3; ++i)
                {
                    DrawMarker(dc, m_onClickPoints[i]);

                }
            }
            else
            {
                // 외접원의 중심 계산
                double centerX = ((x1 * x1 + y1 * y1) * (y2 - y3) +
                    (x2 * x2 + y2 * y2) * (y3 - y1) +
                    (x3 * x3 + y3 * y3) * (y1 - y2)) / D;
                double centerY = ((x1 * x1 + y1 * y1) * (x3 - x2) +
                    (x2 * x2 + y2 * y2) * (x1 - x3) +
                    (x3 * x3 + y3 * y3) * (x2 - x1)) / D;
                // 반지름은 임의의 점과 중심 사이의 거리로 계산
                double rad = sqrt((centerX - x1) * (centerX - x1) + (centerY - y1) * (centerY - y1));

                int cx = static_cast<int>(centerX + 0.5);
                int cy = static_cast<int>(centerY + 0.5);
                int r = static_cast<int>(rad + 0.5);

                CPen pen(PS_SOLID, borderWidth, RGB(255, 0, 0));
                CPen* pOldPen = dc.SelectObject(&pen);
                CBrush* pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);

                dc.Ellipse(cx - r, cy - r, cx + r, cy + r);

                dc.SelectObject(pOldBrush);
                dc.SelectObject(pOldPen);

                for (int i = 0; i < 3; ++i)
                {
                    DrawMarker(dc, m_onClickPoints[i]);
                }
            }


        }
    }
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR Ctask1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Ctask1Dlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}

void Ctask1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

    if (m_clickCnt == 3)
    {
        const int markerClickRange = 5; // 마커 주변 5픽셀 클릭판단
        for (int i = 0; i < 3; ++i)
        {
            if (abs(point.x - m_onClickPoints[i].x) <= markerClickRange &&
                abs(point.y - m_onClickPoints[i].y) <= markerClickRange)
            {
                m_dragIndex = i;
                m_isDragging = true;
                SetCapture();// 상자 영역 밖도 허용
                break;
            }
        }
    }
    else if (m_clickCnt < 3)
    {
        
        m_onClickPoints[m_clickCnt] = point; // 클릭 위치 좌표 저장
        ++m_clickCnt;
        m_isClicked = true;                  
        Invalidate();
    }
    CDialogEx::OnLButtonDown(nFlags, point);
}





void Ctask1Dlg::OnBnClickedBtnReset()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_clickCnt = 0;
	for (int i = 0; i < 3; i++) {
		m_onClickPoints[i] = CPoint(0, 0);
	}
    Invalidate();
    
}

void Ctask1Dlg::OnBnClickedBtnBorderWidth()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    borderWidth = 1;
    UpdateData(true);
    // 두께는 10보다 크게 못함
    if (borderWidth > 10)
    {
		borderWidth = 10;
		UpdateData(false);
        AfxMessageBox(_T("가장자리 두께는 10을 초과할 수 없습니다."));
        
        UpdateData(false); 
    }
    Invalidate();
   
}

void Ctask1Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    if (m_isDragging && m_dragIndex != -1)
    {
        m_onClickPoints[m_dragIndex] = point;// 드래그 할 때마다 위치 갱신
        Invalidate();// 드래그 실시간 적용  
    }
    
    CDialogEx::OnMouseMove(nFlags, point);
}


void Ctask1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    if (m_isDragging)
    {
        m_isDragging = false;
        m_dragIndex = -1;
        ReleaseCapture();// 상자 영역 밖 해제
    }
    CDialogEx::OnLButtonUp(nFlags, point);
}


void Ctask1Dlg::OnBnClickedBtnRandomMarkerThread()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    AfxBeginThread(AutoRandomMarkerThread, this);
}

UINT AutoRandomMarkerThread(LPVOID pParam)
{
    Ctask1Dlg* pDlg = reinterpret_cast<Ctask1Dlg*>(pParam);
    if (!pDlg) { 
        return 0;
    };

    for (int i = 0; i < 10; i++)
    {
        Sleep(200); // 0.2sec

        // 영역 크기
        CRect rect;
        pDlg->GetClientRect(&rect);

        // 마커 3으로 고정
        pDlg->m_clickCnt = 3;

        // 마커 랜덤 이동
        for (int j = 0; j < 3; ++j)
        {
            int newX = rect.left + rand() % (rect.Width());
            int newY = rect.top + rand() % (rect.Height());
            pDlg->m_onClickPoints[j] = CPoint(newX, newY);
        }

       
        pDlg->Invalidate();
    }
    return 0;
}
LRESULT Ctask1Dlg::OnAutoRandomMarkerUpdate(WPARAM wParam, LPARAM lParam)
{
    
    Invalidate(); 
    return 0;
}