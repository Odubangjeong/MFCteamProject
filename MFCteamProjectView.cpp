
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

#include <iostream>
#include <filesystem>
#include <typeinfo>
namespace fs = std::filesystem;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//vtk코드
#include <vtkSTLReader.h>
#include <vtkLight.h>



#include <locale.h>
#include <iostream>
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
//ON_STN_CLICKED(IDC_PIC, &CMFCteamProjectView::OnStnClickedPic)
ON_STN_CLICKED(IDC_CHARSHEET, &CMFCteamProjectView::OnStnClickedCharsheet)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TYPE, &CMFCteamProjectView::OnDeltaposSpinType)
ON_NOTIFY(NM_CLICK, IDC_LIST_CHARS, &CMFCteamProjectView::OnNMClickListChars)


ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_CHARS, &CMFCteamProjectView::OnNMCustomdrawListChars)
ON_STN_CLICKED(IDC_STATIC_TYPES, &CMFCteamProjectView::OnStnClickedStaticTypes)
ON_STN_CLICKED(IDC_allnum, &CMFCteamProjectView::OnStnClickedallnum)
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
	DDX_Control(pDX, IDC_CHARSHEET, CharSheet);
	DDX_Control(pDX, IDC_CHARLINE, CharLine);
	DDX_Control(pDX, IDC_CHARORDER, CharOrder);
	DDX_Control(pDX, IDC_CHAR, m_char_pic);
	DDX_Control(pDX, IDC_BUTTON_OPEN, CharChar);
	DDX_Control(pDX, IDC_CHARNAME, CharCode);
	DDX_Control(pDX, IDC_STATIC_TYPES, mTypes);
	DDX_Control(pDX, IDC_EDIT_BOOKNAME, m_pname);
	DDX_Control(pDX, IDC_allnum, m_allnum);
	DDX_Control(pDX, IDC_alltypenum, m_alltypenum);
	DDX_Control(pDX, IDC_allhwallnum, m_allhwallnum);
	DDX_Control(pDX, IDC_pnum, m_pnum);
	DDX_Control(pDX, IDC_ptype, m_ptype);
	DDX_Control(pDX, IDC_phwall, m_phwall);
	DDX_Control(pDX, IDC_EDIT_TYPE, m_edType);
	DDX_Control(pDX, IDC_SPIN_TYPE, m_spType);
	DDX_Control(pDX, IDC_LIST_CHARS, mListChars);
	DDX_Control(pDX, IDC_TYPE_PIC, m_type_pic);
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
//	CString str;
//	str.Format(L"월인천강지곡 권상\\typeDB.csv");
//	db.ReadCSVFILE(str);

	
	//vtk 코드
	if (this->GetDlgItem(IDC_STATIC_FRAME))
	{
		this->InitVtkWindow(this->GetDlgItem(IDC_STATIC_FRAME)->GetSafeHwnd());
		this->ResizeVtkWindow();
	}


	// 활자 정보 스핀 컨트롤 초기화
	CString s_ed;
	s_ed.Format(_T("%d"), currentType);
	m_edType.SetWindowTextW(s_ed);
	// 스핀 컨트롤 범위 설정
	m_spType.SetRange(minnumberType, numberType);
	// 초기값 1으로 설정
	m_spType.SetPos(1);
//	mTypes.SetWindowTextW(_T("/ 1개"));
	
	// 리스트 컨트롤 초기화
	CRect ret;
	mListChars.GetWindowRect(&ret);
	mListChars.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	mListChars.InsertColumn(0, _T("장"), LVCFMT_CENTER, ret.Width() * 0.2);
	mListChars.InsertColumn(1, _T("x"), LVCFMT_CENTER, ret.Width() * 0.35);
	mListChars.InsertColumn(2, _T("y"), LVCFMT_CENTER, ret.Width() * 0.35);
	//flag_type = -1;
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
	CWaitCursor wait;
	CString strInitPath = _T("C:\\");
	CString strFolderPath;
	CFolderPickerDialog Picker(strInitPath, OFN_FILEMUSTEXIST, NULL, 0);
	if (Picker.DoModal() == IDOK)
	{
		// 경로 가져오기
		RootPath = Picker.GetPathName();
	//	m_pname.SetWindowText(RootPath);
	//	AfxMessageBox(RootPath);
		strFolderPath = Picker.GetFileName();
		m_pname.SetWindowText(strFolderPath);
	}

	if (strFolderPath == _T("월인천강지곡 권상")) {
		opend = 1;
		CString path = RootPath;
		path += _T("\\typeDB.csv");
		db.ReadCSVFILE(path);
		Invalidate();
		LoadNewImage(1);
	}
		// 호출
	//	char* setlocae(int category, const char* locale);
	//	setlocale(LC_ALL, "");
	
	db.numcount();
	m_allnum.SetWindowText(db.num);
	m_alltypenum.SetWindowText(db.type);
	m_allhwallnum.SetWindowText(db.hwall);

	m_pnum.SetWindowText(db.num1);
	m_ptype.SetWindowText(db.type1);
	m_phwall.SetWindowText(db.hwall1);
	
	//CString str;
	//str.Format(_T("%d"), info.m_type);
	//CString ppath = _T("04_3d\\") + info.m_char + _T("_") + str;   //이름.확장자
	

//	CString path = RootPath;
//	path += _T("\\typeDB.csv");
//	db.ReadCSVFILE(path);
//	LoadNewImage(1);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	wait.Restore();
}


void CMFCteamProjectView::OnDeltaposSpinSheet(NMHDR* pNMHDR, LRESULT* pResult)
{
	CWaitCursor wait;
	// if (opend == 0) return;
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	cv::Mat img;

	int iVal = pNMUpDown->iPos + pNMUpDown->iDelta;
	
	if (iVal >= 0 && iVal <= 3) {
		LoadNewImage(iVal);
		DrawImage();
		db.numcount();
		if (iVal == 1)
		{
			m_pnum.SetWindowText(db.num1);
			m_ptype.SetWindowText(db.type1);
			m_phwall.SetWindowText(db.hwall1);
		}
		else if (iVal == 2)
		{
			m_pnum.SetWindowText(db.num2);
			m_ptype.SetWindowText(db.type2);
			m_phwall.SetWindowText(db.hwall2);
		}
		else if (iVal == 3)
		{
			m_pnum.SetWindowText(db.num3);
			m_ptype.SetWindowText(db.type3);
			m_phwall.SetWindowText(db.hwall3);
		}
	}
	
		
	wait.Restore();
} 



void CMFCteamProjectView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
				  // TODO: 여기에 메시지 처리기 코드를 추가합니다.
				  // 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.

	
	DrawImage();
	DrawcharImage(char_path);

	if (flag_type == 1) {
		drawVTKnSChar(initvtk);
	//	DrawtypeImage();
	}
	
	//flag_type = 1;
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
	CWaitCursor wait;
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

		// 글자 정보 표시
		DisplayCharInfo(img_selected_rect);
	//	DrawcharImage(img_selected_rect);
	}

	
	
	CFormView::OnLButtonDown(nFlags, point);
	wait.Restore();
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
		CString t = RootPath;
		t += _T("\\01_scan\\001.jpg");
		std::string s((CT2CA)t);
		img = cv::imread(s, cv::ImreadModes::IMREAD_UNCHANGED);
	}
	else if (img_index == 2)
	{
		CString t = RootPath;
		t += _T("\\01_scan\\002.jpg");
		std::string s((CT2CA)t);
		img = cv::imread(s, cv::ImreadModes::IMREAD_UNCHANGED);
	}
	else if (img_index == 3)
	{
		CString t = RootPath;
		t += _T("\\01_scan\\003.jpg");
		std::string s((CT2CA)t);
		img = cv::imread(s, cv::ImreadModes::IMREAD_UNCHANGED);
	}
	int img_width = img.size().width;
	int img_height = img.size().height;

	// 그렸던 사각형 기록 삭제
	img_rects.RemoveAll();
	img_selected_rect = -1;

	// 이미지 크기 조절
	cv::resize(img, img, cv::Size(rect_width, rect_height));
	bool end_count = 0;
	count = 0;

	int isFirst = -1;

	for (int i = 0; i < db.getLength(); i++) {
		// 글자 페이지 비교하기
		// SCharInfo chInfo = m_Chars.GetAt(i);
		// m_ind = i;
		// m_sx = chInfo.m_sx;
		// ...
		// TODO : CArray를 가져오시오...
		
		SCharInfo chInfo = db.getChars(i);
		if (chInfo.m_sheet != img_index) {
			if (end_count != 1) count++;
			continue;
		}
		end_count = 1;
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


		if (isFirst == -1) {
			cv::rectangle(img, p_rect, cv::Scalar(0, 0, 255), p_thickness, 4, 0);
			img_selected_rect = 0;
			isFirst = 1;
			DisplayCharInfo(img_selected_rect);
		}
		else {
			cv::rectangle(img, p_rect, cv::Scalar(0, 255, 0), p_thickness, 4, 0);
		}
		

		// 그린 사각형 몇번째인지 저장
		img_rects.Add(p_info);
	}
}

void CMFCteamProjectView::DrawImage() {
	if (opend == 0) return;


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

void CMFCteamProjectView::DrawcharImage(CString f_path) {
	if (opend == 0) return;
	std::string s((CT2CA)f_path);
	cimg = cv::imread(s, cv::ImreadModes::IMREAD_UNCHANGED);


	CDC* pDC;
	CImage* mfcImg = nullptr;
	pDC = m_char_pic.GetDC();

	CRect rect;
	m_char_pic.GetClientRect(&rect);
	cv::resize(cimg, cimg, cv::Size(rect.Width() / 8 * 8, rect.Height() / 8 * 8));


	// 이미지를 사각형으로 출력
	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	bitmapInfo.bmiHeader.biBitCount = 24;
	bitmapInfo.bmiHeader.biWidth = cimg.cols;
	bitmapInfo.bmiHeader.biHeight = -cimg.rows;
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biClrImportant = 0;
	bitmapInfo.bmiHeader.biClrUsed = 0;
	bitmapInfo.bmiHeader.biSizeImage = 0;
	bitmapInfo.bmiHeader.biXPelsPerMeter = 0;

	if (cimg.channels() == 3)
	{
		mfcImg = new CImage();
		mfcImg->Create(cimg.cols, cimg.rows, 24);
	}
	else if (cimg.channels() == 1)
	{
		cv::cvtColor(cimg, cimg, cv::COLOR_GRAY2RGB);
		mfcImg = new CImage();
		mfcImg->Create(cimg.cols, cimg.rows, 24);
	}
	else if (cimg.channels() == 4)
	{
		bitmapInfo.bmiHeader.biBitCount = 32;
		mfcImg = new CImage();
		mfcImg->Create(cimg.cols, cimg.rows, 32);
	}

	::StretchDIBits(mfcImg->GetDC(), 0, 0, cimg.cols, cimg.rows,
		0, 0, cimg.cols, cimg.rows, cimg.data, &bitmapInfo,
		DIB_RGB_COLORS, SRCCOPY);

	mfcImg->BitBlt(::GetDC(m_char_pic.m_hWnd), 0, 0);

	mfcImg->ReleaseDC();
	delete mfcImg;
	mfcImg = nullptr;
	ReleaseDC(pDC);
//	flag_type = 1;
}

void CMFCteamProjectView::DrawtypeImage(CString f_path) {
	if (opend == 0) return;
	std::string s((CT2CA)f_path);
	timg = cv::imread(s, cv::ImreadModes::IMREAD_UNCHANGED);


	CDC* pDC;
	CImage* mfcImg = nullptr;
	pDC = m_type_pic.GetDC();

	CRect rect;
	m_type_pic.GetClientRect(&rect);
	cv::resize(timg, timg, cv::Size(rect.Width() / 8 * 8, rect.Height() / 8 * 8));


	// 이미지를 사각형으로 출력
	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	bitmapInfo.bmiHeader.biBitCount = 24;
	bitmapInfo.bmiHeader.biWidth = timg.cols;
	bitmapInfo.bmiHeader.biHeight = -timg.rows;
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biClrImportant = 0;
	bitmapInfo.bmiHeader.biClrUsed = 0;
	bitmapInfo.bmiHeader.biSizeImage = 0;
	bitmapInfo.bmiHeader.biXPelsPerMeter = 0;

	if (timg.channels() == 3)
	{
		mfcImg = new CImage();
		mfcImg->Create(timg.cols, timg.rows, 24);
	}
	else if (timg.channels() == 1)
	{
		cv::cvtColor(timg, timg, cv::COLOR_GRAY2RGB);
		mfcImg = new CImage();
		mfcImg->Create(timg.cols, timg.rows, 24);
	}
	else if (timg.channels() == 4)
	{
		bitmapInfo.bmiHeader.biBitCount = 32;
		mfcImg = new CImage();
		mfcImg->Create(timg.cols, timg.rows, 32);
	}

	::StretchDIBits(mfcImg->GetDC(), 0, 0, timg.cols, timg.rows,
		0, 0, timg.cols, timg.rows, timg.data, &bitmapInfo,
		DIB_RGB_COLORS, SRCCOPY);

	mfcImg->BitBlt(::GetDC(m_type_pic.m_hWnd), 0, 0);

	mfcImg->ReleaseDC();
	delete mfcImg;
	mfcImg = nullptr;
	ReleaseDC(pDC);

}

void CMFCteamProjectView::DisplayCharInfo(int index) {
	CWaitCursor wait;

	// img_selected_rect 번째 출력
	// db.ReadCSVFILE(_T("월인천강지곡 권상\\typeDB.csv"));

	// 시작하기 전에 리스트 초기화주기
	dir_list.RemoveAll();
	type_lists.RemoveAll();

	CString filename;
	filename.Empty();

	CString sheet;
	CString line;
	CString order;
	CString type;

	CString filelist;

	// 현재 선택한 글자 chinfo
	chinfo = db.getChars(index + count);

	type.Format(_T("%d"), chinfo.m_type);
	// 파일 위치 열기
	// RootPath : 월인천강지...
	CString filepath = RootPath;
	filepath += _T("\\03_type\\");
	filepath += chinfo.m_char;

	type_map.RemoveAll();


	CArray <int> tmpp;
	// 폴더 목록(활자 모양 번호) 리스트 작성
	for (auto& entry : fs::directory_iterator(std::string(CT2CA(filepath)))) {
		CString dir;
		dir = entry.path().stem().string().c_str();
		dir_list.Add(dir);
		tmpp.Add(_ttoi(dir));
	}

	std::sort(tmpp.GetData(), tmpp.GetData() + tmpp.GetSize());

	for (int i = 0; i < dir_list.GetCount(); i++) {
		CString dir;
		dir.Format(_T("%d"), tmpp.GetAt(i));
		type_map.SetAt(i + 1, dir);
	}

	// 각 폴더에서 파일 가져오기
	// filepath += _T("\\");
	// filepath += type;

	TypeInfo info;
	bool _first = 0;
	CString imgPath;


	for (int i = 0; i < dir_list.GetSize(); i++) {
		CString path = filepath;
		path += _T("\\");
		// 폴더를 전부 열어 리스트에 저장
		path += dir_list.GetAt(i);
		for (auto& entry : fs::directory_iterator(std::string(CT2CA(path)))) {
			int n = 0;
			CString sheet, sx, sy;

			info.f_type = _ttoi(dir_list.GetAt(i));
			//AfxMessageBox(info.f_type);
			info.path = entry.path().string().c_str();
			// 
	//		if (_first == 0) {
	//			imgPath = info.path;
	//			_first = 1;
				//AfxMessageBox(imgPath);
	//		}
			CString sub;
			sub = entry.path().stem().string().c_str();
			AfxExtractSubString(sheet, sub, n++, '_');
			AfxExtractSubString(sx, sub, n++, '_');
			AfxExtractSubString(sy, sub, n++, '_');
			//AfxExtractSubString(sy, info.m_char_name, n++, '_');

			// type 폴더 안에서 맞는 sheet의 경로 저장
			
			info.m_char_name = chinfo.m_char;
			info.f_sheet = _ttoi(sheet);
			info.f_sx = sx;
			info.f_sy = sy;
			if (info.f_sheet == chinfo.m_sheet && info.f_type == chinfo.m_type) {
				imgPath = info.path;
			}
			//info.f_sx = _ttoi(sx);
			//info.f_sy = _ttoi(sy);
			type_lists.Add(info);
		}

	}

	// 해당 글자 이미지 보여주기

	// 선택한 글자 정보 문자 출력
	char_path = imgPath;
	DrawcharImage(char_path);


	// 활자 정보 출력
	// 활자 종류의 갯수는 dir의 수
	int numoftype = dir_list.GetCount();
	CString text;
	text.Format(_T("/%2d개"), numoftype);
	mTypes.SetWindowTextW(text);



	
	// map에서 chinfo.m_types과 일치하는
	makeTypeList(chinfo.m_type);
	
	// 활자 정보 spin control 수정
	numberType = dir_list.GetCount();
	m_spType.SetRange(minnumberType, numberType);
	int typess = 1;
	CString ttt;
	CString tmp;
	tmp.Format(_T("%d"), chinfo.m_type);
	for (int i = 0; i < type_map.GetCount(); i++) {
		type_map.Lookup(i + 1, ttt);
		if (ttt == tmp) typess = i + 1;
		//makeTypeList();
	}
	m_spType.SetPos(typess);
	CString ch;
	ch.Format(_T("%d"), typess);
	m_edType.SetWindowTextW(ch);
	//m_edType.SetWindowTextW()
//	m_edType.SetWindowTextW(_T("1"));
	//CString types;
//	mTypes.SetWindowTextW();








	// 글자 정보 출력
	// 출력할 장, 행, 번 string으로 변경
	sheet.Format(_T("%d"), chinfo.m_sheet);
	line.Format(_T("%d"), chinfo.m_line);
	order.Format(_T("%d"), chinfo.m_order);
	// 출력
	CharCode.SetWindowTextW((LPCTSTR)chinfo.m_char);
	CharSheet.SetWindowTextW(sheet);
	CharLine.SetWindowTextW(line);
	CharOrder.SetWindowTextW(order);


	wait.Restore();
}

// 현재 선택한 활자 currentType에 따라 리스트를 구성하는 함수
// 
void CMFCteamProjectView::makeTypeList(int sel) {
	


	// 현재 모든 아이템 지우기
	mListChars.DeleteAllItems();
	// 현재 타입은?
	CString currenttype;
//	if (type_map.Lookup(sel, currenttype) == 0) {
//		AfxMessageBox(_T("해당하는 활자가 없습니다"));
//	}

	// int로의 변환, 현재 선택한 활자 type이 무엇인지 selectedType에 저장됨.
	//int selectedType = _ttoi(currenttype);
	int selectedType = sel;


	// 활자 정보를 가져왔으니, type_lists에서 sheet가 맞는 것들을 가져오자
	item_lists.RemoveAll();

	int listcount = 0;
	int initsel = -1;
	TypeInfo tmp;
	int n = type_lists.GetSize();
	for (int i = 0; i < type_lists.GetSize(); i++) {
		tmp = type_lists.GetAt(i);
		// type이 일치하면 아이템 어레이에 넣자
		if (tmp.f_type == selectedType) {
			// sheet도 일치하면 그건 초기 항목이다.
			if (tmp.f_sheet == chinfo.m_sheet) initsel = listcount;
			listcount++;
			item_lists.Add(tmp);
			// 구성 글자 리스트에 아이템들을 넣자
			// 들어가는 데이터는 CString이다.
			int num = mListChars.GetItemCount();
			// 장
			CString sh;
			sh.Format(_T("%d"), tmp.f_sheet);
			mListChars.InsertItem(num, sh);
			// sx, sy
			mListChars.SetItem(num, 1, LVIF_TEXT, tmp.f_sx, NULL, NULL, NULL, NULL);
			mListChars.SetItem(num, 2, LVIF_TEXT, tmp.f_sy, NULL, NULL, NULL, NULL);
		}
	}
	initvtk = 0;
	int debugcheck = item_lists.GetCount();
	if (initsel != -1) {
		mListChars.SetItemState(initsel, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		initvtk = initsel;
	}
	else mListChars.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	initvtk = 0;
	

	drawVTKnSChar(initvtk);
	
	//mListChars.SetSelectionMark(initsel);


}
void CMFCteamProjectView::drawVTKnSChar(int number) {

	typecharpath = item_lists.GetAt(number).path;
	std::string ps = ((CT2CA)typecharpath);
	DrawtypeImage(typecharpath);
	

	CString vpath;
	typecharpath.Format(_T("%d"), item_lists.GetAt(number).f_type);
	vpath = _T("04_3d\\");
	vpath += item_lists.GetAt(number).m_char_name;
	vpath += _T("_");
	vpath += typecharpath;
	vpath += _T(".stl");
	displayVTK(vpath);
	flag_type = 1;
}

//void CMFCteamProjectView::OnStnClickedPic()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}


void CMFCteamProjectView::OnStnClickedCharsheet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// vtk 코드
void CMFCteamProjectView::InitVtkWindow(void* hWnd)
{
	if (m_vtkWindow == NULL)
	{
		vtkSmartPointer<vtkRenderWindowInteractor> interactor =
			vtkSmartPointer<vtkRenderWindowInteractor>::New();
	
		interactor->SetInteractorStyle(
			vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New());
		
		vtkSmartPointer<vtkRenderer> renderer =
			vtkSmartPointer<vtkRenderer>::New();
		renderer->SetBackground(255, 255, 255); // 배경을 흰색으로 만든다.

		m_vtkWindow = vtkSmartPointer<vtkRenderWindow>::New();
		m_vtkWindow->SetParentId(hWnd);
		m_vtkWindow->SetInteractor(interactor);
		m_vtkWindow->AddRenderer(renderer);
		m_vtkWindow->Render();
	}
}
void CMFCteamProjectView::ResizeVtkWindow()
{
	CRect rc;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_FRAME);
	if (pWnd)
	{
		pWnd->GetClientRect(rc);
		m_vtkWindow->SetSize(rc.Width(), rc.Height());
	}
}

void CMFCteamProjectView::displayVTK(CString ppath)
{
	_tsetlocale(LC_ALL, _T(""));

	CString str;

	
	std::string  s((CT2CA)ppath);

	vtkSmartPointer<vtkSTLReader> pSTLReader =
		vtkSmartPointer<vtkSTLReader>::New();
	pSTLReader->SetFileName((CT2CA)ppath);
	//pSTLReader->SetFileName((const char *) charstr); // 여기서 읽어오는 것임 PATH를 잘 설정해주면 된다.
	//pSTLReader->SetFileName("m_char + "_" + m_type");
	pSTLReader->Update();

	vtkSmartPointer<vtkPolyData> pPolyData =
		pSTLReader->GetOutput();

	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(pPolyData);
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->SetBackground(.1, .2, .3);
	renderer->ResetCamera();

	
	m_vtkWindow->AddRenderer(renderer);
	m_vtkWindow->Render();

}

void CMFCteamProjectView::OnDeltaposSpinType(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iVal = pNMUpDown->iPos + pNMUpDown->iDelta;
	
	if ((minnumberType <= iVal) && iVal <= numberType) {
		currentType = iVal;
		CString s;
		s.Format(_T("%d"), iVal);
		m_edType.SetWindowTextW(s);
		CString types;
		if (type_map.Lookup(iVal, types) == 1) {
			makeTypeList(_ttoi(types));
		}
		
	}

	*pResult = 0;
}

// 리스트 항목 선택시
void CMFCteamProjectView::OnNMClickListChars(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 선택한 행 값 가져오기
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int idx = pNMListView->iItem;
	
	// 선택한 항목이 표시된 항목수보다 크면 무효화
	if (idx < 0) return;

	// 현재 선택중인 항목의 sheet를 가져와, 현재 설정된 type과 합쳐 vtk 경로를 생성하자
	// 현재 m_char는 chinfo.m_char이다. 
	// 아니면 list에서 같은 sheet을 가진 놈을 가져와

	// idx 번째 아이템의 모형, 이미지를 출력합니다.
	CString s;
	s = item_lists.GetAt(idx).path;
	std::string ps = ((CT2CA)s);
	TypeCharP = item_lists.GetAt(idx).path;
	DrawtypeImage(item_lists.GetAt(idx).path);
	
	CString vpath;
	s.Format(_T("%d"), item_lists.GetAt(idx).f_type);
	vpath = _T("04_3d\\");
	vpath += item_lists.GetAt(idx).m_char_name;
	vpath += _T("_");
	vpath += s;
	vpath += _T(".stl");
	displayVTK(vpath);
	*pResult = 0;
}


void CMFCteamProjectView::OnNMCustomdrawListChars(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	*pResult = 0;
}


void CMFCteamProjectView::OnStnClickedStaticTypes()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMFCteamProjectView::OnStnClickedallnum()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
