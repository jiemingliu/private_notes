/******************************************************************************
* 文件名: jcnamedpipe.h
* 描述: -
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/04/11 21:10:33, gaopeng 创建文件
*
******************************************************************************/

#ifndef _JCNAMEDPIPE_H
#define _JCNAMEDPIPE_H

#ifndef _INC_WINDOWS
#include <windows.h>
#endif

#include <stdio.h>
#include "jcbase.h"
#include "jclog.h"


#define TEMS_PIPE_NAME "\\\\.\\pipe\\TemsNamedPipe"
#define BUFSIZE 1024*4

#define MAX_CONNECT 128

class JCThread;
class ISocketListener;

namespace SNACC
{
	class JCDbusMsgData;
}

using namespace SNACC;

typedef struct {
	JCThread*	m_hTread ;
	HANDLE	m_hPipe ;
	HANDLE	m_hEvent ;
} PIPE_INSTRUCT;

class DLL_EXPORT JCNamedPipeServer
{
public:
	JCNamedPipeServer();
	JCNamedPipeServer(ISocketListener* listener);
	~JCNamedPipeServer();

public:
	void InitPara(ISocketListener* islister);
	bool RunPipeServer();
	int Send(JCDbusMsgData& data);
	int Recv(JCDbusMsgData& data);
	static unsigned int WINAPI ServerThread(LPVOID lpParameter);
	void _Run();
	void HandleNamePipeEvent(PIPE_INSTRUCT& CurPipeInst, OVERLAPPED& OverLapStruct);

	void Stop();

private:
	char m_chBuf[BUFSIZE];
	int m_nConNum;
	PIPE_INSTRUCT m_tPipeInst[MAX_CONNECT] ;
	ISocketListener* m_ptListener;
	JCLogWrapper m_tLog;
	HANDLE m_tStopEvent;
};

class DLL_EXPORT JCNamedPipeClient
{
public:
	JCNamedPipeClient();
	~JCNamedPipeClient();

public:
	bool Connect();
	int Send(JCDbusMsgData& data);
	int Recv(JCDbusMsgData& data);

private:
	HANDLE m_hPipe;
	char m_chBuf[BUFSIZE];
};

#endif /* _JCNAMEDPIPE_H */

