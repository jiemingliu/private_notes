/******************************************************************************
* 文件名: jcLock.hpp
* 描述: 临界区、互斥体、智能锁等线程安全类型
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/03/19 11:2:3, lys 创建文件
*
******************************************************************************/
#ifndef _JCLOCK_H_
#define _JCLOCK_H_

#include "jcnonecopy.h"

#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
#include <pthread.h>
#define  CRITICAL_SECTION     pthread_mutex_t
#define  EnterCriticalSection pthread_mutex_lock  
#define  LeaveCriticalSection pthread_mutex_unlock 
#define  DeleteCriticalSection pthread_mutex_destroy
#else
#include <windows.h>
#endif

class JCCriticalSection
{
public:
    /******************************************************************************
    * 函数名:JCCriticalSection.JCCriticalSection
    * 描述: Windows用户态临界段封装.线程间互斥和同步优选用户态方式,不需用户态内核态
    *       切换,效率高
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/21 10:29:1, lys 创建函数
    *
    ******************************************************************************/
	JCCriticalSection()
	{
#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
		pthread_mutexattr_init(&m_tMutexAttr); 
		pthread_mutexattr_settype(&m_tMutexAttr , PTHREAD_MUTEX_RECURSIVE_NP);
        pthread_mutex_init(&m_tCriticalSection, &m_tMutexAttr);
#else
		InitializeCriticalSectionAndSpinCount(&m_tCriticalSection, 4000);/*降低等不到资源的线程用户态与内核态切换几率*/
#endif
	}

	~JCCriticalSection()
	{
		DeleteCriticalSection(&m_tCriticalSection);
	}

    /******************************************************************************
    * 函数名:JCCriticalSection.Lock
    * 描述: 进入临界区,加锁
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/21 10:30:43, lys 创建函数
    *
    ******************************************************************************/
	void Lock()
	{
		EnterCriticalSection(&m_tCriticalSection);
	}

    /******************************************************************************
    * 函数名:JCCriticalSection.Unlock
    * 描述: 离开临界区,解锁
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/21 10:31:5, lys 创建函数
    *
    ******************************************************************************/
	void Unlock()
	{
		LeaveCriticalSection(&m_tCriticalSection);
	}

private:
	CRITICAL_SECTION m_tCriticalSection;
#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
	pthread_mutexattr_t m_tMutexAttr;
#endif
};


template <class LockType>
class JCAutoLock : public JCNonecopy
{

public:
    /******************************************************************************
    * 函数名:JCAutoLock.JCAutoLock
    * 描述: 使用锁类型实例化的智能自动锁类模板
    *       对象初始化即自动加锁,析构时自动解锁,若加锁后不进行手动解锁(调用Unlock),保护
    *       区域就是该锁对象生命周期所覆盖区域
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/21 10:31:32, lys 创建函数
    *
    ******************************************************************************/
	JCAutoLock(LockType &tLock)
	:m_tLock(tLock)
	{
		m_tLock.Lock();
	}

    /******************************************************************************
    * 函数名:JCAutoLock.~JCAutoLock
    * 描述: 析构自动解锁
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/21 10:35:6, lys 创建函数
    *
    ******************************************************************************/
	~JCAutoLock()
	{
		m_tLock.Unlock();
	}

    /******************************************************************************
    * 函数名:JCAutoLock.Lock
    * 描述: 手动加锁
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/21 10:35:18, lys 创建函数
    *
    ******************************************************************************/
	void Lock()
	{
		m_tLock.Lock();
	}

    /******************************************************************************
    * 函数名:JCAutoLock.Unlock
    * 描述: 手动解锁
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/21 10:35:33, lys 创建函数
    *
    ******************************************************************************/
	void Unlock()
	{
		m_tLock.Unlock();
	}

private:
	/*注:引用类型,多处使用此锁保护同一资源时,必须保证其用于实例化的锁对象为
	同一个, 且生命期应一直持续到最后一次保护结束*/
	LockType &m_tLock;
};

/*以临界段实例化的智能锁类型*/
typedef JCAutoLock<JCCriticalSection> JCCSAutoLock;

#endif /* _JCLOCK_H_ */

