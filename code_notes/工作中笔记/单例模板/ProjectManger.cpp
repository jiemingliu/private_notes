#include "stdafx.h"
#include ".\projectmanger.h"
#include "LoadLib.h"
#include "ProjectParam.h"
#include "DataRoot.h"
#include "DataOperation.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include "Commond_String.h"
#include "SimDataSet.h"
#include "../Data/DataContainer.h"
#include "../Data/GridData.h"
#include "../Data/GridRecive.h"
#include "../Data/DataCalculate.h"
#include "../Data/FaultData.h"
#include "../Data/FlowLineData.h"
#include "../Data/WellData.h"
#include "LocalRes.h"
#include <boost/function.hpp>
#include "CLgrData.h"

typedef std::map<CString,void*> MapDataClass;
static MapDataClass s_mapDataClass;

CProjectManger::CProjectManger(void)
{
	m_actDataRoot = NULL;
}

CProjectManger::~CProjectManger(void)
{
	DataRoots::iterator itDataRoot = m_dataRoots.begin();
	for (;itDataRoot != m_dataRoots.end();itDataRoot++)
	{
		delete itDataRoot->second;
	}
	m_dataRoots.clear();
}

void CProjectManger::ClearProjectMangerData()
{
	DataRoots::iterator itDataRoot = m_dataRoots.begin();
	for (;itDataRoot != m_dataRoots.end();itDataRoot++)
	{
		delete itDataRoot->second;
	}
	m_dataRoots.clear();
	
	DataOperations::iterator itDataOperations = m_dataOperations.begin();
	for(;itDataOperations != m_dataOperations.end();itDataOperations++)
	{
		delete itDataOperations->second;
	}
	m_dataOperations.clear();

	m_actDataRoot = NULL;
	m_strDatatypeParam = _T("");
	m_strProjectName = _T("");
	m_strMainCase = _T("");
	m_strPrjPath = _T("");	
}
//zg: create data roots(CDataEclipse) for all cases
BOOL CProjectManger::ReadProjectInfo(CString& modelPath)
{
	ProjectParam projectParam;
	
	//CDataOperation *dd = GetDataOperation();
	if ( GetDataOperation()->ReadProjectInfo(modelPath, projectParam) == FALSE )
	{
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////
	//加载模型之前, 释放上一次加载的模型的内存
	for (unsigned int i = 0; i < m_dataRoots.size(); i++)
	{
		delete m_dataRoots[i];
	}

	m_dataRoots.clear();
	///////////////////////////////////////////////////////////////////////

	
	//设置项目信息
	m_strProjectName = projectParam.strProjectName;
	m_strMainCase = projectParam.strMainCaseName;
	m_strPrjPath = projectParam.strProjectPath;
	

	//首先设置主方案,其余方案号依次递增
	unsigned int nCaseID = 1;

	//根据传出参数构造项目信息及对应的DataRoot对象
	for (size_t nIndex = 0;nIndex < projectParam.caseNames.size();nIndex++)
	{
		//主方案名称标识为0，其他的
		if (projectParam.caseNames[nIndex] == projectParam.strMainCaseName)
		{
			ASSERT(m_dataRoots.find(0) == m_dataRoots.end());

			m_dataRoots[0] = GetDataOperation()->CreateDataRoot(projectParam.caseNames[nIndex]);
		}
		else
		{
			m_dataRoots[nCaseID] = GetDataOperation()->CreateDataRoot(projectParam.caseNames[nIndex]);

			nCaseID++;
		}
	}
	
	//设置当前活动的方案
	SetActiveCase(m_strMainCase);

	return TRUE;
}

CDataRoot* CProjectManger::GetActiveCase()
{
	//	ASSERT(m_actDataRoot != NULL);

	return m_actDataRoot;
}

void CProjectManger::SetActiveCase(const CString& strCaseName)
{
	if (strCaseName.IsEmpty())
	{
		return;
	}

	DataRoots::iterator itDataRoot = m_dataRoots.begin();
	for (;itDataRoot != m_dataRoots.end();itDataRoot++)
	{
		if (itDataRoot->second->GetTitle() == strCaseName)
		{
			m_actDataRoot = itDataRoot->second;
			return;
		}
	}
	//不可能找不到当前方案
	ASSERT(FALSE);
}

CDataRoot* CProjectManger::GetDataCase( const CString& strCaseName )
{
	DataRoots::iterator itDataRoot = m_dataRoots.begin();
	for (;itDataRoot != m_dataRoots.end();itDataRoot++)
	{
		if (itDataRoot->second->GetTitle() == strCaseName)
		{
			return itDataRoot->second;
		}
	}

	//不可能找不到当前方案
	//ASSERT(FALSE);
	return NULL;
}

CDataRoot* CProjectManger::GetDataCase( unsigned int nCaseID )
{
	if (m_dataRoots.find(nCaseID) != m_dataRoots.end())
	{
		return m_dataRoots[nCaseID];
	}

	return NULL;
}

CaseNos CProjectManger::GetCaseNos()
{
	CaseNos caseNos;
	DataRoots::iterator itDataRoot = m_dataRoots.begin();
	for (;itDataRoot != m_dataRoots.end();itDataRoot++)
	{
		caseNos.push_back(itDataRoot->first);
	}
	return caseNos;
}

CaseNames CProjectManger::GetCaseNames()
{
	CaseNames caseNames;
	DataRoots::iterator itDataRoot = m_dataRoots.begin();
	for (;itDataRoot != m_dataRoots.end();itDataRoot++)
	{
		caseNames.push_back(itDataRoot->second->GetTitle());
	}
	return caseNames;
}

CString CProjectManger::GetProjectName()
{
	return m_strProjectName;
}

CString CProjectManger::GetMainCaseName()
{
	return m_strMainCase;
}



const CString& CProjectManger::GetPrjPath()
{
	return m_strPrjPath;
}

CDataRoot* CProjectManger::GetMainDataCase()
{
	return GetDataCase(m_strMainCase);
}

CString CProjectManger::GetCaseNameByNoID( unsigned int nCaseID )
{
	ASSERT(m_dataRoots.find(nCaseID) != m_dataRoots.end());
	return m_dataRoots[nCaseID]->GetTitle();
}

unsigned int CProjectManger::GetCaseNoByNoName( const CString& strCaseName )
{
	DataRoots::iterator itDataRoot = m_dataRoots.begin();
	for (;itDataRoot != m_dataRoots.end();itDataRoot++)
	{
		if (itDataRoot->second->GetTitle() == strCaseName)
		{
			return itDataRoot->first;
		}
	}

	//不可能找不到当前方案
	ASSERT(FALSE);
	return NULL;
}



BOOL CProjectManger::WriteProjectInfo()
{
	GetDataOperation()->SaveDataRoot(SaveAll);

	//try
	//{
	//	CString strFile = m_strPrjPath + m_strProjectName + ".smg";
	//	std::ofstream fs(strFile.GetBuffer(),std::ios::binary);
	//	assert(fs.good());
	//	boost::archive::binary_oarchive archive(fs);
	//	//序列化写入
	//	Serialize(archive);
	//	return TRUE;
	//}
	//catch (std::exception const& e)
	//{
	//	AfxMessageBox(e.what());
	//	return FALSE;
	//}

	return TRUE;
}

CDataRoot* CProjectManger::CreateNewDataCase( const CString& strCaseName )
{
	CDataRoot* pDataCase =  GetDataOperation()->CreateDataRoot(strCaseName);
	//////////////////////////////////////////////////////////////////////////

	ASSERT(pDataCase != NULL);
	if (m_dataRoots.empty())
	{
		m_strMainCase = strCaseName;
	}
	m_dataRoots[m_dataRoots.size()] = pDataCase;
	pDataCase->SetCaseReaded(TRUE);
	return pDataCase;
}

CDataRoot* CProjectManger::CreateModeCase(const CString& strModeName)
{
	CDataRoot* pDataCase =  GetDataOperation()->CreateDataRoot(strModeName);
	//////////////////////////////////////////////////////////////////////////

	ASSERT(pDataCase != NULL);
	if (m_dataRoots.empty())
	{
		m_strMainCase = strModeName;
	}
	m_dataRoots[m_dataRoots.size()] = pDataCase;
	pDataCase->SetCaseReaded(TRUE);
	return pDataCase;
}

BOOL CProjectManger::DeleteDataCase(const CString& strCaseName,BOOL bIsDeleteFile)
{
	return DeleteDataCase(GetDataCase(strCaseName),bIsDeleteFile);
}

BOOL CProjectManger::DeleteDataCase( CDataRoot* dataCase,BOOL bIsDeleteFile)
{
	ASSERT(dataCase != NULL);

	DataRoots::iterator itDataRoot = m_dataRoots.begin();
	for (;itDataRoot != m_dataRoots.end();itDataRoot++)
	{
		if (itDataRoot->second == dataCase)
		{//是否删除打开的文件
	//		if (bIsDeleteFile)
	//		{	//获得打开的文件
	//			CCaseHeadInfo CaseInfo = dataCase->GetCaseHeadInfo();
	//			//取得文件路径
	//			CString strPath = CProjectManger::getSingletonPtr()->GetPrjPath();
	//			//后处理文件
	//			{
	//				const FileNames &files = CaseInfo.GetPoFileNames();
	//				for (size_t size=0;size<files.size();size++)
	//				{
	//					DeleteFile(strPath+files[size]);
	//				}
	//			}
	//			//前处理文件
	//			{
	//				const FileNames &files = CaseInfo.GetPtFileNames();
	//				for (size_t size=0;size<files.size();size++)
	//				{
	//					 DeleteFile(strPath+files[size]);
	//				}
	//			}
	//		}
			m_dataRoots.erase(itDataRoot);
			delete dataCase;
			return TRUE;
		}
	}

	return FALSE;
}

void CProjectManger::RegisterDataClass( const CString& strClassName,void* fun_Create )
{
	ASSERT(!strClassName.IsEmpty());
	ASSERT(fun_Create != NULL);
	ASSERT(s_mapDataClass.find(strClassName) == s_mapDataClass.end());
	s_mapDataClass[strClassName] = fun_Create;
}

void CProjectManger::RegisterDataOperation(CDataOperation* pDataOperation)
{
	ASSERT(pDataOperation != NULL);
	ASSERT(!pDataOperation->GetDataType().IsEmpty());
	ASSERT(m_dataOperations.find(pDataOperation->GetDataType()) == m_dataOperations.end());
	m_dataOperations[pDataOperation->GetDataType()] = pDataOperation;
}



CString CProjectManger::GetDataTypeParam()
{
	return m_strDatatypeParam;
}

void CProjectManger::SetDataTypeParam( const CString& strType )
{
	m_strDatatypeParam = strType;
}

CDataOperation* CProjectManger::GetDataOperation()
{
	ASSERT(m_dataOperations.find(m_strDatatypeParam) != m_dataOperations.end());
	return m_dataOperations[m_strDatatypeParam];
}