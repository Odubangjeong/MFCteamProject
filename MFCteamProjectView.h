
// MFCteamProjectView.h: CMFCteamProjectView 클래스의 인터페이스
//

#pragma once


class CMFCteamProjectView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CMFCteamProjectView() noexcept;
	DECLARE_DYNCREATE(CMFCteamProjectView)

public:
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
};

#ifndef _DEBUG  // MFCteamProjectView.cpp의 디버그 버전
inline CMFCteamProjectDoc* CMFCteamProjectView::GetDocument() const
   { return reinterpret_cast<CMFCteamProjectDoc*>(m_pDocument); }
#endif

