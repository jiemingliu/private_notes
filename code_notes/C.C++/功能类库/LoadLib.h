/*------------------------------------------------------------------------------------------------------
* 文件名：						LoadLib.h
* 功能：						加载DLL,以仿函数形式调用DLL里的函数,最多支持10参数
* 作者：						---
* 注意:							使用该类调用DLL时一定要使用try...catch(runtime_error)....语句捕捉异常
* 创建日期:					---
* 最后修改日期：			---
* 使用说明:						
*								try
*								{
*									CLoadLib lib("DLL路径");
*									// 或CLoadLib lib;
*									// lib.LoadLib("DLL路径");
*									lib.call<返回值类型, 调用约定>("调用函数名字", 参数列表(0至10个));
*								}
*								catch(runtime_error e)
*								{
*									AfxMessageBox(e.what());
*								}
*								catch(...)
*								{
*								}
*								// 调用约定见枚举call_convention_e或call_convention_e_
* 使用案例：
	CLoadLib eclDlgLib;
	std::map<CString,KeyInfo>  mapKeys;
	std::map<CString,std::set<CString>> allKeys;
	try
	{
		eclDlgLib.LoadLib("DataEclipseDlg.dll");
		//获取所有关键词信息
		eclDlgLib.call<void,_std_>(_T("GetMapKeys"),&mapKeys,&allKeys);
	}
	catch(std::runtime_error e)
	{
		AfxMessageBox(e.what());
		return FALSE;
	}
-----------------------------------------------------------------------------------------------------*/

#pragma once

#include <iostream>
#include <string>

template<class String>
String get_app_path()
{
	char chPath[MAX_PATH];
	//CString strPath;
	int nCount;

	GetModuleFileName(NULL,chPath,MAX_PATH);	//得到执行文件名的全路径
	strPath = chPath;
	nCount = strPath.ReverseFind('\\');
	strPath = strPath.Mid(0, nCount + 1);
	return strPath;
}

enum call_convention_e_ { _std_ = 0, _cd_ = 1, _fast_ = 2 };
enum call_convention_e  { _stdcall_ = _std_, _cdecl_ = _cd_, _fastcall_ = _fast_ };

class CLoadLib
{
public:
	//////////////////////////////////////////////////////////////////////////
	// 辅助类,用一个整数映射到一个型别
	template<int index> struct type {};

public:
	CLoadLib()
	{
		m_bDetach = false;
		m_hInst = NULL;
	}

	CLoadLib(LPCTSTR lpLibFileName)				// 构造函数,调用HLoadLibrary加载DLL
	{
		m_bDetach = false;
		m_hInst = LoadLib(lpLibFileName);
	}

	~CLoadLib(void)
	{
		if(!m_bDetach)
			FreeLibrary(m_hInst);
		m_hInst = NULL;
	}

	HINSTANCE Detach(void)
	{
		m_bDetach = true;
		return m_hInst;
	}

	HINSTANCE LoadLib(LPCTSTR lpLibFileName)	// 加载DLL
	{
		if(m_hInst != NULL)
		{
			m_bDetach = false;
			FreeLibrary(m_hInst);
			m_hInst = NULL;
		}

		m_hInst = ::LoadLibrary(lpLibFileName);
		if(m_hInst == NULL)
		{
			throw std::runtime_error(std::string(lpLibFileName) + "\n加载DLL失败，请确认是否在当前目录下");
		}

		return m_hInst;
	}

	template<class FunType>
		FunType addr(LPCTSTR lpProcName)		// 得到指定函数类型的地址
	{
		FunType funaddr = (FunType)NULL;

		if(m_hInst == NULL)
		{
			throw std::runtime_error("您还没有加载DLL");
		}
		else
		{
			funaddr = (FunType)::GetProcAddress(m_hInst, lpProcName);
			if(funaddr == (FunType)NULL)
			{
				throw std::runtime_error("调用函数失败，请确认有此函数");
			}
		}

		return funaddr;
	}

public:
	//////////////////////////////////////////////////////////////////////////
	// 对外接口
	template<class RetType, int CallCon>
		RetType call(LPCTSTR lpProcName)
	{
		return call<RetType>(lpProcName, type<CallCon>());
	}

	// 一个参数
	template<class RetType, int CallCon, class T1>
		RetType call(LPCTSTR lpProcName, T1 t1)
	{
		return call<RetType>(lpProcName, t1, type<CallCon>());
	}

	// 二个参数
	template<class RetType, int CallCon, class T1, class T2>
		RetType call(LPCTSTR lpProcName, T1 t1, T2 t2)
	{
		return call<RetType>(lpProcName, t1, t2, type<CallCon>());
	}

	// 三个参数
	template<class RetType, int CallCon, class T1, class T2, class T3>
		RetType call(LPCTSTR lpProcName, T1 t1, T2 t2, T3 t3)
	{
		return call<RetType>(lpProcName, t1, t2, t3, type<CallCon>());
	}

	// 四个参数
	template<class RetType, int CallCon, class T1, class T2, class T3, class T4>
		RetType call(LPCTSTR lpProcName, T1 t1, T2 t2, T3 t3, T4 t4)
	{
		return call<RetType>(lpProcName, t1, t2, t3, t4, type<CallCon>());
	}

	// 五个参数
	template<class RetType, int CallCon, class T1, class T2, class T3, class T4, class T5>
		RetType call(LPCTSTR lpProcName, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
	{
		return call<RetType>(lpProcName, t1, t2, t3, t4, t5, type<CallCon>());
	}

	// 六个参数
	template<class RetType, int CallCon, class T1, class T2, class T3, class T4, class T5, class T6>
		RetType call(LPCTSTR lpProcName, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
	{
		return call<RetType>(lpProcName, t1, t2, t3, t4, t5, t6, type<CallCon>());
	}

	// 七个参数
	template<class RetType, int CallCon, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
		RetType call(LPCTSTR lpProcName, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
	{
		return call<RetType>(lpProcName, t1, t2, t3, t4, t5, t6, t7, type<CallCon>());
	}	

	// 八个参数
	template<class RetType, int CallCon, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
		RetType call(LPCTSTR lpProcName, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
	{
		return call<RetType>(lpProcName, t1, t2, t3, t4, t5, t6, t7, t8, type<CallCon>());
	}

	// 九个参数
	template<class RetType, int CallCon, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
		RetType call(LPCTSTR lpProcName, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
	{
		return call<RetType>(lpProcName, t1, t2, t3, t4, t5, t6, t7, t8, t9, type<CallCon>());
	}

	// 十个参数
	template<class RetType, int CallCon, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
		RetType call(LPCTSTR lpProcName, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10)
	{
		return call<RetType>(lpProcName, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, type<CallCon>());
	}

private:
	//////////////////////////////////////////////////////////////////////////
	// 对__stdcall调用约定的实现
	template<class RetType>
		RetType call(LPCTSTR lpProcName, type<_stdcall_>)
	{
		typedef RetType (__stdcall* LPProcName)();
		return addr<LPProcName>(lpProcName)();
	}

	// 一个参数
	template<class RetType, class T1>
		RetType call(LPCTSTR lpProcName, T1& t1, type<_stdcall_>)
	{
		typedef RetType (__stdcall* LPProcName)(T1);
		return addr<LPProcName>(lpProcName)(t1);
	}	

	// 二个参数
	template<class RetType, class T1, class T2>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, type<_stdcall_>)
	{
		typedef RetType (__stdcall* LPProcName)(T1, T2);
		return addr<LPProcName>(lpProcName)(t1, t2);
	}

	// 三个参数
	template<class RetType, class T1, class T2, class T3>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, type<_stdcall_>)
	{
		typedef RetType (__stdcall* LPProcName)(T1, T2, T3);
		return addr<LPProcName>(lpProcName)(t1, t2, t3);
	}

	// 四个参数
	template<class RetType, class T1, class T2, class T3, class T4>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, type<_stdcall_>)
	{
		typedef RetType (__stdcall* LPProcName)(T1, T2, T3, T4);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4);
	}

	// 五个参数
	template<class RetType, class T1, class T2, class T3, class T4, class T5>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, type<_stdcall_>)
	{
		typedef RetType (__stdcall* LPProcName)(T1, T2, T3, T4, T5);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4, t5);
	}

	// 六个参数
	template<class RetType, class T1, class T2, class T3, class T4, class T5, class T6>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, type<_stdcall_>)
	{
		typedef RetType (__stdcall* LPProcName)(T1, T2, T3, T4, T5, T6);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4, t5, t6);
	}

	// 七个参数
	template<class RetType, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, type<_stdcall_>)
	{
		typedef RetType (__stdcall* LPProcName)(T1, T2, T3, T4, T5, T6, T7);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4, t5, t6, t7);
	}	

	// 八个参数
	template<class RetType, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, type<_stdcall_>)
	{
		typedef RetType (__stdcall* LPProcName)(T1, T2, T3, T4, T5, T6, T7, T8);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4, t5, t6, t7, t8);
	}

	// 九个参数
	template<class RetType, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9, type<_stdcall_>)
	{
		typedef RetType (__stdcall* LPProcName)(T1, T2, T3, T4, T5, T6, T7, T8, T9);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4, t5, t6, t7, t8, t9);
	}

	// 十个参数
	template<class RetType, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9, T10& t10, type<_stdcall_>)
	{
		typedef RetType (__stdcall* LPProcName)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
	}

	//////////////////////////////////////////////////////////////////////////
	// 对__cdecl调用约定的实现
	template<class RetType>
		RetType call(LPCTSTR lpProcName, type<_cdecl_>)
	{
		typedef RetType (__cdecl* LPProcName)();
		return addr<LPProcName>(lpProcName)();
	}

	// 一个参数
	template<class RetType, class T1>
		RetType call(LPCTSTR lpProcName, T1& t1, type<_cdecl_>)
	{
		typedef RetType (__cdecl* LPProcName)(T1);
		return addr<LPProcName>(lpProcName)(t1);
	}	

	// 二个参数
	template<class RetType, class T1, class T2>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, type<_cdecl_>)
	{
		typedef RetType (__cdecl* LPProcName)(T1, T2);
		return addr<LPProcName>(lpProcName)(t1, t2);
	}

	// 三个参数
	template<class RetType, class T1, class T2, class T3>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, type<_cdecl_>)
	{
		typedef RetType (__cdecl* LPProcName)(T1, T2, T3);
		return addr<LPProcName>(lpProcName)(t1, t2, t3);
	}

	// 四个参数
	template<class RetType, class T1, class T2, class T3, class T4>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, type<_cdecl_>)
	{
		typedef RetType (__cdecl* LPProcName)(T1, T2, T3, T4);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4);
	}

	// 五个参数
	template<class RetType, class T1, class T2, class T3, class T4, class T5>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, type<_cdecl_>)
	{
		typedef RetType (__cdecl* LPProcName)(T1, T2, T3, T4, T5);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4, t5);
	}

	// 六个参数
	template<class RetType, class T1, class T2, class T3, class T4, class T5, class T6>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, type<_cdecl_>)
	{
		typedef RetType (__cdecl* LPProcName)(T1, T2, T3, T4, T5, T6);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4, t5, t6);
	}

	// 七个参数
	template<class RetType, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, type<_cdecl_>)
	{
		typedef RetType (__cdecl* LPProcName)(T1, T2, T3, T4, T5, T6, T7);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4, t5, t6, t7);
	}	

	// 八个参数
	template<class RetType, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, type<_cdecl_>)
	{
		typedef RetType (__cdecl* LPProcName)(T1, T2, T3, T4, T5, T6, T7, T8);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4, t5, t6, t7, t8);
	}

	// 九个参数
	template<class RetType, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9, type<_cdecl_>)
	{
		typedef RetType (__cdecl* LPProcName)(T1, T2, T3, T4, T5, T6, T7, T8, T9);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4, t5, t6, t7, t8, t9);
	}

	// 十个参数
	template<class RetType, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9, T10& t10, type<_cdecl_>)
	{
		typedef RetType (__cdecl* LPProcName)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
	}

	//////////////////////////////////////////////////////////////////////////
	// 对__fastcall调用约定的实现
	template<class RetType>
		RetType call(LPCTSTR lpProcName, type<_fastcall_>)
	{
		typedef RetType (__fastcall* LPProcName)();
		return addr<LPProcName>(lpProcName)();
	}

	// 一个参数
	template<class RetType, class T1>
		RetType call(LPCTSTR lpProcName, T1& t1, type<_fastcall_>)
	{
		typedef RetType (__fastcall* LPProcName)(T1);
		return addr<LPProcName>(lpProcName)(t1);
	}	

	// 二个参数
	template<class RetType, class T1, class T2>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, type<_fastcall_>)
	{
		typedef RetType (__fastcall* LPProcName)(T1, T2);
		return addr<LPProcName>(lpProcName)(t1, t2);
	}

	// 三个参数
	template<class RetType, class T1, class T2, class T3>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, type<_fastcall_>)
	{
		typedef RetType (__fastcall* LPProcName)(T1, T2, T3);
		return addr<LPProcName>(lpProcName)(t1, t2, t3);
	}

	// 四个参数
	template<class RetType, class T1, class T2, class T3, class T4>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, type<_fastcall_>)
	{
		typedef RetType (__fastcall* LPProcName)(T1, T2, T3, T4);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4);
	}

	// 五个参数
	template<class RetType, class T1, class T2, class T3, class T4, class T5>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, type<_fastcall_>)
	{
		typedef RetType (__fastcall* LPProcName)(T1, T2, T3, T4, T5);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4, t5);
	}

	// 六个参数
	template<class RetType, class T1, class T2, class T3, class T4, class T5, class T6>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, type<_fastcall_>)
	{
		typedef RetType (__fastcall* LPProcName)(T1, T2, T3, T4, T5, T6);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4, t5, t6);
	}

	// 七个参数
	template<class RetType, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, type<_fastcall_>)
	{
		typedef RetType (__fastcall* LPProcName)(T1, T2, T3, T4, T5, T6, T7);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4, t5, t6, t7);
	}	

	// 八个参数
	template<class RetType, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, type<_fastcall_>)
	{
		typedef RetType (__fastcall* LPProcName)(T1, T2, T3, T4, T5, T6, T7, T8);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4, t5, t6, t7, t8);
	}

	// 九个参数
	template<class RetType, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9, type<_fastcall_>)
	{
		typedef RetType (__fastcall* LPProcName)(T1, T2, T3, T4, T5, T6, T7, T8, T9);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4, t5, t6, t7, t8, t9);
	}

	// 十个参数
	template<class RetType, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
		RetType call(LPCTSTR lpProcName, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9, T10& t10, type<_fastcall_>)
	{
		typedef RetType (__fastcall* LPProcName)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
		return addr<LPProcName>(lpProcName)(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
	}

public:
	HINSTANCE m_hInst;
private:
	bool	  m_bDetach;
};