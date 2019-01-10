/******************************************************************************
* 文件名: jcexception.h
* 描述: 异常基类类型定义
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/03/18 11:54:34, lys 创建文件
*
******************************************************************************/
#ifndef _JCEXCEPTION_H_
#define _JCEXCEPTION_H_

#include <string>
#include <exception>
#include "jcbase.h"

using namespace std;

class DLL_EXPORT JCException : public exception
{
public:

    /******************************************************************************
    * 函数名:JCException.JCException
    * 描述: 构造函数
    *
    * 输入: e 抛出异常所带的错误编码  s 抛出异常所带的错误描述信息
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/20 14:27:49, lys 创建函数
    *
    ******************************************************************************/
	explicit JCException(unsigned int nExcp, const string &strInfo);
	virtual ~JCException() throw();

    /******************************************************************************
    * 函数名:JCException.ErrMsg
    * 描述: 获取错误信息
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/20 14:22:34, lys 创建函数
    *
    ******************************************************************************/
	virtual const string &ErrMsg() const;

    /******************************************************************************
    * 函数名:JCException.SetErrMsg
    * 描述: 设置错误信息字串
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/20 14:23:30, lys 创建函数
    *
    ******************************************************************************/
	virtual void SetErrMsg(const string &strInfo);

    /******************************************************************************
    * 函数名:JCException.ErrCode
    * 描述: 获取错误编码
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/20 14:24:1, lys 创建函数
    *
    ******************************************************************************/
	virtual unsigned int ErrCode() const;

    /******************************************************************************
    * 函数名:JCException.SetErrCode
    * 描述: 设置错误编码
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/20 14:27:31, lys 创建函数
    *
    ******************************************************************************/
	virtual void SetErrCode(unsigned int e);

protected:
	unsigned int m_nErrorCode;
	string m_strWhat;
};


class DLL_EXPORT JCWinErrException : public JCException
{
public:

    /******************************************************************************
    * 函数名:JCWinErrException.JCWinErrException
    * 描述: Windows错误异常构造函数, 以当前位置GetLastError返回值初始化异常错误码,
    *       其对应错误描述初始化异常信息
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/20 14:28:41, lys 创建函数
    *
    ******************************************************************************/
	explicit JCWinErrException();

	virtual ~JCWinErrException() throw();
	void CleanError();

protected:
    /*Windows错误异常不允许人为改动错误码及描述*/
	virtual void SetErrMsg(const string &strInfo);
	virtual void SetErrCode(unsigned int nExcp);

private:
	string FormatError(unsigned long nErrorCode);
};

#endif /* _JCEXCEPTION_H_ */


