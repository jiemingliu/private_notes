/******************************************************************************
* 文件名: jcnetwork.h
* 描述: -网络,串口服务封装, 引用此头文件时应置于最前, 避免socket相关重定义
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/03/28 21:12:14, gaopeng 创建文件
*
******************************************************************************/
#ifndef _JCNETWORK_H
#define _JCNETWORK_H

#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "jcbase.h"
#define	 SOCKET			int
#define  SOCKET_ERROR	-1
#define	 INVALID_SOCKET (SOCKET)(~0)
#define  closesocket	close
typedef void * LPVOID;
typedef char CHAR;

static int speed_arr[] =
{ B115200, B38400,  B19200, B9600, B4800, B2400, B1200, B300,  };
static int name_arr[] =
{ 115200, 38400, 19200, 9600, 4800, 2400, 1200, 300, };

#else

#ifndef _WINSOCK2API_

// Prevent inclusion of winsock.h
#ifdef _WINSOCKAPI_
#error Header winsock.h is included unexpectedly.
#endif
#endif

#include <WinSock2.h>
#include <mstcpip.h>  
#define WIN32_LEAN_AND_MEAN
#include <iphlpapi.h>
typedef int socklen_t;
#endif

#include <map>
#include <vector>
#include "jcexception.h"
#include "jcLock.hpp"

class JCThread;

/*提取串口通信控制属性宏定义*/
#define FBINARY(attrbits)             (((attrbits) & BIT0) >> 0)
#define FPARITY(attrbits)             (((attrbits) & BIT1) >> 1)
#define FOUTXCTSFLOW(attrbits)        (((attrbits) & BIT2) >> 2)
#define FOUTXDSRFLOW(attrbits)        (((attrbits) & BIT3) >> 3)
#define FDTRCONTROL(attrbits)         (((attrbits) & (BIT4 | BIT5)) >> 4)
#define FDSRSENSITIVITY(attrbits)     (((attrbits) & BIT6) >> 6)
#define FTXCONTINUEONXOFF(attrbits)   (((attrbits) & BIT7) >> 7)
#define FOUTX(attrbits)               (((attrbits) & BIT8) >> 8)
#define FINX(attrbits)                (((attrbits) & BIT9) >> 9)
#define FERRORCHAR(attrbits)          (((attrbits) & BIT10) >> 10)
#define FNULL(attrbits)               (((attrbits) & BIT11) >> 11)
#define FRTSCONTROL(attrbits)         (((attrbits) & (BIT12 | BIT13)) >> 12)
#define FABORTONERROR(attrbits)       (((attrbits) & BIT14) >> 14)

enum SOCKET_TYPE
{
    IP_SOCKET,
    UDP_SOCKET,
    TCP_SOCKET,
    SERIAL_PORT,
	INVALID_SERVER_TYPE
};

/******************************************************************************
* 结构名:_SocketInfo_
* 描述: -创建tcp,udp,serialPort的参数结构体
* std::string sendIp; 源IP, 用于Tcp,Udp
* int         sendPort; 源端口,用于Tcp,Udp
* std::string recvIp; 目的IP,用于Tcp,Udp
* int         recvPort; 目的端口, 用于Tcp,Udp
* std::string serialPort; 串口名称,用于创建串口服务
* bool        isBroadcast; 是否广播,用于Udp
* SOCKET_TYPE type; 服务类型
* 修改记录:
* --------------------
*    2014/03/29 12:22:9, gaopeng 创建函数
*
******************************************************************************/

typedef struct _SerialPortCfg_
{
	std::string m_strSerialPort;
	DWORD		m_nBaudRate;
	DWORD		m_nByteSize;
	DWORD		m_nParity;
	DWORD		m_nStopBits;
	DWORD		m_nSerialAttrBits;
}SerialPortCfg;

typedef struct _SocketInfo_
{
	/*
	  此结构均从本机发送的角度描述.
	  SendIp及SendPort为本地的IP和端口
	  RecvIp及RecvPort为连接的目的IP和端口
	
	  对于TCP CLIENT: 只需要关心连接的目的服务器IP及端口, 配置RecvIp及RecvPort
	  对于TCP SERVER: 关心监听本地地址的本地端口, 配置SendIp及SendPort
	
      对于UDP: 关心用本地的什么端口向目的地址的目的端口发送, 本地的发送端口也
	  是接收端口; 因此配置本地地址SendIp及本地的发送端口SendPort, 同时也配置目
	  的地址RecvIp及RecvPort

	  注意: JCTcpClientSocket初始化使用了sendid及sendport, 与以上说明是反的, 因为
	  目前已经有了很多实用场景, 所以保持原状, 大家在使用时应注意
	*/
    std::string m_strSendIp;
    int m_nSendPort;
    std::string m_strRecvIp;
    int m_nRecvPort;
    bool m_bIsBroadcast;
	SerialPortCfg m_tSerialPortCfg;
    SOCKET_TYPE m_eSockType;
} SocketInfo,*pSocketInfo;

typedef std::vector<SocketInfo> SocketInfoVer;
typedef std::map<SOCKET, int> SocketRackMap;


/* JC网络异常类型 */
class DLL_EXPORT JCNetworkException: public JCWinErrException
{
public:
    JCNetworkException():JCWinErrException() {};
    virtual ~JCNetworkException()throw() {};

public:
    /* 添加网络异常定制化信息 */
    void AddMsg(const string& s);
};


/*网络回调接口*/
class DLL_EXPORT ISocketListener
{
public:
	virtual int RecvCallBack(char* buf, int len, int iIndex=0, const string &source = "")=0;
	virtual int TcpClearBuf(){return 0;};

	virtual int TcpClientConnect(int nSocketIndex){return 0;};     /*供TCP服务端SOCKET监测连接使用*/
	virtual int TcpClientConnect(int nSocketIndex, string strClientIp){return 0;};     /*供TCP服务端SOCKET监测连接使用*/

	virtual int TcpClientDisConnect(int nSocketIndex){return 0;};  /*供TCP服务端SOCKET监测连接使用*/
	virtual int TcpClientDisConnect(int nSocketIndex, string strClientIp){return 0;};  /*供TCP服务端SOCKET监测连接使用*/

	virtual int ConnectState(const int &nState){return 0;};  /*供TCP客户端SOCKET监测连接使用, nState 0: 断开 1: 连接*/
	
	virtual int UdtClientConnect(int nSocketId, string strClientIp){return 0;};     /*供UDT服务端SOCKET监测连接使用*/
	virtual int UdtClientDisConnect(int nSocketId, string strClientIp){return 0;};     /*供UDT服务端SOCKET监测连接使用*/
};

/******************************************************************************
* 类名:SocketAddress
* 描述: -socketaddr_in初始化辅助类
* 修改记录:
* --------------------
*    2014/03/29 20:57:58, gaopeng 创建函数
*
******************************************************************************/
class DLL_EXPORT SocketAddress
{
    friend class SocketBase;
public:
    SocketAddress();
    SocketAddress(std::string ip, u_short port);

    SocketAddress(const SocketAddress &rht);

    SocketAddress & operator = (const SocketAddress &rht);

    void InitAddress(std::string ip, u_short port, bool isBroadcase = false);
    void AddressCopyFrom(sockaddr_in &addr);

    bool BindWith(SOCKET socket);
    bool ConnectWith(SOCKET socket);

public:
    sockaddr_in m_tSockAddr;
    std::string m_strIp;
    u_short m_nPort;
};

/******************************************************************************
* 类名:SocketBase
* 描述: -socket基类,tcp,udp,serial重载虚函数
* 修改记录:
* --------------------
*    2014/03/29 21:0:27, gaopeng 创建函数
*
******************************************************************************/
class DLL_EXPORT SocketBase
{
    friend class JCNetworkManager;
public:
    SocketBase();
    virtual ~SocketBase();

public:
    void InitAddress(SocketInfo socketInfo);
    bool InitWsa();
    bool ClearWsa();
    virtual bool InitSocket(SocketInfo& socketInfo, ISocketListener* listener)=0;
    virtual bool ClearSocket()=0;
    virtual bool ResetSocket()=0;
    virtual int Send(const char* buf, int bufLen, int socket=0)=0;
    virtual int Recv(char* buf, int bufLen, string &source, int socket=0)=0;/*source在UDP时,返回接收到的报文发送端IP地址*/

    bool SetOption(int optName, void *optval, int optlen, int level = SOL_SOCKET);
    bool GetOption(int optname, void *optval, int *optlen,int level = SOL_SOCKET);

    bool IsValid(SOCKET socket);

    void ThrowLastError(std::string ip, SOCKET socketId);

    SOCKET_TYPE GetType();
    int GetSocketId();
	std::string GetMacAddrByIp(std::string strIp);
	std::string ResolveHostIpBaseonDestIp(std::string dest);
	bool IsIPInterfaceUp(std::string & strIp, unsigned int nLookUpCount = 5);

protected:
	void LookUpRightIp(std::string& strIpPrifex, 
						std::string& strLocalIp, 
						std::string& strMacAddr,
						unsigned int nLookUpCount = 5);

protected:
    int m_nSocket;
    SocketAddress m_tHostAddress;
    SocketAddress m_tDestAddress;
    SOCKET_TYPE m_eSockType;
	ISocketListener* m_ptSockListener;
    JCNetworkException m_tException;
};

/******************************************************************************
* 类名:JCUdpSocket
* 描述: -生成udp socket,实现udp的send,recv
* 修改记录:
* --------------------
*    2014/03/29 21:11:32, gaopeng 创建函数
*
******************************************************************************/
class DLL_EXPORT JCUdpSocket: public SocketBase
{
public:
    JCUdpSocket();
    virtual ~JCUdpSocket();

public:
    virtual int Send(const char* buf, int bufLen, int socket=0);
    virtual int Recv(char* buf, int bufLen, string &source, int socket=0);
    virtual bool InitSocket(SocketInfo& socketInfo, ISocketListener* listener);
    virtual bool ClearSocket();
    virtual bool ResetSocket();

public:
	int SendToIp(const char* pBuf, int bufLen, std::string ip, u_short port);
};

/******************************************************************************
* 类名:JCTcpSocket
* 描述: -生成tcp socket,实现tcp的send,recv
* 修改记录:
* --------------------
*    2014/03/29 21:11:32, gaopeng 创建函数
*
******************************************************************************/
class DLL_EXPORT JCTcpSocket: public SocketBase
{
public:
    JCTcpSocket();
    virtual ~JCTcpSocket();

public:
    virtual int Send(const char* buf, int bufLen, int socket=0);
    virtual int Recv(char* buf, int bufLen, string &source, int socket=0);
    virtual bool InitSocket(SocketInfo& socketInfo, ISocketListener* listener);
    virtual bool ClearSocket();
    virtual bool ResetSocket();
};


/************************************************************************/
/* JCTcpClientSocket JCSerialPort JCSerialPortSync  JCNetworkManager	*/
/* 在linux版本需要重新实现												*/
/*																		*/
/************************************************************************/

#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)

class DLL_EXPORT JCTcpClientSocket
{
#define MAX_BUF_LEN  4096
#define MAX_WAIT_OBJS 16
public:
	JCTcpClientSocket();
	virtual ~JCTcpClientSocket();

public:
	int Send(const char* buf, int bufLen);
	virtual int Recv(char* buf, int bufLen);
	bool ClearSocket() ;

	bool InitSocket(SocketInfo& socketInfo, ISocketListener* listener =NULL);
	bool Start();
	static unsigned long WINAPI RunListen(LPVOID pParam);
	bool Stop();

	void  Listen() ;

	void HandleEvent();
	
private:
	int m_nSocket;
	SocketInfo m_SocketInfo ;
	JCThread* m_ptThread;

	char  m_chBuf[MAX_BUF_LEN];
	int     m_nBufLen ; 
	int     m_nState ;

	ISocketListener *m_pListener ;
};

/******************************************************************************
* 类名:JCSerialPort
* 描述: -生成serialPort文件名柄,实现serialPort的send,recv
* 修改记录:
* --------------------
*    2014/03/29 21:11:32, gaopeng 创建函数
*
******************************************************************************/
class DLL_EXPORT JCSerialPort: public SocketBase
{
#define  DEFAULT_BUFFER_LEN 1024
public:
    JCSerialPort();
    virtual ~JCSerialPort();

public:
    virtual int Send(const char* buf, int bufLen, int socket=0);
    virtual int Recv(char* buf, int bufLen, string &source, int socket=0);
    virtual bool InitSocket(SocketInfo& socketInfo, ISocketListener* listener);
    virtual bool ClearSocket();
    virtual bool ResetSocket();

public:
    bool  UartSetSpeed(int speed);
    bool  UartSetParity(int databits, int stopbits, int parity);

public:
    int GetFileHandle();
	string GetName();

private:
    JCCriticalSection m_tCsSerialPort;
	string m_strSerialName;
};


class DLL_EXPORT JCSerialPortSync
{
#define  DEFAULT_BUFFER_LEN 1024
public:
	JCSerialPortSync();
	virtual ~JCSerialPortSync();

public:
	virtual int Send(const char* pBuf, int bufLen, int socket=0);
	virtual DWORD Recv(char* pBuf, int bufSize, int socket=0);
	virtual bool InitSerialPort(SocketInfo& socketInfo);
//	bool ReInitSerialPort(SocketInfo& socketInfo ,COMMTIMEOUTS &TimeOuts);
private:
	int m_hHandle;
	JCNetworkException m_tException;
};

/******************************************************************************
* 类名:JCNetworkManager
* 描述: -网络管理辅助类
* 修改记录:
* --------------------
*    2014/03/29 21:13:49, gaopeng 创建函数
*
******************************************************************************/
class DLL_EXPORT JCNetworkManager
{
    /* Tcp client辅助数据类 */
    class TcpClientItem
    {
    public:
        SOCKET m_nSocketId;
		string m_clientIp;
    };

/* 服务端最大监听HANDLE句柄个数 */
#define MAX_WAIT_OBJS 16

    typedef std::vector<SocketBase*> SocketBasePVer;
    typedef std::vector<TcpClientItem> SocketVer;
	typedef std::map<int, int>   IndexSocketMap;

public:
    JCNetworkManager();
    ~JCNetworkManager();

public:
	/* 初始化服务类型*/
	void InitServerType(SOCKET_TYPE type);
    /* 添加服务类到管理器中，nIndex用来指定服务类编号，目前用于指定多个UDP的序号 */
    bool AddService(SocketBase* pitem, int nIndex=0);
    /* 启动服务线程 ***启动前必须先设置服务类型，调用InitServerType*/
    bool Start();
    static unsigned long WINAPI RunServer(LPVOID pParam);
    /* 关闭服务线程 */
    void Stop();
	/* 重置manager中的参数到类的初始状态，方便网络重新初始化操作 */
	bool Reset();
	SOCKET_TYPE GetServerType();
public:
    /* Tcp专用函数 */
    bool AcceptTcpClient();
    bool RemoveTcpClient(int iIndex);
	bool RemoveTcpClientByIP(string strIp);
    void _RunTcp();
    void HandleTcpEvent(int nIndex);

public:
    /* Udp专用函数 */
    void _RunUdp();
    void HandleUdpEvent(size_t iIndex);

public:
    /* SerialPort专用函数 */
    void _RunSerial();
    void HandleSerialEvent(int iIndex);

public:
	/*socketIndex参数定义如下:
	-1:表示所有socket都发送
	不是-1的情况,判断socketIndex合法性发送.
	*/
	int Send(const char* buf, int len, int socketIndex=-1, string strTcpClientIpPort = "");

private:
	int GetMaxFD();

private:
    JCThread* m_ptThread;					/* recv 监听线程 */
    SocketBasePVer m_tSocketBasePVer;		/* socket server管理数组 */
	SOCKET_TYPE m_eServerType;				/* 管理类型，每次只能初始化一种类型 */

private:
    SocketVer m_tTcpClientSocketVer;		/* tcp client 管理数组*/
	IndexSocketMap m_tIndexSocketMap;		/* UDP socket 排序map*/
	int m_nSocketBaseCount;					/* UDP 排序内部索引 */		
	bool m_bStop;							/*控制监听线程的退出*/
};

#else /*end linux*/

class DLL_EXPORT JCTcpClientSocket
{
#define MAX_BUF_LEN  4096
#define MAX_WAIT_OBJS 16
public:
	JCTcpClientSocket();
	virtual ~JCTcpClientSocket();

public:
	int Send(const char* buf, int bufLen);
	virtual int Recv(char* buf, int bufLen);
	bool ClearSocket() ;

	bool InitSocket(SocketInfo& socketInfo, ISocketListener* listener =NULL);
	bool Start();
	static unsigned long WINAPI RunListen(LPVOID pParam);
	bool Stop();
	void SetIndex(int nIndex);

	void  Listen() ;

	void HandleEvent();
	int  GetConnectState();
	
private:
	int m_nSocket;
	SocketInfo m_SocketInfo ;
	JCThread* m_ptThread;

	char  m_chBuf[MAX_BUF_LEN];
	int     m_nBufLen ; 
	int     m_nState ;
	int		m_nIndex;

	ISocketListener *m_pListener ;

	HANDLE m_tHandleVer[MAX_WAIT_OBJS];		/* Wait event, 包括stop,rewait,socket event*/
	int m_nEventNum;						/* wait event数, reset时，应跳过stopt,rewait event*/
};

/******************************************************************************
* 类名:JCSerialPort
* 描述: -生成serialPort文件名柄,实现serialPort的send,recv
* 修改记录:
* --------------------
*    2014/03/29 21:11:32, gaopeng 创建函数
*
******************************************************************************/
class DLL_EXPORT JCSerialPort: public SocketBase
{
#define  DEFAULT_BUFFER_LEN 1024
public:
    JCSerialPort();
    virtual ~JCSerialPort();

public:
    virtual int Send(const char* buf, int bufLen, int socket=0);
    virtual int Recv(char* buf, int bufLen, string &source, int socket=0);
    virtual bool InitSocket(SocketInfo& socketInfo, ISocketListener* listener);
    virtual bool ClearSocket();
    virtual bool ResetSocket();

public:
    HANDLE GetOvEvent();
    HANDLE GetFileHandle();
	string GetName();
	void   ClearLock();
	
    OVERLAPPED m_ov;

private:
    JCCriticalSection m_tCsSerialPort;
    HANDLE m_hHandle;
	string m_strSerialName;
};


class DLL_EXPORT JCSerialPortSync
{
#define  DEFAULT_BUFFER_LEN 1024
public:
	JCSerialPortSync();
	virtual ~JCSerialPortSync();

public:
	virtual int Send(const char* pBuf, int bufLen, int socket=0);
	virtual DWORD Recv(char* pBuf, int bufSize, int socket=0);
	virtual bool InitSerialPort(SocketInfo& socketInfo);
	bool ReInitSerialPort(SocketInfo& socketInfo, COMMTIMEOUTS &TimeOuts);

private:
	void Close();
private:
	HANDLE m_hHandle;	
	JCNetworkException m_tException;
};

/******************************************************************************
* 类名:JCNetworkManager
* 描述: -网络管理辅助类
* 修改记录:
* --------------------
*    2014/03/29 21:13:49, gaopeng 创建函数
*
******************************************************************************/
class DLL_EXPORT JCNetworkManager
{
    /* Tcp client辅助数据类 */
    class TcpClientItem
    {
    public:
        SOCKET m_nSocketId;
        HANDLE m_hEvent;
		string m_clientIp;
    };

#define MAX_WAIT_OBJS 16			/* 服务端最大监听HANDLE句柄个数 */
#define SOCKET_EVENT_BEGIN_INDEX 2  /* socket事件起始序号，前面： 0：stopt  1：rewait */

    typedef std::vector<SocketBase*> SocketBasePVer;
    typedef std::vector<TcpClientItem> SocketVer;
	typedef std::map<int, int>   IndexSocketMap;
	typedef std::map<int, HANDLE>   IndexEventMap;

public:
    JCNetworkManager();
    ~JCNetworkManager();

public:
	/* 初始化服务类型*/
	void InitServerType(SOCKET_TYPE type);
    /* 添加服务类到管理器中，nIndex用来指定服务类编号，目前用于指定多个UDP的序号 */
    bool AddService(SocketBase* pitem, int nIndex=0);
	bool DelUdpService(int nIndex);
    /* 启动服务线程 ***启动前必须先设置服务类型，调用InitServerType*/
    bool Start();
    static unsigned long WINAPI RunServer(LPVOID pParam);
    /* 关闭服务线程 */
    void Stop();
	/* 重置manager中的参数到类的初始状态，方便网络重新初始化操作 */
	bool Reset();
	SOCKET_TYPE GetServerType();

	bool IsServerRunning();

	void UpdateEventToWait();

public:
    /* Tcp专用函数 */
    bool AcceptTcpClient();
    bool RemoveTcpClient(int iIndex);
	bool RemoveTcpClientByIP(string strIp);
	bool RemoveLastTcpClient();
    void _RunTcp();
    void HandleTcpEvent();

public:
    /* Udp专用函数 */
    void _RunUdp();
    void HandleUdpEvent(size_t iIndex);
	int SendTo(const char* pBuf, int bufLen, std::string ip, u_short port);

public:
    /* SerialPort专用函数 */
    void _RunSerial();
    void HandleSerialEvent(int iIndex, DWORD CommEvent);

public:
	/*socketIndex参数定义如下:
	-1:表示所有socket都发送
	不是-1的情况,判断socketIndex合法性发送.
	*/
	int Send(const char* buf, int len, int socketIndex=-1, string strTcpClientIpPort = "");

private:
    JCThread*		m_ptThread;					/* recv 监听线程 */
    SocketBasePVer	m_tSocketBasePVer;		/* socket server管理数组 */
	SOCKET_TYPE		m_eServerType;				/* 管理类型，每次只能初始化一种类型 */
    HANDLE			m_tHandleVer[MAX_WAIT_OBJS];		/* Wait event, 包括stop,rewait,socket event*/
    int				m_nEventNum;						/* wait event数, reset时，应跳过stopt,rewait event*/
	IndexEventMap	m_tServerEvent;			/* 管理server事件*/
	IndexSocketMap	m_tEventSocketMap;		/* 事件与socket的对应关系*/
private:
    SocketVer m_tTcpClientSocketVer;		/* tcp client 管理数组*/
};

#endif /*end win32*/
#endif /* _JCNETWORK_H */
