
// MFCteamProjectView.h: CMFCteamProjectView 클래스의 인터페이스
//

#pragma once

#include "opencv2/opencv.hpp"

#include "CTypeDB.h"
#include <iostream>
#include <filesystem>



struct RectInfo
{
	CRect rect;
	int char_index;
};

struct TypeInfo
{
	// 글자의 경로
	CString path;
	//std::filesystem::path charpath;
	// 글자의 유니코드 = 폴더명
	CString m_char_name;
	// 활자 = 하위 폴더명
	int f_type;
	// 장
	int f_sheet;
	// x 좌표
	CString f_sx;
	// y 좌표
	CString f_sy;
};



class CMFCteamProjectView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CMFCteamProjectView() noexcept;
	DECLARE_DYNCREATE(CMFCteamProjectView)

public:
	vtkSmartPointer<vtkRenderWindow> m_vtkWindow;
	void InitVtkWindow(void* hwnd);
	void ResizeVtkWindow();

#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_MFCTEAMPROJECT_FORM };
#endif

// 특성입니다.
public:
	CMFCteamProjectDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CMFCteamProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnDeltaposSpinSheet(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton2();
	CSpinButtonCtrl m_spin;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CStatic m_pic;
	afx_msg void OnPaint();
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

// CSV 파일 관련
	CTypeDB db;

// 폴더 경로
protected:
	CString RootPath;
	bool opend = 0;



	// 현재 선택한 글자 chinfo
protected:
	SCharInfo chinfo;

// 03_type 폴더 내 파일 리스트
protected:
	CArray<CString, CString&> char_lists;
	// 선택한 문자를 활자들의 리스트 (하위 폴더)
	CArray <CString, CString&> dir_list;
	// 선택한 문자를 표현한 활자를 사용한 모든 문자의 리스트 type_lists
	CArray< TypeInfo, TypeInfo&> type_lists;

	CMap<int, int, CString, CString> type_map;

	// 구성 글자 리스트에 올라가는 것들
	CArray< TypeInfo, TypeInfo&> item_lists;


//	LPNMUPDOWN pNMUpDown;

// 글자 정보 출력
public:
	void CMFCteamProjectView::DisplayCharInfo(int index);
//	void CMFCteamProjectView::DisplayTypeInfo(void);
// 이미지 상에 그려지는 사각형 좌표
protected:
	const int p_thickness = 1;
	cv::Mat img;
	int selected_img = 0;
	CArray<RectInfo, RectInfo&> img_rects;
	int img_selected_rect = -1;

	cv::Mat cimg;
	cv::Mat timg;

	CString char_path;

// 활자 정보 출력 관련
protected:
	// 현재 활자 정보창에서 선택한 활자
	int currentType = 1;
	// 선택한 문자를 표현하는 활자의 개수 (m_char 아래 하위 폴더의 갯수)
	int numberType = 1;
	int minnumberType = 1;

	int flag_type = -1;
	CString typecharpath;
	CString TypeCharP;
	int initvtk;
	// 현재 리스트에서 선택중인 항목
//	int idx;

	// 현재 선택한 활자 currentType에 따라 리스트를 구성하는 함수
	void CMFCteamProjectView::makeTypeList(int sel);
	// 현재 모형과 선택 글자 그림 출력
	void CMFCteamProjectView::drawVTKnSChar(int number);
// 카운팅
protected:
	int count;
protected:
	void LoadNewImage(int img_index);
	void DrawImage(void);
	void CMFCteamProjectView::DrawcharImage(CString f_path);
	void CMFCteamProjectView::DrawtypeImage(CString f_path);

//VTK
public:
	void displayVTK(CString path);
public:
//	afx_msg void OnStnClickedPic();
	CStatic CharSheet;
	CStatic CharLine;
	CStatic CharOrder;
	CStatic m_char_pic;
	CButton CharChar;
	CStatic CharCode;
	CStatic mTypes;
	CEdit m_pname;
	afx_msg void OnStnClickedCharsheet();
	CStatic m_allnum;
	CStatic m_alltypenum;
	CStatic m_allhwallnum;
	CStatic m_pnum;
	CStatic m_ptype;
	CStatic m_phwall;
	CEdit m_edType;
	CSpinButtonCtrl m_spType;
	afx_msg void OnDeltaposSpinType(NMHDR* pNMHDR, LRESULT* pResult);
	CListCtrl mListChars;
	afx_msg void OnNMClickListChars(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawListChars(NMHDR* pNMHDR, LRESULT* pResult);
	CStatic m_type_pic;
	afx_msg void OnStnClickedStaticTypes();
	afx_msg void OnStnClickedallnum();
};

#ifndef _DEBUG  // MFCteamProjectView.cpp의 디버그 버전
inline CMFCteamProjectDoc* CMFCteamProjectView::GetDocument() const
   { return reinterpret_cast<CMFCteamProjectDoc*>(m_pDocument); }
#endif

