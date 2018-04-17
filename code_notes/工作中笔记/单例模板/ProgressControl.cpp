#include "stdafx.h"
#include "ProgressControl.h"
#include "DataPoProgressDlg.h"
#include "Tools/LoadLib.h"
#include <boost\bind.hpp>
CProgressControl::CProgressControl()
{
	m_pThread = NULL;
	m_progress = NULL;
	m_bsetUpdateFunc = FALSE;
	m_isProgressRun = FALSE;
}

CProgressControl::~CProgressControl()
{
	if (m_pThread != NULL)
	{
		TerminateThread(m_pThread , 0);
		m_pThread = NULL;
	}
	if (m_progress != NULL)
	{
		m_progress->DeleteProgress();
	}
	m_progress = NULL;
}

void CProgressControl::BeginProgress()
{
	if (m_isProgressRun)
	{
		return;
	}
	if (m_progress == NULL)
	{
		m_progress = new CDataPoProgressDlg;
		CString titleInfo = _T("正在处理数据，请稍等...");
		m_progress->Create(IDD_DIALOG_PROGRESS);
		m_progress->ShowWindow(TRUE);
		m_progress->SetTitleInfo(titleInfo);
		m_progress->CenterWindow();
		m_progress->SetMyPtrPtr(&m_progress);
		m_hProgressWnd = m_progress->m_hWnd;	
	}
	m_isProgressRun = TRUE;
}

BOOL CProgressControl::EndProgress()
{
	m_isProgressRun = FALSE;
	if (m_bsetUpdateFunc)
	{
		//刷新函数
		::SendMessage(m_hProgressWnd , DLG_UPDATEVIEW , NULL , NULL);
		m_bsetUpdateFunc = FALSE;
	}
	if (m_progress == NULL)
	{
		return FALSE;
	}
	try
	{
		m_progress->DeleteProgress();
	}
	catch (CException* e)
	{
		delete m_progress;
	}
	m_progress = NULL;
	return TRUE;
}

void CProgressControl::BindReadFunc( boost::function<BOOL ()> readFunc )
{
	if (m_progress == NULL)
	{
		return;
	}
	m_progress->BindReadFunc().push_back(readFunc);
}

void CProgressControl::BeginReadCase()
{
	if (m_progress == NULL)
	{
		return;
	}
	//执行完任务后结束对话框
	BindReadFunc(boost::bind(&CProgressControl::EndProgress ,this));
	//开始执行任务
	m_progress->BeginReadCase();
}

void CProgressControl::StepProgress( int pos , const CString fileInfo , const CString titleInfo )
{
	if (m_progress == NULL)
	{
		return;
	}
	m_progress->StepProgress(pos , fileInfo , titleInfo);
}

void CProgressControl::StepProgress()
{
	if (m_progress == NULL)
	{
		return;
	}
	m_progress->StepProgress();
}

void CProgressControl::StepProgress(const CString titleInfo)
{
	if (m_progress == NULL)
	{
		return;
	}
	m_progress->StepProgress(titleInfo);
}

void CProgressControl::BindUpdateFunc( fun_UpdateFunc updateFunc )
{
	m_bsetUpdateFunc = TRUE;
	m_progress->BindUpdateFunc(updateFunc);
}

void CProgressControl::AddErrorInfo( CString strError )
{
	if (m_progress == NULL)
	{
		return;
	}
	m_progress->AddErrorInfo(strError);
}
inline UINT AutoRunProgressThread(LPVOID threadParam)
{
	CDataPoProgressDlg* progressDlg = new CDataPoProgressDlg;
	(/*(CDataPoProgressDlg*)*/threadParam) = progressDlg;
	CString titleInfo = _T("正在处理数据，请稍等...");
	progressDlg->Create(IDD_DIALOG_PROGRESS);
	progressDlg->ShowWindow(TRUE);
	progressDlg->SetTitleInfo(titleInfo);
	progressDlg->CenterWindow();
	progressDlg->GetCapture();
	progressDlg->UpdateWindow();
	progressDlg->RunProgress();
	return 1;
}

void CProgressControl::AutoRunProgress()
{
	if (m_progress != NULL)
	{
		m_progress->DeleteProgress();
		m_progress = NULL;
	}
	m_pThread = CreateThread(NULL ,0,(LPTHREAD_START_ROUTINE)AutoRunProgressThread,
		m_progress,0,NULL);
}

void CProgressControl::StopAutoProgress()
{
	//结束自动滚动的进度条线程
	if (m_pThread != NULL)
	{
		TerminateThread(m_pThread , 0);
		//线程句柄置空
		m_pThread = NULL;
	}
	if (m_progress == NULL)
	{
		return ;
	}
	delete m_progress;
	m_progress = NULL;
}

BOOL CProgressControl::IsProgressRun()
{
	return m_isProgressRun;
}