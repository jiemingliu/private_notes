/******************************************************************************
* 文件名: jcbase.h
* 描述: 基础库公用头文件
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/03/14 12:1:8, lys 创建文件
*
******************************************************************************/
#ifndef _JCBASE_H_
#define _JCBASE_H_

#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
#include <linux/limits.h>
#define  DLL_EXPORT 
#define  WINAPI
#define  INVALID_HANDLE_VALUE -1
#define Sleep(time) usleep(time*1000)
#define MAX_PATH PATH_MAX
typedef void * HANDLE;
typedef unsigned short WORD;
typedef unsigned char  UINT8;
typedef unsigned short UINT16;
typedef unsigned int   UINT32;
typedef unsigned int   DWORD;
typedef unsigned int   UINT;
typedef unsigned char  BYTE;
typedef long long	   __int64;

#else

/*屏蔽告警*/
#pragma warning(disable: 4251 4275 4996)

#define  DLL_EXPORT __declspec( dllexport )
#endif

#define DEBUG_VERSION
#undef DEBUG_VERSION/*避免影响自动单元测试用例结果解析,本地测试时自己打开即可*/
#define DEBUG_TIMER
#undef DEBUG_TIMER

/*导出符号*/

#endif /* _JCBASE_H_ */


