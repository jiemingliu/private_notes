/******************************************************************************
* 文件名: jcudtsocket.h
* 描述: UDT SOCKET封装及周边定义
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/10/28 10:6:33, lys 创建文件
*
******************************************************************************/
#ifndef _JCUDTSOCKET_H_
#define _JCUDTSOCKET_H_

#include "jcnetwork.h"
#include "jcLock.hpp"

using namespace std;

class JCThread;

class JCUdtSocketBase
{
public:
	JCUdtSocketBase();
	virtual ~JCUdtSocketBase();

	static bool StartUpUdtModule();
	static bool CleanUpUdtModule();

public:
	virtual bool Open(ISocketListener* pListener
		, unsigned int nSocketType = SOCK_DGRAM
		, SocketAddress *ptLocalAddr = NULL
		, bool bRendezvous = false)=0;
	virtual void Close()=0;
	void SetListener(ISocketListener *pListener);
	bool SetOption(int nOptName, void *pOptval, int nOptlen, int nLevel = SOL_SOCKET);
	bool GetOption(int nOptName, void *pOptval, int *pOptlen, int nLevel = SOL_SOCKET);
	unsigned int GetType();
	unsigned int GetSocketId();
	unsigned char *GetRecvBuffer();
	string GetLocalIp();
	string GetRemoteIp();
	unsigned int GetLocalPort();
	unsigned int GetRemotePort();
	bool IsValid();
	bool IsOpen();
	bool IsBind();

protected:
	bool m_bOpen;
	bool m_bBind;
	bool m_bRecving;
	JCThread *m_pRecvThread;

	int m_nSocket;
	unsigned int m_nSocketType;
	SocketAddress m_tLocalAddr;
	SocketAddress m_tRemoteAddr;
	ISocketListener* m_ptSockListener;
	unsigned char* m_pRecvBuff;
};

class JCUdtNormalSocket : public JCUdtSocketBase
{
public:
	JCUdtNormalSocket();
	virtual ~JCUdtNormalSocket();

	virtual bool Open(ISocketListener* pListener
		, unsigned int nSocketType = SOCK_DGRAM
		, SocketAddress *ptLocalAddr = NULL
		, bool bRendezvous = false);
	virtual void Close();
	virtual bool Connect(SocketAddress *ptRemoteAddr);
	virtual unsigned int Send(const char* pBuf, int nBufLen);
	static unsigned long WINAPI ThreadRecv(LPVOID pParam);

protected:
	bool RunRecv();
	virtual void ProcRecv();
	virtual int Recv(string &strRemoteIp);

private:
	JCUdtNormalSocket(const JCUdtNormalSocket& rhs);
	JCUdtNormalSocket& operator=(const JCUdtNormalSocket& rhs);
};

class JCUdtServerSocket : public JCUdtSocketBase
{
public:
	JCUdtServerSocket();
	virtual ~JCUdtServerSocket();

	virtual bool Open(ISocketListener* pListener
		, unsigned int nSocketType = SOCK_DGRAM
		, SocketAddress *ptLocalAddr = NULL
		, bool bRendezvous = false);
	virtual void Close();

	virtual unsigned int Send(unsigned int nSocketId, const char* pBuf, int nBufLen);

	static unsigned long WINAPI ThreadRecv(LPVOID pParam);
	static unsigned long WINAPI ThreadAccept(LPVOID pParam);

protected:
	string GetClientSocketIp(unsigned int nSocketId);
	unsigned int AddClientSocket(unsigned int nSocketId, string &strIp);
	unsigned int EraseClientSocket(unsigned int nSocketId);
	unsigned int ClientCount();
	bool Listen();
	bool RunAccept();
	bool RunRecv();
	virtual void ProcAccept();
	virtual void ProcRecv();

	virtual unsigned int Recv(unsigned int nSocketId);

private:
	bool m_bAccepting;
	unsigned int m_nEpollId;
	JCThread *m_pAcceptThread;
	JCCriticalSection m_tCs;
	map<unsigned int, string> m_tClientSocketMap;

	JCUdtServerSocket(const JCUdtServerSocket& rhs);
	JCUdtServerSocket& operator=(const JCUdtServerSocket& rhs);
};

#endif /*_JCUDTSOCKET_H_*/