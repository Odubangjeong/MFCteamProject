﻿
// MFCteamProjectView.cpp: CMFCteamProjectView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCteamProject.h"
#endif

#include "MainFrm.h"

#include "MFCteamProjectDoc.h"
#include "MFCteamProjectView.h"

#include "opencv2/opencv.hpp"

#include "CTypeDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCteamProjectView

IMPLEMENT_DYNCREATE(CMFCteamProjectView, CFormView)

BEGIN_MESSAGE_MAP(CMFCteamProjectView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CMFCteamProjectView::OnBnClickedButtonOpen)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SHEET, &CMFCteamProjectView::OnDeltaposSpinSheet)
	//ON_BN_CLICKED(IDC_BUTTON2, &CMFCteamProjectView::OnBnClickedButton2)
	//ON_WM_VSCROLL()
	ON_WM_PAINT()
//	ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMFCteamProjectView 생성/소멸

CMFCteamProjectView::CMFCteamProjectView() noexcept
	: CFormView(IDD_MFCTEAMPROJECT_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFCteamProjectView::~CMFCteamProjectView()
{
}

void CMFCteamProjectView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_SHEET, m_spin);
	//DDX_Control(pDX, IDC_PIC, m_pic);
	DDX_Control(pDX, IDC_PIC, m_pic);
}

BOOL CMFCteamProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CMFCteamProjectView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_spin.SetRange(1, 3);
	m_spin.SetPos(1);
	
	db.ReadCSVFILE(_T("월인천강지곡 권상\\typeDB.csv"));

	LoadNewImage(1);
}


// CMFCteamProjectView 진단

#ifdef _DEBUG
void CMFCteamProjectView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMFCteamProjectView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMFCteamProjectDoc* CMFCteamProjectView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCteamProjectDoc)));
	return (CMFCteamProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCteamProjectView 메시지 처리기


void CMFCteamProjectView::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMFCteamProjectView::OnBnClickedButtonOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMFCteamProjectView::OnDeltaposSpinSheet(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	cv::Mat img;
	int iVal = pNMUpDown->iPos + pNMUpDown->iDelta;
	
	LoadNewImage(iVal);
	DrawImage();
} 



void CMFCteamProjectView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
				  // TODO: 여기에 메시지 처리기 코드를 추가합니다.
				  // 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.

	DrawImage();
}


//void CMFCteamProjectView::OnMouseMove(UINT nFlags, CPoint point)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	CString str;
//	str.Format(_T("(%4d,%4d)"), point.x, point.y);
//	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
//	pMainFrame->m_wndStatusBar.SetPaneText(1, str);
//	CFormView::OnMouseMove(nFlags, point);
//}


void CMFCteamProjectView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	GetCursorPos(&point);
	m_pic.ScreenToClient(&point);

	int selected = -1;

	// 
	for (int i = 0; i < img_rects.GetCount(); i++) {
		RectInfo info = img_rects.GetAt(i);
		CRect rect = info.rect;

		// 클릭한 사각형이 몇번째인가?
		if (rect.PtInRect(point)) {
			selected = i;
		}
	}
	
	// "새로운" 사각형이 "선택되었을 때"만
	if (selected != -1 && img_selected_rect != selected) {
		// 기존에 선택된 사각형을 초록색으로
		if (img_selected_rect != -1) { // 새로 선택된 경우만
			RectInfo info = img_rects.GetAt(img_selected_rect);

			CRect rect = info.rect;
			int x1 = rect.TopLeft().x;
			int y1 = rect.TopLeft().y;
			int x2 = rect.BottomRight().x;
			int y2 = rect.BottomRight().y;
			cv::Rect p_rect(cv::Point(x1, y1), cv::Point(x2, y2));
			
			cv::rectangle(img, p_rect, cv::Scalar(0, 255, 0), p_thickness, 4, 0);
		}

		// 새로 선택된 사각형을 빨간색으로
		RectInfo info = img_rects.GetAt(selected);

		CRect rect = info.rect;
		int x1 = rect.TopLeft().x;
		int y1 = rect.TopLeft().y;
		int x2 = rect.BottomRight().x;
		int y2 = rect.BottomRight().y;
		cv::Rect p_rect(cv::Point(x1, y1), cv::Point(x2, y2));

		cv::rectangle(img, p_rect, cv::Scalar(0, 0, 255), p_thickness, 4, 0);

		img_selected_rect = selected;
		DrawImage();
	}

	/*
	CString str;
	str.Format(L"%d    %d", point.x, point.y);
	AfxMessageBox(str);
	*/
	CFormView::OnLButtonDown(nFlags, point);
}


void CMFCteamProjectView::LoadNewImage(int img_index) {
	if (img_index == selected_img) { // 이미지 변경시에만 이미지 로드
		return;
	}
	selected_img = img_index;

	img.release();

	CRect rect;
	m_pic.GetClientRect(&rect);
	int rect_width = rect.Width() / 8 * 8;
	int rect_height = rect.Height() / 8 * 8;

	// 이미지 불러오기, CSV 파일 읽기
	if (img_index == 1)
	{
		img = cv::imread("월인천강지곡 권상\\01_scan\\001.jpg", cv::ImreadModes::IMREAD_UNCHANGED);
	}
	else if (img_index == 2)
	{
		img = cv::imread("월인천강지곡 권상\\01_scan\\002.jpg", cv::ImreadModes::IMREAD_UNCHANGED);
	}
	else if (img_index == 3)
	{
		img = cv::imread("월인천강지곡 권상\\01_scan\\003.jpg", cv::ImreadModes::IMREAD_UNCHANGED);
	}
	int img_width = img.size().width;
	int img_height = img.size().height;

	// 그렸던 사각형 기록 삭제
	img_rects.RemoveAll();
	img_selected_rect = -1;

	// 이미지 크기 조절
	cv::resize(img, img, cv::Size(rect_width, rect_height));
	
	

	int count = -1;

	for (int i = 0; i < db.getLength(); i++) {
		// 글자 페이지 비교하기
		// SCharInfo chInfo = m_Chars.GetAt(i);
		// m_ind = i;
		// m_sx = chInfo.m_sx;
		// ...
		// TODO : CArray를 가져오시오...
		
		SCharInfo chInfo = db.getChars(i);
		if (chInfo.m_sheet != img_index) continue;
		
		int m_ind = i;

		int m_sx = chInfo.m_sx;
		int m_sy = chInfo.m_sy;
		int m_width = chInfo.m_width;
		int m_height = chInfo.m_height; // 1번 글자 위치

		int p_x1 = m_sx * rect_width / img_width - p_thickness;
		int p_y1 = m_sy * rect_height / img_height - p_thickness;
		int p_x2 = (m_sx + m_width) * rect_width / img_width + p_thickness;
		int p_y2 = (m_sy + m_height) * rect_height / img_height + p_thickness; // 좌표계 조정

		cv::Rect p_rect(cv::Point(p_x1, p_y1), cv::Point(p_x2, p_y2));
		RectInfo p_info;
		p_info.char_index = m_ind;
		p_info.rect = CRect(p_x1, p_y1, p_x2, p_y2);


		if (count == -1) {
			cv::rectangle(img, p_rect, cv::Scalar(0, 0, 255), p_thickness, 4, 0);
			img_selected_rect = 0;
			count = 1;
		}
		else {
			cv::rectangle(img, p_rect, cv::Scalar(0, 255, 0), p_thickness, 4, 0);
		}
		

		// 그린 사각형 몇번째인지 저장
		img_rects.Add(p_info);
		/*
		{
			// 이미지 위에 네모 그리기
			int m_ind = 0;
			int m_sx = 8498;
			int m_sy = 1264;
			int m_width = 312;
			int m_height = 352; // 1번 글자 위치

			int p_x1 = m_sx * rect_width / img_width - p_thickness;
			int p_y1 = m_sy * rect_height / img_height - p_thickness;
			int p_x2 = (m_sx + m_width) * rect_width / img_width + p_thickness;
			int p_y2 = (m_sy + m_height) * rect_height / img_height + p_thickness; // 좌표계 조정

			cv::Rect p_rect(cv::Point(p_x1, p_y1), cv::Point(p_x2, p_y2));
			RectInfo p_info;
			p_info.char_index = m_ind;
			p_info.rect = CRect(p_x1, p_y1, p_x2, p_y2);

			cv::rectangle(img, p_rect, cv::Scalar(0, 255, 0), p_thickness, 4, 0);
			img_rects.Add(p_info);
		}

		{
			// 이미지 위에 네모 그리기
			int m_ind = 0;
			int m_sx = 8491;
			int m_sy = 1953;
			int m_width = 313;
			int m_height = 345; // 2번 글자 위치

			int p_x1 = m_sx * rect_width / img_width - p_thickness;
			int p_y1 = m_sy * rect_height / img_height - p_thickness;
			int p_x2 = (m_sx + m_width) * rect_width / img_width + p_thickness;
			int p_y2 = (m_sy + m_height) * rect_height / img_height + p_thickness; // 좌표계 조정

			cv::Rect p_rect(cv::Point(p_x1, p_y1), cv::Point(p_x2, p_y2));
			RectInfo p_info;
			p_info.char_index = m_ind;
			p_info.rect = CRect(p_x1, p_y1, p_x2, p_y2);

			cv::rectangle(img, p_rect, cv::Scalar(0, 255, 0), p_thickness, 4, 0);
			img_rects.Add(p_info);
		}
		*/
	}


}

void CMFCteamProjectView::DrawImage() {
	CRect rect;
	m_pic.GetClientRect(&rect);

	CDC* pDC;
	CImage* mfcImg = nullptr;
	pDC = m_pic.GetDC();

	// 이미지를 사각형으로 출력
	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	bitmapInfo.bmiHeader.biBitCount = 24;
	bitmapInfo.bmiHeader.biWidth = img.cols;
	bitmapInfo.bmiHeader.biHeight = -img.rows;
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biClrImportant = 0;
	bitmapInfo.bmiHeader.biClrUsed = 0;
	bitmapInfo.bmiHeader.biSizeImage = 0;
	bitmapInfo.bmiHeader.biXPelsPerMeter = 0;

	if (img.channels() == 3)
	{
		mfcImg = new CImage();
		mfcImg->Create(img.cols, img.rows, 24);
	}
	else if (img.channels() == 1)
	{
		cv::cvtColor(img, img, cv::COLOR_GRAY2RGB);
		mfcImg = new CImage();
		mfcImg->Create(img.cols, img.rows, 24);
	}
	else if (img.channels() == 4)
	{
		bitmapInfo.bmiHeader.biBitCount = 32;
		mfcImg = new CImage();
		mfcImg->Create(img.cols, img.rows, 32);
	}

	::StretchDIBits(mfcImg->GetDC(), 0, 0, img.cols, img.rows,
		0, 0, img.cols, img.rows, img.data, &bitmapInfo,
		DIB_RGB_COLORS, SRCCOPY);

	mfcImg->BitBlt(::GetDC(m_pic.m_hWnd), 0, 0);

	mfcImg->ReleaseDC();
	delete mfcImg;
	mfcImg = nullptr;
	ReleaseDC(pDC);
}