#pragma once
#include "Singleton.h"
#include <vector>
#include "DataRoot.h"

//定义方案名称结构
typedef std::vector<CString> CaseNames;
//定义方案编号
typedef std::vector<unsigned int> CaseNos;

class CDataRoot;
class CDataOperation;

#ifdef COMMON_LIB
#define COMMON_EXPORT __declspec(dllexport)
#else
#define COMMON_EXPORT __declspec(dllimport)
#endif // COMMON_LIB


class COMMON_EXPORT CProjectManger:
	public Singleton<CProjectManger>
{
public:
	CProjectManger(void);
	~CProjectManger(void);

	enum SaveProjFlag
	{
		SaveActive,
		SaveAll
	};
public:
	/**
	 * 方法:	读取工区信息
	 * 全名:	CProjectManger::ReadProjectInfo
	 * 类型:	public 
	 * 
	 * @date	2008/11/03
	 * @author	XX
	 * @return	BOOL
	 */
	BOOL ReadProjectInfo(CString& modelPath);

	/**
	 * 方法:	写入项目数据信息
	 * 全名:	CProjectManger::WriteProjectInfo
	 * 类型:	public 
	 * 
	 * @date	2008/11/13
	 * @author	XX
	 * @return	BOOL
	 */
	BOOL WriteProjectInfo();

	/**
	 * 方法:	创建新的方案
	 * 全名:	CProjectManger::CreateNewDataCase
	 * 类型:	public 
	 * 
	 * @date	2008/11/14
	 * @author	XX
	 * @param	const CString & strCaseName
	 * @return	CDataRoot*
	 */
	CDataRoot* CreateNewDataCase(const CString& strCaseName);

	/**
	* 方法:	创建模型方案数据
	* 全名:	CProjectManger::CreateModeCase
	* 类型:	protected 
	* 
	* @date	  2009/02/16
	* @author	Wangxh
	* @param	  const CString & strModeName
	* @return	CDataRoot*
	*/
	CDataRoot* CreateModeCase(const CString& strModeName);
	/**
	 * 方法:	删除方案
	 * 全名:	CProjectManger::DeleteDataCase
	 * 类型:	public 
	 * 
	 * @date	2008/11/24
	 * @author	XX
	 * @param	const CString & strCaseName
	 * @param	BOOL bIsDeleteFile
	 * @return	BOOL
	 */
	BOOL DeleteDataCase(const CString& strCaseName,BOOL bIsDeleteFile);

	/**
	 * 方法:	删除方案
	 * 全名:	CProjectManger::DeleteDataCase
	 * 类型:	public 
	 * 
	 * @date	2008/11/15
	 * @author	XX
	 * @param	CDataRoot * dataCase
	 * @param	BOOL bIsDeleteFile
	 * @return	BOOL
	 */
	BOOL DeleteDataCase(CDataRoot* dataCase,BOOL bIsDeleteFile);

	/**
	 * 方法:  清空该管理对象中的所有数据
	 * 全名:
	 * 类型:
	 * 
	 * @date
	 * @author
	 * @return
	*/
	void ClearProjectMangerData();

public:
	/**
	 * 方法:	获取数据类型
	 * 全名:	CProjectManger::GetDataTypeParam
	 * 类型:	public 
	 * 
	 * @date	2010/07/16
	 * @author	Administrator
	 * @return	CString
	 */
	CString GetDataTypeParam();

	/**
	 * 方法:	设置数据类型
	 * 全名:	CProjectManger::SetDataTypeParam
	 * 类型:	public 
	 * 
	 * @date	2010/07/16
	 * @author	Administrator
	 * @param	const CString & strType
	 * @return	void
	 */
	void SetDataTypeParam(const CString& strType);

	/**
	 * 方法:	获取项目路径
	 * 全名:	CProjectManger::GetPrjPath
	 * 类型:	public 
	 * 
	 * @date	2008/11/07
	 * @author	XX
	 * @return	const CString&
	 */
	const CString& GetPrjPath();

	/**
	 * 方法:	获取项目名称
	 * 全名:	CProjectManger::GetProjectName
	 * 类型:	public 
	 * 
	 * @date	2008/10/30
	 * @author	XX
	 * @return	CString
	 */
	CString GetProjectName();

	/**
	 * 方法:	获取主方案
	 * 全名:	CProjectManger::GetMainDataCase
	 * 类型:	public 
	 * 
	 * @date	2008/11/07
	 * @author	XX
	 * @return	CDataRoot*
	 */
	CDataRoot* GetMainDataCase();

	/**
	 * 方法:	获取主方案名称
	 * 全名:	CProjectManger::GetMainCaseName
	 * 类型:	public 
	 * 
	 * @date	2008/10/30
	 * @author	XX
	 * @return	CString
	 */
	CString GetMainCaseName();

	/**
	 * 方法:	获取所有方案名称
	 * 全名:	CProjectManger::GetCaseNames
	 * 类型:	public 
	 * 
	 * @date	2008/10/30
	 * @author	XX
	 * @return	CaseNames
	 */
	CaseNames GetCaseNames();

	/**
	 * 方法:	获取项目编号
	 * 全名:	CProjectManger::GetCaseNos
	 * 类型:	public 
	 * 
	 * @date	2008/11/10
	 * @author	XX
	 * @return	CaseNos
	 */
	CaseNos GetCaseNos();

	/**
	 * 方法:	通过方案号码查找方案名称
	 * 全名:	CProjectManger::GetCaseNameByNoID
	 * 类型:	public 
	 * 
	 * @date	2008/11/10
	 * @author	XX
	 * @param	unsigned int nCaseID
	 * @return	CString
	 */
	CString GetCaseNameByNoID(unsigned int nCaseID);

	/**
	 * 方法:	通过方案名称获取方案ID
	 * 全名:	CProjectManger::GetCaseNoByNoName
	 * 类型:	public 
	 * 
	 * @date	2008/11/10
	 * @author	XX
	 * @param	const CString & strCaseName
	 * @return	unsigned int
	 */
	unsigned int GetCaseNoByNoName(const CString& strCaseName);

	/**
	 * 方法:	获取当前活动的数据根对象
	 * 全名:	CProjectManger::GetActiveCase
	 * 类型:	public 
	 * 
	 * @date	2008/10/27
	 * @author	XX
	 * @return	CDataRoot*
	 */
	CDataRoot* GetActiveCase();

	/**
	 * 方法:	通过方案号获取指定方案
	 * 全名:	CProjectManger::GetDataCase
	 * 类型:	public 
	 * 
	 * @date	2008/11/10
	 * @author	XX
	 * @param	unsigned int nCaseID
	 * @return	CDataRoot*
	 */
	CDataRoot* GetDataCase(unsigned int nCaseID);

	/**
	 * 方法:	获取指定方案
	 * 全名:	CProjectManger::GetDataCase
	 * 类型:	public 
	 * 
	 * @date	2008/10/30
	 * @author	XX
	 * @param	const CString & strCaseName
	 * @return	CDataRoot*
	 */
	CDataRoot* GetDataCase(const CString& strCaseName);

	/**
	 * 方法:	设置活动方案名称
	 * 全名:	CProjectManger::SetActiveCase
	 * 类型:	public 
	 * 
	 * @date	2008/10/30
	 * @author	XX
	 * @param	const CString & strCaseName
	 * @return	void
	 */
	void SetActiveCase(const CString& strCaseName);

public:
	/**
	 * 方法:	注册数据类型
	 * 全名:	CProjectManger::RegisterDataClass
	 * 类型:	public 
	 * 
	 * @date	2010/07/07
	 * @author	Administrator
	 * @param	const CString & strClassName
	 * @param	void * fun_Create
	 * @return	void
	 */
	void RegisterDataClass(const CString& strClassName,void* fun_Create);

	/**
	 * 方法:	注册数据操作类
	 * 全名:	CProjectManger::RegisterDataOperation
	 * 类型:	public 
	 * 
	 * @date	2010/08/20
	 * @author	Administrator
	 * @param	CDataOperation * pDataOperation
	 * @return	void
	 */
	void RegisterDataOperation(CDataOperation* pDataOperation);

	/**
	 * 方法:	获取数据操作对象
	 * 全名:	CProjectManger::GetDataOperation
	 * 类型:	protected 
	 * 
	 * @date	2010/08/20
	 * @author	Administrator
	 * @return	CDataOperation*
	 */
	CDataOperation* GetDataOperation();

protected:
	//定义方案集合类型
	typedef std::map<unsigned int,CDataRoot*> DataRoots;
	//方案集合
	DataRoots m_dataRoots;
	//当前活动方案
	CDataRoot* m_actDataRoot;
protected:
	//定义数据操作集合类型
	typedef std::map<CString,CDataOperation*> DataOperations;
	//数据操作集合
	DataOperations m_dataOperations;
protected:
	//当前数据类型
	CString m_strDatatypeParam;
	//当前项目名称
	CString m_strProjectName;
	//主方案名称
	CString m_strMainCase;
	//当前项目路径
	CString m_strPrjPath;
};
