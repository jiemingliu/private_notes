/******************************************************************************
* 文件名: jcshell.h
* 描述: 与命令行进程交互类型
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2017/02/27 17:33:45, lys 创建文件
*
******************************************************************************/
#ifndef _JCSHELL_H_
#define _JCSHELL_H_

#include <windows.h>
#include <string>
#include "jcnonecopy.h"
#include "jcbase.h"

using namespace std;

class DLL_EXPORT JCShell : public JCNonecopy
{
public:
	JCShell();
	~JCShell();

	bool RunShell(const string &strProcess);/*strProcess指定要执行CMD进程具体位置*/
	bool StopShell(void);
	bool RecvReply(const string &strEndMark, int nTimeOut, string &strResponse);
	bool SendRequest(const string &strCmd);

private:
	void CloseInputHandle();
	void CloseOutputHandle();

private:
	HANDLE m_hChildInputWrite;
	HANDLE m_hChildInputRead;
	HANDLE m_hChildOutputWrite;
	HANDLE m_hChildOutputRead;
	PROCESS_INFORMATION m_tCmdPI; /*CMD进程信息*/
};

#endif

