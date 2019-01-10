/******************************************************************************
* 文件名: jcdbusmsg.h
* 描述: JC-DBUS消息类型
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/03/31 20:13:23, lys 创建文件
*
******************************************************************************/
#ifndef _JCDBUSMSG_H_
#define _JCDBUSMSG_H_

#include "jcbase.h"
#include "jclink.h"
#include "jcdbusmsgdata.h"

using namespace SNACC;

/******************************************************************************
* 函数名:JCDbusMsgRoute
* 描述: 消息路由信息,用于定位消息收发对象
*
* 输入:
* 输出:
* 返回值:
*
* 修改记录:
* --------------------
*    2014/04/03 20:0:56, lys 创建函数
*
******************************************************************************/
#define  INVALID_ID   0xFFFFFFFF
class DLL_EXPORT JCDbusMsgRoute
{
public:
	JCDbusMsgRoute();
	JCDbusMsgRoute(unsigned int nDaemonId, unsigned int nModuleId);
	JCDbusMsgRoute(const JCDbusMsgRoute &tInfo);
	JCDbusMsgRoute &operator = (const JCDbusMsgRoute &tInfo);
	friend bool DLL_EXPORT operator == (const JCDbusMsgRoute &lhs, const JCDbusMsgRoute &rhs);
	friend bool DLL_EXPORT operator != (const JCDbusMsgRoute &lhs, const JCDbusMsgRoute &rhs);

	virtual ~JCDbusMsgRoute();
	unsigned int m_nDaemonId;/*通信框架daemon ID*/
	unsigned int m_nModuleId;/*daemon同一框架内的模块ID*/
};

/******************************************************************************
* 函数名:JCDbusMsgInfo
* 描述: JCDbus消息头结构
*
* 输入:
* 输出:
* 返回值:
*
* 修改记录:
* --------------------
*    2014/04/02 16:49:39, lys 创建函数
*
******************************************************************************/
class DLL_EXPORT JCDbusMsgInfo
{
public:
	JCDbusMsgInfo();

    /******************************************************************************
    * 函数名:JCDbusMsgInfo.JCDbusMsgInfo
    * 描述:构造函数
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/03 20:1:49, lys 创建函数
    *
    ******************************************************************************/
	JCDbusMsgInfo(unsigned int nSendDaemonId, unsigned int nSendModuleId, unsigned int nReceDaemonId
		, unsigned int nReceModuleId, unsigned int nMessageId, unsigned int nClientId, unsigned int nExecCode = 0);
    /******************************************************************************
    * 函数名:JCDbusMsgInfo.JCDbusMsgInfo
    * 描述: 构造函数
    *
    * 输入: 发送模块信息,接收模块信息,消息代码,消息类型
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/02 16:52:15, lys 创建函数
    *
    ******************************************************************************/
	JCDbusMsgInfo(const JCDbusMsgRoute &tSenderId, const JCDbusMsgRoute &tReceiverId
		, unsigned int nMessageId, unsigned int nClientId, unsigned int nExecCode = 0);

    /******************************************************************************
    * 函数名:JCDbusMsgInfo.JCDbusMsgInfo
    * 描述: 复制构造函数
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/02 16:53:16, lys 创建函数
    *
    ******************************************************************************/
    JCDbusMsgInfo(const JCDbusMsgInfo &tInfo);

    /******************************************************************************
    * 函数名:JCDbusMsgInfo.operator=
    * 描述: 赋值操作
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/02 16:53:40, lys 创建函数
    *
    ******************************************************************************/
	JCDbusMsgInfo &operator = (const JCDbusMsgInfo &tInfo);
	friend bool DLL_EXPORT operator == (const JCDbusMsgInfo &lhs, const JCDbusMsgInfo &rhs);
	friend bool DLL_EXPORT operator != (const JCDbusMsgInfo &lhs, const JCDbusMsgInfo &rhs);
	virtual~JCDbusMsgInfo();
	JCDbusMsgInfo &SwapMsgRoute();

	JCDbusMsgRoute m_tSenderId;
	JCDbusMsgRoute m_tReceiverId;
	unsigned int m_nMessageId;
	unsigned int m_nClientId;
	unsigned int m_nExecCode;/*执行结果代码不参与比较操作*/
};

class JCDbusQueue;
/******************************************************************************
* 函数名:JCDbusMsg
* 描述: JCDbus消息结构
*
* 输入:
* 输出:
* 返回值:
*
* 修改记录:
* --------------------
*    2014/04/02 16:54:33, lys 创建函数
*
******************************************************************************/
class DLL_EXPORT JCDbusMsg
{
	friend class JCDbusQueue;
public:
	JCDbusMsg();

    /******************************************************************************
    * 函数名:JCDbusMsg.JCDbusMsg
    * 描述: 构造函数
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/03 20:3:42, lys 创建函数
    *
    ******************************************************************************/
	JCDbusMsg(unsigned int nSendDaemonId, unsigned int nSendModuleId, unsigned int nReceDaemonId
		, unsigned int nReceModuleId, unsigned int nMessageId, unsigned int nClientId, unsigned int nExecCode = 0);

    /******************************************************************************
    * 函数名:JCDbusMsg.JCDbusMsg
    * 描述: 构造函数
    *
    * 输入:发送模块ID,接收模块ID,消息代码,消息类型
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/02 16:55:7, lys 创建函数
    *
    ******************************************************************************/
	JCDbusMsg(const JCDbusMsgRoute &tSenderId, const JCDbusMsgRoute &tReceiverId
		, unsigned int nMessageId, unsigned int nClientId, unsigned int nExecCode = 0);

    /******************************************************************************
    * 函数名:JCDbusMsg.JCDbusMsg
    * 描述: 消息头作构造参数的构造函数
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/02 16:55:56, lys 创建函数
    *
    ******************************************************************************/
    JCDbusMsg(const JCDbusMsgInfo &tInfo);

    /******************************************************************************
    * 函数名:JCDbusMsg.JCDbusMsg
    * 描述: 复制构造函数
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/02 16:56:43, lys 创建函数
    *
    ******************************************************************************/
	JCDbusMsg(const JCDbusMsg &tMsg);
	JCDbusMsg &operator = (const JCDbusMsg &tMsg);
	virtual ~JCDbusMsg();

    /******************************************************************************
    * 函数名:JCDbusMsg.LinkToHead
    * 描述: 将消息链接到指定消息为链表头的链表的头部(即指定消息之后)
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/02 16:57:25, lys 创建函数
    *
    ******************************************************************************/
	void LinkToHead(JCDbusMsg &tMsg);

    /******************************************************************************
    * 函数名:JCDbusMsg.LinkToHead
    * 描述: 通过消息结构的链接节点链接,功能同上
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/02 16:58:39, lys 创建函数
    *
    ******************************************************************************/
	void LinkToHead(JCDoubleLinkNode &tNode);

    /******************************************************************************
    * 函数名:JCDbusMsg.LinkToTail
    * 描述: 将消息链接到指定消息为链表头的链表的头部(即指定消息之前)
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/02 16:59:24, lys 创建函数
    *
    ******************************************************************************/
	void LinkToTail(JCDbusMsg &tMsg);

    /******************************************************************************
    * 函数名:JCDbusMsg.LinkToTail
    * 描述: 通过消息结构的链接节点链接,功能同上
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/02 17:0:13, lys 创建函数
    *
    ******************************************************************************/
	void LinkToTail(JCDoubleLinkNode &tNode);

    /******************************************************************************
    * 函数名:JCDbusMsg.BreakLink
    * 描述: 断开消息链接,将消息从链表取下
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/02 17:0:41, lys 创建函数
    *
    ******************************************************************************/
	void BreakLink();

    /******************************************************************************
    * 函数名:JCDbusMsg.SetMsgInfo
    * 描述: 设置消息头
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/02 17:1:26, lys 创建函数
    *
    ******************************************************************************/
	void SetMsgInfo(const JCDbusMsgInfo &tInfo);

    /******************************************************************************
    * 函数名:JCDbusMsg.MsgInfo
    * 描述: 获取消息头结构
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/02 17:1:50, lys 创建函数
    *
    ******************************************************************************/
	JCDbusMsgInfo &MsgInfo();

    /******************************************************************************
    * 函数名:JCDbusMsg.SetMsgData
    * 描述: 设置消息数据
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/02 17:2:12, lys 创建函数
    *
    ******************************************************************************/
	void SetMsgData(const JCDbusMsgData &tData);

    /******************************************************************************
    * 函数名:JCDbusMsg.MsgData
    * 描述: 获取消息数据
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/02 17:2:32, lys 创建函数
    *
    ******************************************************************************/
	JCDbusMsgData &MsgData();

	unsigned int GetRecvDaemonId();
	unsigned int GetSendDaemonId();
	unsigned int GetRecvModuleId();
	unsigned int GetSendModuleId();
	unsigned int GetMessageId();
	unsigned int GetClientId();

    /******************************************************************************
    * 函数名:JCDbusMsg.ShowMassage
    * 描述: 显示消息信息,供调试用
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/02 17:2:55, lys 创建函数
    *
    ******************************************************************************/
	void ShowMassage();

	JCDoubleLinkNode m_tLinkNode;
	JCDbusMsgInfo m_tMsgInfo;
	JCDbusMsgData m_tMsgData;
};

#endif /* _JCDBUSMSG_H_ */

