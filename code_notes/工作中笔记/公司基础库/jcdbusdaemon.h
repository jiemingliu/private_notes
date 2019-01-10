/******************************************************************************
* 文件名: jcdbusdaemon.h
* 描述: -
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/04/03 17:6:50, gaopeng 创建文件
*
******************************************************************************/

#ifndef _JCDUBSDAEMON_H
#define _JCDUBSDAEMON_H

#include <vector>
#include <map>
#include <string>
#include "jcnetwork.h"
#include "jcbase.h"
#include "jcdbusmsg.h"
#include "jcdbusdaemonqueue.h"
#include "jcdbusdaemonconfig.h"
#include "jcxmlparse.h"
#include "jctimer.h"
#include "jclog.h"
#include "jcversionitem.h"
#include "jcudtsocket.h"
#include "tsmsgbuffmanager.h"

/*版本号实际显示格式为*.*.*.*, 此处定义用于初始化,务必保持此格式*/
#define JCDBUS_VERSION  1,0,0,1

using namespace std;

class JCDbusDaemon;

class DLL_EXPORT JCDbusLog : public JCLogWrapper
{
public:
	JCDbusLog();
	~JCDbusLog();

protected:
	string ModuleName(unsigned int nModuleId);

};

class JCDbusDaemonUdtProxy : public ISocketListener
{
public:
	JCDbusDaemonUdtProxy();
	virtual ~JCDbusDaemonUdtProxy();

	bool InitDaemonUdt(JCDbusDaemon *ptDaemon);
	bool SendMessage(JCDbusMsg &tMsg);
	bool IsUdtInited();
	void ResetInitedStatus();
	JCUdtNormalSocket& GetUdtNormalSocket();

private:
	bool InitDaemonNormalUdt(JCDbusDaemonConfigItem &tItem);
	bool InitDaemonServerUdt(JCDbusDaemonConfigItem &tItem);
	bool IsOutsideMsgValid(JCDbusMsgData &tData);
	bool SendToDbusInner(unsigned int nSocketId, const string &strSourceIp);

	virtual int RecvCallBack(char* pBuf, int nLen, int nIndex=0, const string &source = "");

	/*监测UDT服务端获取的UDT客户端连接状态, 向JCDBUS客户端提供通知*/
	virtual int UdtClientConnect(int nSocketId, string strClientIp); 
	virtual int UdtClientDisConnect(int nSocketId, string strClientIp); 

	/*借用重写TCP客户端接口监测UDT客户端连接状态, 向JCDBUS客户端提供通知*/
	virtual int ConnectState(const int &nState);

private:
	bool m_bInit;	
	JCDbusDaemon *m_ptDaemon;
	JCUdtNormalSocket m_tNormalSocket;
	JCUdtServerSocket m_tServerSocket;
	TSMsgBuffManager m_tDbusMsgBuffer;	
};

class DLL_EXPORT JCDbusDaemon : public ISocketListener
{
	friend class JCDbusDaemonUdtProxy;
public:
	JCDbusDaemon();
	JCDbusDaemon(unsigned int nDaemonId);
	JCDbusDaemon(const string& xmlFile);
	JCDbusDaemon(const string& logFileName, const string& xmlFile);

	~JCDbusDaemon();

public:   //模块延迟初始化接口	
	void InitLog(const string& strLogFileName="");
	bool InitDaemon(const string& xmlFile);
	void SetLogMask(__int64 m_nModuleMask,
		unsigned char m_nLevelMask,
		unsigned int m_nMode);

public:
	bool InitSocketManager(unsigned int nMode = JCDbusDaemonConfigItem::CLIENT);
	virtual int RecvCallBack(char* buf, int len, int iIndex=0, const string &source = "");
	static void TimerFuncSend(void *p);
	static void TimerFuncCheckUdpInitState(void *p);
public:
	string  m_xmlFile;

public:   //client模块功能接口
	bool ConnectToDaemon(unsigned int clientId);
	bool DisconnectFromDaemon(unsigned int clientId);
	bool IsActive(unsigned int clientId);
	bool SendMsg(JCDbusMsg& dbusMsg);
	bool GetFrontMessage(JCDbusMsg& dbusMsg );
	bool GetSpecMessage(JCDbusMsg& dbusMsg, unsigned int nTimeOut=3000);
	bool GetSpecClientMessage(JCDbusMsg& dbusMsg, unsigned int nTimeOut=3000);
	int	 ClearAllSpecIDMsg(JCDbusMsgInfo& tMsg);
	unsigned int GetDaemonId();
	unsigned int GetClientMessageCount(unsigned int clientId);
	JCDbusLog &GetLog();
	JCDbusDaemonConfig &GetDaemonConfig();
	string GetVersion();
	JCUdtNormalSocket &GetUdtNormalSocket();

private:
	bool SendMsgInner(JCDbusMsg& dbusMsg);
	bool SendMsgOuter(JCDbusMsg& dbusMsg);
	bool SendUdtConnectStatusToInnerAll(bool bConnect, unsigned int nRemoteDaemonId);
	bool UpdateUdtClientInfo(JCDbusMsg &tMsg, unsigned int nSocketId, const string &strClientIp);
	bool UpdateUdtServerInfo(JCDbusMsg &tMsg, unsigned int nSocketId, const string &strClientIp);

private:
	JCDbusDaemonQueue m_tDaemonQueue;
	JCDbusDaemonConfig m_tDaemonConfig;
	JCDbusLog m_tLog;
	JCDbusDaemonUdtProxy m_tDaemonUdtProxy;
	JCVersionItem m_tVersion;
	JCNetworkManager m_tUdpManager;
	JCTimerManager m_tTimerManager;
	JCTimer	m_tTimerSend;
	JCTimer m_tTimerInitUdp;
};

#endif /* _JCDUBSDAEMON_H */

