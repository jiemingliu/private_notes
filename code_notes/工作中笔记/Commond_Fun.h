#pragma once
#include <iomanip>
#include <iostream>
#include <fstream>
#include "Ini.h"
#include "Path.h"

//获取应用程序路径
inline CString GetAppPath()
{
	char chPath[MAX_PATH];
	CString strPath;
	int nCount;
	GetModuleFileName(NULL,chPath,MAX_PATH);	//得到执行文件名的全路径
	strPath = chPath;
	nCount = strPath.ReverseFind('\\');
	strPath = strPath.Mid(0, nCount + 1);
	return strPath;
}

//执行模拟器计算
inline void RunEclCalc(const CString& strFileName)
{
	//取得模拟器路径
	CString eclExePath;
	//读取配置文件
	CIni iniFile(GetAppPath()+ _T("\\Option.ini"));
	if ( iniFile.IsSectionExist("Option") == TRUE)
	{//判断是否存在信息
		eclExePath = iniFile.GetString("Option", "EclipseexePath", "");
	}

	if (eclExePath.IsEmpty())
	{
		AfxMessageBox("请先设置 ECLIPSE 模拟器路径!");
		return;
	}

	CFileFind   find;  
	if(!find.FindFile(eclExePath))  
	{
		CString strMsg;
		strMsg.Format("ECLIPSE 模拟器路径: %s 不存在,请重新设置!",eclExePath);
		AfxMessageBox(strMsg);
		find.Close();
		return;
	} 
	find.Close();

	CPath path(strFileName);
	//文件路径
	CString strFilePath = path.GetLocation();
	//文件标题
	CString strFileTittle = path.GetFileTitle();
	//////////////////////////////////////////////////////////////////////////
	CString strTempPathFile;
	GetTempPath(MAX_PATH,strTempPathFile.GetBuffer(MAX_PATH));
	strTempPathFile.ReleaseBuffer();
	strTempPathFile = strTempPathFile + "Launcher.bat";
	//输出到文本
	std::ofstream out(strTempPathFile.GetBuffer());
	//写注释信息
	out << "@ECHO off" << std::endl;
	out << "echo -------------------------------------------------------------------------" << std::endl;
	//写入输出时间
	COleDateTime time = COleDateTime::GetCurrentTime();
	CString strTime = "echo " + time.Format("%Y-%m-%d %H:%M:%S");
	out << strTime.GetBuffer() << std::endl;
	//写入执行信息
	CString strRunInfo = "echo Eclipse dataset " + strFilePath;
	out << strRunInfo.GetBuffer() << std::endl;
	out << "echo -------------------------------------------------------------------------" << std::endl;
	//写执行配置环境信息$eclrc.bat
	out << "call $eclrc.bat" << std::endl;
	//写切换目录消息
	out << "cd " << strFilePath.GetBuffer() << std::endl;
	//写执行命令
	//执行语句
	CString strRun;
	strRun.Format("%s %s",eclExePath,strFileTittle);
	out << strRun.GetBuffer() << std::endl;
	//写入关闭命令
	out << "exit" << std::endl;
	//关闭文件
	out.close();
	CString strParRun;
	strParRun = "/k " + strTempPathFile;	
	//执行批处理
	ShellExecute(NULL,"open","cmd.exe",strParRun.GetBuffer(),strFilePath.GetBuffer(), SW_SHOWNORMAL);
}

inline void StartTools(CString& strToolName)
{
	CString strToolPath;
	//读取配置文件
	CIni iniFile(GetAppPath()+ _T("\\Option.ini"));
	if ( iniFile.IsSectionExist("Option") == TRUE)
	{//判断是否存在信息
		strToolPath = iniFile.GetString("Option",strToolName);
	}
	if (strToolPath.IsEmpty())
	{
		AfxMessageBox("没有设置程序路径,请先在工具选项中设置路径!");
		return;
	}
	//判断该路径是否存在
	CFileFind   find;  
	if(!find.FindFile(strToolPath))  
	{
		CString strMsg;
		strMsg.Format("程序路径:%s不存在,请重新设置!",strToolName);
		AfxMessageBox(strMsg);
		find.Close();
		return;
	} 
	find.Close();

	if (strToolName == "EclipseRunPath")
	{
		//设置参数
		CString strTool = strToolPath;
		strTool += "\\macros\\geoquest.exe";
		CString	strFilePath = strToolPath;
		strFilePath += "\\macros\\RUNECLIPSE.exe";
		//起始
		CString strdir = strToolPath;
		strdir += "\\home"; 
		//执行程序
		ShellExecute(NULL,"open",strTool, strFilePath ,strdir, SW_SHOWNORMAL);
	}
	else
	{
		//执行程序
		ShellExecute(NULL,"open",strToolPath,NULL,NULL, SW_SHOWNORMAL);
	}
}

// 判断文件夹的名字是否有效
inline BOOL IsFolderNameValid( const CString& strFolderName )
{
	if (strFolderName.IsEmpty())
	{
		return FALSE;
	}

	CString strTemp = strFolderName;

	if (strTemp.Find('/') == -1 && 
		strTemp.Find('\\') == -1 && 
		strTemp.Find(':') == -1 && 
		strTemp.Find('*') == -1 && 
		strTemp.Find('?') == -1 && 

		strTemp.Find('"') == -1 && 
		strTemp.Find('>') == -1 && 
		strTemp.Find('<') == -1 && 
		strTemp.Find('|') == -1)
	{
		strTemp.TrimLeft();
		strTemp.TrimRight();
		if (strFolderName == strTemp)
		{
			return TRUE;
		}
	}
	return FALSE;
}

// 判断一个字符串是否是有效的文件目录
inline BOOL IsValidDir(CString& strPath)
{
	CPath path(strPath, TRUE);
	if (path.IsRelativePath())		// 相对路径
	{
		strPath = path.GetAbsolutePath(GetAppPath());
		path.SetPath(strPath);
	}

	// 无效的路径
	if (!path.IsLocalPath() || strPath.GetAt(2) != '\\')
	{
		AfxMessageBox(_T("无效的存储路径"));
		return FALSE;
	}

	CString strTemp = strPath.Right(strPath.GetLength() - 3);
	int nPos = strTemp.Find('\\');
	while (nPos != -1)
	{
		CString strMid = strTemp.Left(nPos);
		if (!IsFolderNameValid(strMid))
		{
			AfxMessageBox(_T("无效的存储路径"));
			return FALSE;
		}	
		strTemp = strTemp.Right(strTemp.GetLength() - nPos - 1);
		nPos = strTemp.Find('\\');
	}

	if (!strTemp.IsEmpty() && !IsFolderNameValid(strTemp))
	{
		AfxMessageBox(_T("无效的存储路径"));
		return FALSE;
	}

	// 路径不存在则提示创建
	if (!path.ExistLocation())
	{
		if (IDNO == AfxMessageBox(_T("路径不存在，是否创建"), MB_YESNO | MB_ICONASTERISK))
		{
			return FALSE;
		}
		SHCreateDirectoryEx(NULL, strPath, NULL);
	}

	if (strPath.Right(1) != _T("\\"))
	{
		strPath.Append("\\");
	}
	return TRUE;
}