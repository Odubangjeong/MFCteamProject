
// MFCteamProjectView.cpp: CMFCteamProjectView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCteamProject.h"
#endif

#include "MFCteamProjectDoc.h"
#include "MFCteamProjectView.h"

#include "opencv2/opencv.hpp"

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
	if ( iVal == 1)
	{
		img = cv::imread("C:\\MFCteamProject\\월인천강지곡 권상\\01_scan\\001.jpg", cv::ImreadModes::IMREAD_UNCHANGED);
	}
	if ( iVal == 2)
	{
		img = cv::imread("C:\\MFCteamProject\\월인천강지곡 권상\\01_scan\\002.jpg", cv::ImreadModes::IMREAD_UNCHANGED);
	}
	if (iVal == 3)
	{
		img = cv::imread("C:\\MFCteamProject\\월인천강지곡 권상\\01_scan\\003.jpg", cv::ImreadModes::IMREAD_UNCHANGED);
	}

	CDC* pDC;
	CImage* mfcImg = nullptr;
	pDC = m_pic.GetDC();

	CRect rect;
	m_pic.GetClientRect(&rect);
	cv::resize(img, img, cv::Size(rect.Width() / 8*8, rect.Height() / 8*8));

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
	img.release();
	ReleaseDC(pDC);
	*pResult = 0;
}




/*void CMFCteamProjectView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	cv::Mat img;
	if (nPos == 1)
	{
		img = cv::imread("C:\\Users\\darco\\Documents\\3학년 1학기\\응용프로그래밍\\월인천강지곡 권상\\01_scan\\001.jpg", cv::ImreadModes::IMREAD_UNCHANGED);
	}
	if (nPos == 2)
	{
		img = cv::imread("C:\\Users\\darco\\Documents\\3학년 1학기\\응용프로그래밍\\월인천강지곡 권상\\01_scan\\002.jpg", cv::ImreadModes::IMREAD_UNCHANGED);
	}
	if (nPos == 3)
	{
		img = cv::imread("C:\\Users\\darco\\Documents\\3학년 1학기\\응용프로그래밍\\월인천강지곡 권상\\01_scan\\003.jpg", cv::ImreadModes::IMREAD_UNCHANGED);
	}

	CDC* pDC;
	CImage* mfcImg = nullptr;
	pDC = m_pic.GetDC();

	CRect rect;
	m_pic.GetClientRect(&rect);
	cv::resize(img, img, cv::Size(rect.Width() / 8 * 8, rect.Height() / 8 * 8));

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
	img.release();
	ReleaseDC(pDC);



	CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
}
*/