/******************************************************************************
* 文件名: jcsingleton.hpp
* 描述: 单例模式定义
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/03/19 15:56:39, lys 创建文件
*
******************************************************************************/
#ifndef _JCSINGLETON_H_
#define _JCSINGLETON_H_

#include "jcnonecopy.h"
#include "jcLock.hpp"
#include "jcsmartpointer.hpp"

template <class T>
class JCSingleTon : public JCNonecopy
{
public:
    /******************************************************************************
    * 函数名:JCSingleTon.Ref
    * 描述: 获取唯一实例的引用,该函数并不能保证其所引用的对象是线程安全的,只能
    *       保证其对象的全局唯一性.此引用对象的线程安全应在用于实例化该模板
    *       的类型中实现
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/21 10:39:52, lys 创建函数
    *
    ******************************************************************************/
	static T &Ref()
	{
		return *ConstructInstance();
	}

    /******************************************************************************
    * 函数名:JCSingleTon.Addr
    * 描述: 获取单例对象指针
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/07 11:3:46, lys 创建函数
    *
    ******************************************************************************/
	static T *Addr()
	{
		return ConstructInstance();
	}

protected:
    /******************************************************************************
    * 函数名:JCSingleTon.JCSingleTon
    * 描述: 不支持显式构造类对象,避免虚假构造多个相同对象
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/21 10:37:21, lys 创建函数
    *
    ******************************************************************************/
	JCSingleTon(){}

private:
    /******************************************************************************
    * 函数名:JCSingleTon.ConstructInstance
    * 描述: 构造唯一实例对象 JCBasicPointer指针托管类保证析构时托管指针所指向
    *       内存区域自动释放
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/21 10:37:56, lys 创建函数
    *
    ******************************************************************************/
	static T *ConstructInstance()
	{
		static JCBasicPointer<T> pObject;
		static bool bInitiated(false);
		static JCCriticalSection tCriticalSection;/*静态很重要,若局部变量可能引起多线程竞态*/

		if (!bInitiated)/*双校验模式确保线程安全*/
		{
			JCCSAutoLock tLocked(tCriticalSection);

			if (!bInitiated)
			{
				if (NULL == pObject.m_p)
				{
					pObject.m_p = (new T());/*创建实例*/
					bInitiated = true;
					return pObject.m_p;
				}
			}
		}
		return pObject.m_p;
	}
};


#endif /* _JCSINGLETON_H_ */


