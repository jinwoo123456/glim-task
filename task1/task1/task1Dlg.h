
// task1Dlg.h: 헤더 파일
//

#pragma once


// Ctask1Dlg 대화 상자
class Ctask1Dlg : public CDialogEx
{
// 생성입니다.
public:
	Ctask1Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASK1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	CPoint m_onClickPoints[3];; // 클릭 위치
	bool m_isClicked = false ;// 클릭 여부
	int m_clickCnt = 0; // 클릭 횟수
	int  m_dragIndex;  // 드래그 마커 인덱스 
	bool m_isDragging;  // 드래그 여부 
	int borderWidth; // 가장자리 두께
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);


private:
	// 원 그리기 함수
	void Ctask1Dlg::DrawMarker(CDC& dc, const CPoint& pt)
	{
		CBrush markerBrush(RGB(0, 0, 0));// 마커 색깔
		CBrush* pOldBrush = dc.SelectObject(&markerBrush);
		// 6 * 6 원 그리기
		dc.Ellipse(pt.x - 6, pt.y - 6, pt.x + 6, pt.y + 6);
		dc.SelectObject(pOldBrush); 
	};
public:
	
	

	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnBorderWidth();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	
	afx_msg void OnBnClickedBtnRandomMarkerThread();
	afx_msg LRESULT OnAutoRandomMarkerUpdate(WPARAM wParam, LPARAM lParam);
	friend UINT AutoRandomMarkerThread(LPVOID pParam); // 스레드 접근지정 허용
};
