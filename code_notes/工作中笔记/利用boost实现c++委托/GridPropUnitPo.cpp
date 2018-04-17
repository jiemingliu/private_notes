#include "StdAfx.h"
#include ".\GridPropUnitPo.h"
#include "DataRoot.h"
#include "..\Tools\Datadispose_inline.h"
#include "..\..\CommonTools\Tools\LoadLib.h"
#include <boost/bind.hpp>
#include "..\..\CommonData\GlobalVar.h"
#include "ProgressControl/ProgressControl.h"
#include "HiSimHandle/HiSimHandle.h"
#include "HiSimHandle/KWMapper.h"
#include "Tools/DataEcl.h"
#include "PropTypes.h"

//////////////////////////////////////////////////////////////////////////
//属性数据,每次只有一种属性的数据
TimedGridProp CGridPropUnitPo::m_GridProps;
std::map<long , long> CGridPropUnitPo::m_realOffset;
std::map<long , long> CGridPropUnitPo::m_totalOffset;
std::map<long,long> CGridPropUnitPo::m_realGridIndexOffset;
CString CGridPropUnitPo::m_strCurStoreDataProp = _T("");
CString CGridPropUnitPo::m_strCurStoreDataCase = _T("");
BOOL CGridPropUnitPo::m_bCalcState = FALSE;
BOOL CGridPropUnitPo::m_bCalcAllProp = FALSE;
//////////////////////////////////////////////////////////////////////////

BOOL CGridPropUnitPo::InitOption(Structure::CALC_PARAMSOPTION& options)
{
	CString str;
	::GetModuleFileName(NULL,str.GetBuffer(128),128);
	char diver[5],dir[128];
	_splitpath(str,diver,dir,NULL,NULL);
	CString pathName = diver;
	pathName+=dir;
	pathName+="option.ini";
	return options.LoadOption(pathName, "CalcParams");
}

BOOL CGridPropUnitPo::MakeFileOpen(CString fileName)
{
	//文件被关闭则再打开
	if (m_bFile)
	{
		if ( !m_bFileOpen )
		{
			::SetFileAttributes(fileName ,FILE_ATTRIBUTE_NORMAL); 
			//打开当前时间数据所在文件
			if (m_file.Open(fileName , CFile::typeBinary | CFile::modeReadWrite | CFile::shareDenyNone))
			{
				m_bFileOpen = TRUE;
				return TRUE;
			}
		}
	}
	else
	{
		if ( !m_bFileOpen )
		{
			::SetFileAttributes(fileName ,FILE_ATTRIBUTE_NORMAL); 
			//打开当前时间数据所在文件
			if (m_dateFile.Open(fileName , CFile::typeBinary | CFile::modeRead | CFile::shareDenyNone))
			{
				m_bFileOpen = TRUE;
				return TRUE;
			}
		}
		else
		{
			return TRUE;
		}
	}
	return FALSE;
}

CGridPropUnitPo::CGridPropUnitPo(void)
{
	m_strPropName = "";
	//是否已写入文件
	m_bFile = FALSE;
	m_bNewProp = FALSE;
	m_bReadNewProp = FALSE;
	m_bFlush = FALSE;
	m_pretime.SetDate(0,0,0);
	m_dataType = TRUE;
	m_dateFile.m_hFile = INVALID_HANDLE_VALUE;
	m_file.m_hFile = INVALID_HANDLE_VALUE;
	m_bFileOpen = FALSE;
	m_bTotalGridData = false; //周纵横
}

CGridPropUnitPo::CGridPropUnitPo( const CString& strPropName)
	:CGridPropUnit(strPropName)
{
	m_strPropName = strPropName;
	//是否已写入文件
	m_bFile = FALSE;
	m_bNewProp = FALSE;
	m_bReadNewProp = FALSE;
	m_bFlush = FALSE;
	m_pretime.SetDate(0,0,0);
	m_dataType = TRUE;
	m_dateFile.m_hFile = INVALID_HANDLE_VALUE;
	m_file.m_hFile = INVALID_HANDLE_VALUE;
	m_bFileOpen = FALSE;
}
CGridPropUnitPo::~CGridPropUnitPo(void)
{
	if (m_dateFile.m_hFile != INVALID_HANDLE_VALUE)
	{
		//打开过文件
		m_dateFile.Close();
	}
	if (m_file.m_hFile != INVALID_HANDLE_VALUE)
	{
		m_file.Flush();
		m_file.Close();
	}
}

void CGridPropUnitPo::ClearData()
{
	//清空临时数据
	m_namedGridProps.clear();
}

void CGridPropUnitPo::CreateGridPropData( const CGridRecive& gridRev,const COleDateTime& time,
									   const CString& strPropName )
{
	if (m_strPropName.IsEmpty())
	{
		m_strPropName = strPropName;
	}
	ASSERT(!m_strPropName.IsEmpty());
	ASSERT(strPropName == m_strPropName);	
	//直接根据网格数分配内存
	long lISize = 0;
	long lJSize = 0;
	long lKSize = 0;
	gridRev.GetGridSize(lISize ,lJSize ,lKSize);
	ASSERT(lISize * lJSize * lKSize != 0);

	if ( m_strCurStoreDataProp != m_strPropName )
	{
		//只是临时取的属性,读到m_namedGridProps,再写到文件
		//确定未读取过该时间
		ASSERT(m_namedGridProps.find(time) == m_namedGridProps.end());
		////调整容器大小！
		//m_namedGridProps[time].resize(lISize * lJSize * lKSize,UNACTVALUE);
	}
	else
	{
		//确定未读取过该时间
		ASSERT(m_GridProps.find(time) == m_GridProps.end());
		////调整容器大小！
		//m_GridProps[time].resize(lISize * lJSize * lKSize,UNACTVALUE);
	}	
	if (m_dataOffsets.empty())
	{
		//新建的属性
		m_bNewProp = TRUE;
	}
	if (m_bNewProp && !m_bReadNewProp )
	{
		//新建属性，且不是读属性时调用
		//设置新建属性的文件信息
		m_dataOffsets[time] = 0;
		m_strFiles[time] = _T("");
		WriteData();
	}
}

void CGridPropUnitPo::AddGridPropData( const CGridRecive& gridRev,const COleDateTime& time,const GridNo& gridNo,const double& dValue )
{
	//属性名不能为空
	ASSERT(!m_strPropName.IsEmpty());
	//事务必须开始(相当于多线程的互斥体)
	//ASSERT(gridRev.IsTransactionData());	
	long ISize = 0;
	long JSize = 0;
	long lKsize = 0;
	gridRev.GetGridSize(ISize,JSize,lKsize);
	//网格属性索引
	long gridIndex = gridNo.noK * JSize  * ISize  + gridNo.noJ * ISize + gridNo.noI;

	if (!m_bTotalGridData)
	{
		gridIndex = gridRev.GetGridValueIndex(gridIndex);
	}

	//1  gridNo.noK * JSize  * ISize  + gridNo.noJ * ISize + gridNo.noI
	//2  gridNo.noK * JSize  * ISize  + gridNo.noI * JSize + gridNo.noJ
	//如果是已插值的全部数据，则不需插值
	if ( m_strCurStoreDataProp != m_strPropName )
	{
		//只是临时取的属性,读到m_namedGridProps,再写到文件
		//当前时间步进数据必须存在
		ASSERT(m_namedGridProps.find(time) != m_namedGridProps.end());
		m_namedGridProps[time][gridIndex] = dValue;
	}
	else
	{
		//当前时间步进数据必须存在
		ASSERT(m_GridProps.find(time) != m_GridProps.end());
		m_GridProps[time][gridIndex] = dValue;
	}
	m_bRecal = TRUE;
}

void CGridPropUnitPo::FlushFile()
{
	if ( !m_bFlush )
	{
		//文件缓存未写入
		m_file.Flush();
		//已写入文件
		m_bFlush = TRUE;
	}
}

//修改网格数据
void CGridPropUnitPo::ModifyGridPropData( CGridRecive& gridRev,const COleDateTime& time,
									   const GridNo& gridNo,const double& dValue )									  
{
	ASSERT(!m_strPropName.IsEmpty());	
	
	//检查数据是否要计算
	double resVal;
	CheckAndCalcProp(resVal , gridNo , time ,TRUE);
	//获取网格数
	long ISize = 0;
	long JSize = 0;
	long KSize = 0;
	gridRev.GetGridSize(ISize , JSize , KSize);
	//计算网格索引
	long gridIndex = gridNo.noK * JSize  * ISize  + gridNo.noJ * ISize + gridNo.noI;
	//设置指定网格的属性值	
	COleDateTime  curtime = time;
	if (m_dataOffsets.find(time) == m_dataOffsets.end())
	{
		TimedDataOffset::iterator propIter = m_dataOffsets.end();
		--propIter;
		//时间设为最后一个时间
		curtime = propIter->first;
	}

	if (!m_bTotalGridData)
	{
		gridIndex = gridRev.GetGridValueIndex(gridIndex);
	}

	if (gridIndex == -1) //无效网格
	{
		return;
	}

	//修改文件中的数据
	ModifyGridPropData( curtime, gridIndex, dValue );

	if ( m_strCurStoreDataProp != m_strPropName )
	{
		//只是临时修改的属性,直接写文件
		if (m_bCalcState)
		{
			////计算时修改
			//if (m_namedGridProps.empty())
			//{
			//	ReadData();
			//}
			if (m_namedGridProps[curtime].size() <= gridIndex)
			{
				m_namedGridProps[curtime].resize(gridIndex+1, UNACTVALUE);
			}
			m_namedGridProps[curtime][gridIndex] = dValue; 
		}
		return;
	}

	//修改内存中的数据
	ASSERT(m_GridProps.find(curtime) != m_GridProps.end());
	if(m_GridProps[curtime].size() == 0)
	{
		m_GridProps[curtime].resize(ISize * JSize * KSize, UNACTVALUE);
	}

	m_GridProps[curtime][gridIndex] = dValue;

	//设置要重新计算属性最大最小值
	m_bRecal = TRUE;
}

double CGridPropUnitPo::GetPropValue( const CGridRecive& gridRev,const COleDateTime& time,const GridNo& gridNo )
{
	//查询属性数据时，先查询数据map是否为空
	//如果为空，再查询数据所对应的文件信息是否为空
	//如果为空则读以属性名命名的文件
	//查询到的属性数据
	double dbRes = UNACTVALUE;
	if (!m_bCalcState)
	{
		//不是计算时取值才判断是否有效网格
		//计算时在取值前已判断
		if (!m_pReciveData->IsGridAct(GridNo(gridNo.noI+1 , gridNo.noJ + 1 ,gridNo.noK + 1)))
		{
			//无效网格
			return dbRes;
		}
	}

	//属性值时间	
	COleDateTime  curtime = time;
	//检查数据是否要计算
	if (CheckAndCalcProp(dbRes , gridNo , curtime , m_bCalcAllProp))
	{
		return dbRes;
	}

	if (m_dataOffsets.find(curtime) == m_dataOffsets.end())
	{
		TimedDataOffset::iterator propIter = m_dataOffsets.end();
		--propIter;
		//时间设为最后一个时间
		curtime = propIter->first;
	}

	//当前时间数据所在文件名
	CString fileName;
	fileName = m_strFiles[curtime];
	MakeFileOpen(fileName);

	//网格数
	long ISize = 0;
	long JSize = 0;
	long KSize = 0;

	//获取网格数
	gridRev.GetGridSize(ISize ,JSize ,KSize);

	//属性值索引 注意是long long
	long long gridIndex = gridNo.noK * JSize  * ISize  + gridNo.noJ * ISize + gridNo.noI;

	if ( m_strCurStoreDataProp != m_strPropName )
	{
		if (m_bCalcState)
		{
			//计算时取值
			return GetCalcPropValue( curtime, gridIndex , TRUE ); 
		}

		//只是临时取的属性,读到m_namedGridProps,再写到文件
		if (!m_bFile)
		{
			//未生成文件
			//属性值实际索引
			if (m_realOffset.empty())
			{
				//未生成实际索引对应关系
				GetRealOffset( );
			}

			//当前属性记录所有网格数据时
			if (!m_bTotalGridData)
			{
				gridIndex = m_realOffset[gridIndex];				
			}
			else
			{
				gridIndex = GetTotalGridValueOffset(gridIndex);
			}

			gridIndex += m_dataOffsets[curtime];
			//从原文件里取数据
			float dbData;
			float dbDataRes;
			int    intDataRes;
			if (curtime != m_pretime)
			{
				if (m_bFileOpen )
				{
					if ( m_dateFile.GetFilePath() != fileName)
					{
						//打开过文件则先关闭
						m_dateFile.Close();
						m_bFileOpen = FALSE;
					}					
				}

				if (!m_bFileOpen)
				{
					::SetFileAttributes(fileName ,FILE_ATTRIBUTE_NORMAL); 
					//打开当前时间数据所在文件
					if (m_dateFile.Open(fileName , CFile::typeBinary | CFile::modeRead | CFile::shareDenyNone))
					{
						m_bFileOpen = TRUE;
					}
				}

				m_pretime = curtime;
			}

			if (!m_bFileOpen)
			{
				//AfxMessageBox("文件 " + fileName + " 打开失败");
				return UNACTVALUE;
			}

			m_dateFile.Seek(gridIndex , CFile::begin);
			m_dateFile.Read(&dbData , 4);

			if (m_dataRealType == _T("INTE"))
			{
				intread(&intDataRes,(char*)&dbData);
				return intDataRes;
			}
			else
			{
				realread(&dbDataRes,(char*)&dbData);
				return dbDataRes;
			}			
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
		}
		else
		{
			//缓存写入文件
			FlushFile();
			//查找文件中的数据
			dbRes = GetPropValue( curtime, gridIndex );
		}	

		return dbRes;
	}

	if (m_bCalcState)
	{
		//计算时取值
		return GetCalcPropValue( curtime, gridIndex , FALSE ); 
	}

	//缓存写入文件
	FlushFile();
	//获取指定网格属性值，如该时间无值，则返回最后一个时间的值
	//double tempVal = GetCalcPropValue( curtime, gridIndex , FALSE );
	return GetCalcPropValue( curtime, gridIndex , FALSE ); 
}

std::vector<double> CGridPropUnitPo::getPropValues(const COleDateTime& time,const std::vector<GridNo>& gridNos){
	//属性值时间	
	COleDateTime curtime = time;
	if(m_dataOffsets.find(curtime) == m_dataOffsets.end())
	{
		TimedDataOffset::iterator propIter = m_dataOffsets.end();
		--propIter;
		//时间设为最后一个时间
		curtime = propIter->first;
	}
	std::vector<double> values;
	CString fileName = m_strFiles[time];
	//文件对象
	HANDLE handle = CreateFile(fileName,GENERIC_READ,FILE_SHARE_READ,NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
	if(INVALID_HANDLE_VALUE == handle)
		return values;
	//文件映射对象
	HANDLE fileMap = CreateFileMapping(handle,NULL,PAGE_READONLY,0,0,NULL);
	if(NULL == fileMap){
		CloseHandle(handle);
		return values;
	}
	//得到文件尺寸
	DWORD dwFileSizeHigh;
	__int64 qwFileSize = GetFileSize(handle, &dwFileSizeHigh);
	qwFileSize |= (((__int64)dwFileSizeHigh) << 32); //MSDN

	//偏移量
	__int64 qwFileOffset = 0;

	//得到系统分配粒度
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
	DWORD dwGran = SysInfo.dwAllocationGranularity;

	//块大小
	DWORD dwBlockBytes = 1500 * dwGran;//文件数据分段大小
	if (dwBlockBytes > qwFileSize)
	{
		dwBlockBytes = (DWORD)qwFileSize;
	}

	//映射视图
	char *lpbMapAddress = (char*)MapViewOfFile(fileMap, FILE_MAP_READ, (DWORD)(qwFileOffset >> 32), (DWORD)(qwFileOffset & 0xFFFFFFFF),dwBlockBytes);
	if (NULL == lpbMapAddress){
		CloseHandle(handle);	
		CloseHandle(fileMap);
		return values;
	}
	if (curtime != m_pretime)
		m_pretime = curtime;
	//标志位偏移量
	__int64 start = m_dataOffsets[curtime];
	//起始偏移量
	qwFileOffset = start + 28;
	//终止偏移量
	__int64 endOffset = qwFileOffset + m_dataCount*4 + m_dataCount/1000*8;
	//偏移地址
	lpbMapAddress += qwFileOffset;
	//转换后的网格数据
	float result;
	while(qwFileOffset < endOffset){
		if((qwFileOffset-start)/4%1000 == 0){
			qwFileOffset += 8;
			lpbMapAddress += 8;
			continue;
		}
		realread(&result,lpbMapAddress);
		values.push_back(result);
		qwFileOffset += 4;
		lpbMapAddress += 4;
	}
	CloseHandle(handle);	
	CloseHandle(fileMap);
	if (m_realGridIndexOffset.empty()){
		getGridIndexOffset();
	}
	//网格数
	long lIsize = 0;
	long lJsize = 0;
	long lKsize = 0;
	m_pReciveData->GetGridSize(lIsize,lJsize,lKsize);
	//无效网格个数
	long nUnAct = 0;
	std::vector<double> valueResult(0);
	for(std::vector<GridNo>::const_iterator iter = gridNos.begin();iter != gridNos.end(); ++iter){
		if(m_pReciveData->IsGridAct(*iter)){
			__int64 gridIndex = ((*iter).noK-1) * lIsize  * lJsize + ((*iter).noJ-1) * lIsize + (*iter).noI-1;
			valueResult.push_back(values[m_realGridIndexOffset[gridIndex]]);
			/*char str[50];
			sprintf(str,"网格点：I:%d,J:%d,K:%d,索引值：index:%d ,realIndex:%d\n",(*iter).noI,(*iter).noJ,(*iter).noK,gridIndex,m_realGridIndexOffset[gridIndex]);
			::OutputDebugString(str);*/
		}
		else
			valueResult.push_back(UNACTVALUE);
	}
	return valueResult;
}

double CGridPropUnitPo::GetPropValue( COleDateTime curtime, long gridIndex )
{
	double dbRes = UNACTVALUE;
	//查找文件中的数据
	//取数据
	ASSERT(m_dataOffsets.find(curtime) != m_dataOffsets.end());
	//调整文件指针到数据位置
	m_file.Seek(8 * (gridIndex + 1) + m_dataOffsets[curtime] , CFile::begin); //周纵横
	m_file.Read( &dbRes , 8);
	return dbRes;
}

double CGridPropUnitPo::GetCalcPropValue( COleDateTime curtime, long gridIndex , BOOL btemp )
{
	try
	{
		//当前属性包含所有无效网格数据时不需重定位数据偏移
		if (!m_bTotalGridData /*&& !m_bCalcState*/)
		{
			//如果gridIndex=496, 前面有4个无效网格
			gridIndex = m_pReciveData->GetGridValueIndex(gridIndex); //执行了这一句后gridIndex=492
			if (gridIndex == -1) //无效网格
			{
				return UNACTVALUE;
			}
		}

		if (btemp)
		{
			//不是要显示的属性，数据在临时容器里，用完后会清空
			//计算时取值
			if (m_namedGridProps.find(curtime) == m_namedGridProps.end())
			{
				ReadData(curtime);
			}	

			if (m_strCurStoreDataProp == m_strPropName)
			{
				if (gridIndex < m_GridProps[curtime].size())
				{
					return m_GridProps[curtime][gridIndex]; 			
				}
				else
				{
					return UNACTVALUE;
				}
			}
			else
			{
				if (gridIndex < m_namedGridProps[curtime].size())
				{
					return m_namedGridProps[curtime][gridIndex]; 			
				}
				else
				{
					return UNACTVALUE;
				}
			}
		}

		//非计算时取值
		TimedGridProp::iterator it = m_GridProps.find(curtime);
		if (it == m_GridProps.end())
		{
			ReadData(curtime);
		}

		if (gridIndex < m_GridProps[curtime].size())
		{
			return m_GridProps[curtime][gridIndex]; 		
		}
		else
		{
			return UNACTVALUE;
		}
	}
	catch (...)
	{
		//CString strTime = curtime.Format("%Y-%m-%d");
		//size_t n = m_GridProps[curtime].size();
		//CString projectName = CProjectManger::getSingletonPtr()->GetActiveCase()->GetTitle();
		//AfxMessageBox(projectName);
		//projectName.Format("%d" ,n);
		//AfxMessageBox(projectName);
		//AfxMessageBox(strTime);
		return UNACTVALUE;
	}
}

void CGridPropUnitPo::ReCalcPropMaxmin( )
{
	COleDateTime time = CProjectManger::getSingletonPtr()->GetActiveCase()->GetGridData().GetCurTime();

	//初始化最大最小
	m_dMaxValue = m_dMinValue = UNACTVALUE;

	//TimedGridProp::iterator propIter = m_GridProps.begin();
	////遍历所有时间的数据
	//for (;propIter != m_GridProps.end() ; ++propIter)
	//{

	bool bFirstFlag = true;

	TimedGridProp::iterator propIter = m_GridProps.find(time);
	if (propIter != m_GridProps.end())
	{
		GridPropValue& timeVal = m_GridProps[propIter->first];
		for (size_t nProp = 0;nProp < timeVal.size();++nProp)
		{
			if (timeVal[nProp] == UNACTVALUE)
			{
				continue;
			}

			if (bFirstFlag)
			{
				m_dMaxValue = m_dMinValue = timeVal[nProp];
				bFirstFlag = false;
				continue;
			}

			m_dMaxValue = max(m_dMaxValue , timeVal[nProp]);
			m_dMinValue = min(m_dMinValue , timeVal[nProp]);
		}
	}

	//重置是否要重新计算最大最小值
	m_bRecal = FALSE;
}

BOOL CGridPropUnitPo::GetKMinMaxValue(CGridRecive& gridRecive , const COleDateTime& time,long layer,double& dMaxValue,double& dMinValue)
{
	//设置当前显示的属性名
	m_strCurStoreDataProp = m_strPropName;

	long ISize = 0;
	long JSize = 0;
	long KSize = 0;
	gridRecive.GetGridSize(ISize ,JSize ,KSize);

	//获取当前时间当前层属性最大最小值
	BOOL bInit = FALSE;
	long nSkip = layer * JSize * ISize;
	long surSize = JSize * ISize;

	double curVal = UNACTVALUE;
	for (long nIndex = 0; nIndex < surSize; ++nIndex)
	{
		int gridnoI;
		int gridnoJ;
		if ((nIndex + 1) % ISize == 0)
		{
			gridnoI = ISize;
			gridnoJ = (nIndex + 1) / ISize;
		}
		else
		{
			gridnoI = (nIndex + 1) % ISize;
			gridnoJ = (nIndex + 1) / ISize + 1;
		}

		GridNo gridNo(gridnoI, gridnoJ, layer + 1);
		if (gridRecive.IsGridAct(gridNo) == false) //无效网格
		{
			continue;
		}

		int newIndex;
		if (!m_bTotalGridData)
		{
			//如果nSkip + nIndex=496, 前面有4个无效网格
			newIndex = gridRecive.GetGridValueIndex(nSkip + nIndex); //执行了这一句后newIndex=492
		}
		else
		{
			newIndex = nSkip + nIndex;
		}

		if (m_GridProps.size() == 1)
		{
			//对于静态属性DZNET, 切换时间的时候, m_GridProps里面始终只有一个时间的数据
			curVal = m_GridProps.begin()->second[newIndex];
		}
		else
		{
			curVal = m_GridProps[time][newIndex];
		}

		if (curVal != UNACTVALUE)
		{
			if (!bInit)
			{
				//未初始化则初始化
				dMaxValue = dMinValue = curVal;
				bInit = TRUE;
			}
			else
			{
				dMaxValue = max(dMaxValue , curVal);
				dMinValue = min(dMinValue , curVal);
			}
		}
	}

	return TRUE;
}

void CGridPropUnitPo::GetMinMaxValue( double& dMaxValue,double& dMinValue )
{
	if (m_strCurStoreDataProp != m_strPropName || m_strCurStoreDataCase != GetDataCaseName())
	{
		//属性切换了
		m_GridProps.clear();
		//设置当前显示的属性名
		m_strCurStoreDataProp = m_strPropName;
		m_strCurStoreDataCase = GetDataCaseName();
	}
	
	COleDateTime time = CProjectManger::getSingletonPtr()->GetActiveCase()->GetGridData().GetCurTime();
	TimedGridProp::iterator itData = m_GridProps.find(time);
	if (itData == m_GridProps.end())
	{
		//未读取数据,则先读取数据
		ReadData(time);
	}
	//所有时间所有层最大最小值
	if (m_bRecal)
	{
		//需要重新计算最大最小值
		ReCalcPropMaxmin();
	}
	dMaxValue = m_dMaxValue;
	dMinValue = m_dMinValue;
}

void CGridPropUnitPo::BindGridFunction( CGridRecive& gridRev  /*, int nDataType */)
{
	//绑定回调函数
	GridFunction gridFun;
	gridFun.createGridPropData = boost::bind(&CGridPropUnitPo::CreateGridPropData, this , _1,_2,_3);
	gridFun.getKMinMaxValue = boost::bind(&CGridPropUnitPo::GetKMinMaxValue, this , _1,_2,_3,_4,_5);
	gridFun.setValue = boost::bind(&CGridPropUnitPo::ModifyGridPropData, this , _1,_2,_3,_4);
	gridFun.getValue = boost::bind(&CGridPropUnitPo::GetPropValue, this , _1,_2,_3);
	gridFun.getValues = boost::bind(&CGridPropUnitPo::getPropValues,this,_1,_2);
	gridFun.getMinMax = boost::bind(&CGridPropUnitPo::GetMinMaxValue, this , _1,_2);
	gridFun.clearData = boost::bind(&CGridPropUnitPo::ClearData, this );
	gridFun.getDataType = boost::bind(&CGridPropUnitPo::GetDataType, this );
	gridFun.isExistTime = boost::bind(&CGridPropUnitPo::IsExistTime, this ,_1);
	gridFun.checkCurPropData = boost::bind(&CGridPropUnitPo::CheckCurPropData, this );
//	gridFun.copyGridPropUnit = boost::bind(&CGridPropUnitPo::CopyObj, this , _1,_2);
	gridFun.closeFileHandle = boost::bind(&CGridPropUnitPo::CloseFileHandle, this );
	gridRev.SetGridFunction(m_strPropName , gridFun);
	//网格数据
	m_pReciveData = &gridRev;
	//生成的文件名
	//文件名＝项目名.属性名
	CString fileName;
	//文件名
	fileName.Format("%s\\%s.%s" , m_objPath , m_objName , m_strPropName);	
	//打开文件
	if ( m_bFile )
	{
		// 若文件已打开则先关闭文件
		if (m_bFileOpen)
		{
			m_file.Close();
		}

		//打开文件
		if (m_file.Open(fileName , CFile::modeReadWrite | CFile::typeBinary ))
		{
			m_bFileOpen = TRUE;
		}
		else
		{
			//打开失败
			AfxMessageBox(fileName + "打开失败，将重新计算");
			m_bFile = FALSE;
			m_strFiles.clear();
			m_dataOffsets.clear();
		}
	}
}

void CGridPropUnitPo::SetTimePropDatas( CGridRecive& gridRecive ,GridValues& allPropvalues ,
									 const COleDateTime& time )
{
	long lIsize = 0;
	long lJsize = 0;
	long lKsize = 0;
	gridRecive.GetGridSize(lIsize,lJsize,lKsize);
	long nVals = (long)allPropvalues.size();
	if (nVals == 0)
	{
		//没有读到数据
		return;
	}
	//if (nVals == lIsize * lJsize * lKsize)
	{
		//如果是已插值的全部数据，则不需插值
		if ( m_strCurStoreDataProp != m_strPropName )
		{
			//只是临时取的属性,读到m_namedGridProps,再写到文件
			m_namedGridProps[time] = allPropvalues;
		}
		else
		{
			m_GridProps[time] = allPropvalues;
		}
		//结束事务
		allPropvalues.clear();
		return;
	}
	//int nUnAct = 0;
	//long index = 0;
	//GridNo gridNo(0 ,0 ,0);
	////插值
	//for (long kIndex = 0;kIndex < lKsize ;++kIndex)
	//{
	//	for (long jIndex = 0;jIndex < lJsize ;++jIndex)
	//	{
	//		for (long iIndex = 0;iIndex < lIsize ;++iIndex)
	//		{
	//			index = kIndex * lIsize  * lJsize + jIndex * lIsize + iIndex;
	//			//为了外部接口判断网格是否有效时网格起始号统一从1开始，这里加1
	//			gridNo.noI = iIndex + 1;
	//			gridNo.noJ = jIndex + 1;
	//			gridNo.noK = kIndex + 1;
	//			if (gridRecive.IsGridAct(gridNo))
	//			{
	//				gridNo.noI = iIndex;
	//				gridNo.noJ = jIndex;
	//				gridNo.noK = kIndex;
	//				AddGridPropData(gridRecive ,time ,gridNo,allPropvalues[index - nUnAct] );
	//			}
	//			else
	//			{
	//				++nUnAct;
	//			}
	//		}
	//	}
	//}
	//allPropvalues.clear();
}
void CGridPropUnitPo::GetRealOffset(){
	//网格数
	long lIsize = 0;
	long lJsize = 0;
	long lKsize = 0;
	m_pReciveData->GetGridSize(lIsize,lJsize,lKsize);
	long nUnAct = 0;
	long index = 0;
	GridNo gridNo(0 ,0 ,0);
	//////////////////////////////////////////////////////////////////////////
	//插值
	for (long kIndex = 0;kIndex < lKsize ;++kIndex)
	{
		for (long jIndex = 0;jIndex < lJsize ;++jIndex)
		{
			for (long iIndex = 0;iIndex < lIsize ;++iIndex)
			{
				index = kIndex * lIsize  * lJsize + jIndex * lIsize + iIndex;
				//为了外部接口判断网格是否有效时网格起始号统一从1开始，这里加1
				gridNo.noI = iIndex + 1;
				gridNo.noJ = jIndex + 1;
				gridNo.noK = kIndex + 1;
				if (m_pReciveData->IsGridAct(gridNo))
				{
					//有效网格的实际索引
					//关键字相关信息字节数:24 + 4,见ReadPropData函数在读数据前的代码
					//每1000个数据要跳过8个字节
					//*4是将偏移调到数据的起始字节
					m_realOffset[index] = ((index - nUnAct) * 4 + (( index - nUnAct ) / 1000) * 8) + 28;
				}
				else
				{
					//无效网格数
					++nUnAct;
				}
			}
		}
	}
}

void CGridPropUnitPo::getGridIndexOffset(){
	//网格数
	long lIsize = 0;
	long lJsize = 0;
	long lKsize = 0;
	m_pReciveData->GetGridSize(lIsize,lJsize,lKsize);
	long nUnAct = 0;
	long index = 0;
	GridNo gridNo(0 ,0 ,0);
	//////////////////////////////////////////////////////////////////////////
	//插值
	for (long kIndex = 0;kIndex < lKsize ;++kIndex)
	{
		for (long jIndex = 0;jIndex < lJsize ;++jIndex)
		{
			for (long iIndex = 0;iIndex < lIsize ;++iIndex)
			{
				index = kIndex * lIsize  * lJsize + jIndex * lIsize + iIndex;
				//为了外部接口判断网格是否有效时网格起始号统一从1开始，这里加1
				gridNo.noI = iIndex + 1;
				gridNo.noJ = jIndex + 1;
				gridNo.noK = kIndex + 1;
				if (m_pReciveData->IsGridAct(gridNo))
				{
					//有效网格的实际索引
					m_realGridIndexOffset[index] = index - nUnAct;
				}
				else
				{
					//无效网格数
					++nUnAct;
				}
			}
		}
	}
}

long CGridPropUnitPo::GetTotalGridValueOffset( long lGridIndex )
{
	std::map<long, long>::iterator itFind = m_totalOffset.find(lGridIndex);
	if (itFind != m_totalOffset.end())
	{
		return itFind->second;
	}

	long lOffset = lGridIndex * 4 + (lGridIndex / 1000) * 8 + 28;
	m_totalOffset[lGridIndex] = lOffset;
	return lOffset;
}

void CGridPropUnitPo::SetDataInfo( CString strPropName , CGridRecive* gridRecive ,CString strFile,	const COleDateTime& curTime ,__int64 dataOffset,__int64 dataCount)
{
	//查找时间
	if (m_dataOffsets.find( curTime ) != m_dataOffsets.end())
	{
		return;
	}
	//没有读到过，则保存数据信息
	//数据所在文件名
	m_strFiles[curTime] = strFile;
	//网格数据
	m_pReciveData = gridRecive;
	//时间
	m_dataOffsets[curTime] = dataOffset;
	//属性名
	m_strPropName = strPropName;
	//网格个数
	m_dataCount = dataCount;
	if(m_realGridIndexOffset.empty())
		getGridIndexOffset();
}

void CGridPropUnitPo::SetDataInfo(CString strPropName,CGridRecive* gridRecive ,const COleDateTime& curTime){
	//查找时间
	if (m_dataOffsets.find( curTime ) != m_dataOffsets.end())
	{
		return;
	}
	//没有读到过，则保存数据信息
	//数据所在文件名
	//m_strFiles[curTime] = strFile;
	//网格数据
	m_pReciveData = gridRecive;
	//时间
	//m_dataOffsets[curTime] = dataOffset;
	//属性名
	m_strPropName = strPropName;
}

BOOL CGridPropUnitPo::IsExistTime(const COleDateTime& time)
{
	if (m_dataOffsets.find( time ) == m_dataOffsets.end())
	{
		//该时间没有数据
		if (m_bCalcState == TRUE)
		{
			double dbRes;
			CheckAndCalcProp(dbRes , GridNo(0 , 0 , 0) , time , TRUE);
		}
		if (m_dataOffsets.find( time ) == m_dataOffsets.end())
		{
			return FALSE;
		}
	}
	return TRUE;
}

void CGridPropUnitPo::ReadData(const COleDateTime& time)
{
	//检查数据是否要计算
	if (m_strFiles.empty())
	{
		double resVal;
		GridNo gridNo(0,0,0);
		m_strCurStoreDataProp = m_strPropName;
		COleDateTime curTime;
		CheckAndCalcProp(resVal , gridNo , curTime , TRUE);
		return;
	}
	//读取所有时间步数据	
	TimedDataOffset::iterator it_dataIf = m_dataOffsets.begin();
	CString fileName;
	fileName = m_strFiles[it_dataIf->first];
	MakeFileOpen(fileName);
	if (m_bFile)
	{
		//AfxMessageBox("全部读取");
		//数据已写入文件，则直接读取
		for (int nDate = 0; it_dataIf != m_dataOffsets.end() ;++it_dataIf, ++nDate)
		{
			//用文件名和数据在文件中的起始位置信息来读取数据
			//添加一个时间步数据
			m_bReadNewProp = TRUE;
			CreateGridPropData(*m_pReciveData , it_dataIf->first  ,m_strPropName);
			if (m_bFileOpen)
			{
				//调整文件指针到数据起始位置
				m_file.Seek(it_dataIf->second , CFile::begin);
				//读取数据
				if ( m_strPropName != m_strCurStoreDataProp )
				{
					ULONGLONG nDataSize;
					m_file.Read(&nDataSize, 8);

					m_namedGridProps[it_dataIf->first].resize(nDataSize, UNACTVALUE);

					//只是临时取的属性,从文件读到m_namedGridProps
// 					m_file.Read(&m_namedGridProps[it_dataIf->first][0] , ((int)m_namedGridProps[it_dataIf->first].size()) * 8 );
					m_file.Read(&m_namedGridProps[it_dataIf->first][0] , nDataSize * 8 );
				}
				else
				{
					ULONGLONG nDataSize;
					m_file.Read(&nDataSize, 8);

					if (m_GridProps[it_dataIf->first].size() == 0)
					{
						//获取网格数
						long ISize = 0;
						long JSize = 0;
						long KSize = 0;
						m_pReciveData->GetGridSize(ISize , JSize , KSize);

						//m_GridProps[it_dataIf->first].resize(nDataSize, UNACTVALUE);
						m_GridProps[it_dataIf->first].resize(ISize * JSize * KSize, UNACTVALUE);
					}

					//从文件读到m_GridProps
					m_file.Read(&m_GridProps[it_dataIf->first][0] , ((int)m_GridProps[it_dataIf->first].size()) * 8 );
				}
			}
		}

		m_bReadNewProp = FALSE;
		return;
	}

	//读取数据使用
	ReadProp(time);
}

//将数据写入自定义文件(SimDataCath)
void CGridPropUnitPo::WriteData()
{
	//将当前时间的数据写入文件
	TimedGridProp::iterator it_time = m_GridProps.begin();	
	TimedDataOffset::iterator it_dataIf = m_dataOffsets.begin();
	if ( m_strCurStoreDataProp != m_strPropName )
	{
		//只是临时取的属性,读到m_namedGridProps,再写到文件
		it_time = m_namedGridProps.begin();
	}
	//生成的文件名
	//文件名＝项目名.属性名
	CString fileName;
	//打开文件
	if ( !m_bFile )
	{
		m_objPath = CProjectManger::getSingletonPtr()->GetPrjPath();
		m_objPath += _T("simCatche");
		m_objName = CProjectManger::getSingletonPtr()->GetActiveCase()->GetTitle();
		//新建自定义目录
		::CreateDirectory(m_objPath , NULL);
		//文件名
		fileName.Format("%s\\%s.%s" , m_objPath , m_objName , m_strPropName);	
		//打开文件
		CFileFind fileFind;
		if (fileFind.FindFile(fileName))
		{
			if (m_lgrName != "")
			{
				fileName.Format("%s\\%s.%s.%s" , m_objPath , m_objName ,m_lgrName, m_strPropName);
			}
		}
		m_file.Open(fileName , CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary );
		m_bFileOpen = TRUE;
	}
	if (m_bNewProp)
	{
		if (!m_namedGridProps.empty())
		{
			//写入新建属性,只写最后一个时间的数据
			it_time = m_namedGridProps.end();
			if (m_strCurStoreDataProp == m_strPropName)
			{
				//如果选中要计算的属性
				it_time = m_GridProps.end();
			}
			--it_time;
			it_dataIf = m_dataOffsets.end();
			--it_dataIf;
			//修改当前时间的数据信息
			m_strFiles[it_dataIf->first] = fileName;
			//写入文件结尾
			it_dataIf->second = m_file.SeekToEnd();
			ULONGLONG nDataSize = it_time->second.size();
			m_file.Write(&nDataSize, 8);
			m_file.Write(&it_time->second[0] , (int)it_time->second.size() * 8 );
		}
	}
	else
	{
		for (int nDate = 0; it_dataIf != m_dataOffsets.end() ; ++it_dataIf, ++nDate)
		{
			//修改当前时间的数据信息
			m_strFiles[it_dataIf->first] = fileName;
			//写入文件
			it_dataIf->second = m_file.GetPosition();
			ULONGLONG nDataSize = it_time->second.size();
			m_file.Write(&nDataSize, 8);
			m_file.Write(&it_time->second[0] , (int)it_time->second.size() * 8 );		
		}
	}
	m_bFile = TRUE;
	//缓存写入文件
	m_file.Flush();
}

void CGridPropUnitPo::ModifyGridPropData( COleDateTime curtime, long gridIndex, const double& dValue )
{
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//属性值时间	
	if (m_dataOffsets.find(curtime) == m_dataOffsets.end())
	{
		TimedDataOffset::iterator propIter = m_dataOffsets.end();
		--propIter;
		//时间设为最后一个时间
		curtime = propIter->first;
	}
	//当前时间数据所在文件名
	CString fileName;
	fileName = m_strFiles[curtime];
	MakeFileOpen(fileName);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	if (!m_bFile)
	{
		if ((m_GridProps.empty() && m_strCurStoreDataProp == m_strPropName) ||
			(m_namedGridProps.empty() && m_strCurStoreDataProp != m_strPropName))
		{
			//当前属性未读或临时属性未读
			ReadData(curtime);
		}
		//未生成文件
		WriteData();
	}
	ASSERT(m_dataOffsets.find(curtime) != m_dataOffsets.end());
	//修改文件中的数据
	//修改数据
	if (m_bFileOpen)
	{
		if (m_file.SeekToEnd() == 0)
		{
			m_file.SeekToBegin();
			ULONGLONG nDataSize = 1;
			m_file.Write(&nDataSize, 8);
		}
		else
		{
			m_file.Seek(m_dataOffsets[curtime], CFile::begin);
			ULONGLONG nDataSize;
			m_file.Read(&nDataSize, 8);
			nDataSize++;
			m_file.Seek(m_dataOffsets[curtime], CFile::begin);
			m_file.Write(&nDataSize, 8);
		}
		//调整到数据所在位置
		m_file.Seek(8 * (gridIndex + 1) + m_dataOffsets[curtime] , CFile::begin);
		//修改数据
		m_file.Write( &dValue , 8);
		m_bFlush = FALSE;
	}
}

BOOL CGridPropUnitPo::CheckAndCalcProp(double& resVal , GridNo gridNo, COleDateTime curTime ,BOOL bAllCalc /*= FALSE*/ )
{
	//有数据则m_strFiles(数据所在文件)绝不为空
	if (m_strFiles.empty())
	{
		//计算状态，用到的属性放到临时数据容器里，优化计算速度
		m_bCalcState = TRUE;
		if (!bAllCalc)
		{
			//设置为取数据为非计算状态
			m_bCalcState = FALSE;
			++gridNo.noI;
			++gridNo.noJ;
			++gridNo.noK;
			if (!m_pReciveData->IsGridAct(gridNo))
			{
				//无效网格，直接给无效值
				return TRUE;
			}
			--gridNo.noI;
			--gridNo.noJ;
			--gridNo.noK;
		}
		//可计算的属性
		if (m_strPropName == _T("DZNET"))
		{
			//计算DZNET DZNET = NTG * DZ
			if (!bAllCalc)
			{
				GridFunction dzFunc = m_pReciveData->GetGridFunction(_T("DZ"));
				GridFunction ntgFunc = m_pReciveData->GetGridFunction(_T("NTG"));
				double dzVal = dzFunc.getValue(*m_pReciveData , curTime ,gridNo);
				double ntgVal = ntgFunc.getValue(*m_pReciveData ,curTime ,gridNo);
				if (dzVal == UNACTVALUE || ntgVal == UNACTVALUE)
				{
					return TRUE;
				}
				resVal = dzVal * ntgVal;
				return TRUE;
			}
			CalcData(boost::bind(&CGridPropUnitPo::DZNETdata ,this));
		}
		if (m_strPropName == _T("PORV"))
		{
			//计算PORV//PORV = DX*DY*DZ*PORO*NTG
			if (!bAllCalc)
			{
				//取得各属性值
				GridFunction dzFunc = m_pReciveData->GetGridFunction(_T("DZ"));
				GridFunction ntgFunc = m_pReciveData->GetGridFunction(_T("NTG"));
				GridFunction poroFunc = m_pReciveData->GetGridFunction(_T("PORO"));
				GridFunction dxFunc = m_pReciveData->GetGridFunction(_T("DX"));
				GridFunction dyFunc = m_pReciveData->GetGridFunction(_T("DY"));
				double dzVal = dzFunc.getValue(*m_pReciveData , curTime ,gridNo);
				double ntgVal = ntgFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
				double dxVal = dxFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
				double dyVal = dyFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
				double poroVal = poroFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
				//PORV = DX*DY*DZ*PORO*NTG
				if (dxVal == UNACTVALUE || dyVal == UNACTVALUE || poroVal == UNACTVALUE ||
					dzVal == UNACTVALUE || ntgVal == UNACTVALUE)
				{
					resVal = UNACTVALUE;
					return TRUE;
				}				
				resVal = dxVal * dyVal * dzVal * poroVal * ntgVal;
				return TRUE;
			}
			CalcData(boost::bind(&CGridPropUnitPo::PORVdata ,this));
		}


		if (m_strPropName == _T("SOIL"))
		{
			if (!bAllCalc)
			{
				//各属性回调函数
				GridFunction sgasFunc;
				if (m_pReciveData->IsExistGridFunction("SGAS"))
				{
					sgasFunc = m_pReciveData->GetGridFunction(_T("SGAS"));
				}
				
				GridFunction swatFunc = m_pReciveData->GetGridFunction(_T("SWAT"));			

				if (m_pReciveData->IsExistGridFunction("SGAS"))
				{
					double sgasVal = sgasFunc.getValue(*m_pReciveData , curTime ,gridNo);
					double swatVal = swatFunc.getValue(*m_pReciveData , curTime ,gridNo);
					if (sgasVal == UNACTVALUE || swatVal == UNACTVALUE )
					{
						return TRUE;
					}

					resVal = 1.0 - sgasVal - swatVal;

					return TRUE;
				}
				else
				{
					double swatVal = swatFunc.getValue(*m_pReciveData , curTime ,gridNo);
					if (swatVal == UNACTVALUE )
					{
						return TRUE;
					}

					resVal = 1.0 - swatVal;

					return TRUE;
				}
			}

			CalcData(boost::bind(&CGridPropUnitPo::SOILdata ,this));
		}


		//if (m_strPropName == _T("OGIP"))
		//{
		//	if (!bAllCalc)
		//	{
		//		//各属性回调函数
		//		GridFunction sgasFunc = m_pReciveData->GetGridFunction(_T("SGAS"));
		//		GridFunction porvFunc = m_pReciveData->GetGridFunction(_T("PORV"));
		//		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//		//要用到的变量，避免频繁定义
		//		double porvVal = porvFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
		//		double sgasVal = sgasFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
		//		if (sgasVal == UNACTVALUE || porvVal == UNACTVALUE )
		//		{
		//			return TRUE;
		//		}
		//		resVal = sgasVal * porvVal;
		//		return TRUE;
		//	}
		//	CalcData(boost::bind(&CGridPropUnitPo::OGIPdata ,this));
		//}
		//if (m_strPropName == _T("OWIP"))
		//{
		//	if (!bAllCalc)
		//	{
		//		//各属性回调函数
		//		GridFunction swatFunc = m_pReciveData->GetGridFunction(_T("SWAT"));
		//		GridFunction porvFunc = m_pReciveData->GetGridFunction(_T("PORV"));	
		//		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//		//要用到的变量，避免频繁定义
		//		double porvVal = porvFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
		//		double swatVal = swatFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
		//		if (porvVal == UNACTVALUE || swatVal == UNACTVALUE )
		//		{
		//			return TRUE;
		//		}
		//		resVal = swatVal * porvVal;
		//		return TRUE;
		//	}
		//	CalcData(boost::bind(&CGridPropUnitPo::OWIPdata ,this));
		//}
		//if (m_strPropName == _T("OOIP"))
		//{
		//	if (!bAllCalc)
		//	{
		//		//各属性回调函数
		//		GridFunction soilFunc = m_pReciveData->GetGridFunction(_T("SOIL"));
		//		GridFunction porvFunc = m_pReciveData->GetGridFunction(_T("PORV"));
		//		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//		//要用到的变量，避免频繁定义
		//		double porvVal = porvFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
		//		double soilVal = soilFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
		//		if (porvVal == UNACTVALUE || soilVal == UNACTVALUE )
		//		{
		//			return TRUE;
		//		}
		//		resVal = soilVal * porvVal;
		//		return TRUE;
		//	}
		//	CalcData(boost::bind(&CGridPropUnitPo::OOIPdata ,this));
		//}
		//if (m_strPropName == _T("TSAT"))
		//{
		//	if (!bAllCalc)
		//	{
		//		//各属性回调函数
		//		GridFunction soilFunc = m_pReciveData->GetGridFunction(_T("SOIL"));	
		//		GridFunction swatFunc = m_pReciveData->GetGridFunction(_T("SWAT"));
		//		GridFunction sgasFunc = m_pReciveData->GetGridFunction(_T("SGAS"));
		//		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//		//要用到的变量，避免频繁定义
		//		double swatVal = UNACTVALUE ;
		//		double soilVal = UNACTVALUE ;
		//		double sgasVal = UNACTVALUE ;
		//		swatVal = swatFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
		//		soilVal = soilFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
		//		sgasVal = sgasFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
		//		if ( swatVal != UNACTVALUE && soilVal != UNACTVALUE && sgasVal != UNACTVALUE)
		//		{
		//			Structure::CALC_PARAMSOPTION options;
		//			InitOption(options);
		//			double a = options.m_TSATaValue;
		//			double b = options.m_TSATbValue;
		//			double c = options.m_TSATcValue;
		//			resVal =  a * swatVal + b * soilVal + c * sgasVal;
		//		}
		//		return TRUE;
		//	}
		//	CalcData(boost::bind(&CGridPropUnitPo::TSATdata ,this));
		//}

		//if (m_strPropName == _T("FIPOIL1"))
		//{
		//	if (!bAllCalc)
		//	{
		//		double gridArea = CProjectManger::getSingletonPtr()->GetActiveCase()->GetGridData().GetGridArea(GridNo(gridNo.noI+1 , gridNo.noJ + 1 ,gridNo.noK + 1));

		//		double fipoiltVal = UNACTVALUE;
		//		GridFunction fipoilFunc = m_pReciveData->GetGridFunction(_T("FIPOIL"));
		//		fipoiltVal = fipoilFunc.getValue(*m_pReciveData, curTime, gridNo);
		//		if (fipoiltVal != UNACTVALUE && gridArea != 0)
		//		{
		//			//剩余油丰度 FIPOIL1 = FIPOIL / 网格面积          单位是 万吨/平方公里
		//			resVal =  fipoiltVal / gridArea;
		//		}

		//		return TRUE;
		//	}

		//	CalcData(boost::bind(&CGridPropUnitPo::FIPOIL1data, this));
		//}

		//if (m_strPropName == _T("FIPOIL2"))
		//{
		//	if (!bAllCalc)
		//	{
		//		//FIPOIL2 (剩余可动油储量) FIPOIL2 = FIPOIL * ( ( SOIL - SOR) / SOIL )   SOR 为输入参数(剩余油饱和度)
		//		//各属性回调函数
		//		GridFunction fipoilFunc = m_pReciveData->GetGridFunction(_T("FIPOIL"));
		//		GridFunction soilFunc = m_pReciveData->GetGridFunction(_T("SOIL"));
		//		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//		//要用到的变量，避免频繁定义
		//		double fipoilVal = UNACTVALUE ;
		//		double soilVal = UNACTVALUE ;
		//		fipoilVal = fipoilFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
		//		soilVal = soilFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
		//		if ( fipoilVal != UNACTVALUE && soilVal != 0 && soilVal != UNACTVALUE)
		//		{
		//			Structure::CALC_PARAMSOPTION options;
		//			InitOption(options);
		//			double sor = options.m_SORValue;
		//			resVal =  fipoilVal * (soilVal - sor) / soilVal;
		//		}
		//		return TRUE;
		//	}
		//	CalcData(boost::bind(&CGridPropUnitPo::FIPOIL2data ,this));
		//}

		//if (m_strPropName == _T("fw"))
		//{
		//	if (!bAllCalc)
		//	{
		//		GridFunction WATKR_Func = m_pReciveData->GetGridFunction(_T("WATKR"));
		//		GridFunction WAT_VISC_Func = m_pReciveData->GetGridFunction(_T("WAT_VISC"));
		//		GridFunction OILKR_Func = m_pReciveData->GetGridFunction(_T("OILKR"));
		//		GridFunction OIL_VISC_Func = m_pReciveData->GetGridFunction(_T("OIL_VISC"));

		//		double WATKR = WATKR_Func.getValue(*m_pReciveData, curTime, gridNo);
		//		double WAT_VISC = WAT_VISC_Func.getValue(*m_pReciveData, curTime, gridNo);
		//		double OILKR = OILKR_Func.getValue(*m_pReciveData, curTime, gridNo);
		//		double OIL_VISC = OIL_VISC_Func.getValue(*m_pReciveData, curTime, gridNo);

		//		if (WATKR == UNACTVALUE || WAT_VISC == UNACTVALUE 
		//			|| OILKR == UNACTVALUE || OIL_VISC == UNACTVALUE)
		//		{
		//			return TRUE;
		//		}				

		//		if (WATKR < 1e-3 && OILKR < 1e-3) //高老师
		//		{
		//			resVal = 0;
		//		}
		//		else
		//		{
		//			resVal = 100 * (WATKR / WAT_VISC) / (WATKR / WAT_VISC + OILKR / OIL_VISC);
		//		}

		//		return TRUE;
		//	}

		//	CalcData(boost::bind(&CGridPropUnitPo::fwdata ,this));
		//}


		//设置为取数据为非计算状态
		m_bCalcState = FALSE;
		return FALSE;

	} //if (m_strFiles.empty())

	return FALSE;
}

void CGridPropUnitPo::CalcData( boost::function<BOOL ()> readFunc )
{
	try
	{
		//是否要在此函数中结束进度条
		BOOL bNeedEnd = FALSE;
		m_bCalcAllProp = TRUE;
		if (!CProgressControl::getSingletonPtr()->IsProgressRun())
		{
			CProgressControl::getSingletonPtr()->BeginProgress();
			bNeedEnd = TRUE;
		}
		readFunc();
		m_bCalcAllProp = FALSE;
		if (bNeedEnd)
		{
			CProgressControl::getSingletonPtr()->EndProgress();
		}
	}
	catch (...)
	{
		m_bCalcAllProp = FALSE;
	}
}

BOOL CGridPropUnitPo::DZNETdata()
{
	//显示进度条
	CString strCurInfo;
	///////////////////////////////////////////////////////////////////////////////////////////
	//计算DZNET
	//DZNET = NTG * DZ
	//设置当前时间，用于设置值时使用初始时间
	COleDateTime curTime = m_pReciveData->m_SimTimes[0];
	strCurInfo = curTime.Format("%Y-%m-%d");
	CProgressControl::getSingletonPtr()->StepProgress(0 , _T("正在计算数据："),_T("当前属性：DZNET ,当前时间：") + strCurInfo);
	CreateGridPropData(*m_pReciveData, curTime ,m_strPropName);
	//没有DZNET属性则计算，有则读取
	GridFunction dznetFunc = m_pReciveData->GetGridFunction(_T("DZNET"));
	GridFunction dzFunc = m_pReciveData->GetGridFunction(_T("DZ"));
	GridFunction ntgFunc = m_pReciveData->GetGridFunction(_T("NTG"));
	//要用到的变量，避免频繁定义
	double dzVal = UNACTVALUE;
	double ntgVal = UNACTVALUE;
	GridNo gridNo(0 ,0 ,0 );
	// 网格规模
	long nI, nJ, nK;
	m_pReciveData->GetGridSize(nI, nJ, nK);

	//没有DZNET数据则计算
	for (long kIndex = 0;kIndex < nK ;++kIndex)
	{
		for (long iIndex = 0;iIndex < nI ;++iIndex)
		{
			for (long jIndex = 0;jIndex < nJ ;++jIndex)
			{
				gridNo.noI = iIndex + 1;
				gridNo.noJ = jIndex + 1;
				gridNo.noK = kIndex + 1;
				if (!m_pReciveData->IsGridAct(gridNo))
				{
					//无效网格，直接给无效值
					continue;
				}
				gridNo.noI = iIndex;
				gridNo.noJ = jIndex;
				gridNo.noK = kIndex;
				dzVal = dzFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
				ntgVal = ntgFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
				if (dzVal == UNACTVALUE || ntgVal == UNACTVALUE)
				{
					continue;
				}
				dznetFunc.setValue(*m_pReciveData ,curTime ,gridNo, dzVal * ntgVal);
			}
		}	
		CProgressControl::getSingletonPtr()->StepProgress(_T("当前属性：DZNET"));
	}

	dznetFunc.clearData();
	dzFunc.clearData();
	ntgFunc.clearData();

	//////////////////////////////////////////////////////////////////////////
	//结束进度条显示
	CProgressControl::getSingletonPtr()->StepProgress(100 , _T("完成数据计算！"),_T("当前属性：DZNET") );
	return TRUE;
}

BOOL CGridPropUnitPo::PORVdata()
{
	//显示进度条
	CProgressControl::getSingletonPtr()->StepProgress(0 , _T("正在计算数据："),_T("当前属性：PORV") );
	///////////////////////////////////////////////////////////////////////////////////////////
	//计算PORV//PORV = DX*DY*DZ*PORO*NTG
	//设置当前时间，用于设置值时使用初始时间
	COleDateTime curTime = m_pReciveData->m_SimTimes[0];
	CreateGridPropData(*m_pReciveData, curTime ,m_strPropName);
	//用到的数据
	GridFunction dzFunc = m_pReciveData->GetGridFunction(_T("DZ"));
	GridFunction ntgFunc = m_pReciveData->GetGridFunction(_T("NTG"));
	GridFunction porvFunc = m_pReciveData->GetGridFunction(_T("PORV"));
	GridFunction dxFunc = m_pReciveData->GetGridFunction(_T("DX"));
	GridFunction dyFunc = m_pReciveData->GetGridFunction(_T("DY"));
	GridFunction poroFunc = m_pReciveData->GetGridFunction(_T("PORO"));

	//要用到的变量，避免频繁定义
	double dzVal = UNACTVALUE;
	double ntgVal = UNACTVALUE;
	double dxVal  = UNACTVALUE;
	double dyVal = UNACTVALUE;
	double poroVal = UNACTVALUE;
	GridNo gridNo(0 ,0 ,0 );
	// 网格规模
	long nI, nJ, nK;
	m_pReciveData->GetGridSize(nI, nJ, nK);

	//可计算PORV
	for (long kIndex = 0;kIndex < nK ;++kIndex)
	{
		for (long iIndex = 0;iIndex < nI ;++iIndex)
		{
			for (long jIndex = 0;jIndex < nJ ;++jIndex)
			{
				gridNo.noI = iIndex + 1;
				gridNo.noJ = jIndex + 1;
				gridNo.noK = kIndex + 1;
				if (!m_pReciveData->IsGridAct(gridNo))
				{
					//无效网格，直接给无效值
					continue;
				}
				gridNo.noI = iIndex;
				gridNo.noJ = jIndex;
				gridNo.noK = kIndex;
				//取得各属性值
				dzVal = dzFunc.getValue(*m_pReciveData , curTime ,gridNo);
				ntgVal = ntgFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
				dxVal = dxFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
				dyVal = dyFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
				poroVal = poroFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
				//PORV = DX*DY*DZ*PORO*NTG
				if (dxVal == UNACTVALUE || dyVal == UNACTVALUE || poroVal == UNACTVALUE ||
					dzVal == UNACTVALUE || ntgVal == UNACTVALUE)
				{
					continue;
				}
				porvFunc.setValue(*m_pReciveData , curTime ,gridNo, dxVal * dyVal * dzVal * poroVal * ntgVal);
			}
		}	
		CProgressControl::getSingletonPtr()->StepProgress(_T("当前属性：PORV"));
	}
	dzFunc.clearData();
	ntgFunc.clearData();
	porvFunc.clearData();
	dxFunc.clearData();
	dyFunc.clearData();
	poroFunc.clearData();
	//////////////////////////////////////////////////////////////////////////
	//结束进度条显示
	CProgressControl::getSingletonPtr()->StepProgress(100 , _T("完成数据计算！"),_T("当前属性：PORV") );
	return TRUE;
}

#include <time.h>
#include <sstream>
//计算SOIL
BOOL CGridPropUnitPo::SOILdata()
{
	//显示进度条
	CProgressControl::getSingletonPtr()->StepProgress(0 , _T("正在计算数据："),_T("当前属性：SOIL") );

	//各属性回调函数
	GridFunction soilFunc = m_pReciveData->GetGridFunction(_T("SOIL"));
	GridFunction swatFunc = m_pReciveData->GetGridFunction(_T("SWAT"));

	GridFunction sgasFunc;
	if (m_pReciveData->IsExistGridFunction("SGAS"))
	{
		sgasFunc = m_pReciveData->GetGridFunction(_T("SGAS"));
	}
	
	COleDateTime curTime;

	double sgasVal = UNACTVALUE;
	double swatVal = UNACTVALUE;
	double soilVal = UNACTVALUE;

	GridNo gridNo(0, 0, 0);

	//网格规模
	long nI, nJ, nK;
	m_pReciveData->GetGridSize(nI, nJ, nK);

	time_t rawtime;
	time(&rawtime);

	struct tm* timeinfo;
	timeinfo = localtime(&rawtime);
	stringstream stream;
	stream << "开始时间" << timeinfo->tm_hour<<":"<<timeinfo->tm_min<<":"<<timeinfo->tm_sec;
	::OutputDebugString(stream.str().c_str());

	for (size_t timeNo = 0;timeNo < m_pReciveData->m_SimTimes.size();++timeNo)
	{
		curTime = m_pReciveData->m_SimTimes[timeNo];
		//增加一个时间步进的数据
		CreateGridPropData(*m_pReciveData , curTime ,m_strPropName);

		//计算数据
		for (long kIndex = 0;kIndex < nK ;++kIndex)
		{
			for (long iIndex = 0;iIndex < nI ;++iIndex)
			{
				for (long jIndex = 0;jIndex < nJ ;++jIndex)
				{
					gridNo.noI = iIndex + 1;
					gridNo.noJ = jIndex + 1;
					gridNo.noK = kIndex + 1;

					if (!m_pReciveData->IsGridAct(gridNo))
					{
						//无效网格，直接给无效值
						continue;
					}

					gridNo.noI = iIndex;
					gridNo.noJ = jIndex;
					gridNo.noK = kIndex;

					//计算Soil
					if (m_pReciveData->IsExistGridFunction("SGAS"))
					{
						sgasVal = sgasFunc.getValue(*m_pReciveData , curTime ,gridNo);
						swatVal = swatFunc.getValue(*m_pReciveData , curTime ,gridNo);

						if (sgasVal != UNACTVALUE && swatVal != UNACTVALUE )
						{
							soilFunc.setValue(*m_pReciveData ,curTime ,gridNo, 1.0 - sgasVal - swatVal);
						}
					}
					else
					{
						swatVal = swatFunc.getValue(*m_pReciveData , curTime ,gridNo);

						if (swatVal != UNACTVALUE )
						{
							soilFunc.setValue(*m_pReciveData ,curTime ,gridNo, 1.0 - swatVal);
						}
					}
				}
			}
		}

		CProgressControl::getSingletonPtr()->StepProgress(_T("当前属性：SOIL"));
	}

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	stream << "开始时间" << timeinfo->tm_hour<<":"<<timeinfo->tm_min<<":"<<timeinfo->tm_sec;
	::OutputDebugString(stream.str().c_str());
	

	//清空临时数据
	soilFunc.clearData();
	if (m_pReciveData->IsExistGridFunction("SGAS"))
	{
		sgasFunc.clearData();
	}
	swatFunc.clearData();
	
	//结束进度条显示
	CProgressControl::getSingletonPtr()->StepProgress(100 , _T("完成数据计算！"),_T("当前属性：SOIL") );

	return TRUE;
}

BOOL CGridPropUnitPo::OGIPdata()
{
	//显示进度条
	CProgressControl::getSingletonPtr()->StepProgress(0 , _T("正在计算数据："),_T("当前属性：OGIP") );
	///////////////////////////////////////////////////////////////////////////////////////////
	//计算OGIP
	////OGIP = PORV*SGAS
	//各属性回调函数
	GridFunction ogipFunc = m_pReciveData->GetGridFunction(_T("OGIP"));
	GridFunction sgasFunc = m_pReciveData->GetGridFunction(_T("SGAS"));
	GridFunction porvFunc = m_pReciveData->GetGridFunction(_T("PORV"));
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	COleDateTime curTime;
	//要用到的变量，避免频繁定义
	double porvVal = UNACTVALUE ;
	double sgasVal = UNACTVALUE ;
	GridNo gridNo(0  , 0 , 0 );
	// 网格规模
	long nI, nJ, nK;
	m_pReciveData->GetGridSize(nI, nJ, nK);

	for (size_t timeNo = 0;timeNo < m_pReciveData->m_SimTimes.size();++timeNo)
	{
		curTime = m_pReciveData->m_SimTimes[timeNo];
		//增加一个时间步进的数据
		CreateGridPropData(*m_pReciveData , curTime , m_strPropName);
		//计算
		for (long kIndex = 0;kIndex < nK ;++kIndex)
		{
			for (long iIndex = 0;iIndex < nI ;++iIndex)
			{
				for (long jIndex = 0;jIndex < nJ ;++jIndex)
				{
					gridNo.noI = iIndex + 1;
					gridNo.noJ = jIndex + 1;
					gridNo.noK = kIndex + 1;
					if (!m_pReciveData->IsGridAct(gridNo))
					{
						//无效网格，直接给无效值
						continue;
					}
					gridNo.noI = iIndex;
					gridNo.noJ = jIndex;
					gridNo.noK = kIndex;
					porvVal = porvFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
					if ( porvVal != UNACTVALUE )
					{
						//OGIP = PORV*SGAS
						sgasVal = sgasFunc.getValue(*m_pReciveData , curTime ,gridNo) ;	
						if (sgasVal != UNACTVALUE )
						{
							ogipFunc.setValue(*m_pReciveData ,curTime ,gridNo, sgasVal * porvVal);	
						}
					}
				}
			}
		}
		CProgressControl::getSingletonPtr()->StepProgress(_T("当前属性：OGIP"));
	}
	//清空临时数据
	porvFunc.clearData();
	sgasFunc.clearData();
	ogipFunc.clearData();
	//////////////////////////////////////////////////////////////////////////
	//结束进度条显示
	CProgressControl::getSingletonPtr()->StepProgress(100 , _T("完成数据计算！"),_T("当前属性：OGIP") );
	return TRUE;
}

BOOL CGridPropUnitPo::OWIPdata()
{
	//显示进度条
	CProgressControl::getSingletonPtr()->StepProgress(0 , _T("正在计算数据："),_T("当前属性：OWIP") );
	///////////////////////////////////////////////////////////////////////////////////////////
	//计算OWIP
	//OWIP = PORV*SWAT	
	//各属性回调函数
	GridFunction owipFunc = m_pReciveData->GetGridFunction(_T("OWIP"));
	GridFunction swatFunc = m_pReciveData->GetGridFunction(_T("SWAT"));
	GridFunction porvFunc = m_pReciveData->GetGridFunction(_T("PORV"));	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	COleDateTime curTime;
	//要用到的变量，避免频繁定义
	double porvVal = UNACTVALUE ;
	double swatVal = UNACTVALUE ;
	GridNo gridNo(0  , 0 , 0 );
	// 网格规模
	long nI, nJ, nK;
	m_pReciveData->GetGridSize(nI, nJ, nK);

	for (size_t timeNo = 0;timeNo < m_pReciveData->m_SimTimes.size();++timeNo)
	{
		curTime = m_pReciveData->m_SimTimes[timeNo];
		//增加一个时间步进的数据
		CreateGridPropData(*m_pReciveData , curTime ,m_strPropName);
		//计算
		for (long kIndex = 0;kIndex < nK ;++kIndex)
		{
			for (long iIndex = 0;iIndex < nI ;++iIndex)
			{
				for (long jIndex = 0;jIndex < nJ ;++jIndex)
				{
					gridNo.noI = iIndex + 1;
					gridNo.noJ = jIndex + 1;
					gridNo.noK = kIndex + 1;
					if (!m_pReciveData->IsGridAct(gridNo))
					{
						//无效网格，直接给无效值
						continue;
					}
					gridNo.noI = iIndex;
					gridNo.noJ = jIndex;
					gridNo.noK = kIndex;
					porvVal = porvFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
					if ( porvVal != UNACTVALUE )
					{
						//OWIP = PORV*SWAT
						//计算OWIP
						swatVal = swatFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
						if (swatVal != UNACTVALUE )
						{
							owipFunc.setValue(*m_pReciveData , curTime , gridNo , swatVal * porvVal);	
						}
					}
				}
			}
		}
		CProgressControl::getSingletonPtr()->StepProgress(_T("当前属性：OWIP"));
	}
	//清空临时数据
	porvFunc.clearData();
	swatFunc.clearData();
	owipFunc.clearData();
	//////////////////////////////////////////////////////////////////////////
	//结束进度条显示
	CProgressControl::getSingletonPtr()->StepProgress(100 , _T("完成数据计算！"),_T("当前属性：OWIP") );
	return TRUE;
}

BOOL CGridPropUnitPo::OOIPdata()
{
	//显示进度条
	CProgressControl::getSingletonPtr()->StepProgress(0 , _T("正在计算数据："),_T("当前属性：OOIP") );
	///////////////////////////////////////////////////////////////////////////////////////////
	//计算OOIP
	//OOIP = PORV*SOIL
	//各属性回调函数
	GridFunction soilFunc = m_pReciveData->GetGridFunction(_T("SOIL"));
	GridFunction ooipFunc = m_pReciveData->GetGridFunction(_T("OOIP"));
	GridFunction porvFunc = m_pReciveData->GetGridFunction(_T("PORV"));
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	COleDateTime curTime;
	//要用到的变量，避免频繁定义
	double porvVal = UNACTVALUE ;
	double soilVal = UNACTVALUE ;
	GridNo gridNo(0  , 0 , 0 );
	// 网格规模
	long nI, nJ, nK;
	m_pReciveData->GetGridSize(nI, nJ, nK);

	for (size_t timeNo = 0;timeNo < m_pReciveData->m_SimTimes.size();++timeNo)
	{
		curTime = m_pReciveData->m_SimTimes[timeNo];
		//增加一个时间步进的数据
		CreateGridPropData(*m_pReciveData , curTime ,m_strPropName);
		//计算
		for (long kIndex = 0;kIndex < nK ;++kIndex)
		{
			for (long iIndex = 0;iIndex < nI ;++iIndex)
			{
				for (long jIndex = 0;jIndex < nJ ;++jIndex)
				{
					gridNo.noI = iIndex + 1;
					gridNo.noJ = jIndex + 1;
					gridNo.noK = kIndex + 1;
					if (!m_pReciveData->IsGridAct(gridNo))
					{
						//无效网格，直接给无效值
						continue;
					}
					gridNo.noI = iIndex;
					gridNo.noJ = jIndex;
					gridNo.noK = kIndex;
					porvVal = porvFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
					soilVal = soilFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
					if ( porvVal != UNACTVALUE && soilVal != UNACTVALUE )
					{
						ooipFunc.setValue(*m_pReciveData ,curTime ,gridNo, soilVal * porvVal);
					}
				}
			}
		}
		CProgressControl::getSingletonPtr()->StepProgress(_T("当前属性：OOIP") );
	}
	//清空临时数据
	porvFunc.clearData();
	soilFunc.clearData();
	ooipFunc.clearData();
	//////////////////////////////////////////////////////////////////////////
	//结束进度条显示
	CProgressControl::getSingletonPtr()->StepProgress(100 , _T("完成数据计算！"),_T("当前属性：OOIP") );
	return TRUE;
}
BOOL CGridPropUnitPo::TSATdata()
{
	//（三相图） TSAT = a * SWAT + b * SOIL + c * SGAS a,b,c为输入参数
	//显示进度条
	CProgressControl::getSingletonPtr()->StepProgress(0 , _T("正在计算数据："),_T("当前属性：TSAT") );
	///////////////////////////////////////////////////////////////////////////////////////////
	//计算TSAT
	// TSAT = a * SWAT + b * SOIL + c * SGAS
	//各属性回调函数
	GridFunction tsatFunc = m_pReciveData->GetGridFunction(_T("TSAT"));
	GridFunction soilFunc = m_pReciveData->GetGridFunction(_T("SOIL"));	
	GridFunction swatFunc = m_pReciveData->GetGridFunction(_T("SWAT"));
	GridFunction sgasFunc = m_pReciveData->GetGridFunction(_T("SGAS"));
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	COleDateTime curTime;
	//要用到的变量，避免频繁定义
	double swatVal = UNACTVALUE ;
	double soilVal = UNACTVALUE ;
	double sgasVal = UNACTVALUE ;
	Structure::CALC_PARAMSOPTION options;
	InitOption(options);
	double a = options.m_TSATaValue;
	double b = options.m_TSATbValue;
	double c = options.m_TSATcValue;
	GridNo gridNo(0  , 0 , 0 );
	// 网格规模
	long nI, nJ, nK;
	m_pReciveData->GetGridSize(nI, nJ, nK);

	for (size_t timeNo = 0;timeNo < m_pReciveData->m_SimTimes.size();++timeNo)
	{
		curTime = m_pReciveData->m_SimTimes[timeNo];
		//增加一个时间步进的数据
		CreateGridPropData(*m_pReciveData , curTime ,m_strPropName);
		//计算
		for (long kIndex = 0;kIndex < nK ;++kIndex)
		{
			for (long iIndex = 0;iIndex < nI ;++iIndex)
			{
				for (long jIndex = 0;jIndex < nJ ;++jIndex)
				{
					gridNo.noI = iIndex + 1;
					gridNo.noJ = jIndex + 1;
					gridNo.noK = kIndex + 1;
					if (!m_pReciveData->IsGridAct(gridNo))
					{
						//无效网格，直接给无效值
						continue;
					}
					gridNo.noI = iIndex;
					gridNo.noJ = jIndex;
					gridNo.noK = kIndex;
					swatVal = swatFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
					soilVal = soilFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
					sgasVal = sgasFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
					if ( swatVal != UNACTVALUE && soilVal != UNACTVALUE && sgasVal != UNACTVALUE)
					{
						tsatFunc.setValue(*m_pReciveData ,curTime ,gridNo, a * swatVal + b * soilVal + c * sgasVal);
					}
				}
			}
		}
		CProgressControl::getSingletonPtr()->StepProgress(_T("当前属性：TSAT"));
	}
	//清空临时数据
	swatFunc.clearData();
	soilFunc.clearData();
	sgasFunc.clearData();
	tsatFunc.clearData();
	//////////////////////////////////////////////////////////////////////////
	//结束进度条显示
	CProgressControl::getSingletonPtr()->StepProgress(100 , _T("完成数据计算！"),_T("当前属性：TSAT") );
	return TRUE;
}

BOOL CGridPropUnitPo::FIPOIL1data()
{
	//FIPOIL1 （剩余油丰度） FIPOIL1 = FIPOIL / 网格面积 单位为 万吨/平方公里 
	//各属性回调函数
	GridFunction fipoil1Func = m_pReciveData->GetGridFunction(_T("FIPOIL1"));
	GridFunction fipoilFunc = m_pReciveData->GetGridFunction(_T("FIPOIL"));
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//要用到的变量，避免频繁定义
	double fipoiltVal = UNACTVALUE ;
	double gridArea = 0;
	//显示进度条
	CProgressControl::getSingletonPtr()->StepProgress(0 , _T("正在计算数据："),_T("当前属性：FIPOIL1") );
	///////////////////////////////////////////////////////////////////////////////////////////
	//计算	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	COleDateTime curTime;
	GridNo gridNo(0  , 0 , 0 );
	// 网格规模
	long nI, nJ, nK;
	m_pReciveData->GetGridSize(nI, nJ, nK);

	for (size_t timeNo = 0;timeNo < m_pReciveData->m_SimTimes.size();++timeNo)
	{
		curTime = m_pReciveData->m_SimTimes[timeNo];
		//增加一个时间步进的数据
		CreateGridPropData(*m_pReciveData , curTime ,m_strPropName);
		//计算
		for (long kIndex = 0;kIndex < nK ;++kIndex)
		{
			for (long iIndex = 0;iIndex < nI ;++iIndex)
			{
				for (long jIndex = 0;jIndex < nJ ;++jIndex)
				{
					gridNo.noI = iIndex + 1;
					gridNo.noJ = jIndex + 1;
					gridNo.noK = kIndex + 1;
					if (!m_pReciveData->IsGridAct(gridNo))
					{
						//无效网格，直接给无效值
						continue;
					}
					gridArea = CProjectManger::getSingletonPtr()->GetActiveCase()->GetGridData().GetGridArea(gridNo);
					gridNo.noI = iIndex;
					gridNo.noJ = jIndex;
					gridNo.noK = kIndex;
					fipoiltVal = fipoilFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
					if ( fipoiltVal != UNACTVALUE && gridArea != 0)
					{
						fipoil1Func.setValue(*m_pReciveData ,curTime ,gridNo, fipoiltVal / gridArea);
					}
				}
			}
		}
		CProgressControl::getSingletonPtr()->StepProgress(_T("当前属性：FIPOIL1"));
	}
	//清空临时数据
	fipoil1Func.clearData();
	fipoilFunc.clearData();
	//////////////////////////////////////////////////////////////////////////
	//结束进度条显示
	CProgressControl::getSingletonPtr()->StepProgress(100 , _T("完成数据计算！"),_T("当前属性：FIPOIL1") );
	return TRUE;
}

BOOL CGridPropUnitPo::FIPOIL2data()
{
	//FIPOIL2 (剩余可动油储量) FIPOIL2 = FIPOIL * ( ( SOIL - SOR) / SOIL )   SOR 为输入参数(剩余油饱和度)
	//各属性回调函数
	GridFunction fipoil2Func = m_pReciveData->GetGridFunction(_T("FIPOIL2"));
	GridFunction fipoilFunc = m_pReciveData->GetGridFunction(_T("FIPOIL"));
	GridFunction soilFunc = m_pReciveData->GetGridFunction(_T("SOIL"));
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//要用到的变量，避免频繁定义
	double fipoilVal = UNACTVALUE ;
	double soilVal = UNACTVALUE ;
	Structure::CALC_PARAMSOPTION options;
	InitOption(options);
	double sor = options.m_SORValue;
	//显示进度条
	CProgressControl::getSingletonPtr()->StepProgress(0 , _T("正在计算数据："),_T("当前属性：FIPOIL2") );
	///////////////////////////////////////////////////////////////////////////////////////////
	//计算	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	COleDateTime curTime;
	GridNo gridNo(0  , 0 , 0 );
	// 网格规模
	long nI, nJ, nK;
	m_pReciveData->GetGridSize(nI, nJ, nK);

	for (size_t timeNo = 0;timeNo < m_pReciveData->m_SimTimes.size();++timeNo)
	{
		curTime = m_pReciveData->m_SimTimes[timeNo];
		//增加一个时间步进的数据
		CreateGridPropData(*m_pReciveData , curTime ,m_strPropName);
		//计算
		for (long kIndex = 0;kIndex < nK ;++kIndex)
		{
			for (long iIndex = 0;iIndex < nI ;++iIndex)
			{
				for (long jIndex = 0;jIndex < nJ ;++jIndex)
				{
					gridNo.noI = iIndex + 1;
					gridNo.noJ = jIndex + 1;
					gridNo.noK = kIndex + 1;
					if (!m_pReciveData->IsGridAct(gridNo))
					{
						//无效网格，直接给无效值
						continue;
					}
					gridNo.noI = iIndex;
					gridNo.noJ = jIndex;
					gridNo.noK = kIndex;
					fipoilVal = fipoilFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
					soilVal = soilFunc.getValue(*m_pReciveData , curTime ,gridNo) ;
					if (  fipoilVal != UNACTVALUE && soilVal != 0 && soilVal != UNACTVALUE)
					{
						fipoil2Func.setValue(*m_pReciveData ,curTime ,gridNo, fipoilVal * (soilVal - sor) / soilVal);
					}
				}
			}
		}
		CProgressControl::getSingletonPtr()->StepProgress(_T("当前属性：FIPOIL2"));
	}
	//清空临时数据
	fipoil2Func.clearData();
	fipoilFunc.clearData();
	soilFunc.clearData();
	//////////////////////////////////////////////////////////////////////////
	//结束进度条显示
	CProgressControl::getSingletonPtr()->StepProgress(100 , _T("完成数据计算！"),_T("当前属性：FIPOIL2") );
	return TRUE;
}

//计算fw
BOOL CGridPropUnitPo::fwdata()
{
	//显示进度条

	GridFunction fw_Func = m_pReciveData->GetGridFunction(_T("fw"));

	GridFunction WATKR_Func = m_pReciveData->GetGridFunction(_T("WATKR"));
	GridFunction WAT_VISC_Func = m_pReciveData->GetGridFunction(_T("WAT_VISC"));
	GridFunction OILKR_Func = m_pReciveData->GetGridFunction(_T("OILKR"));
	GridFunction OIL_VISC_Func = m_pReciveData->GetGridFunction(_T("OIL_VISC"));

	double WATKR = UNACTVALUE;    //水相相对渗透率
	double WAT_VISC = UNACTVALUE; //水粘度
	double OILKR = UNACTVALUE;    //油相相对渗透率
	double OIL_VISC = UNACTVALUE; //油粘度

	COleDateTime curTime;
	GridNo gridNo(0, 0, 0);
	
	long nI, nJ, nK;
	m_pReciveData->GetGridSize(nI, nJ, nK);

	for (size_t timeNo = 0; timeNo < m_pReciveData->m_SimTimes.size(); timeNo++)
	{
		curTime = m_pReciveData->m_SimTimes[timeNo];
		CreateGridPropData(*m_pReciveData, curTime, m_strPropName);

		for (long kIndex = 0; kIndex < nK; kIndex++)
		{
			for (long iIndex = 0; iIndex < nI; iIndex++)
			{
				for (long jIndex = 0; jIndex < nJ; jIndex++)
				{
					gridNo.noI = iIndex + 1;
					gridNo.noJ = jIndex + 1;
					gridNo.noK = kIndex + 1;

					if (!m_pReciveData->IsGridAct(gridNo))
					{					
						continue; //无效网格
					}

					gridNo.noI = iIndex;
					gridNo.noJ = jIndex;
					gridNo.noK = kIndex;

					WATKR = WATKR_Func.getValue(*m_pReciveData, curTime, gridNo);
					WAT_VISC = WAT_VISC_Func.getValue(*m_pReciveData, curTime, gridNo);
					OILKR = OILKR_Func.getValue(*m_pReciveData, curTime, gridNo);
					OIL_VISC = OIL_VISC_Func.getValue(*m_pReciveData, curTime, gridNo);

					if (WATKR == UNACTVALUE || WAT_VISC == UNACTVALUE
					 || OILKR == UNACTVALUE || OIL_VISC == UNACTVALUE)
					{
						continue;
					}

					double resVal = 0;
					if (WATKR < 1e-3 && OILKR < 1e-3) //高老师
					{
						resVal = 0;
					}
					else
					{
						resVal = 100 * (WATKR / WAT_VISC) / (WATKR / WAT_VISC + OILKR / OIL_VISC);
					}

					fw_Func.setValue(*m_pReciveData, curTime, gridNo, resVal);
				}
			}

			CProgressControl::getSingletonPtr()->StepProgress(_T("当前属性：fw"));
		}
	}

	fw_Func.clearData();

	WATKR_Func.clearData();
	WAT_VISC_Func.clearData();
	OILKR_Func.clearData();
	OIL_VISC_Func.clearData();	

	//结束进度条显示
	CProgressControl::getSingletonPtr()->StepProgress(100 , _T("完成数据计算！"), _T("当前属性：fw") );
	return TRUE;
}

//检查当前数据是否要计算
BOOL CGridPropUnitPo::CheckCurPropData()
{
	if ( m_bFile)
	{
		return TRUE;
	}
	if ( !m_dataOffsets.empty() && !m_strFiles.empty() )
	{
		return TRUE;
	}
	double resVal;
	COleDateTime timetemp;
	CheckAndCalcProp( resVal , GridNo(0,0,0), timetemp ,TRUE );
	return FALSE;
}

void CGridPropUnitPo::CopyObj( CGridPropUnitPo& propUnit  ,CGridRecive* pReciveData)
{
	//是否要重新计算最大最小值
	propUnit.m_bRecal = m_bRecal;
	//当前记录属性名称
	propUnit.m_strPropName = m_strPropName;
	//属性最大最小值
	propUnit.m_dMaxValue = m_dMaxValue;
	propUnit.m_dMinValue = m_dMinValue;
	//TRUE 为后处理，FALSE为前处理
	propUnit.m_dataType = m_dataType;
	propUnit.m_dataRealType = m_dataRealType;
	//当前属性名
	propUnit.m_strCurStoreDataProp = m_strCurStoreDataProp;
	//当前方案名
	propUnit.m_strCurStoreDataCase = m_strCurStoreDataCase;
	//属性所属的加密区域名
	propUnit.m_lgrName = m_lgrName;
	propUnit.m_namedGridProps = m_namedGridProps;

	//////////////////////////////////////////////////////////////////////////
	//是否刷新文件缓存
	propUnit.m_bFlush = m_bFlush;
	//是否已写入文件
	propUnit.m_bFile = m_bFile;
	//是否新属性
	propUnit.m_bNewProp = m_bNewProp;
	//是否在读新属性
	propUnit.m_bReadNewProp = m_bReadNewProp;
	//属性数据的起始偏移
	propUnit.m_dataOffsets = m_dataOffsets;
	//属性数据所在文件名
	propUnit.m_strFiles = m_strFiles;
	//网格数据
	propUnit.m_pReciveData = pReciveData;
	//属性数据
	propUnit.m_GridProps = m_GridProps;
	//项目名
	propUnit.m_objName = m_objName;
	//项目路径
	propUnit.m_objPath = m_objPath;
	//网格实际偏移
	propUnit.m_realOffset = m_realOffset;
	propUnit.m_bTotalGridData = m_bTotalGridData;
}

void CGridPropUnitPo::CloseFileHandle()
{
	if (m_dateFile.m_hFile != INVALID_HANDLE_VALUE)
	{
		//打开过文件
		m_dateFile.Close();
	}
	if (m_file.m_hFile != INVALID_HANDLE_VALUE)
	{
		m_file.Flush();
		m_file.Close();
	}
	m_bFileOpen = FALSE;
}

void CGridPropUnitPo::ReadProp(const COleDateTime& time)
{
	TimedDataOffset::iterator it_dataIf = m_dataOffsets.begin();
	//////////////////////////////////////////////////////////////////////////	
	std::vector<float> sourceData ;
	GridValues  allPropvalues;
	//数据为整型或实型;  数据为整型	bIntOrReal = TRUE
	BOOL bIntOrReal = FALSE;
	//for (int nDate = 0; it_dataIf != m_dataOffsets.end() ;++it_dataIf , ++nDate)
	//COleDateTime time = CProjectManger::getSingletonPtr()->GetActiveCase()->GetGridData().GetCurTime();
	it_dataIf = m_dataOffsets.find(time);
	if (it_dataIf != m_dataOffsets.end())
	{
		//用文件名和数据在文件中的起始位置信息来读取数据
		//将数据块COPY到vector			
		ReadPropData( m_strFiles[it_dataIf->first] ,  sourceData ,	it_dataIf->second , bIntOrReal);
		//转换数据
		if (bIntOrReal)
		{
			DataIntread(sourceData,allPropvalues);
		}
		else
		{
			DataRealread(sourceData,allPropvalues);
		}
		//清空临时数据
		sourceData.clear();
		//添加一个时间步数据
		m_bReadNewProp = TRUE;
		CreateGridPropData(*m_pReciveData , it_dataIf->first  ,m_strPropName);
		//设置数据
		SetTimePropDatas(*m_pReciveData , allPropvalues , it_dataIf->first );
		m_bReadNewProp = FALSE;
		//////////////////////////////////////////////////////////////////////////
	}
}

///////////////////////////////////////////////////////////
CString& CGridPropUnitPo::get_m_strCurStoreDataCase()
{
	return m_strCurStoreDataCase;
}

CString& CGridPropUnitPo::get_m_strCurStoreDataProp()
{
	return m_strCurStoreDataProp;
}

TimedGridProp& CGridPropUnitPo::get_m_GridProps()
{
	return m_GridProps;
}

std::map<long, long>& CGridPropUnitPo::get_m_realOffset()
{
	return m_realOffset;
}

std::map<long, long>& CGridPropUnitPo::get_m_totalOffset()
{
	return m_totalOffset;
}

BOOL& CGridPropUnitPo::get_m_bCalcState()
{
	return m_bCalcState;
}

BOOL& CGridPropUnitPo::get_m_bCalcAllProp()
{
	return m_bCalcAllProp;
}
///////////////////////////////////////////////////////////

/************************************************************************/
/* 设置HiSim模拟数据回调方式和数据读取方式                                */
/************************************************************************/

void CGridPropUnitPo::BindHSGridFunction( CGridRecive& gridRev )
{
	//绑定回调函数
	GridFunction gridFun;
	gridFun.createGridPropData = boost::bind(&CGridPropUnitPo::CreateHSGridPropData, this , _1,_2,_3);
	gridFun.getKMinMaxValue = boost::bind(&CGridPropUnitPo::GetHSKMinMaxValue, this , _1,_2,_3,_4,_5);
	gridFun.setValue = boost::bind(&CGridPropUnitPo::ModifyHSGridPropData, this , _1,_2,_3,_4);
	gridFun.getValue = boost::bind(&CGridPropUnitPo::GetHSPropValue, this , _1,_2,_3);
	gridFun.getMinMax = boost::bind(&CGridPropUnitPo::GetHSMinMaxValue, this , _1,_2);
	gridFun.clearData = boost::bind(&CGridPropUnitPo::ClearHSData, this );
	gridFun.getDataType = boost::bind(&CGridPropUnitPo::GetHSDataType, this );
	gridFun.isExistTime = boost::bind(&CGridPropUnitPo::IsExistHSTime, this ,_1);
	gridFun.checkCurPropData = boost::bind(&CGridPropUnitPo::CheckHSCurPropData, this );
//	gridFun.copyGridPropUnit = boost::bind(&CGridPropUnitPo::CopyObj, this , _1,_2);
	gridFun.closeFileHandle = boost::bind(&CGridPropUnitPo::CloseHSFileHandle, this );
	gridRev.SetGridFunction(m_strPropName , gridFun);
	//网格数据
	m_pReciveData = &gridRev;
	//生成的文件名
	//文件名＝项目名.属性名
	CString fileName;
	//文件名
	fileName.Format("%s\\%s.%s" , m_objPath , m_objName , m_strPropName);	
	//打开文件
	if ( m_bFile )
	{
		// 若文件已打开则先关闭文件
		if (m_bFileOpen)
		{
			m_file.Close();
		}

		//打开文件
		if (m_file.Open(fileName , CFile::modeReadWrite | CFile::typeBinary ))
		{
			m_bFileOpen = TRUE;
		}
		else
		{
			//打开失败
			AfxMessageBox(fileName + "打开失败，将重新计算");
			m_bFile = FALSE;
			m_strFiles.clear();
			m_dataOffsets.clear();
		}
	}
}

void CGridPropUnitPo::CreateHSGridPropData( const CGridRecive& gridRev,const COleDateTime& time,
										   const CString& strPropName )
{
	if (m_strPropName.IsEmpty())
	{
		m_strPropName = strPropName;
	}
	ASSERT(!m_strPropName.IsEmpty());
	ASSERT(strPropName == m_strPropName);	
	//直接根据网格数分配内存
	long lISize = 0;
	long lJSize = 0;
	long lKSize = 0;
	gridRev.GetGridSize(lISize ,lJSize ,lKSize);
	ASSERT(lISize * lJSize * lKSize != 0);

	if ( m_strCurStoreDataProp != m_strPropName )
	{
		//只是临时取的属性,读到m_namedGridProps,再写到文件
		//确定未读取过该时间
		ASSERT(m_namedGridProps.find(time) == m_namedGridProps.end());
		////调整容器大小！
		//m_namedGridProps[time].resize(lISize * lJSize * lKSize,UNACTVALUE);
	}
	else
	{
		//确定未读取过该时间
		ASSERT(m_GridProps.find(time) == m_GridProps.end());
		////调整容器大小！
		//m_GridProps[time].resize(lISize * lJSize * lKSize,UNACTVALUE);
	}	
	if (m_dataOffsets.empty())
	{
		//新建的属性
		m_bNewProp = TRUE;
	}
	if (m_bNewProp && !m_bReadNewProp )
	{
		//新建属性，且不是读属性时调用
		//设置新建属性的文件信息
		m_dataOffsets[time] = 0;
		m_strFiles[time] = _T("");
		WriteData();
	}
}

//修改网格数据
void CGridPropUnitPo::ModifyHSGridPropData( CGridRecive& gridRev,const COleDateTime& time,
										   const GridNo& gridNo,const double& dValue )									  
{
	ASSERT(!m_strPropName.IsEmpty());	

	//检查数据是否要计算
	double resVal;
	CheckAndCalcProp(resVal , gridNo , time ,TRUE);
	//获取网格数
	long ISize = 0;
	long JSize = 0;
	long KSize = 0;
	gridRev.GetGridSize(ISize , JSize , KSize);
	//计算网格索引
	long gridIndex = gridNo.noK * JSize  * ISize  + gridNo.noJ * ISize + gridNo.noI;
	//设置指定网格的属性值	
	COleDateTime  curtime = time;
	if (m_dataOffsets.find(time) == m_dataOffsets.end())
	{
		TimedDataOffset::iterator propIter = m_dataOffsets.end();
		--propIter;
		//时间设为最后一个时间
		curtime = propIter->first;
	}

	if (!m_bTotalGridData)
	{
		gridIndex = gridRev.GetGridValueIndex(gridIndex);
	}

	if (gridIndex == -1) //无效网格
	{
		return;
	}

	//修改文件中的数据
	ModifyGridPropData( curtime, gridIndex, dValue );

	if ( m_strCurStoreDataProp != m_strPropName )
	{
		//只是临时修改的属性,直接写文件
		if (m_bCalcState)
		{
			////计算时修改
			//if (m_namedGridProps.empty())
			//{
			//	ReadData();
			//}
			if (m_namedGridProps[curtime].size() <= gridIndex)
			{
				m_namedGridProps[curtime].resize(gridIndex+1, UNACTVALUE);
			}
			m_namedGridProps[curtime][gridIndex] = dValue; 
		}
		return;
	}

	//修改内存中的数据
	ASSERT(m_GridProps.find(curtime) != m_GridProps.end());
	if(m_GridProps[curtime].size() == 0)
	{
		m_GridProps[curtime].resize(ISize * JSize * KSize, UNACTVALUE);
	}

	m_GridProps[curtime][gridIndex] = dValue;

	//设置要重新计算属性最大最小值
	m_bRecal = TRUE;
}

double CGridPropUnitPo::GetHSPropValue( const CGridRecive& gridRev,const COleDateTime& time,const GridNo& gridNo )
{
	//查询属性数据时，先查询数据map是否为空
	//如果为空，再查询数据所对应的文件信息是否为空
	//如果为空则读以属性名命名的文件
	//查询到的属性数据
	double dbRes = UNACTVALUE;
	if (!m_bCalcState)
	{
		//不是计算时取值才判断是否有效网格
		//计算时在取值前已判断
		if (!m_pReciveData->IsGridAct(GridNo(gridNo.noI+1 , gridNo.noJ + 1 ,gridNo.noK + 1)))
		{
			//无效网格
			return dbRes;
		}
	}

	//属性值时间	
	COleDateTime  curtime = time;

	COleDateTime originTime = static_cast<CDataEclipse*>(CProjectManger::getSingletonPtr()->GetActiveCase())->GetStartDate();

	/*std::vector<COleDateTime> allTimes = gridRev.GetAllTimes();
	COleDateTime originTime = allTimes[0];*/
	COleDateTime currentTime = curtime - originTime;
	//检查数据是否要计算
	/*if (CheckAndCalcProp(dbRes , gridNo , curtime , m_bCalcAllProp))
	{
	return dbRes;
	}*/

	//网格数
	long ISize = 0;
	long JSize = 0;
	long KSize = 0;

	//获取网格数
	gridRev.GetGridSize(ISize ,JSize ,KSize);

	//属性值索引 注意是long long
	long long gridIndex = gridNo.noK * JSize  * ISize  + gridNo.noJ * ISize + gridNo.noI;

	/*if ( m_strCurStoreDataProp != m_strPropName )
	{*/
		//if (m_bCalcState)
		//{
		//	//计算时取值
		//	return GetCalcPropValue( curtime, gridIndex , TRUE ); 
		//}

		//只是临时取的属性,读到m_namedGridProps,再写到文件
		//从原文件里取数据
		if (curtime != m_pretime)
		{
			m_pretime = curtime;
		}
		KWMapper maper;
		CString propName = maper.getItem(m_strPropName);


		double propResult;
		bool getResult;
		PropTypes propType;
		HiSimHandle* hsHandle = HiSimHandle::Instance();

		if(propType.GetPropType(m_strPropName) == 0){
			//std::vector<double> propVec;
            propResult = hsHandle->getListAt<double>((LPSTR)(LPCTSTR)propName,gridIndex,&getResult);
				//propResult = propVec.at(gridIndex);
				//getResult = true;
			//}
		}
		else if(propType.GetPropType(m_strPropName) == 1){
			propResult = hsHandle->getlistAtTime<double>((LPSTR)(LPCTSTR)propName,gridIndex,currentTime.m_dt,&getResult);
		}
		return getResult ? propResult : dbRes;
	//}
	//if (m_bCalcState)
	//{
	//	//计算时取值
	//	return GetCalcPropValue( curtime, gridIndex , FALSE ); 
	//}

	////缓存写入文件
	//FlushFile();
	////获取指定网格属性值，如该时间无值，则返回最后一个时间的值
	//return GetCalcPropValue( curtime, gridIndex , FALSE ); 
}

double CGridPropUnitPo::GetHSPropValue( COleDateTime curtime, long gridIndex )
{
	double dbRes = UNACTVALUE;
	////查找文件中的数据
	////取数据
	//ASSERT(m_dataOffsets.find(curtime) != m_dataOffsets.end());
	////调整文件指针到数据位置
	//m_file.Seek(8 * (gridIndex + 1) + m_dataOffsets[curtime] , CFile::begin); //周纵横
	//m_file.Read( &dbRes , 8);
	return dbRes;
}

void CGridPropUnitPo::GetHSMinMaxValue( double& dMaxValue,double& dMinValue )
{
	if (m_strCurStoreDataProp != m_strPropName || m_strCurStoreDataCase != GetDataCaseName())
	{
		//属性切换了
		m_GridProps.clear();
		//设置当前显示的属性名
		m_strCurStoreDataProp = m_strPropName;
		m_strCurStoreDataCase = GetDataCaseName();
	}

	KWMapper kwmaper;
	CString propName = kwmaper.getItem(m_strCurStoreDataProp);
    PropTypes propType;
    HiSimHandle* hsHandle = HiSimHandle::Instance();
    bool success = false;
    if(propType.GetPropType(m_strPropName) == 0){
        hsHandle->getListRange<double>((LPSTR)(LPCTSTR)propName,&dMinValue,&dMaxValue,&success);
    }
    else if(propType.GetPropType(m_strPropName) == 1){
        hsHandle->getListSequenceRange<double>((LPSTR)(LPCTSTR)propName,&dMinValue,&dMaxValue,&success);
    }
	
}

void CGridPropUnitPo::ClearHSData()
{
	//清空临时数据
	m_namedGridProps.clear();
}

BOOL CGridPropUnitPo::GetHSKMinMaxValue(CGridRecive& gridRecive , const COleDateTime& time,long layer,double& dMaxValue,double& dMinValue)
{
	//设置当前显示的属性名
	m_strCurStoreDataProp = m_strPropName;

	long ISize = 0;
	long JSize = 0;
	long KSize = 0;
	gridRecive.GetGridSize(ISize ,JSize ,KSize);

	//获取当前时间当前层属性最大最小值
	BOOL bInit = FALSE;
	long nSkip = layer * JSize * ISize;
	long surSize = JSize * ISize;

	double curVal = UNACTVALUE;
	for (long nIndex = 0; nIndex < surSize; ++nIndex)
	{
		int gridnoI;
		int gridnoJ;
		if ((nIndex + 1) % ISize == 0)
		{
			gridnoI = ISize;
			gridnoJ = (nIndex + 1) / ISize;
		}
		else
		{
			gridnoI = (nIndex + 1) % ISize;
			gridnoJ = (nIndex + 1) / ISize + 1;
		}

		GridNo gridNo(gridnoI, gridnoJ, layer + 1);
		if (gridRecive.IsGridAct(gridNo) == false) //无效网格
		{
			continue;
		}

		int newIndex;
		if (!m_bTotalGridData)
		{
			//如果nSkip + nIndex=496, 前面有4个无效网格
			newIndex = gridRecive.GetGridValueIndex(nSkip + nIndex); //执行了这一句后newIndex=492
		}
		else
		{
			newIndex = nSkip + nIndex;
		}

		if (m_GridProps.size() == 1)
		{
			//对于静态属性DZNET, 切换时间的时候, m_GridProps里面始终只有一个时间的数据
			curVal = m_GridProps.begin()->second[newIndex];
		}
		else
		{
			curVal = m_GridProps[time][newIndex];
		}

		if (curVal != UNACTVALUE)
		{
			if (!bInit)
			{
				//未初始化则初始化
				dMaxValue = dMinValue = curVal;
				bInit = TRUE;
			}
			else
			{
				dMaxValue = max(dMaxValue , curVal);
				dMinValue = min(dMinValue , curVal);
			}
		}
	}

	return TRUE;
}
BOOL CGridPropUnitPo::IsExistHSTime(const COleDateTime& time )
{
	if (m_dataOffsets.find( time ) == m_dataOffsets.end())
	{
		//该时间没有数据
		if (m_bCalcState == TRUE)
		{
			double dbRes;
			CheckAndCalcProp(dbRes , GridNo(0 , 0 , 0) , time , TRUE);
		}
		if (m_dataOffsets.find( time ) == m_dataOffsets.end())
		{
			return FALSE;
		}
	}
	return TRUE;
}

//检查当前数据是否要计算
BOOL CGridPropUnitPo::CheckHSCurPropData()
{
	/*if ( m_bFile)
	{
	return TRUE;
	}
	if ( !m_dataOffsets.empty() && !m_strFiles.empty() )
	{
	return TRUE;
	}
	double resVal;
	COleDateTime timetemp;
	CheckAndCalcProp( resVal , GridNo(0,0,0), timetemp ,TRUE )*/;
	return FALSE;
}


void CGridPropUnitPo::CloseHSFileHandle()
{
	if (m_dateFile.m_hFile != INVALID_HANDLE_VALUE)
	{
		//打开过文件
		m_dateFile.Close();
	}
	if (m_file.m_hFile != INVALID_HANDLE_VALUE)
	{
		m_file.Flush();
		m_file.Close();
	}
	m_bFileOpen = FALSE;
}