
#pragma once

#include "Tools/Singleton.h"
#include <boost/function.hpp>
//刷新回调函数
typedef boost::function<void ()> fun_UpdateFunc;
class CLoadLib;
class CDataPoProgressDlg;
class AFX_EXT_CLASS CProgressControl:
	public Singleton<CProgressControl>
{
public:
	CProgressControl();
	~CProgressControl();

public:
	/**
	* 方法:	进度条是否已创建
	* 全名:	CProgressControl::IsProgressRun
	* 类型:	public 
	* @日期 	2008-12-30
	* @作者  iuhiao
	* @返回类型 	BOOL
	*/
	BOOL IsProgressRun();
	//************************************
	// Method:    开始进度条
	// FullName:  CProgressControl::BeginProgress
	// Access:    public 
	// Returns:   void
	// Qualifier:
	//************************************
	void BeginProgress();

	//************************************
	// Method:    结束进度条
	// FullName:  CProgressControl::EndProgress
	// Access:    public static 
	// Returns:   void
	// Qualifier:
	//************************************
	BOOL EndProgress();

	//************************************
	// Method:    绑定读文件函数
	// FullName:  CProgressControl::BindReadFunc
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: fun_CaseReadFunc readFunc
	//************************************
	void BindReadFunc(boost::function<BOOL ()> readFunc);

	//************************************
	// Method:    绑定刷新函数
	// FullName:  CProgressControl::BindUpdateFunc
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: fun_UpdateFunc updateFunc
	//************************************
	void BindUpdateFunc(fun_UpdateFunc updateFunc);

	//************************************
	// Method:    开始读方案
	// FullName:  CProgressControl::BeginReadCase
	// Access:    public 
	// Returns:   void
	// Qualifier:
	//************************************
	void BeginReadCase();

	/************************************
	* 方　　法:    设置进度条
	* 全　　名:    CProgressControl::StepProgress
	* 访问权限:    public 
	* 返回类型:    void

	* 参　　数:    int pos
	* 参　　数:    const CString titleInfo
	* 作　　者:    iuhiao
	************************************/
	void StepProgress(int pos , const CString fileInfo , const CString titleInfo);
	void StepProgress(const CString titleInfo);
	void StepProgress();

	/**
	* 方法:	添加错误信息
	* 全名:	CProgressControl::AddErrorInfo
	* 类型:	public 
	* @作者  iuhiao
	* @参数 	CString strError
	* @返回类型 	void
	*/
	void AddErrorInfo(CString strError);

	/**
	* 方法:	进度条自动滚动
	* 全名:	CProgressControl::AutoRunProgress
	* 类型:	public 
	* @日期 	2008-12-22
	* @作者  iuhiao
	* @返回类型 	void
	*/
	void AutoRunProgress();

	/**
	* 方法:	中止自动滚动
	* 全名:	CDataPoProgressDlg::StopAutoProgress
	* 类型:	public 
	* @日期 	2008-12-22
	* @作者  iuhiao
	* @返回类型 	void
	*/
	void StopAutoProgress();

private:
	//进度条线程
	HANDLE m_pThread;
	//进度条对话框
	CDataPoProgressDlg* m_progress;
	HWND m_hProgressWnd;
	//进度条是否已创建
	BOOL m_isProgressRun;
	//是否设置了刷新函数
	BOOL m_bsetUpdateFunc;
};