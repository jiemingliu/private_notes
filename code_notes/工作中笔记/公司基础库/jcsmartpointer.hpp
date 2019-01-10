/******************************************************************************
* 文件名: jcsmartpointer.hpp
* 描述: 智能指针
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/03/19 16:16:41, lys 创建文件
*
******************************************************************************/
#ifndef _JCSMARTPOINTER_H_
#define _JCSMARTPOINTER_H_

#include "jcnonecopy.h"

template <class T>
class JCBasicPointer : public JCNonecopy
{
public:
    /******************************************************************************
    * 函数名:JCBasicPointer.JCBasicPointer
    * 描述: 构造指针托管对象
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/21 10:45:6, lys 创建函数
    *
    ******************************************************************************/
	JCBasicPointer()
	: m_p(NULL)
	{

	}

    /******************************************************************************
    * 函数名:JCBasicPointer.~JCBasicPointer
    * 描述: 析构自动释放其托管的内存区域
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/21 10:45:43, lys 创建函数
    *
    ******************************************************************************/
	~JCBasicPointer()
	{
		if (NULL != m_p)
		{
			delete m_p;
			m_p = NULL;
		}
	}

	T *m_p;
};


template <class T>
class JCMemberPointer : public JCNonecopy
{
public:
	/******************************************************************************
	* 函数名:JCMemberPointer.JCMemberPointer
	* 描述: 指针托管类模板扩展
	*
	* 输入:
	* 输出:
	* 返回值:
	*
	* 修改记录:
	* --------------------
	*    2014/03/21 10:48:20, lys 创建函数
	*
	******************************************************************************/
	explicit JCMemberPointer(T *p = NULL)
	: m_p(p)
	{

	}

	~JCMemberPointer()
	{
		Reset(NULL);
	}

	const T& operator*() const
	{
		return *m_p;
	}

	T& operator*()
	{
		return *m_p;
	}

	const T* operator->() const
	{
		return m_p;
	}

	T* operator->()
	{
		return m_p;
	}

	const T* Get() const
	{
		return m_p;
	}

	T* Get()
	{
		return m_p;
	}

	T* Release()
	{
		T *old_p = m_p;
		m_p = 0;
		return old_p;
	}

	void Reset(T *p = 0)
	{
		if (NULL != m_p)
		{
			delete m_p;
		}
		m_p = p;
	}

private:
	T *m_p;
};

#endif /* _JCSMARTPOINTER_H_ */

