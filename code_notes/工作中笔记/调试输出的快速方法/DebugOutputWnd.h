#pragma once


// CDebugOutputWnd类用于程序发布形式(或者Release)单独运行时辅助调试
// 对于大数据量时，Debug调试显得很无力(太慢了)

/* 使用方法:

   *输出信息：
		CString strInfo = "调试信息"
		CDebugOutputWnd::Output(strInfo);

   *析构：在程序退出时调用
		CDebugOutputWnd::DestoryDebugWindow();
*/

class CDebugOutputWnd : public CWnd
{
	DECLARE_DYNAMIC(CDebugOutputWnd)

public:
	static CDebugOutputWnd* ShowDebugWindow();
	static void CloseDebugWindow();
	static void Output(CString& strInfo);
	static void DestoryDebugWindow();
	virtual ~CDebugOutputWnd();

protected:
	CDebugOutputWnd();
	DECLARE_MESSAGE_MAP()

private:
	static CDebugOutputWnd* m_pDebugOutputWnd;
	static CRichEditCtrl* m_pRichEdit;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


