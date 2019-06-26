
#include "jcnetwork.h"
#include "jcthread.h"
#include "jcmacrotools.h"
#include "jctypetrans.h"
#include "jcdebug.h"

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

typedef unsigned char uchar;

const int UDP_RCVBUF_SIZE = 5120;

void JCNetworkException::AddMsg(const string &s)
{
	m_strWhat = s + m_strWhat;
}

SocketAddress::SocketAddress(): m_strIp("")
{
	memset(&m_tSockAddr, 0, sizeof(sockaddr_in));
}

SocketAddress::SocketAddress(std::string ip, u_short port)
{
	InitAddress(ip, port);
}

SocketAddress::SocketAddress(const SocketAddress &rht)
: m_strIp(rht.m_strIp), m_nPort(rht.m_nPort)
{
	memcpy(&m_tSockAddr, &rht.m_tSockAddr, sizeof(sockaddr_in));
}

SocketAddress& SocketAddress::operator = (const SocketAddress &rht)
{
	if(this != &rht)
	{
		m_strIp = rht.m_strIp;
		m_nPort = rht.m_nPort;
		memcpy(&m_tSockAddr, &rht.m_tSockAddr, sizeof(sockaddr_in));
	}
	return *this;
}

void SocketAddress::InitAddress(std::string ip, u_short port ,bool isBroadcase)
{
	m_strIp = ip;
	m_nPort = port;
	memset(&m_tSockAddr, 0, sizeof(sockaddr_in));
	m_tSockAddr.sin_family = AF_INET;
	m_tSockAddr.sin_port = htons(port);
	if (isBroadcase && m_strIp.empty())
		m_tSockAddr.sin_addr.s_addr = INADDR_BROADCAST;
	else
		m_tSockAddr.sin_addr.s_addr = inet_addr(ip.c_str());
}

void SocketAddress::AddressCopyFrom(sockaddr_in &addr)
{
	memcpy(&m_tSockAddr, &addr, sizeof(sockaddr_in));
	m_strIp = std::string(inet_ntoa(addr.sin_addr));
	m_nPort = ntohs(addr.sin_port);
}

bool SocketAddress::BindWith(SOCKET socket)
{
	int err = ::bind(socket, (sockaddr *)&m_tSockAddr, sizeof(m_tSockAddr));
	return (err != SOCKET_ERROR);
}
bool SocketAddress::ConnectWith(SOCKET socket)
{
	return ::connect(socket, (sockaddr *)&m_tSockAddr, sizeof(m_tSockAddr)) != SOCKET_ERROR;
}


SocketBase::SocketBase()
:m_nSocket(INVALID_SOCKET)
{
	//清空errorCode
	m_tException.CleanError();
}

void SocketBase::InitAddress(SocketInfo socketInfo)
{
	if (socketInfo.m_strSendIp != "")
	{
		m_tHostAddress.InitAddress(socketInfo.m_strSendIp, 
			socketInfo.m_nSendPort);
	}
	else
	{
		m_tHostAddress.InitAddress(ResolveHostIpBaseonDestIp(socketInfo.m_strRecvIp),
			socketInfo.m_nSendPort);
	}

	m_tDestAddress.InitAddress(socketInfo.m_strRecvIp, 
		socketInfo.m_nRecvPort,
		socketInfo.m_bIsBroadcast);
	m_eSockType = socketInfo.m_eSockType;
}

SocketBase::~SocketBase()
{
}

bool SocketBase::InitWsa()
{
#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
	return true;
#else
	WSADATA wsa;
	return (0 == WSAStartup(MAKEWORD(2,2), &wsa))?true:false;
#endif
}

bool SocketBase::ClearWsa()
{
#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
	return true;
#else
	return (0 == WSACleanup())?true:false;
#endif
}

bool SocketBase::SetOption(int optName, void *optval, int optlen, int level)
{
	return (::setsockopt(m_nSocket, level, optName, (char *)optval, optlen) != SOCKET_ERROR);
}

bool SocketBase::GetOption(int optname, void *optval, int *optlen,int level)
{
	socklen_t nSockLen = *optlen;
	return (::getsockopt(m_nSocket, level, optname, (char *)optval, &nSockLen) != SOCKET_ERROR);
}

bool SocketBase::IsValid(SOCKET socket)
{
	return m_nSocket != INVALID_SOCKET;
}

void SocketBase::ThrowLastError(std::string ip, SOCKET socketId)
{

}

SOCKET_TYPE SocketBase::GetType()
{
	return m_eSockType;
}

int SocketBase::GetSocketId()
{
	return m_nSocket;
}

std::string SocketBase::ResolveHostIpBaseonDestIp(std::string dest)
{
	std::string strIpPrifex = dest.substr(0, dest.rfind("."));

	std::string strLocalIp = "";
	std::string strMacAddr = "";
	LookUpRightIp(strIpPrifex, strLocalIp, strMacAddr);
	return strLocalIp;
}

void SocketBase::LookUpRightIp(std::string& strIpPrifex,
							   std::string& strLocalIp, 
							   std::string& strMacAddr,
							   unsigned int nLookUpCount)
{
#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
	struct ifaddrs * ifAddrStruct=NULL;
	void * tmpAddrPtr=NULL;

	getifaddrs(&ifAddrStruct);
	while (ifAddrStruct!=NULL)
	{
		std::string tmp;
		if (ifAddrStruct->ifa_addr->sa_family==AF_INET) { // check it is IP4
			// is a valid IP4 Address
			tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
			char addressBuffer[INET_ADDRSTRLEN];
			tmp = inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
		}

		if (!tmp.empty())
		{
			if (tmp.find(strIpPrifex) != std::string::npos)
			{
				strLocalIp = tmp;
				if (tmp == strIpPrifex)
				{
					int s;
					struct ifreq buffer;

					s = socket(PF_INET, SOCK_DGRAM, 0);
					memset(&buffer, 0x00, sizeof(buffer));
					strcpy(buffer.ifr_name, ifAddrStruct->ifa_name);
					ioctl(s, SIOCGIFHWADDR, &buffer);
					close(s);

					for( s = 0; s < 5; s++ )
					{
						strMacAddr += buffer.ifr_hwaddr.sa_data[s];
						strMacAddr += "-";
					}
					strMacAddr += buffer.ifr_hwaddr.sa_data[s];
				}
				break;
			}
		}
		ifAddrStruct=ifAddrStruct->ifa_next;
	}
#else

	PIP_ADAPTER_ADDRESSES AdapterList = NULL;
	PIP_ADAPTER_ADDRESSES AdapterAdress = NULL ;
	ULONG OutBufferLength = 0;
	ULONG RetVal = 0, i;    

	// The size of the buffer can be different 
	// between consecutive API calls.
	// In most cases, i < 2 is sufficient;
	// One call to get the size and one call to get the actual parameters.
	// But if one more interface is added or addresses are added, 
	// the call again fails with BUFFER_OVERFLOW. 
	// So the number is picked slightly greater than 2. 
	// We use i <5 in the example
	for (i = 0; i < nLookUpCount; i++) 
	{
		RetVal = 
			GetAdaptersAddresses(
			AF_INET, 
			0,
			NULL, 
			AdapterAdress, 
			&OutBufferLength);

		if (RetVal != ERROR_BUFFER_OVERFLOW) 
		{
			break;
		}

		if (AdapterAdress != NULL)
		{
			FREE(AdapterAdress);
		}

		AdapterAdress = (PIP_ADAPTER_ADDRESSES) 
			MALLOC(OutBufferLength);
		if (AdapterAdress == NULL) 
		{
			m_tException = JCNetworkException();
		}
	}
	AdapterList = AdapterAdress;
	while (AdapterList) 
	{
		if (AdapterList->OperStatus == IfOperStatusUp)
		{
			std::string tmp;
			if (AdapterList->FirstUnicastAddress != NULL && 
				AdapterList->FirstUnicastAddress->Address.lpSockaddr != NULL)
			{
				tmp = string(
					inet_ntoa(((sockaddr_in*)(AdapterList->FirstUnicastAddress->Address.lpSockaddr))->sin_addr));
			}

			if (!tmp.empty())
			{
				if (tmp.find(strIpPrifex) != std::string::npos)
				{
					strLocalIp = tmp;
					if (tmp == strIpPrifex)
					{
						strMacAddr = JCAssist::BufToHexString((char*)AdapterList->PhysicalAddress, AdapterList->PhysicalAddressLength);
						strMacAddr = JCAssist::ReplaceSpecChar(strMacAddr, ' ', '-');
					}
					break;
				}
			}
		}
		AdapterList = AdapterList->Next;
	}


	if (AdapterAdress != NULL)
	{
		FREE(AdapterAdress);
	}

	if (m_tException.ErrCode() != 0)
	{
		throw m_tException;
	}

#endif
}

bool SocketBase::IsIPInterfaceUp( std::string & strIp, unsigned int nLookUpCount )
{
	PIP_ADAPTER_ADDRESSES AdapterList = NULL;
	PIP_ADAPTER_ADDRESSES AdapterAdress = NULL ;
	ULONG OutBufferLength = 0;
	ULONG RetVal = 0, i; 
	bool bRet = false;

	// The size of the buffer can be different 
	// between consecutive API calls.
	// In most cases, i < 2 is sufficient;
	// One call to get the size and one call to get the actual parameters.
	// But if one more interface is added or addresses are added, 
	// the call again fails with BUFFER_OVERFLOW. 
	// So the number is picked slightly greater than 2. 
	// We use i <5 in the example
	for (i = 0; i < nLookUpCount; i++) 
	{
		RetVal = 
			GetAdaptersAddresses(
			AF_INET, 
			0,
			NULL, 
			AdapterAdress, 
			&OutBufferLength);

		if (RetVal != ERROR_BUFFER_OVERFLOW) 
		{
			break;
		}

		if (AdapterAdress != NULL)
		{
			FREE(AdapterAdress);
		}

		AdapterAdress = (PIP_ADAPTER_ADDRESSES) 
			MALLOC(OutBufferLength);
		if (AdapterAdress == NULL) 
		{
			m_tException = JCNetworkException();
		}
	}

	AdapterList = AdapterAdress; 
	while (AdapterList) 
	{
		if (AdapterList->OperStatus == IfOperStatusUp)
		{
			std::string tmp;
			if (AdapterList->FirstUnicastAddress != NULL && 
				AdapterList->FirstUnicastAddress->Address.lpSockaddr != NULL)
			{
				tmp = string(
					inet_ntoa(((sockaddr_in*)(AdapterList->FirstUnicastAddress->Address.lpSockaddr))->sin_addr));
			}

			if (!tmp.empty() && tmp == strIp)
			{
				bRet = true;
				break;
			}
			if (AdapterList->FirstUnicastAddress != NULL)
			{
				_IP_ADAPTER_UNICAST_ADDRESS_LH *next = AdapterList->FirstUnicastAddress->Next;
				while(next != NULL && 
					next->Address.lpSockaddr != NULL)
				{
					tmp = string(
						inet_ntoa(((sockaddr_in*)(next->Address.lpSockaddr))->sin_addr));
					if (!tmp.empty() && tmp == strIp)
					{
						bRet = true;
						goto end ;
					}
					next = next->Next;
				}
			}
		}
		AdapterList = AdapterList->Next;
	}

end:
	if (AdapterAdress != NULL)
	{
		FREE(AdapterAdress);
	}

	if (m_tException.ErrCode() != 0)
	{
		throw m_tException;
	}

	return bRet;
}

std::string SocketBase::GetMacAddrByIp(std::string strIp)
{
	std::string strIpPrifex = strIp;

	std::string strLocalIp = "";
	std::string strMacAddr = "";

	LookUpRightIp(strIpPrifex, strLocalIp, strMacAddr);

	return strMacAddr;
}

JCUdpSocket::JCUdpSocket()
{
	m_eSockType = UDP_SOCKET;
}

JCUdpSocket::~JCUdpSocket()
{
	if (m_nSocket != INVALID_SOCKET)
	{
		ClearSocket();
		ClearWsa();
	}
}

bool JCUdpSocket::InitSocket(SocketInfo& socketInfo, ISocketListener* listener)
{
	InitWsa();
	InitAddress(socketInfo);
	m_ptSockListener = listener;

	if(m_eSockType == UDP_SOCKET){
		m_nSocket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}
	else {
		std::string errMsg = "IP:";
		errMsg += m_tHostAddress.m_strIp;
		errMsg += " Port:";
		errMsg += JCAssist::IntToDecString(m_tHostAddress.m_nPort);
		errMsg += " SocketType: ";
		errMsg += JCAssist::IntToDecString(m_eSockType);
		errMsg += " is not UDP!";
		m_tException = JCNetworkException();
		m_tException.AddMsg(errMsg);
		throw m_tException;
	}

	if (!m_tHostAddress.BindWith(m_nSocket))
	{
		std::string errMsg = "IP:";
		errMsg += m_tHostAddress.m_strIp;
		errMsg += " Port:";
		errMsg += JCAssist::IntToDecString(m_tHostAddress.m_nPort);
		errMsg += "! ";
		m_tException = JCNetworkException();
		m_tException.AddMsg(errMsg);
		throw m_tException;
	}

	if (socketInfo.m_bIsBroadcast)
	{
		int optVal = 1;
		SetOption(SO_BROADCAST,(CHAR*)&optVal,sizeof(int));

		/* 设置缓冲区大小*/
		int nSendBufLen = 1024*1024*1;
		int nRecvBufLen = 1024*1024*1;
		int milliseconds = 500;

		SetOption(SO_SNDBUF, (CHAR* )&nSendBufLen, sizeof(int));
		SetOption(SO_RCVBUF, (CHAR* )&nRecvBufLen, sizeof(int));

		//		SetOption(SO_RCVTIMEO, (CHAR* )&milliseconds, sizeof(int));
	}

	return true;
}

bool JCUdpSocket::ClearSocket()
{
	int ret = 0;
	if(m_nSocket != INVALID_SOCKET)
	{
		shutdown(m_nSocket, 0x01);
		ret=closesocket(m_nSocket);		
		m_nSocket = INVALID_SOCKET;
	}
	return (ret==0)?true:false;
}

int JCUdpSocket::Send(const char* pBuf, int bufLen, int socketId)
{
	if (m_nSocket != INVALID_SOCKET)
	{
		int nSendCount = 0;
		int nSended = 0;
		do 
		{
			nSended = ::sendto(m_nSocket, (pBuf+nSendCount), (bufLen-nSendCount), 0, 
				(struct sockaddr *)&m_tDestAddress.m_tSockAddr, 
				sizeof(m_tDestAddress.m_tSockAddr));
			if (SOCKET_ERROR == nSended)
			{
				return SOCKET_ERROR;
			}

			nSendCount += nSended;
		} while (nSendCount < bufLen);
		return nSendCount;		
	}
	else
	{
		return 0;
	}
}

int JCUdpSocket::SendToIp(const char* pBuf, int bufLen, std::string ip, u_short port)
{
	SocketAddress tmpDest(ip, port);
	if (m_nSocket != INVALID_SOCKET)
	{
		int nSendCount = 0;
		int nSended = 0;
		do 
		{
			nSended = ::sendto(m_nSocket, (pBuf+nSendCount), (bufLen-nSendCount), 0, 
				(struct sockaddr *)&tmpDest.m_tSockAddr, 
				sizeof(tmpDest.m_tSockAddr));
			if (SOCKET_ERROR == nSended)
			{
				return SOCKET_ERROR;
			}

			nSendCount += nSended;
		} while (nSendCount < bufLen);
		return nSendCount;		
	}
	else
	{
		return 0;
	}
}


int JCUdpSocket::Recv(char* pBuf, int bufLen, string &source, int socketId)
{
	if (m_nSocket != INVALID_SOCKET)
	{
		struct sockaddr_in recvFromIP;
		socklen_t len = sizeof(recvFromIP);
		int ret = recvfrom(m_nSocket, (char*)pBuf, bufLen, 0, (struct sockaddr *)&recvFromIP, &len);
		source.assign(inet_ntoa(recvFromIP.sin_addr));
		return ret;
	}
	else
	{
		return 0;
	}
}


bool JCUdpSocket::ResetSocket()
{
	return false;
}


JCTcpSocket::JCTcpSocket()
{}

JCTcpSocket::~JCTcpSocket()
{
	if (m_nSocket != INVALID_SOCKET)
	{
		ClearSocket();
		ClearWsa();
	}
}


bool JCTcpSocket::InitSocket(SocketInfo& socketInfo, ISocketListener* listener)
{
	InitWsa();
	InitAddress(socketInfo);
	m_ptSockListener = listener;

	if(m_eSockType == TCP_SOCKET) {
		m_nSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	else {
		std::string errMsg = "IP:";
		errMsg += m_tHostAddress.m_strIp;
		errMsg += " Port:";
		errMsg += JCAssist::IntToDecString(m_tHostAddress.m_nPort);
		errMsg += " SocketType: ";
		errMsg += JCAssist::IntToDecString(m_eSockType);
		errMsg += " is not TCP!";
		m_tException = JCNetworkException();
		m_tException.AddMsg(errMsg);
		throw m_tException;
	}

#if defined (WIN32)
	int optVal = 1;
	if (!SetOption(SO_EXCLUSIVEADDRUSE,(CHAR*)&optVal,sizeof(BOOL)))
	{
		std::string errMsg = "IP:";
		errMsg += m_tHostAddress.m_strIp;
		errMsg += " Port:";
		errMsg +=  JCAssist::IntToDecString(m_tHostAddress.m_nPort);
		errMsg += "! "; 
		m_tException = JCNetworkException();
		m_tException.AddMsg(errMsg);
		throw m_tException;
	}

#endif

	if (!m_tHostAddress.BindWith(m_nSocket))
	{
		std::string errMsg = "IP:";
		errMsg += m_tHostAddress.m_strIp;
		errMsg += " Port:";
		errMsg +=  JCAssist::IntToDecString(m_tHostAddress.m_nPort);
		errMsg += "! ";
		m_tException = JCNetworkException();
		m_tException.AddMsg(errMsg);
		throw m_tException;
	}

	if (listen(m_nSocket, 10))
	{
		std::string errMsg = "IP:";
		errMsg += m_tHostAddress.m_strIp;
		errMsg += " Port:";
		errMsg += JCAssist::IntToDecString(m_tHostAddress.m_nPort);
		errMsg += '\n';
		m_tException = JCNetworkException();
		m_tException.AddMsg(errMsg);
		throw m_tException;
	}
	return true;
}

bool JCTcpSocket::ClearSocket()
{
	int ret = 0;
	if(m_nSocket != INVALID_SOCKET)
	{
		shutdown(m_nSocket, 0x01);
		ret=::closesocket(m_nSocket);		
		m_nSocket = INVALID_SOCKET;
	}
	return (ret==0)?true:false;
}

bool JCTcpSocket::ResetSocket()
{
	return false;
}

int JCTcpSocket::Send(const char* pBuf, int bufLen, int socketId)
{
	if (socketId != INVALID_SOCKET)
	{
		int nSendCount = 0;
		int nSended = 0;
		do 
		{
			nSended = ::send(socketId, (pBuf+nSendCount), (bufLen-nSendCount), 0);
			if (SOCKET_ERROR == nSended)
			{
				return SOCKET_ERROR;
			}

			nSendCount += nSended;
		} while (nSendCount < bufLen);

		return nSendCount;
	}
	else
	{
		return 0;
	}
}

int JCTcpSocket::Recv(char* pBuf, int bufLen, string &source, int socketId)
{
	if (socketId != INVALID_SOCKET)
	{
		return (::recv(socketId, (char*)pBuf, bufLen, 0));
	}
	else
	{
		return 0;
	}
}

/************************************************************************/
/* JCTcpClientSocket JCSerialPort JCSerialPortSync  JCNetworkManager	*/
/* 在linux版本需要重新实现												*/
/*																		*/
/************************************************************************/
#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)

JCTcpClientSocket::JCTcpClientSocket()
{
	m_nSocket = INVALID_SOCKET ;
	m_ptThread = NULL ;
	m_pListener = NULL ;
}

JCTcpClientSocket::~JCTcpClientSocket()
{
	Stop();
}

int JCTcpClientSocket::Send(const char* buf, int bufLen)
{
	if (m_nSocket != INVALID_SOCKET)
	{
		return ::send(m_nSocket,buf,bufLen,0);
	}
	return 0 ;
}

int JCTcpClientSocket::Recv(char* buf, int bufLen)
{
	if(m_pListener)
	{
		m_pListener->RecvCallBack(buf, bufLen);
	}
	return 0 ;
}

bool JCTcpClientSocket::ClearSocket() 
{
	if(m_nSocket != INVALID_SOCKET)
	{
		closesocket(m_nSocket);
		m_nSocket = INVALID_SOCKET ;
	}
	return true ;
}

bool JCTcpClientSocket::InitSocket(SocketInfo& socketInfo, ISocketListener* listener)
{

	m_SocketInfo = socketInfo;

	m_pListener = listener ;

	m_nState = 0 ;

	int err;

	m_nSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(INVALID_SOCKET == m_nSocket)
	{
		JCNetworkException tException = JCNetworkException();
		std::string errMsg = "IP:";
		errMsg += m_SocketInfo.m_strSendIp;
		errMsg += " Port:";
		errMsg +=  JCAssist::IntToDecString(m_SocketInfo.m_nSendPort);
		errMsg += "! ";
		tException.AddMsg(errMsg);
		throw tException;
		return false ;
	}

	sockaddr_in clientsock_in;
	clientsock_in.sin_addr.s_addr = inet_addr(m_SocketInfo.m_strSendIp.c_str());
	clientsock_in.sin_family=AF_INET;
	clientsock_in.sin_port=htons(m_SocketInfo.m_nSendPort);
	err = connect(m_nSocket,(struct sockaddr*)&clientsock_in,sizeof(struct sockaddr));

	if(SOCKET_ERROR == err)
	{
		JCNetworkException tException = JCNetworkException();
		std::string errMsg = "IP:";
		errMsg += m_SocketInfo.m_strSendIp;
		errMsg += " Port:";
		errMsg +=  JCAssist::IntToDecString(m_SocketInfo.m_nSendPort);
		errMsg += "! ";
		tException.AddMsg(errMsg);
		throw tException;
		return false ;
	}

	m_nState = 1; 
	return true ;
}

bool JCTcpClientSocket::Start()
{	
	VALRET_ASSERT(m_ptThread == NULL, false);

	m_ptThread = new JCThread((THREAD_ENTRY)RunListen,this);

	if (m_ptThread == NULL)
	{
		return false;
	}

	m_ptThread->Resume();

	return true ;
}

unsigned long WINAPI JCTcpClientSocket::RunListen(LPVOID pParam)
{
	JCTcpClientSocket *pTcpClient = (JCTcpClientSocket *)pParam ;

	pTcpClient->Listen();

	return 0 ;
}

bool JCTcpClientSocket::Stop()
{	
	if (NULL != m_ptThread)
	{
		m_ptThread->Exit(0);
		while(m_ptThread->IsRunning())
		{
			;
		}

		FREE_POINT(m_ptThread);

		ClearSocket();
	}

	return true ;
}

void  JCTcpClientSocket::Listen() 
{
	bool bWorkStop = false;

	while(!bWorkStop)
	{
		fd_set readFd;
		FD_ZERO(&readFd);
		FD_SET(m_nSocket, &readFd);

		pthread_testcancel();   
		int ret = select(m_nSocket+1, &readFd, NULL, NULL, NULL);
		pthread_testcancel();   

		if(ret < 0)
		{
			//throw JCNetworkException();
		}
		else if(ret == 0)
		{
			//time out
		}
		else
		{
			HandleEvent();
		}

	}// end while
}

void  JCTcpClientSocket::HandleEvent()
{
	if (m_nSocket != INVALID_SOCKET)
	{
		m_nBufLen = recv(m_nSocket,m_chBuf,MAX_BUF_LEN,0);

		if(m_nBufLen != SOCKET_ERROR && m_nBufLen!=0)
		{	
			Recv(m_chBuf,m_nBufLen);	
		}
		else
		{
			if(m_nState ==1)
			{
				m_nState = 0;
				if(m_pListener)
				{
					m_pListener->ConnectState(m_nState) ;
				}
			}
		}
	}
}

JCSerialPort::JCSerialPort()
{
	m_nSocket = NULL;
}

JCSerialPort::~JCSerialPort()
{
	if (m_nSocket != NULL)
	{
		close(m_nSocket);
		m_nSocket = NULL;
	}
}


bool JCSerialPort::InitSocket(SocketInfo& socketInfo, ISocketListener* listener)
{
	m_eSockType = socketInfo.m_eSockType;
	m_ptSockListener = listener;

	m_strSerialName = socketInfo.m_tSerialPortCfg.m_strSerialPort;
	m_nSocket = open(m_strSerialName.c_str(), O_RDWR | O_NDELAY | O_NOCTTY);

	if(m_nSocket==INVALID_SOCKET)
	{  
		m_tException = JCNetworkException();
		close(m_nSocket);

		std::string errMsg = socketInfo.m_tSerialPortCfg.m_strSerialPort + " Init Error! ";
		m_tException.AddMsg(errMsg);
		throw m_tException;
	}

	return UartSetSpeed(socketInfo.m_tSerialPortCfg.m_nBaudRate )
		&& UartSetParity(socketInfo.m_tSerialPortCfg.m_nByteSize,
		socketInfo.m_tSerialPortCfg.m_nStopBits,
		socketInfo.m_tSerialPortCfg.m_nParity );
}

bool  JCSerialPort::UartSetSpeed(int speed)
{
	int status;
	struct termios   Opt;

	tcgetattr(m_nSocket, &Opt);

	for (int i = 0; i < sizeof (speed_arr) / sizeof (int); i++)
	{
		if (speed == name_arr[i])
		{
			tcflush (m_nSocket, TCIOFLUSH);
			cfsetispeed (&Opt, speed_arr[i]);
			cfsetospeed (&Opt, speed_arr[i]);
			status = tcsetattr (m_nSocket, TCSANOW, &Opt);
			if (status != 0)
			{
				perror ("tcsetattr fd1");
				return false;
			}
			tcflush (m_nSocket, TCIOFLUSH);
		}
	}
	return true;
}

bool JCSerialPort::UartSetParity(int databits, int stopbits, int parity) {
	struct termios options;

	if (tcgetattr(m_nSocket, &options) != 0) {
		perror("SetupSerial 1");
		return false;
	}

	options.c_cflag &= ~CSIZE;
	switch (databits) /*设置数据位数*/
	{
	case 5:
		options.c_cflag |= CS5;
		break;
	case 6:
		options.c_cflag |= CS6;
		break;
	case 7:
		options.c_cflag |= CS7;
		break;
	case 8:
		options.c_cflag |= CS8;
		break;
	default:
		fprintf(stderr, "Unsupported data size/n");
		return (false);
	}

	switch (parity) {
	case 0:
		options.c_cflag &= ~PARENB; /* Clear parity enable */
		options.c_iflag &= ~INPCK; /* Enable parity checking */
		break;
	case 1:
		options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
		options.c_iflag |= INPCK; /* Disnable parity checking */
		break;
	case 2:
		options.c_cflag |= PARENB; /* Enable parity */
		options.c_cflag &= ~PARODD; /* 转换为偶效验*/
		options.c_iflag |= INPCK; /* Disnable parity checking */
		break;
	case 3:
	case 4: /*as no parity*/
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;
	default:
		fprintf(stderr, "Unsupported parity/n");
		return false;
	}

	/* 设置停止位*/
	switch (stopbits) {
	case 0:
	case 1:
		options.c_cflag &= ~CSTOPB;
		break;
	case 2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		fprintf(stderr, "Unsupported stop bits/n");
		return false;
	}

	/* Set input parity option */

	if (parity != 0)
		options.c_iflag |= INPCK;

	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG | INLCR | ICRNL | IEXTEN);	//   Choosing   Raw   Input
	//options.c_oflag   &=~(OCRNL|ONLCR|ONLRET);

	options.c_cflag |= (CLOCAL | CREAD);

	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	options.c_oflag &= ~OPOST;
	options.c_oflag &= ~(ONLCR | OCRNL);	//ìí?óμ?

	options.c_iflag &= ~(ICRNL | INLCR);
	options.c_iflag &= ~(IXON | IXOFF | IXANY);	//ìí?óμ?

	tcflush (m_nSocket, TCIOFLUSH);
	options.c_cflag &= ~CRTSCTS;

	options.c_cc[VTIME] = 1;	/* éè??3?ê±15 seconds */
	options.c_cc[VMIN] = 1;		/* Update the options and do it NOW */


	if (tcsetattr(m_nSocket, TCSANOW, &options) != 0) {
		perror("SetupSerial 3");
		return false;
	}

	return true;
}

bool JCSerialPort::ClearSocket()
{
	if(m_nSocket != INVALID_SOCKET)
	{  
		close(m_nSocket);
		return true; 
	}

	return false;
}

bool JCSerialPort::ResetSocket()
{
	return false;
}

string JCSerialPort::GetName()
{
	return m_strSerialName;
}

int JCSerialPort::Send(const char* pBuf, int bufLen, int socketId)
{
	int nSend = 0;
	m_tCsSerialPort.Lock();

	if (m_nSocket >= 0)
	{
		nSend = write(m_nSocket, pBuf, bufLen);
		if (nSend != bufLen)
		{
			tcflush(m_nSocket, TCOFLUSH);
		}
	}

	m_tCsSerialPort.Unlock();

	return 0;
}

int JCSerialPort::Recv(char* pBuf, int bufLen, string &source, int socketId)
{
	return read(m_nSocket, pBuf, bufLen);
}

JCSerialPortSync::JCSerialPortSync()
{
}

JCSerialPortSync::~JCSerialPortSync()
{
	close(m_hHandle);
}

int JCSerialPortSync::Send(const char* pBuf, int bufLen, int socket)
{
	DWORD iByteSend = 0;

	return iByteSend;
}

DWORD JCSerialPortSync::Recv(char* pBuf, int nBufSize, int socket)
{
	DWORD wBufLen = 0 ;
	return wBufLen ;
}

bool JCSerialPortSync::InitSerialPort(SocketInfo& socketInfo)
{
	return true;
}
/*
bool JCSerialPortSync::ReInitSerialPort(SocketInfo& socketInfo ,COMMTIMEOUTS &TimeOuts)
{
return true;
}
*/

JCNetworkManager::JCNetworkManager()
{
	m_ptThread = NULL;
	m_eServerType = INVALID_SERVER_TYPE;
	m_nSocketBaseCount = 0;
	m_bStop = false;
}
JCNetworkManager::~JCNetworkManager()
{
	Stop();

	SocketBasePVer::iterator it = m_tSocketBasePVer.begin();
	for (;it != m_tSocketBasePVer.end(); ++it)
	{
		if (*(it))
		{
			delete *it;
		}
	}

	SocketVer::iterator itClient = m_tTcpClientSocketVer.begin();
	for (; itClient != m_tTcpClientSocketVer.end(); ++itClient)
	{
		if (itClient->m_nSocketId != INVALID_SOCKET)
		{
			closesocket(itClient->m_nSocketId);
			itClient->m_nSocketId = INVALID_SOCKET;
		}
	}
}

void JCNetworkManager::InitServerType(SOCKET_TYPE type)
{
	m_eServerType = type;
}


bool JCNetworkManager::AddService(SocketBase* pItem, int nIndex)
{
	if (pItem != NULL)
	{
		m_tSocketBasePVer.push_back(pItem);
		m_tIndexSocketMap.insert(map<int, int>::value_type(m_nSocketBaseCount, nIndex));
		++m_nSocketBaseCount;

		int nError = 0;
		m_eServerType = pItem->GetType();
		if (m_eServerType == TCP_SOCKET)
		{
		}
		else if (m_eServerType == UDP_SOCKET)
		{
		}
		else if (m_eServerType == SERIAL_PORT)
		{
		}
		else
		{}

		if (nError == SOCKET_ERROR)
		{
			throw JCNetworkException();
		}

	}

	return true;
}

bool JCNetworkManager::AcceptTcpClient()
{
	sockaddr_in saRemote;
	socklen_t RemoteLen;
	SOCKET acceptSocket;

	RemoteLen = sizeof(saRemote);
	acceptSocket = accept(m_tSocketBasePVer[0]->GetSocketId(), (sockaddr*)&saRemote, &RemoteLen);

	if (SOCKET_ERROR== acceptSocket)  
	{  
		JCNetworkException tException = JCNetworkException();
		std::string errMsg = "accept failed";
		tException.AddMsg(errMsg);
		throw tException;
		return false;  
	}  

	TcpClientItem tmp;
	tmp.m_nSocketId = acceptSocket;
	tmp.m_clientIp.assign(inet_ntoa(saRemote.sin_addr));
	tmp.m_clientIp += ":";
	tmp.m_clientIp += JCAssist::U32ToDecString(ntohs(saRemote.sin_port));
	m_tTcpClientSocketVer.push_back(tmp);

	return true;
}

bool JCNetworkManager::RemoveTcpClient(int iIndex)
{
	if (iIndex < 0 || iIndex > m_tTcpClientSocketVer.size())
		return false;

	SOCKET tmp = m_tTcpClientSocketVer[iIndex].m_nSocketId;
	closesocket(tmp);

	SocketVer::iterator it = m_tTcpClientSocketVer.begin();
	for (; it != m_tTcpClientSocketVer.end(); ++it)
	{
		if (it->m_nSocketId == tmp)
		{
			m_tTcpClientSocketVer.erase(it);
			break;
		}
	}
	return true;
}

bool JCNetworkManager::RemoveTcpClientByIP(string strIp)
{
	int nIndex = -1;
	for (size_t i = 0; i < m_tTcpClientSocketVer.size(); ++i)
	{
		if (m_tTcpClientSocketVer[i].m_clientIp == strIp)
		{
			nIndex = i;
			break;
		}
	}

	if(RemoveTcpClient(nIndex))
	{
	}
	return false;
}

bool JCNetworkManager::Start()
{
	VALRET_ASSERT(m_eServerType != INVALID_SERVER_TYPE, false);
	VALRET_ASSERT(!m_tSocketBasePVer.empty(), false);
	VALRET_ASSERT(m_ptThread == NULL, false);

	m_bStop = false;
	m_ptThread = new JCThread((THREAD_ENTRY)RunServer,this);

	//提高网络线程优先级
	//	bool ret = m_ptThread->SetPriority(THREAD_PRIORITY_TIME_CRITICAL);
	if (m_ptThread == NULL)
	{
		return false;
	}
	m_ptThread->Resume();

	return true;
}

void JCNetworkManager::Stop()
{
	VOIDRET_ASSERT(NULL != m_ptThread);
	m_bStop = true;
	m_ptThread->Exit(0);
	while(m_ptThread->IsRunning())
	{
		;
	}

	FREE_POINT(m_ptThread);
}

bool JCNetworkManager::Reset()
{
	//stop监听线程
	Stop();

	//清空管理socketbase指针
	SocketBasePVer::iterator itBase = m_tSocketBasePVer.begin();
	for (; itBase!=m_tSocketBasePVer.end(); ++itBase)
	{
		if ((*itBase) != NULL)
		{
			delete (*itBase);
			*itBase = NULL;
		}
	}
	m_tSocketBasePVer.clear();

	//清空UDP索引
	m_tIndexSocketMap.clear();
	m_nSocketBaseCount = 0;

	//关闭，清空tcp client连接
	for (size_t i=0; i<m_tTcpClientSocketVer.size(); ++i)
	{
		if (m_tTcpClientSocketVer[i].m_nSocketId != INVALID_SOCKET)
		{
			closesocket(m_tTcpClientSocketVer[i].m_nSocketId);
			m_tTcpClientSocketVer[i].m_nSocketId = INVALID_SOCKET;
		}
	}
	m_tTcpClientSocketVer.clear();

	return true;
}

SOCKET_TYPE JCNetworkManager::GetServerType()
{
	return m_eServerType;
}

unsigned long WINAPI JCNetworkManager::RunServer(LPVOID pParam)
{
	JCNetworkManager* pSock = static_cast<JCNetworkManager*>(pParam);
	SOCKET_TYPE eType = pSock->GetServerType();
	if (eType == UDP_SOCKET)
		pSock->_RunUdp();
	else if (eType == TCP_SOCKET)
		pSock->_RunTcp();
	else if (eType == SERIAL_PORT)
		pSock->_RunSerial();

	return 0;
}

int JCNetworkManager::GetMaxFD()
{
	int nMaxfd = 0;

	for(int i=0; i < m_tSocketBasePVer.size(); ++i)
	{
		nMaxfd =   (nMaxfd <	m_tSocketBasePVer[0]->GetSocketId()) ? m_tSocketBasePVer[0]->GetSocketId() : nMaxfd;
	}

	if (m_eServerType == TCP_SOCKET)
	{
		for(int i=0; i < m_tTcpClientSocketVer.size(); ++i)
		{
			nMaxfd = (nMaxfd < m_tTcpClientSocketVer[i].m_nSocketId) ? m_tTcpClientSocketVer[i].m_nSocketId : nMaxfd;
		}
	}

	return nMaxfd;
}

void JCNetworkManager::_RunTcp()
{
	if (m_eServerType != TCP_SOCKET)
		return;

	if (m_tSocketBasePVer.empty())
		return;

	while (!m_bStop)
	{
		fd_set readFd;
		FD_ZERO(&readFd);
		FD_SET(m_tSocketBasePVer[0]->GetSocketId(), &readFd);
		for (int i=0; i<m_tTcpClientSocketVer.size(); ++i)
		{
			FD_SET(m_tTcpClientSocketVer[i].m_nSocketId, &readFd);
		}

		pthread_testcancel();   
		int ret = select(GetMaxFD()+1, &readFd, NULL, NULL, NULL);
		pthread_testcancel();   

		if(ret < 0)
		{
			//throw JCNetworkException();
		}
		else if(ret == 0)
		{
			//time out
		}
		else
		{
			if(FD_ISSET(m_tSocketBasePVer[0]->GetSocketId() , &readFd))
			{
				if (!AcceptTcpClient())
					return;

				if (!m_tTcpClientSocketVer.empty())
				{
					string clientIp = m_tTcpClientSocketVer.back().m_clientIp;
					(m_tSocketBasePVer.at(0))->m_ptSockListener->TcpClientConnect(m_tTcpClientSocketVer.size());
					(m_tSocketBasePVer.at(0))->m_ptSockListener->TcpClientConnect(m_tTcpClientSocketVer.size(), clientIp);
				}
			}

			for(int i = 0; i < m_tTcpClientSocketVer.size(); i++)
			{
				if(FD_ISSET(m_tTcpClientSocketVer[i].m_nSocketId, &readFd))
				{
					HandleTcpEvent(i);
				}
			}
		}

	} // end while
}

void JCNetworkManager::HandleTcpEvent(int iIndex)
{
	char buf[1024];
	int recv =0;
	string source;

	SOCKET hSocket   = m_tTcpClientSocketVer[iIndex].m_nSocketId;
	string clientIp = m_tTcpClientSocketVer[iIndex].m_clientIp;

	memset(buf, 0, 1024);
	recv = (m_tSocketBasePVer.at(0))->Recv(buf, DEFAULT_BUFFER_LEN, source, hSocket);
	if (recv > 0)
	{
		(m_tSocketBasePVer.at(0))->m_ptSockListener->RecvCallBack(buf, recv, iIndex, clientIp);
	}
	else if (recv <= 0)
	{
		string clientIp = m_tTcpClientSocketVer[iIndex].m_clientIp;
		RemoveTcpClient(iIndex);
		(m_tSocketBasePVer.at(0))->m_ptSockListener->TcpClientDisConnect(iIndex);
		(m_tSocketBasePVer.at(0))->m_ptSockListener->TcpClientDisConnect(iIndex, clientIp);
	}
}

void JCNetworkManager::_RunUdp()
{
	if (m_eServerType != UDP_SOCKET)
		return;

	if (m_tSocketBasePVer.empty())
		return;

	while (!m_bStop)
	{
		fd_set readFd;
		FD_ZERO(&readFd);
		for (int i=0; i<m_tSocketBasePVer.size(); ++i)
		{
			FD_SET(m_tSocketBasePVer[i]->GetSocketId(), &readFd);
		}

		pthread_testcancel();   
		int ret = select(GetMaxFD()+1, &readFd, NULL, NULL, NULL);
		pthread_testcancel();   

		if(ret < 0)
		{
			//throw JCNetworkException();
		}
		else if(ret == 0)
		{
			//time out
		}
		else
		{
			for(int i = 0; i < m_tSocketBasePVer.size(); i++)
			{
				if(FD_ISSET(m_tSocketBasePVer[i]->GetSocketId(), &readFd))
				{
					HandleUdpEvent(i);
				}
			}
		}

	}// end while
}

void JCNetworkManager::HandleUdpEvent(size_t iIndex)
{
	char buf[UDP_RCVBUF_SIZE];
	string source ;

	memset(buf, 0, UDP_RCVBUF_SIZE);
	int recv = (m_tSocketBasePVer.at(iIndex))->Recv(buf, UDP_RCVBUF_SIZE, source);
	if (recv > 0)
	{
		(m_tSocketBasePVer.at(iIndex))->
			m_ptSockListener->RecvCallBack(buf, recv, m_tIndexSocketMap.find(iIndex)->second, source);
	}
}

void JCNetworkManager::_RunSerial()
{
	if (m_eServerType != SERIAL_PORT)
		return;

	if (m_tSocketBasePVer.empty())
		return;

	while (!m_bStop)
	{
		fd_set readFd;
		FD_ZERO(&readFd);
		for (int i=0; i<m_tSocketBasePVer.size(); ++i)
		{
			FD_SET(m_tSocketBasePVer[i]->GetSocketId(), &readFd);
		}

		pthread_testcancel();
		int ret = select(GetMaxFD()+1, &readFd, NULL, NULL, NULL);
		pthread_testcancel();

		if(ret < 0)
		{
			//throw JCNetworkException();
		}
		else if(ret == 0)
		{
			//time out
		}
		else
		{
			for(int i = 0; i < m_tSocketBasePVer.size(); i++)
			{
				if(FD_ISSET(m_tSocketBasePVer[i]->GetSocketId(), &readFd))
				{
					HandleSerialEvent(i);
				}
			}
		}

	}// end while
}

void JCNetworkManager::HandleSerialEvent(int iIndex)
{
	char buf[UDP_RCVBUF_SIZE];
	string source = ((JCSerialPort*)(m_tSocketBasePVer.at(iIndex)))->GetName();

	memset(buf, 0, UDP_RCVBUF_SIZE);
	int recv = (m_tSocketBasePVer.at(iIndex))->Recv(buf, UDP_RCVBUF_SIZE, source);
	if (recv > 0)
	{
		(m_tSocketBasePVer.at(iIndex))->
			m_ptSockListener->RecvCallBack(buf, recv, m_tIndexSocketMap.find(iIndex)->second, source);
	}
}

int JCNetworkManager::Send(const char* buf, int len, int socketIndex, string strTcpClientIpPort)
{
	SocketVer tTcpClientSocketVer = m_tTcpClientSocketVer;

	int ret = 0;
	if (m_eServerType == TCP_SOCKET)
	{
		if (socketIndex == -1)
		{//所有客户端发送
			SocketVer::iterator it = tTcpClientSocketVer.begin();
			for (; it!= tTcpClientSocketVer.end(); ++it)
			{
				if (it->m_nSocketId != m_tSocketBasePVer[0]->GetSocketId())
					ret = m_tSocketBasePVer[0]->Send(buf, len, it->m_nSocketId);
			}
		}
		else
		{
			ret = -1;
			SocketVer::iterator it = tTcpClientSocketVer.begin();
			for (; it!= tTcpClientSocketVer.end(); ++it)
			{
				if (it->m_clientIp == strTcpClientIpPort)
					ret = m_tSocketBasePVer[0]->Send(buf, len, it->m_nSocketId);
			}
		}
	}
	else if (m_eServerType == UDP_SOCKET || m_eServerType == SERIAL_PORT)
	{
		//全抽发送
		if (socketIndex == -1)
		{
			SocketBasePVer::iterator it = m_tSocketBasePVer.begin();
			for (; it!=m_tSocketBasePVer.end(); ++it)
			{
				ret = (*it)->Send(buf, len);
			}
		}
		else
		{
			if ((size_t)socketIndex >= m_tSocketBasePVer.size())
			{
				return -1;
			}
			ret = m_tSocketBasePVer[socketIndex]->Send(buf, len);
		}
	}
	else
	{
		return -1;
	}

	if (ret < 0)
		return errno;

	return 0;
}

#else /*end linux*/

JCTcpClientSocket::JCTcpClientSocket()
{
	m_nSocket = INVALID_SOCKET ;
	m_ptThread = NULL ;
	m_pListener = NULL ;
	m_nEventNum = 0;
	m_nIndex = 0;
}

JCTcpClientSocket::~JCTcpClientSocket()
{
	Stop();
}

int JCTcpClientSocket::Send(const char* pBuf, int bufLen)
{
	if (m_nSocket != INVALID_SOCKET)
	{
		int nSendCount = 0;
		int nSended = 0;
		do 
		{
			nSended = ::send(m_nSocket, (pBuf+nSendCount), (bufLen-nSendCount), 0);
			if (SOCKET_ERROR == nSended)
			{
				return SOCKET_ERROR;
			}

			nSendCount += nSended;
		} while (nSendCount < bufLen);

		return nSendCount;
	}
	return 0 ;
}

int JCTcpClientSocket::Recv(char* buf, int bufLen)
{
	if(m_pListener)
	{
		m_pListener->RecvCallBack(buf, bufLen, m_nIndex);
	}
	return 0 ;
}

bool JCTcpClientSocket::ClearSocket() 
{
	if(m_nSocket != INVALID_SOCKET)
	{
		closesocket(m_nSocket);
		m_nSocket = INVALID_SOCKET ;
	}
	WSACleanup();
	return true ;
}

bool JCTcpClientSocket::InitSocket(SocketInfo& socketInfo, ISocketListener* listener)
{
	for (int i=0; i < MAX_WAIT_OBJS; ++i)
	{
		m_tHandleVer[i] = 0;
	}

	m_nEventNum = 0;

	HANDLE stopEvent = ::CreateEventA( 0, TRUE, TRUE, NULL);
	m_tHandleVer[0] = stopEvent; 
	++m_nEventNum;

	m_SocketInfo = socketInfo;

	m_pListener = listener ;

	m_nState = 0 ;

	int err;

	WORD versionRequired;

	WSADATA wsaData;

	versionRequired=MAKEWORD(1,1);
	err=WSAStartup(versionRequired,&wsaData);//协议库的版本信息

	if(err)
	{
		std::string errMsg = " Require version error! ";
		JCNetworkException tException = JCNetworkException();
		tException.AddMsg(errMsg);
		throw tException;
		return false ;
	}

	m_nSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(INVALID_SOCKET == m_nSocket)
	{
		JCNetworkException tException = JCNetworkException();
		std::string errMsg = "IP:";
		errMsg += m_SocketInfo.m_strSendIp;
		errMsg += " Port:";
		errMsg +=  JCAssist::IntToDecString(m_SocketInfo.m_nSendPort);
		errMsg += "! ";
		tException.AddMsg(errMsg);
		throw tException;
		return false ;
	}

	/* 设置 KEEPLIVE */
	//	BOOL bKeepAlive = TRUE;  
	//	err = setsockopt(m_nSocket, SOL_SOCKET, SO_KEEPALIVE,   (char*)&bKeepAlive, sizeof(bKeepAlive));  

	//	if (SOCKET_ERROR== err)  
	//	{  
	//		JCNetworkException tException = JCNetworkException();
	//		std::string errMsg = "set keepalive";
	//		tException.AddMsg(errMsg);
	//		throw tException;
	//		return false;  
	//	}  

	//	unsigned int timeout = 1;  // 10s
	//	err = setsockopt(m_nSocket, IPPROTO_TCP, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

	//	if (SOCKET_ERROR== err)  
	//	{  
	//		JCNetworkException tException = JCNetworkException();
	//		std::string errMsg = "set keepalive";
	//		tException.AddMsg(errMsg);
	//		throw tException;
	//		return false;  
	//	}  

	//	tcp_keepalive alive_in;  
	//	tcp_keepalive alive_out;  
	//	alive_in.keepalivetime    = 500;  // 0.5s  
	//	alive_in.keepaliveinterval  = 1000; //1s  
	//	alive_in.onoff                       = TRUE;  
	//	unsigned long ulBytesReturn = 0;  
	//	err = WSAIoctl(m_nSocket, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in),  
	//	                  &alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL);  
	//	if (SOCKET_ERROR== err)  
	//	{  
	//		JCNetworkException tException = JCNetworkException();
	//		std::string errMsg = "WSAIoctl failed";
	//		tException.AddMsg(errMsg);
	//		throw tException;
	//		return false;  
	//	}  

	sockaddr_in clientsock_in;
	clientsock_in.sin_addr.s_addr = inet_addr(m_SocketInfo.m_strSendIp.c_str());
	clientsock_in.sin_family=AF_INET;
	clientsock_in.sin_port=htons(m_SocketInfo.m_nSendPort);
	err = connect(m_nSocket,(struct sockaddr*)&clientsock_in,sizeof(struct sockaddr));

	if(SOCKET_ERROR == err)
	{
		JCNetworkException tException = JCNetworkException();
		std::string errMsg = "IP:";
		errMsg += m_SocketInfo.m_strSendIp;
		errMsg += " Port:";
		errMsg +=  JCAssist::IntToDecString(m_SocketInfo.m_nSendPort);
		errMsg += "! ";
		tException.AddMsg(errMsg);
		throw tException;
		return false ;
	}

	m_nState = 1; 

	HANDLE tcpEvent = ::CreateEventA( 0, false, TRUE, NULL);
	::ResetEvent(tcpEvent);

	int nError = ::WSAEventSelect(m_nSocket, tcpEvent, FD_READ|FD_CLOSE);

	if (nError == SOCKET_ERROR)
	{
		JCNetworkException m_tException = JCNetworkException();
		throw m_tException;
	}

	m_tHandleVer[m_nEventNum] = tcpEvent; 
	++m_nEventNum;

	return true ;
}

bool JCTcpClientSocket::Start()
{	
	VALRET_ASSERT(m_ptThread == NULL, false);

	::ResetEvent(m_tHandleVer[0]);

	m_ptThread = new JCThread((THREAD_ENTRY)RunListen,this);

	if (m_ptThread == NULL)
	{
		return false;
	}

	m_ptThread->Resume();

	return true ;
}

unsigned long WINAPI JCTcpClientSocket::RunListen(LPVOID pParam)
{
	JCTcpClientSocket *pTcpClient = (JCTcpClientSocket *)pParam ;

	pTcpClient->Listen();

	return 0 ;
}

bool JCTcpClientSocket::Stop()
{	
	if (NULL != m_ptThread)
	{
		::SetEvent(m_tHandleVer[0]);

		while(m_ptThread->IsRunning())
		{
			Sleep(1);
		}		
	}

	FREE_POINT(m_ptThread);

	ClearSocket();

	for(int i = 0 ; i <m_nEventNum;i++)
	{
		CloseHandle(m_tHandleVer[i]);
	}

	m_nEventNum = 0 ; 

	return true ;
}

void  JCTcpClientSocket::Listen() 
{
	BOOL bWorkStop = FALSE;

	for (int i=0; i<MAX_WAIT_OBJS; ++i)
	{
		::ResetEvent(m_tHandleVer[i]);
	}

	while(!bWorkStop)
	{
		DWORD dwWait = WaitForMultipleObjects(m_nEventNum, m_tHandleVer, FALSE, INFINITE);
		int nSignaledHandle = dwWait - WAIT_OBJECT_0;
		if (nSignaledHandle == 0) //It was the stop request
			bWorkStop = TRUE;
		else if (nSignaledHandle ==1 )//It was a socket event
		{
			HandleEvent();
		}
		else
		{
			;
		}
	}
}

void  JCTcpClientSocket::HandleEvent()
{
	if (m_nSocket != INVALID_SOCKET)
	{
		memset(m_chBuf, 0, MAX_BUF_LEN);
		m_nBufLen = recv(m_nSocket,m_chBuf,MAX_BUF_LEN,0);

		if(m_nBufLen != SOCKET_ERROR&&m_nBufLen!=0)
		{	
			Recv(m_chBuf,m_nBufLen);	
		}
		else
		{
			if(m_nState ==1)
			{
				m_nState = 0;
				if(m_pListener)
				{
					m_pListener->ConnectState(m_nState) ;
				}
			}
		}
	}
}

int JCTcpClientSocket::GetConnectState()
{
	return m_nState;
}

void JCTcpClientSocket::SetIndex( int nIndex )
{
	m_nIndex = nIndex;
}

JCSerialPort::JCSerialPort()
{
	m_hHandle = NULL;
	m_eSockType = SERIAL_PORT;
	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;
	m_ov.hEvent = NULL;
}

JCSerialPort::~JCSerialPort()
{
	if (m_hHandle != NULL)
	{
		::CloseHandle(m_hHandle);
		m_hHandle = NULL;
	}
}


bool JCSerialPort::InitSocket(SocketInfo& socketInfo, ISocketListener* listener)
{

	if (socketInfo.m_tSerialPortCfg.m_strSerialPort.find("\\\\.\\") == string::npos)
	{
		socketInfo.m_tSerialPortCfg.m_strSerialPort = "\\\\.\\" + socketInfo.m_tSerialPortCfg.m_strSerialPort;
	}

	m_eSockType = socketInfo.m_eSockType;
	m_ptSockListener = listener;

	m_hHandle = ::CreateFile(socketInfo.m_tSerialPortCfg.m_strSerialPort.c_str(),//COM口
		GENERIC_READ|GENERIC_WRITE, //允许读和写  
		0, //独占方式   
		NULL, 
		OPEN_EXISTING, //打开而不是创建 
		FILE_FLAG_OVERLAPPED, //异步方式 
		NULL); 

	m_strSerialName = socketInfo.m_tSerialPortCfg.m_strSerialPort;
	if(m_hHandle==INVALID_HANDLE_VALUE) 
	{  
		m_tException = JCNetworkException();
		::CloseHandle(m_hHandle);
		m_hHandle = NULL;

		std::string errMsg = socketInfo.m_tSerialPortCfg.m_strSerialPort + " Init Error! ";
		m_tException.AddMsg(errMsg);
		throw m_tException;
	}

	if (m_ov.hEvent != NULL)
		ResetEvent(m_ov.hEvent);
	else
		m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL); 

	if (!::SetupComm(m_hHandle,DEFAULT_BUFFER_LEN,DEFAULT_BUFFER_LEN)) //输入缓冲区和输出缓冲区的大小都是1024 
	{
		m_tException = JCNetworkException();
		::CloseHandle(m_hHandle);
		m_hHandle = NULL;

		std::string errMsg = socketInfo.m_tSerialPortCfg.m_strSerialPort + " SetupComm Init Error! ";
		m_tException.AddMsg(errMsg);
		throw m_tException;
	}

	COMMTIMEOUTS TimeOuts; 
	//设定读超时 
	TimeOuts.ReadIntervalTimeout=1000; 
	TimeOuts.ReadTotalTimeoutMultiplier=1000; 
	TimeOuts.ReadTotalTimeoutConstant=1000; 
	//设定写超时 
	TimeOuts.WriteTotalTimeoutMultiplier=1000; 
	TimeOuts.WriteTotalTimeoutConstant=1000; 

	DCB dcb; 
	// configure
	if (SetCommTimeouts(m_hHandle,&TimeOuts)) //设置超时 
	{						   
		if (SetCommMask(m_hHandle, EV_RXCHAR|EV_RXFLAG))
		{
			if (GetCommState(m_hHandle, &dcb))
			{
				dcb.BaudRate = socketInfo.m_tSerialPortCfg.m_nBaudRate;
				dcb.ByteSize = (BYTE)socketInfo.m_tSerialPortCfg.m_nByteSize;
				dcb.Parity = (BYTE)socketInfo.m_tSerialPortCfg.m_nParity;
				dcb.StopBits = (BYTE)socketInfo.m_tSerialPortCfg.m_nStopBits;

				dcb.fBinary           = FBINARY(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);   
				dcb.fParity           = FPARITY(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);                        
				dcb.fOutxCtsFlow      = FOUTXCTSFLOW(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);                              
				dcb.fOutxDsrFlow      = FOUTXDSRFLOW(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
				dcb.fDtrControl       = FDTRCONTROL(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
				dcb.fDsrSensitivity   = FDSRSENSITIVITY(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
				dcb.fTXContinueOnXoff = FTXCONTINUEONXOFF(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
				dcb.fOutX             = FOUTX(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
				dcb.fInX              = FINX(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
				dcb.fErrorChar        = FERRORCHAR(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
				dcb.fNull             = FNULL(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
				dcb.fRtsControl       = FRTSCONTROL(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
				dcb.fAbortOnError     = FABORTONERROR(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits); 

				if (SetCommState(m_hHandle, &dcb))
					; // normal operation... continue
				else
				{
					m_tException = JCNetworkException();
					::CloseHandle(m_hHandle);
					m_hHandle = NULL;

					std::string errMsg = socketInfo.m_tSerialPortCfg.m_strSerialPort + " SetCommState Init Error! ";
					m_tException.AddMsg(errMsg);
					throw m_tException;
				}
			}
			else
			{
				m_tException = JCNetworkException();
				::CloseHandle(m_hHandle);
				m_hHandle = NULL;

				std::string errMsg = socketInfo.m_tSerialPortCfg.m_strSerialPort + " GetCommState Init Error! ";
				m_tException.AddMsg(errMsg);
				throw m_tException;
			}
		}
		else
		{
			m_tException = JCNetworkException();
			::CloseHandle(m_hHandle);
			m_hHandle = NULL;

			std::string errMsg = socketInfo.m_tSerialPortCfg.m_strSerialPort + " SetCommMask Init Error! ";
			m_tException.AddMsg(errMsg);
			throw m_tException;
		}
	}
	else
	{
		m_tException = JCNetworkException();
		::CloseHandle(m_hHandle);
		m_hHandle = NULL;

		std::string errMsg = socketInfo.m_tSerialPortCfg.m_strSerialPort + " SetCommTimeouts Init Error! ";
		m_tException.AddMsg(errMsg);
		throw m_tException;
	}

	PurgeComm(m_hHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);  

	return TRUE;
}

bool JCSerialPort::ClearSocket()
{
	if(m_ov.hEvent != NULL)  
		CloseHandle(m_ov.hEvent);  
	if(m_hHandle != INVALID_HANDLE_VALUE) 
	{  
		CloseHandle(m_hHandle);
		m_hHandle = NULL;
		return true; 
	}

	return false;
}

bool JCSerialPort::ResetSocket()
{
	return false;
}

string JCSerialPort::GetName()
{
	return m_strSerialName;
}

void JCSerialPort::ClearLock()
{
	m_tCsSerialPort.Unlock();
}

HANDLE JCSerialPort::GetOvEvent()
{
	return m_ov.hEvent;
}

HANDLE JCSerialPort::GetFileHandle()
{
	return m_hHandle;
}

int JCSerialPort::Send(const char* pBuf, int bufLen, int socketId)
{
	m_tCsSerialPort.Lock();
	m_ov.Offset = 0;  
	m_ov.OffsetHigh = 0;  
	DWORD iByteSend = 0;
	// Clear buffer   
	PurgeComm(m_hHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);  

	BOOL bResult = WriteFile(m_hHandle,     // Handle to COMM Port   
		pBuf,							   // Pointer to message buffer in calling finction   
		bufLen,   
		&iByteSend,                        // Where to store the number of bytes sent   
		&m_ov);                            // Overlapped structure   

	// deal with any error codes 
	//JCMCAPDebug("write com proc result: %d, need send byte: %d\n", bResult, nSendLen);
	if (!bResult)    
	{  
		DWORD dwError = GetLastError();  
		switch (dwError)  
		{  
		case ERROR_IO_PENDING:  
			{  
				// continue to GetOverlappedResults()   
				iByteSend = 0;  
				break;  
			}  
		default:  
			{  
				m_tCsSerialPort.Unlock();
				return 0;
				// all other error codes   
			}  
		}  
	} 

	bResult = GetOverlappedResult(m_hHandle, // Handle to COMM port    
		&m_ov,       // Overlapped structure   
		&iByteSend,       // Stores number of bytes sent   
		TRUE);            // Wait flag   
	//防止COM句柄无效时，函数一直阻塞，造成线程阻塞；


	m_tCsSerialPort.Unlock();

	return iByteSend;
}

int JCSerialPort::Recv(char* pBuf, int bufLen, string &source, int socketId)
{
	BOOL  bRead = TRUE; 
	BOOL  bResult = TRUE;
	DWORD dwError = 0;
	DWORD BytesRead = 0;
	uchar RXBuff;
	COMSTAT comstat;

	int i=0;
	for (;;) 
	{ 
		m_tCsSerialPort.Lock();
		bResult = ClearCommError(m_hHandle, &dwError, &comstat);
		m_tCsSerialPort.Unlock();
		if (comstat.cbInQue == 0)
			break;//退出读循环，否则无法进行写操作
		m_tCsSerialPort.Lock();

		if (bRead)
		{
			bResult = ReadFile(m_hHandle,		// Handle to COMM port 
				&RXBuff,				// RX Buffer Pointer
				1,					// Read one byte
				&BytesRead,			// Stores number of bytes read
				&m_ov);		// pointer to the m_ov structure
			// deal with the error code 
			if (!bResult)  
			{ 
				switch (dwError = GetLastError()) 
				{ 
				case ERROR_IO_PENDING: 	
				case ERROR_INVALID_PARAMETER:
					{ 
						// asynchronous i/o is still in progress 
						// Proceed on to GetOverlappedResults();
						bRead = FALSE;
						break;
					}
				default:
					{
						// Another error has occured.  Process this error.
						m_tCsSerialPort.Unlock();
						return 0;
					} 
				}
			}
			else
			{
				bRead = TRUE;
			}
		}  // close if (bRead)

		if (!bRead)
		{
			bRead = TRUE;
			bResult = GetOverlappedResult(m_hHandle,	// Handle to COMM port 
				&m_ov,		// Overlapped structure
				&BytesRead,		// Stores number of bytes read
				TRUE); 			// Wait flag

			// deal with the error code 
			if (!bResult)  
			{
			}	
		}  // close if (!bRead)

		if(0 != BytesRead)//读到的数据不为空再处理
		{
			*(pBuf+i) = RXBuff;
			++i;
		}
		m_tCsSerialPort.Unlock();

		if (i >= bufLen)
		{
			break;
		}
	} // end forever loop

	return i;
}


JCSerialPortSync::JCSerialPortSync()
{
}

JCSerialPortSync::~JCSerialPortSync()
{
	if (m_hHandle)
	{
		::CloseHandle(m_hHandle);
		m_hHandle = NULL;
	}
}

int JCSerialPortSync::Send(const char* pBuf, int bufLen, int socket)
{
	//EnterCriticalSection(&m_tCsSerialPort);  
	DWORD iByteSend = 0;
	// Clear buffer   

	PurgeComm(m_hHandle,PURGE_TXCLEAR|PURGE_TXABORT);  

	BOOL bResult = WriteFile(m_hHandle,     // Handle to COMM Port   
		pBuf,							   // Pointer to message buffer in calling finction   
		bufLen,   
		&iByteSend,                        // Where to store the number of bytes sent   
		NULL);                            // Overlapped structure   

	// deal with any error codes 
	//JCMCAPDebug("write com proc result: %d, need send byte: %d\n", bResult, nSendLen);
	if (!bResult)    
	{  
		DWORD dwError = GetLastError();  
		switch (dwError)  
		{  
		case ERROR_IO_PENDING:  
			{  
				// continue to GetOverlappedResults()   
				iByteSend = 0;  
				break;  
			}  
		default:  
			{  
				// all other error codes   
			}  
		}  
	}   

	//LeaveCriticalSection(&m_tCsSerialPort);  
	return iByteSend;
}

DWORD JCSerialPortSync::Recv(char* pBuf, int nBufSize, int socket)
{
	//EnterCriticalSection(&m_tCsSerialPort);  

	DWORD wBufLen = 0 ;
	DWORD dwError = 0 ;
	// 	if(::ClearCommError(m_hHandle, &dwError, &Stat) && dwError > 0)
	// 	{
	// 		//清空输入缓冲区
	// 		::PurgeComm(m_hHandle, PURGE_RXABORT | PURGE_RXCLEAR);
	// 	//	TRACE0("/n清除错误失败！");
	// 		LeaveCriticalSection(&m_tCsSerialPort);  
	// 		return 0;
	// 	}

	BOOL bResult =ReadFile(m_hHandle, 
		pBuf,
		nBufSize,
		&wBufLen,
		NULL) ;
	if (!bResult)    
	{  
		DWORD dwError = GetLastError();  
		switch (dwError)  
		{  
		case ERROR_IO_PENDING:  
			{  
				// continue to GetOverlappedResults()   
				wBufLen = 0;  
				break;  
			}  
		default:  
			{  
				// all other error codes   
			}  
		}  
	}   
	/* 清?帐淙牖捍? */
	PurgeComm(m_hHandle,PURGE_RXABORT|PURGE_RXCLEAR);  

	/*	LeaveCriticalSection(&m_tCsSerialPort);  */

	return wBufLen ;
}

bool JCSerialPortSync::InitSerialPort(SocketInfo& socketInfo)
{
	if (socketInfo.m_tSerialPortCfg.m_strSerialPort.find("\\\\.\\") == string::npos)
	{
		socketInfo.m_tSerialPortCfg.m_strSerialPort = "\\\\.\\" + socketInfo.m_tSerialPortCfg.m_strSerialPort;
	}

	m_hHandle = ::CreateFile(socketInfo.m_tSerialPortCfg.m_strSerialPort.c_str(),//COM口
		GENERIC_READ | GENERIC_WRITE, 
		NULL,// 独占方式
		NULL,
		OPEN_EXISTING,
		NULL,
		NULL);


	if(m_hHandle==INVALID_HANDLE_VALUE) 
	{  
		::CloseHandle(m_hHandle);
		m_hHandle = NULL;

		std::string errMsg = socketInfo.m_tSerialPortCfg.m_strSerialPort + " Init Error! ";
		m_tException = JCNetworkException();
		m_tException.AddMsg(errMsg);
		throw m_tException;
	}

	::SetupComm(m_hHandle,DEFAULT_BUFFER_LEN,DEFAULT_BUFFER_LEN); //输入缓冲区和输出缓冲区的大小都是1024 

	COMMTIMEOUTS TimeOuts; 
	TimeOuts.ReadIntervalTimeout = 2; 	//接收时，两字符间最大的时延 
	TimeOuts.ReadTotalTimeoutMultiplier = 0; 	// 读取每字节的超时 
	TimeOuts.ReadTotalTimeoutConstant = 0; 	// 读串口数据的固定超时 总超时 = ReadTotalTimeoutMultiplier * 字节数 + ReadTotalTimeoutConstant 
	TimeOuts.WriteTotalTimeoutMultiplier = 0; 	// 写每字节的超时 
	TimeOuts.WriteTotalTimeoutConstant = 0; 	// 写串口数据的固定超时 

	SetCommTimeouts(m_hHandle,&TimeOuts); //设置超时 

	DCB dcb; 
	GetCommState(m_hHandle,&dcb); 
	dcb.BaudRate = socketInfo.m_tSerialPortCfg.m_nBaudRate;
	dcb.ByteSize = (BYTE)socketInfo.m_tSerialPortCfg.m_nByteSize;
	dcb.Parity = (BYTE)socketInfo.m_tSerialPortCfg.m_nParity;
	dcb.StopBits = (BYTE)socketInfo.m_tSerialPortCfg.m_nStopBits;

	dcb.fBinary           = FBINARY(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);   
	dcb.fParity           = FPARITY(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);                        
	dcb.fOutxCtsFlow      = FOUTXCTSFLOW(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);                              
	dcb.fOutxDsrFlow      = FOUTXDSRFLOW(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
	dcb.fDtrControl       = FDTRCONTROL(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
	dcb.fDsrSensitivity   = FDSRSENSITIVITY(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
	dcb.fTXContinueOnXoff = FTXCONTINUEONXOFF(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
	dcb.fOutX             = FOUTX(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
	dcb.fInX              = FINX(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
	dcb.fErrorChar        = FERRORCHAR(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
	dcb.fNull             = FNULL(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
	dcb.fRtsControl       = FRTSCONTROL(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
	dcb.fAbortOnError     = FABORTONERROR(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits); 

	SetCommState(m_hHandle, &dcb); 
	PurgeComm(m_hHandle, PURGE_TXCLEAR|PURGE_RXCLEAR| PURGE_RXABORT | PURGE_TXABORT);
	SetCommMask(m_hHandle, EV_RXCHAR|EV_TXEMPTY); 

	return TRUE;
}

bool JCSerialPortSync::ReInitSerialPort(SocketInfo& socketInfo, COMMTIMEOUTS &TimeOuts)
{
	SetCommTimeouts(m_hHandle,&TimeOuts); //设置超时 

	DCB dcb; 
	GetCommState(m_hHandle,&dcb); 
	dcb.BaudRate = socketInfo.m_tSerialPortCfg.m_nBaudRate;
	dcb.ByteSize = (BYTE)socketInfo.m_tSerialPortCfg.m_nByteSize;
	dcb.Parity = (BYTE)socketInfo.m_tSerialPortCfg.m_nParity;
	dcb.StopBits = (BYTE)socketInfo.m_tSerialPortCfg.m_nStopBits;

	dcb.fBinary           = FBINARY(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);   
	dcb.fParity           = FPARITY(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);                        
	dcb.fOutxCtsFlow      = FOUTXCTSFLOW(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);                              
	dcb.fOutxDsrFlow      = FOUTXDSRFLOW(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
	dcb.fDtrControl       = FDTRCONTROL(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
	dcb.fDsrSensitivity   = FDSRSENSITIVITY(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
	dcb.fTXContinueOnXoff = FTXCONTINUEONXOFF(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
	dcb.fOutX             = FOUTX(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
	dcb.fInX              = FINX(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
	dcb.fErrorChar        = FERRORCHAR(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
	dcb.fNull             = FNULL(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
	dcb.fRtsControl       = FRTSCONTROL(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits);     
	dcb.fAbortOnError     = FABORTONERROR(socketInfo.m_tSerialPortCfg.m_nSerialAttrBits); 

	SetCommState(m_hHandle,&dcb); 	

	PurgeComm(m_hHandle,PURGE_TXCLEAR|PURGE_RXCLEAR| PURGE_RXABORT | PURGE_TXABORT);
	SetCommMask(m_hHandle, EV_RXCHAR|EV_TXEMPTY); 

	//::InitializeCriticalSection(&m_tCsSerialPort);
	return true;
}

void JCSerialPortSync::Close()
{
	VOIDRET_ASSERT(NULL != m_hHandle);

	::CloseHandle(m_hHandle);
	m_hHandle = NULL;
}

JCNetworkManager::JCNetworkManager()
{
	m_ptThread = NULL;
	m_eServerType = INVALID_SERVER_TYPE;
	for (int i=0; i < MAX_WAIT_OBJS; ++i)
	{
		m_tHandleVer[i] = 0;
		m_tSocketBasePVer.push_back(NULL);
	}

	m_nEventNum = 0;
	HANDLE stopEvent = ::CreateEventA( 0, TRUE, TRUE, NULL);
	HANDLE rewaitEvent = ::CreateEventA( 0, TRUE, TRUE, NULL);
	m_tHandleVer[0] = stopEvent; 
	m_tHandleVer[1] = rewaitEvent; 
}
JCNetworkManager::~JCNetworkManager()
{
	Stop();
	SocketBasePVer::iterator it = m_tSocketBasePVer.begin();
	for (;it != m_tSocketBasePVer.end(); ++it)
	{
		if (*(it))
		{
			delete *it;
		}
	}


	SocketVer::iterator itClient = m_tTcpClientSocketVer.begin();
	for (; itClient != m_tTcpClientSocketVer.end(); ++itClient)
	{
		if (itClient->m_nSocketId != INVALID_SOCKET)
		{
			closesocket(itClient->m_nSocketId);
			itClient->m_nSocketId = INVALID_SOCKET;
		}
		//WSACloseEvent(itClient->m_hEvent); m_tHandleVer中包含了该Hanle，不能重复关闭
	}

	for (int i=0; i < MAX_WAIT_OBJS; ++i)
	{
		if (m_tHandleVer[i] != NULL)
		{
			CloseHandle(m_tHandleVer[i]);
			m_tHandleVer[i] = 0;
		}
	}
}

void JCNetworkManager::InitServerType(SOCKET_TYPE type)
{
	m_eServerType = type;
}

void JCNetworkManager::UpdateEventToWait()
{
	int i = SOCKET_EVENT_BEGIN_INDEX;

	for (int n= SOCKET_EVENT_BEGIN_INDEX; n < MAX_WAIT_OBJS; ++n)
	{
		m_tHandleVer[n] = 0;
	}

	if (m_eServerType == TCP_SOCKET)
	{
		SocketVer::iterator itClient =  m_tTcpClientSocketVer.begin();
		for (; itClient != m_tTcpClientSocketVer.end(); ++itClient, ++i)
		{
			m_tHandleVer[i] = itClient->m_hEvent;
		}
	}
	else
	{
		IndexEventMap::iterator itEvent = m_tServerEvent.begin();
		for (; itEvent != m_tServerEvent.end(); ++itEvent, ++i)
		{
			m_tHandleVer[i] = itEvent->second;
			m_tEventSocketMap[i] = itEvent->first;
		}
	}

	m_nEventNum = i;
}

bool JCNetworkManager::AddService(SocketBase* pItem, int nIndex)
{
	if (nIndex < 0 || nIndex > MAX_WAIT_OBJS - 1)
	{
		return false;
	}

	if (pItem != NULL)
	{
		delete m_tSocketBasePVer[nIndex];
		m_tSocketBasePVer[nIndex] = pItem;

		HANDLE hEvent = ::CreateEventA( 0, false, TRUE, NULL);
		::ResetEvent(hEvent);

		int nError = 0;
		m_eServerType = pItem->GetType();
		if (m_eServerType == TCP_SOCKET)
		{
			::WSAEventSelect(pItem->GetSocketId(), hEvent, FD_READ|FD_CLOSE|FD_ACCEPT);
			TcpClientItem tmp;
			tmp.m_nSocketId = pItem->GetSocketId();
			tmp.m_hEvent = hEvent;
			m_tTcpClientSocketVer.push_back(tmp);
		}
		else if (m_eServerType == UDP_SOCKET)
		{
			::WSAEventSelect(pItem->GetSocketId(), hEvent, FD_READ|FD_CLOSE);
		}
		else if (m_eServerType == SERIAL_PORT)
		{
			CloseHandle(hEvent);
			hEvent = ((JCSerialPort*)pItem)->GetOvEvent();
		}
		else
		{}

		if (nError == SOCKET_ERROR)
		{
			throw JCNetworkException();
		}

		IndexEventMap::iterator itEvent = m_tServerEvent.find(nIndex);
		if (itEvent != m_tServerEvent.end())
		{
			CloseHandle(itEvent->second);
			m_tServerEvent.erase(itEvent);
		}
		m_tServerEvent.insert(make_pair(nIndex, hEvent)); 

		UpdateEventToWait();
		::SetEvent(m_tHandleVer[1]);
	}
	return false;
}

bool JCNetworkManager::DelUdpService(int nIndex)
{
	if (nIndex < 0 || nIndex > MAX_WAIT_OBJS - 1)
	{
		return false;
	}

	SocketBase* pItem = m_tSocketBasePVer[nIndex];

	if (NULL != pItem && pItem->m_eSockType == UDP_SOCKET)
	{
		delete pItem;
		m_tSocketBasePVer[nIndex] = NULL;
		return true;
	}
	else
	{
		return false;
	}

	return true;
}

bool JCNetworkManager::AcceptTcpClient()
{
	if (m_nEventNum >= MAX_WAIT_OBJS &&
		!RemoveLastTcpClient())
	{
		return false;
	}

	SOCKADDR_IN saRemote;
	int RemoteLen;
	SOCKET acceptSocket;

	RemoteLen = sizeof(saRemote);
	acceptSocket = accept(m_tSocketBasePVer[0]->GetSocketId(), (SOCKADDR*)&saRemote, &RemoteLen);

	if (acceptSocket > 0)
	{
		/* 设置 KEEPLIVE */
		BOOL bKeepAlive = TRUE;  
		int err = setsockopt(acceptSocket, SOL_SOCKET, SO_KEEPALIVE,   (char*)&bKeepAlive, sizeof(bKeepAlive));  

		/* 设置缓冲区大小*/
		int nSendBufLen = 1024*1024*100;
		int nRecvBufLen = 1024*1024*100;
		int milliseconds = 10000;
		setsockopt(acceptSocket, SOL_SOCKET, SO_SNDBUF,   (char*)&nSendBufLen, sizeof(int));
		setsockopt(acceptSocket, SOL_SOCKET, SO_RCVBUF,   (char*)&nRecvBufLen, sizeof(int));
		setsockopt(acceptSocket, SOL_SOCKET, SO_RCVTIMEO,   (char*)&milliseconds, sizeof(int));

		if (SOCKET_ERROR== err)  
		{  
			JCNetworkException tException = JCNetworkException();
			std::string errMsg = "set keepalive";
			tException.AddMsg(errMsg);
			throw tException;
			return false;  
		}  

		tcp_keepalive alive_in;  
		tcp_keepalive alive_out;  
		alive_in.keepalivetime    = 10000;  // 10s  
		alive_in.keepaliveinterval  = 5000; //1s  
		alive_in.onoff                       = TRUE;  
		unsigned long ulBytesReturn = 0;  
		err = WSAIoctl(acceptSocket, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in),  
			&alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL);  
		if (SOCKET_ERROR== err)  
		{  
			JCNetworkException tException = JCNetworkException();
			std::string errMsg = "WSAIoctl failed";
			tException.AddMsg(errMsg);
			throw tException;
			return false;  
		}  

		HANDLE tcpEvent = ::CreateEventA( 0, false, TRUE, NULL);
		::ResetEvent(tcpEvent);

		int nError = ::WSAEventSelect(acceptSocket, tcpEvent, FD_READ|FD_CLOSE|FD_ACCEPT);

		if (nError == SOCKET_ERROR)
		{
			throw JCNetworkException();
		}

		TcpClientItem tmp;
		tmp.m_nSocketId = acceptSocket;
		tmp.m_hEvent = tcpEvent;
		tmp.m_clientIp.assign(inet_ntoa(saRemote.sin_addr));
		tmp.m_clientIp += ":";
		tmp.m_clientIp += JCAssist::U32ToDecString(ntohs(saRemote.sin_port));
		m_tTcpClientSocketVer.push_back(tmp);

		UpdateEventToWait();
	}

	return true;
}

bool JCNetworkManager::RemoveTcpClient(int iIndex)
{
	if (iIndex < 1 || (size_t)iIndex >= m_tTcpClientSocketVer.size())
		return false;

	SOCKET tmp = m_tTcpClientSocketVer[iIndex].m_nSocketId;
	string clientIp = m_tTcpClientSocketVer[iIndex].m_clientIp;
	closesocket(tmp);

	CloseHandle(m_tTcpClientSocketVer[iIndex].m_hEvent); 

	SocketVer::iterator it = m_tTcpClientSocketVer.begin();
	for (; it != m_tTcpClientSocketVer.end(); ++it)
	{
		if (it->m_nSocketId == tmp)
		{
			m_tTcpClientSocketVer.erase(it);
			break;
		}
	}

	UpdateEventToWait();

	(m_tSocketBasePVer.at(0))->m_ptSockListener->TcpClientDisConnect(iIndex);
	(m_tSocketBasePVer.at(0))->m_ptSockListener->TcpClientDisConnect(iIndex, clientIp);
	return true;
}

bool JCNetworkManager::RemoveTcpClientByIP(string strIp)
{
	int nIndex = -1;
	for (size_t i = 0; i < m_tTcpClientSocketVer.size(); ++i)
	{
		if (m_tTcpClientSocketVer[i].m_clientIp == strIp)
		{
			nIndex = i;
			break;
		}
	}

	if(RemoveTcpClient(nIndex))
	{
		::SetEvent(m_tHandleVer[1]);  //set rewait event
		return true;
	}
	return false;
}

bool JCNetworkManager::RemoveLastTcpClient()
{
	if(RemoveTcpClient(m_tTcpClientSocketVer.size()-1))
	{
		::SetEvent(m_tHandleVer[1]);  //set rewait event
		return true;
	}
	return false;
}

bool JCNetworkManager::Start()
{
	VALRET_ASSERT(m_eServerType != INVALID_SERVER_TYPE, false);
	VALRET_ASSERT(m_ptThread == NULL, false);

	::ResetEvent(m_tHandleVer[0]);
	m_ptThread = new JCThread((THREAD_ENTRY)RunServer,this);

	//提高网络线程优先级
	//	bool ret = m_ptThread->SetPriority(THREAD_PRIORITY_TIME_CRITICAL);
	if (m_ptThread == NULL)
	{
		return false;
	}
	m_ptThread->Resume();

	return true;
}

void JCNetworkManager::Stop()
{
	VOIDRET_ASSERT(NULL != m_ptThread);

	::SetEvent(m_tHandleVer[0]);
	while(m_ptThread->IsRunning())
	{
		Sleep(1);
	}

	FREE_POINT(m_ptThread);
}

bool JCNetworkManager::IsServerRunning()
{
	return ((NULL != m_ptThread) && m_ptThread->IsRunning());
}

bool JCNetworkManager::Reset()
{
	//stop监听线程
	Stop();

	::ResetEvent(m_tHandleVer[0]);
	::ResetEvent(m_tHandleVer[1]);
	//清空wait event,跳过stop,rewait
	for (int i=2; i<m_nEventNum; ++i)
	{
		if (m_tHandleVer[i] != NULL)
		{
			CloseHandle(m_tHandleVer[i]);
			m_tHandleVer[i] = 0;
		}
	}

	//重置索引参数
	m_nEventNum = 2;

	//清空管理socketbase指针
	SocketBasePVer::iterator itBase = m_tSocketBasePVer.begin();
	for (; itBase!=m_tSocketBasePVer.end(); ++itBase)
	{
		if ((*itBase) != NULL)
		{
			delete (*itBase);
			*itBase = NULL;
		}
	}

	//关闭，清空tcp client连接
	for (size_t i=0; i<m_tTcpClientSocketVer.size(); ++i)
	{
		if (m_tTcpClientSocketVer[i].m_nSocketId != INVALID_SOCKET)
		{
			closesocket(m_tTcpClientSocketVer[i].m_nSocketId);
			m_tTcpClientSocketVer[i].m_nSocketId = INVALID_SOCKET;
		}
	}

	m_tTcpClientSocketVer.clear();
	m_tServerEvent.clear();
	m_tEventSocketMap.clear();

	return true;
}

SOCKET_TYPE JCNetworkManager::GetServerType()
{
	return m_eServerType;
}

unsigned long WINAPI JCNetworkManager::RunServer(LPVOID pParam)
{
	JCNetworkManager* pSock = static_cast<JCNetworkManager*>(pParam);
	SOCKET_TYPE eType = pSock->GetServerType();
	if (eType == UDP_SOCKET)
		pSock->_RunUdp();
	else if (eType == TCP_SOCKET)
		pSock->_RunTcp();
	else if (eType == SERIAL_PORT)
		pSock->_RunSerial();

	return 0;
}

void JCNetworkManager::_RunTcp()
{
	if (m_eServerType != TCP_SOCKET)
		return;

	BOOL bWorkStop = FALSE;

	for (int i=0; i<m_nEventNum; ++i)
	{
		::ResetEvent(m_tHandleVer[i]);
	}

	while(!bWorkStop)
	{
		DWORD dwWait = WaitForMultipleObjects(m_nEventNum, m_tHandleVer, FALSE, INFINITE);
		int nSignaledHandle = dwWait - WAIT_OBJECT_0;
		if (nSignaledHandle == 0) //It was the stop request
			bWorkStop = TRUE;
		else if (nSignaledHandle == 1) //it's a rewait event
			::ResetEvent(m_tHandleVer[1]);
		else if (nSignaledHandle > 0)//It was a socket event
		{
			HandleTcpEvent();
		}
	}
}

void JCNetworkManager::HandleTcpEvent()
{
	char buf[1024];
	int recv =0;
	string source;

	for(unsigned iIndex = 0; iIndex <m_tTcpClientSocketVer.size(); iIndex ++)
	{
		SOCKET hSocket   = m_tTcpClientSocketVer[iIndex].m_nSocketId;
		HANDLE hNetEvent = m_tTcpClientSocketVer[iIndex].m_hEvent;
		string clientIp = m_tTcpClientSocketVer[iIndex].m_clientIp;
		WSANETWORKEVENTS sNetworkEvents = {0};

		int nRetVal = ::WSAEnumNetworkEvents(hSocket,hNetEvent,&sNetworkEvents);

		if(nRetVal != SOCKET_ERROR)
		{	
			if((sNetworkEvents.lNetworkEvents & FD_READ) == FD_READ &&
				sNetworkEvents.iErrorCode[FD_READ_BIT] == 0L) 
			{
				do 
				{
					memset(buf, 0, 1024);
					recv = (m_tSocketBasePVer.at(0))->Recv(buf, DEFAULT_BUFFER_LEN, source, hSocket);
					if (recv > 0)
					{
						(m_tSocketBasePVer.at(0))->m_ptSockListener->RecvCallBack(buf, recv, iIndex, clientIp);
					}
				} while (recv > 0);
			}
			else if((sNetworkEvents.lNetworkEvents & FD_ACCEPT) == FD_ACCEPT &&
				sNetworkEvents.iErrorCode[FD_ACCEPT_BIT] == 0L) 
			{
				if (!AcceptTcpClient())
					return;

				if (!m_tTcpClientSocketVer.empty())
				{
					clientIp = m_tTcpClientSocketVer.back().m_clientIp;
					(m_tSocketBasePVer.at(0))->m_ptSockListener->TcpClientConnect(iIndex);
					(m_tSocketBasePVer.at(0))->m_ptSockListener->TcpClientConnect(iIndex, clientIp);
				}
			}
			else if ((sNetworkEvents.lNetworkEvents & FD_CLOSE) == FD_CLOSE)
			{
				RemoveTcpClient(iIndex);
				iIndex = 0;
			}
		}
	}
}

void JCNetworkManager::_RunUdp()
{
	if (m_eServerType != UDP_SOCKET)
		return;

	BOOL bWorkStop = FALSE;

	for (int i=0; i<m_nEventNum; ++i)
	{
		::ResetEvent(m_tHandleVer[i]);
	}

	while(!bWorkStop)
	{
		DWORD dwWait = WaitForMultipleObjects(m_nEventNum, m_tHandleVer, FALSE, INFINITE);
		int nSignaledHandle = dwWait - WAIT_OBJECT_0;
		if (nSignaledHandle == 0)      //It's  a stop request
			bWorkStop = TRUE;
		else if (nSignaledHandle == 1) //it's a rewait event
			::ResetEvent(m_tHandleVer[1]);
		else if (nSignaledHandle > 1)  //It's a socket event
		{
			HandleUdpEvent(nSignaledHandle);
		}
	}
}

void JCNetworkManager::HandleUdpEvent(size_t iIndex)
{
	char buf[UDP_RCVBUF_SIZE];
	int recv =0;
	string source;
	
	int nSocketIndex = m_tEventSocketMap[iIndex];
	if ((nSocketIndex < 0 || nSocketIndex >= (int)m_tSocketBasePVer.size()) ||
		NULL == m_tSocketBasePVer.at(nSocketIndex))
	{
		return;
	}
	
	SOCKET hSocket   = m_tSocketBasePVer.at(nSocketIndex)->GetSocketId();
	HANDLE hNetEvent = m_tHandleVer[iIndex];

	WSANETWORKEVENTS sNetworkEvents = {0};

	int nRetVal = ::WSAEnumNetworkEvents(hSocket,hNetEvent,&sNetworkEvents);

	if(nRetVal != SOCKET_ERROR &&
		(sNetworkEvents.lNetworkEvents & FD_READ) == FD_READ &&
		sNetworkEvents.iErrorCode[FD_READ_BIT] == 0L)
	{	
		memset(buf, 0, UDP_RCVBUF_SIZE);

		recv = (m_tSocketBasePVer.at(nSocketIndex))->Recv(buf, UDP_RCVBUF_SIZE, source);
		if (recv > 0)
		{
			(m_tSocketBasePVer.at(nSocketIndex))->
				m_ptSockListener->RecvCallBack(buf, recv, nSocketIndex, source);
		}
	}
}

void JCNetworkManager::_RunSerial()
{
	if (m_eServerType != SERIAL_PORT)
		return;
	BOOL bWorkStop = FALSE;

	for(unsigned iIndex = 0; iIndex <m_tSocketBasePVer.size(); iIndex ++)
	{
		if (NULL == m_tSocketBasePVer.at(iIndex))
			continue;

		HANDLE hSerial = ((JCSerialPort*)m_tSocketBasePVer.at(iIndex))->GetFileHandle();

		if (hSerial == INVALID_HANDLE_VALUE)
			return;

		if (hSerial) 
		{
			PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);  
		}
	}

	for (int i=0; i<m_nEventNum; ++i)
	{
		::ResetEvent(m_tHandleVer[i]);
	}
	DWORD Event = 0;
	DWORD CommEvent = EV_BREAK | EV_RXCHAR;  
	DWORD dwError = 0;  
	COMSTAT comstat; 
	BOOL  bResult = TRUE; 

	while(!bWorkStop)
	{
		for(unsigned iIndex = 0; iIndex <m_tSocketBasePVer.size(); iIndex ++)
		{
			if (NULL == m_tSocketBasePVer.at(iIndex))
				continue;

			HANDLE hSerial = ((JCSerialPort*)m_tSocketBasePVer.at(iIndex))->GetFileHandle();
			bResult = WaitCommEvent(hSerial, &Event, 
				&(((JCSerialPort*)m_tSocketBasePVer.at(iIndex))->m_ov));  

			if (bResult)     
			{  
				bResult = ClearCommError(hSerial, &dwError, &comstat);  

				if (comstat.cbInQue == 0)  
					continue;  
			}
		}

		DWORD dwWait = WaitForMultipleObjects(m_nEventNum, m_tHandleVer, FALSE, INFINITE);	 
		int nSignaledHandle = dwWait - WAIT_OBJECT_0;

		if (nSignaledHandle == 0) //It's the stop request
		{
			bWorkStop = TRUE;
		}
		else if (nSignaledHandle == 1) //it's a rewait event
			::ResetEvent(m_tHandleVer[1]);
		else if (nSignaledHandle > 1) //it's Read event   
		{
			HandleSerialEvent(m_tEventSocketMap[nSignaledHandle], CommEvent); //serial start with 0 index in vector, need minus 2.
		}
	} 
}

void JCNetworkManager::HandleSerialEvent(int iIndex, DWORD CommEvent)
{
	char buf[DEFAULT_BUFFER_LEN];
	int recv =0;
	string source;

	memset(buf, 0, DEFAULT_BUFFER_LEN);

	if (NULL == m_tSocketBasePVer.at(iIndex))
		return;

	HANDLE hSerial = ((JCSerialPort*)m_tSocketBasePVer.at(iIndex))->GetFileHandle();
	string strSerialName = ((JCSerialPort*)m_tSocketBasePVer.at(iIndex))->GetName();
	GetCommMask(hSerial, &CommEvent);  
	if (CommEvent & EV_RXCHAR) //接收到字符,并置于输入缓冲区中 
	{
		recv = m_tSocketBasePVer.at(iIndex)->Recv(buf, DEFAULT_BUFFER_LEN, source);
		if (recv > 0)
		{
			(m_tSocketBasePVer.at(iIndex))->m_ptSockListener->RecvCallBack(buf, recv, iIndex, strSerialName);
		}
	}

	if (CommEvent & EV_BREAK)
	{
		m_tSocketBasePVer.at(iIndex)->m_ptSockListener->ConnectState(0);
	}
}

int JCNetworkManager::SendTo(const char* pBuf, int bufLen, std::string ip, u_short port)
{
	SocketVer tTcpClientSocketVer = m_tTcpClientSocketVer;

	int ret = 0;
	if (m_eServerType == UDP_SOCKET)
	{
		if (m_tSocketBasePVer.size() == 0 || NULL == m_tSocketBasePVer.at(0))
			return -1;

		ret = ((JCUdpSocket*)m_tSocketBasePVer.at(0))->SendToIp(pBuf, bufLen, ip, port);
	}
	else
	{
		return -1;
	}

	if (ret < 0)
		return GetLastError();

	return 0;
}

int JCNetworkManager::Send(const char* buf, int len, int socketIndex, string strTcpClientIpPort)
{
	SocketVer tTcpClientSocketVer = m_tTcpClientSocketVer;

	int ret = 0;
	if (m_eServerType == TCP_SOCKET)
	{
		if (socketIndex == -1)
		{//所有客户端发送
			SocketVer::iterator it = tTcpClientSocketVer.begin();
			for (; it!= tTcpClientSocketVer.end(); ++it)
			{
				if (it->m_nSocketId != m_tSocketBasePVer.at(0)->GetSocketId())
					ret = m_tSocketBasePVer.at(0)->Send(buf, len, it->m_nSocketId);
			}
		}
		else
		{
			ret = -1;
			SocketVer::iterator it = tTcpClientSocketVer.begin();
			for (; it!= tTcpClientSocketVer.end(); ++it)
			{
				if (it->m_clientIp == strTcpClientIpPort)
					ret = m_tSocketBasePVer.at(0)->Send(buf, len, it->m_nSocketId);
			}
		}
	}
	else if (m_eServerType == UDP_SOCKET || m_eServerType == SERIAL_PORT)
	{
		//全抽发送
		if (socketIndex == -1)
		{
			SocketBasePVer::iterator it = m_tSocketBasePVer.begin();
			for (; it!=m_tSocketBasePVer.end(); ++it)
			{
				if (NULL == *it)
				{
					continue;
				}
				ret = (*it)->Send(buf, len);
			}
		}
		else
		{
			if (NULL != m_tSocketBasePVer.at(socketIndex))
			{
				ret = m_tSocketBasePVer.at(socketIndex)->Send(buf, len);
			}
			else
			{
				return -1;
			}
		}
	}
	else
	{
		return -1;
	}

	if (ret < 0)
		return GetLastError();

	return 0;
}

#endif /*end win32*/
