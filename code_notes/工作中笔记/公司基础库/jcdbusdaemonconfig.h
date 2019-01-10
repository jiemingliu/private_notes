/******************************************************************************
* 文件名: jcdbusdaemonconfig.h
* 描述: daemon配置解析
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/10/16 17:6:15, lys 创建文件
*
******************************************************************************/
#ifndef _JCDBUSDAEMONCONFIG_H_
#define _JCDBUSDAEMONCONFIG_H_

#include <string>
#include <map>
#include "jcmacrotools.h"
#include "jcconfigdatabase.h"
#include "jcLock.hpp"

using namespace std;

class DLL_EXPORT JCDbusDaemonConfigItem : public JCParaBase
{
public:
	enum NetworkMode{CLIENT, SERVER, RENDEZVOUS};/*客户端模式 服务端模式 会合模式(对等)*/

	JCDbusDaemonConfigItem();
	JCDbusDaemonConfigItem(const string &strIp, unsigned int nPort
		, unsigned int nDaemonId, unsigned int nMode = RENDEZVOUS);
	JCDbusDaemonConfigItem(const JCDbusDaemonConfigItem &tItem);
	JCDbusDaemonConfigItem &operator=(const JCDbusDaemonConfigItem &tItem);
	virtual ~JCDbusDaemonConfigItem();

	DECLARE_MEMBER(string,       m_strDaemonIp,    0);
	DECLARE_MEMBER(unsigned int, m_nDaemonPort,    1);
	DECLARE_MEMBER(unsigned int, m_nDaemonMode,    2);
	DECLARE_MEMBER(unsigned int, m_nDaemonId,      3);/*daemonId与MacAddr不配置*/
	DECLARE_MEMBER(unsigned int, m_nUdtSocketId,   4);
	DECLARE_MEMBER(string,		 m_strMacAddr,     5);
	DECLARE_MEMBER(string,		 m_strSubnetMask,  6);
	DECLARE_MEMBER(string,		 m_strPCName,      7);
	END_DECLARE_MEMBER(8);

	unsigned long IntergeIp();

protected:
	string MemberInfo(unsigned int nMemberIndex, InfoType eInfoType);
	void SetMemberValue(unsigned int nMemberIndex, const string &strValue);
};


class DLL_EXPORT JCDbusDaemonConfig
{
public:
	JCDbusDaemonConfig();
	virtual ~JCDbusDaemonConfig();

    typedef map<unsigned int, JCDbusDaemonConfigItem> JCDbusDaemonMap;

	bool LoadDaemonConfig(const string &strFile);
	JCDbusDaemonConfigItem &LocalDaemon();
	void SetLocalDaemon(JCDbusDaemonConfigItem& configItem);
	unsigned int RemoteDaemonCount();
	bool GetRemoteDaemonByDaemonId(unsigned int nDaemonId, JCDbusDaemonConfigItem &tGetConfig);
	bool GetRemoteDaemonBySocketId(unsigned int nSocketId, JCDbusDaemonConfigItem &tGetConfig);
	bool GetFirstRemoteDaemon(JCDbusDaemonConfigItem &tGetConfig);/*根据存储排列序号获取daemon配置, order从0开始*/
	unsigned int AddDaemon(const JCDbusDaemonConfigItem &tGetConfig);
	unsigned int DeleteDaemon(unsigned int nDaemonId);
	unsigned int DeleteDaemonBySocketId(unsigned int nUdtSocketId);
	bool ClearRemoteDaemon();

private:
	JCDbusDaemonConfigItem m_tLocalDaemon;
	JCDbusDaemonMap m_tRemoteDaemons;
	JCCriticalSection m_tCs;
};



#endif /* _JCDBUSDAEMONCONFIG_H_ */

