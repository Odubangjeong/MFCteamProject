
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCteamProjectView

IMPLEMENT_DYNCREATE(CMFCteamProjectView, CFormView)

BEGIN_MESSAGE_MAP(CMFCteamProjectView, CFormView)
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
