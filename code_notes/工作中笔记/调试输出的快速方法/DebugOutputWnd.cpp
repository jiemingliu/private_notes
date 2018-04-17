// DebugOutputWnd.cpp : 实现文件
//

#include "stdafx.h"
#include ".\debugoutputwnd.h"

#define ID_DEBUGWND_RICHEDIT 88888

// CDebugOutputWnd

CDebugOutputWnd* CDebugOutputWnd::m_pDebugOutputWnd = NULL;
CRichEditCtrl* CDebugOutputWnd::m_pRichEdit = NULL;

IMPLEMENT_DYNAMIC(CDebugOutputWnd, CWnd)
CDebugOutputWnd::CDebugOutputWnd()
{
	if (!m_pRichEdit)
		m_pRichEdit = new CRichEditCtrl();
}

CDebugOutputWnd::~CDebugOutputWnd()
{
	if (m_pRichEdit)
		delete m_pRichEdit;
	m_pRichEdit = NULL;
}


BEGIN_MESSAGE_MAP(CDebugOutputWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


CDebugOutputWnd* CDebugOutputWnd::ShowDebugWindow()
{
	if ( !m_pDebugOutputWnd )
		m_pDebugOutputWnd = new CDebugOutputWnd();

	if ( !m_pDebugOutputWnd->m_hWnd )
	{
		CRect rect(0, 0, 300, 400);
		m_pDebugOutputWnd->CreateEx( WS_EX_TOPMOST, NULL, _T("调试信息窗口"),
			WS_OVERLAPPEDWINDOW, rect, NULL, 0);
	}

	m_pDebugOutputWnd->ShowWindow(SW_SHOWNORMAL);
	return m_pDebugOutputWnd;
}

void CDebugOutputWnd::CloseDebugWindow()
{
	if (m_pDebugOutputWnd)
		m_pDebugOutputWnd->ShowWindow(SW_HIDE);
}

void CDebugOutputWnd::Output( CString& strInfo )
{
	ShowDebugWindow();
	m_pRichEdit->SetSel(-1,-1);
	m_pRichEdit->ReplaceSel(strInfo);
}
// CDebugOutputWnd 消息处理程序


int CDebugOutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	AfxInitRichEdit2();
	m_pRichEdit->Create(WS_HSCROLL|WS_VSCROLL|ES_AUTOVSCROLL|ES_MULTILINE|ES_NOHIDESEL/*|ES_READONLY*/, 
		CRect(0,0,0,0), this, ID_DEBUGWND_RICHEDIT);
	// 设置控件不自动换中英文字体
	m_pRichEdit->SendMessage(EM_SETLANGOPTIONS, 0, (m_pRichEdit->SendMessage(EM_GETLANGOPTIONS, 0, 0 ) & ~IMF_AUTOFONT ));

	CHARFORMAT2 cf2;
	m_pRichEdit->GetDefaultCharFormat(cf2);
	cf2.dwMask |= CFM_FACE | CFM_WEIGHT;
	lstrcpyn(cf2.szFaceName, _T("宋体"), LF_FACESIZE);
	cf2.yHeight = 210;
	cf2.wWeight = 400;
	m_pRichEdit->SetDefaultCharFormat(cf2);
	m_pRichEdit->ShowWindow(SW_SHOW);

	return 0;
}

void CDebugOutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	m_pRichEdit->MoveWindow(0, 0, cx, cy);
}

void CDebugOutputWnd::DestoryDebugWindow()
{
	if (m_pDebugOutputWnd)
		delete m_pDebugOutputWnd;
}