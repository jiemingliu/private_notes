/******************************************************************************
* 文件名: jcftpclient.h
* 描述: FTP客户端
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/12/19 9:30:20, lys 创建文件
*
******************************************************************************/
#ifndef _JCFTPCLIENT_H_
#define _JCFTPCLIENT_H_

#include <string>
#include <vector>
#include "jcbase.h"
#include "jcnonecopy.h"


using namespace std;

typedef int (*FtpClientCallbackFunc)(unsigned int nTransferedBytes, void *pVoid);

struct NetBuf;

/******************************************************************************
* 函数名:JCFtpClient.JCFtpClient
* 描述: FTP客户端
*
* 输入:
* 输出:
* 返回值:
*
* 修改记录:
* --------------------
*    2014/12/26 18:51:54, lys 创建函数
*
******************************************************************************/
class DLL_EXPORT JCFtpClient : public JCNonecopy
{
public:
	enum ConnectMode
	{
		PASSIVE_MODE,
		PORT_MODE
	};

	enum TransMode
	{
		ASCII_MODE,
		BINARY_MODE
	};

	JCFtpClient();
	JCFtpClient(const string &strFtpIp, const string &strFtpPort = "21");
	virtual ~JCFtpClient();

	/******************************************************************************
    * 函数名:JCFtpClient.Connect
    * 描述: 连接FTP服务器
    *
    * 输入: IP和端口
    * 输出: strResponse服务端返回的应答信息
    * 返回值: true 成功 false 失败
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:52:33, lys 创建函数
    *
    ******************************************************************************/
	bool Connect(const string &strFtpIp, const string &strFtpPort, string &strResponse);

    /******************************************************************************
    * 函数名:JCFtpClient.Connect
    * 描述: 连接FTP服务器
    *
    * 输入:
    * 输出: strResponse服务端返回的应答信息
    * 返回值: true 成功 false 失败
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:52:33, lys 创建函数
    *
    ******************************************************************************/
	bool Connect(string &strResponse);

    /******************************************************************************
    * 函数名:JCFtpClient.Disconnect
    * 描述: -
    *
    * 输入:
    * 输出:strResponse服务端返回的应答信息
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:53:38, lys 创建函数
    *
    ******************************************************************************/
	void Disconnect(string &strResponse);

    /******************************************************************************
    * 函数名:JCFtpClient.Login
    * 描述: 登录
    *
    * 输入:
    * 输出:strResponse服务端返回的应答信息
    * 返回值:true 成功 false 失败
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:53:38, lys 创建函数
    *
    ******************************************************************************/
	bool Login(const string &strUser, const string &strPsw, string &strResponse);

    /******************************************************************************
    * 函数名:JCFtpClient.SetConnectMode
    * 描述: 设置FTP连接模式(默认被动模式)
    *
    * 输入:
    * 输出:
    * 返回值:true 成功 false 失败
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:53:38, lys 创建函数
    *
    ******************************************************************************/
	bool SetConnectMode(ConnectMode eMode);

    /******************************************************************************
    * 函数名:JCFtpClient.RemoteSysType
    * 描述: 获取FTP服务器所在的系统类型
    *
    * 输入:
    * 输出: strSysType带系统类型的描述字段 strResponse服务端返回的应答信息
    * 返回值:true 成功 false 失败
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:53:38, lys 创建函数
    *
    ******************************************************************************/
	bool RemoteSysType(string &strSysType, string &strResponse);

    /******************************************************************************
    * 函数名:JCFtpClient.Changedir
    * 描述: 跳转到指定目录
    *
    * 输入:
    * 输出: strResponse服务端返回的应答信息
    * 返回值:true 成功 false 失败
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:53:38, lys 创建函数
    *
    ******************************************************************************/
	bool Changedir(const string &strDestPath, string &strResponse);

    /******************************************************************************
    * 函数名:JCFtpClient.ChangeDirUp
    * 描述: 跳转到当前目录的上一级目录
    *
    * 输入:
    * 输出: strResponse服务端返回的应答信息
    * 返回值:true 成功 false 失败
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:53:38, lys 创建函数
    *
    ******************************************************************************/
	bool ChangeDirUp(string &strResponse);

    /******************************************************************************
    * 函数名:JCFtpClient.MakeDir
    * 描述: 创建目录, 无完整路径时在当前目录下创建(目录形式/a/b/c)
    *
    * 输入:
    * 输出: strResponse服务端返回的应答信息
    * 返回值:true 成功 false 失败
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:53:38, lys 创建函数
    *
    ******************************************************************************/
	bool MakeDir(const string &strMakePath, string &strResponse);

    /******************************************************************************
    * 函数名:JCFtpClient.RemoveDir
    * 描述: 移除目录, 无完整路径时在当前目录下移除(目录形式/a/b/c)
    *
    * 输入:
    * 输出: strResponse服务端返回的应答信息
    * 返回值:true 成功 false 失败
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:53:38, lys 创建函数
    *
    ******************************************************************************/
	bool RemoveDir(const string &strRmPath, string &strResponse);

    /******************************************************************************
    * 函数名:JCFtpClient.RemoveDir
    * 描述: 获取当前路径
    *
    * 输入:
    * 输出: strResponse服务端返回的应答信息
    * 返回值:true 成功 false 失败
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:53:38, lys 创建函数
    *
    ******************************************************************************/
	bool CurrentDir(string &strCurrentPath, string &strResponse);

    /******************************************************************************
    * 函数名:JCFtpClient.ListDir
    * 描述: 获取指定路径下的目录列表(只包含目录,不包含文件,使用FTP NLST命令,有的服务器不支持该命令)
    *       , 不指定目录情况下列举当前目录
    * 输入:
    * 输出: strResponse服务端返回的应答信息
    * 返回值:true 成功 false 失败
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:53:38, lys 创建函数
    *
    ******************************************************************************/
	bool ListDir(const string &strRemotePath, vector<string> &tDirSet, string &strResponse);

    /******************************************************************************
    * 函数名:JCFtpClient.ListDirDetail
    * 描述: 获取指定路径下的目录及文件列表(使用FTP LIST及MLSD命令), 不指定目录
    *       时列举当前目录下的目录(不指定目录即目录空字符串为空字符串)
    * 输入:
    * 输出: strResponse服务端返回的应答信息
    * 返回值:true 成功 false 失败
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:53:38, lys 创建函数
    *
    ******************************************************************************/
	bool ListDirDetail(const string &strRemotePath, vector<string> &tDirSet, string &strResponse);
	//bool ListFile(const string &strRemotePath, vector<string> &tFileSet, string &strResponse);

    /******************************************************************************
    * 函数名:JCFtpClient.GetFile
    * 描述: 下载服务器上文件到本地
    *
    * 输入:eMode 传输模式, 字符模式及二进制模式
    * 输出: strResponse服务端返回的应答信息
    * 返回值:true 成功 false 失败
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:53:38, lys 创建函数
    *
    ******************************************************************************/
	bool GetFile(const string &strLocalFile, const string &strRemoteFile, TransMode eMode, string &strResponse);

    /******************************************************************************
    * 函数名:JCFtpClient.PutFile
    * 描述: 上传本地文件到服务器
    *
    * 输入:eMode 传输模式, 字符模式及二进制模式
    * 输出: strResponse服务端返回的应答信息
    * 返回值:true 成功 false 失败
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:53:38, lys 创建函数
    *
    ******************************************************************************/
    bool PutFile(const string &strLocalFile, const string &strRemoteFile, TransMode eMode, string &strResponse);

    /******************************************************************************
    * 函数名:JCFtpClient.DeleteFile
    * 描述: 删除服务器上文件
    *
    * 输入:
    * 输出: strResponse服务端返回的应答信息
    * 返回值:true 成功 false 失败
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:53:38, lys 创建函数
    *
    ******************************************************************************/
    bool DelFile(const string &strRemoteFile, string &strResponse);

    /******************************************************************************
    * 函数名:JCFtpClient.RenameFile
    * 描述: 重命名服务器上指定文件
    *
    * 输入:
    * 输出: strResponse服务端返回的应答信息
    * 返回值:true 成功 false 失败
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:53:38, lys 创建函数
    *
    ******************************************************************************/
    bool RenameFile(const string &strCurrentName, const string &strDesireName, string &strResponse);

    /******************************************************************************
    * 函数名:JCFtpClient.RemoteFileSize
    * 描述: 获取服务器上指定文件大小
    *
    * 输入:
    * 输出: strResponse服务端返回的应答信息
    * 返回值:true 成功 false 失败
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:53:38, lys 创建函数
    *
    ******************************************************************************/
    bool RemoteFileSize(const string &strRemoteFile, unsigned int &nSize, TransMode eMode, string &strResponse);

    /******************************************************************************
    * 函数名:JCFtpClient.RegisterCallBack
    * 描述: 为FTP连接注册回调函数,在每传输nTriggerBytes字节或每隔nTriggerTime时长时会
    *       周期性触发, 若回调函数返回0, 则周期调用退出(一般用于传输大文件显示进度用)
    * 输入:
    * 输出:
    * 返回值:true 成功 false 失败
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:53:38, lys 创建函数
    *
    ******************************************************************************/
	bool RegisterCallBack(FtpClientCallbackFunc pCallBack, void *pVoid, unsigned int nTriggerBytes, unsigned int nTriggerTime);

	/******************************************************************************
    * 函数名:JCFtpClient.UnregisterCallBack
    * 描述: 清除注册的回调函数, 回调清除即失效
	*
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 18:53:38, lys 创建函数
    *
    ******************************************************************************/
	void UnregisterCallBack();

    /******************************************************************************
    * 函数名:JCFtpClient.FtpIp
    * 描述: 获取FTP服务器地址
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 19:8:49, lys 创建函数
    *
    ******************************************************************************/
	string &FtpIp();

    /******************************************************************************
    * 函数名:JCFtpClient.FtpPort
    * 描述: 获取FTP服务器端口号
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 19:8:49, lys 创建函数
    *
    ******************************************************************************/
	string &FtpPort();

    /******************************************************************************
    * 函数名:JCFtpClient.FtpUser
    * 描述: 获取FTP服务器用户名
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 19:8:49, lys 创建函数
    *
    ******************************************************************************/
	string &FtpUser();

    /******************************************************************************
    * 函数名:JCFtpClient.FtpUser
    * 描述: 获取FTP服务器密码
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/12/26 19:8:49, lys 创建函数
    *
    ******************************************************************************/
	string &FtpUserPsw();

	static int FtpClientCallBack(NetBuf *ctl, unsigned int xfered, void *arg);

private:
	bool IsFtpCtrlConnValid();
	char TransferMode(TransMode eMode);
	string LastFtpResponse();

private:
	string m_strFtpIp;
	string m_strFtpPort;
	string m_strUser;
	string m_strPsw;
	NetBuf *m_pFtpCtrlConn; /*命令控制连接*/
	NetBuf *m_pFtpDataConn; /*数据传输连接*/
	FtpClientCallbackFunc m_pCallBack;
	void *m_pCallBackArg;
};

class DLL_EXPORT JCFtpLineParse
{
	enum FILE_TYPE
	{
		eUnkonwn,
		eFile,
		eDIR,
	};
public:
	JCFtpLineParse();
	~JCFtpLineParse();
	bool ParseLine(const string &strLine);
	string GetFileDate();
	string GetFileType();
	unsigned int GetFileSize();
	string GetFileName();
	string GetFileAttr();
	vector<string> GetFileGroup();
protected: 
	void MapInit();
	void FilterLeftSpace(unsigned int &nPos,const string &strLine);
	/* linux */
	bool ParseLineForUnix(const string &strLine);
	bool ParseFileType(unsigned int &nPos,const string &strLine,string &strType);
	bool ParseFileAttr(unsigned int &nPos,const string &strLine,string &strAttr);
	bool ParseFileGroup(unsigned int &nPos,const string &strLine,vector<string> &strGroup);
	bool ParseFileSize(unsigned int &nPos,const string &strLine,unsigned int &nSize);
	bool ParseFileDate(unsigned int &nPos,const string &strLine,string &strDate);
	bool ParseFileName(unsigned int &nPos,const string &strLine,string &strName);
	
	/* windows */
	bool ParseLineForDos(const string &strLine);
	bool ParseFileDateForDos(unsigned int &nPos,const string &strLine,string &strDate);
	bool ParseFileSizeForDos(unsigned int &nPos,const string &strLine,unsigned int &nSize);
	bool ParseFileTypeForDos(unsigned int &nPos,const string &strLine,string &strType);
	int  GetTokenForDos(unsigned int nPos,const string &strLine);
private:
	string m_strFileDate;
	unsigned int m_nFileSize;
	string m_strFileType ;
	string m_strFileName ;
	string m_strFileAttr ;
	vector<string> m_strFileGroupVec ;
	map<string,int> m_MonthNamesMap ;
};
#endif /* _JCFTPCLIENT_H_ */



