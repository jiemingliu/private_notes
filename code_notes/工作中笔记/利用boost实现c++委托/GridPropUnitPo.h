/********************************************************************
创建日期:	2008/10/15
文件路径:	\CommonData\Data
文 件 名:	GridPropUnit
文件后缀:	h
作    者:	iuhiao	
说明:	后处理数据管理类
*********************************************************************/
#pragma once

#include "GridPropUnit.h"
#include "..\Sim3DChart\Sim2D3DFlownLineStruct.h"
#include <boost\function.hpp>
class AFX_EXT_CLASS CGridPropUnitPo :
	public CGridPropUnit
{
	friend class CGridReciveEclipse;
	friend class CDataPoFloatTableValue;
public:
	CGridPropUnitPo( const CString& strPropName);
	CGridPropUnitPo(void);
	virtual ~CGridPropUnitPo(void);

public:
	//关闭文件
	void CloseFileHandle();

	//拷贝数据
	void CopyObj(CGridPropUnitPo& propUnit ,CGridRecive* pReciveData);

	//取得数据类型TRUE 为后处理(临时取数据)，FALSE为前处理(数据在m_namedGridProps中)
	BOOL GetDataType() {return m_dataType;}; 

	//检查当前数据是否要计算
	BOOL CheckCurPropData();

	//清空数据
	void ClearData();

	//创建网格属性数据,如果是新建的属性则写文件
	void CreateGridPropData(const CGridRecive& gridRev,const COleDateTime& time,const CString& strPropName);

	//加入网格数据
	void AddGridPropData(const CGridRecive& gridRev,const COleDateTime& time,const GridNo& gridNo,const double& dValue);

	//修改网格数据
	void ModifyGridPropData(CGridRecive& gridRev,const COleDateTime& time,const GridNo& gridNo,const double& dValue);

	//获取指定时间下对应网格属性的值
	double GetPropValue(const CGridRecive& gridRev,const COleDateTime& time,const GridNo& gridNo);

	//获取指定时间所有网格值
	std::vector<double> getPropValues(const COleDateTime&,const std::vector<GridNo>&);

	//获取实际网格索引
	void getGridIndexOffset();

	// 方法:	计算所有时间最大最小值
	void ReCalcPropMaxmin();

	//绑定网格调用函数
	void BindGridFunction(CGridRecive& gridRev/* , int nDataType = 0*/);

	//获取当前属性最大最小值
	void GetMinMaxValue(double& dMaxValue,double& dMinValue);

	//获取当前时间当前层属性最大最小值
	BOOL GetKMinMaxValue(CGridRecive& gridRecive , const COleDateTime& time,long layer,	double& dMaxValue,double& dMinValue);

	//设置属性数据
	void SetTimePropDatas(CGridRecive& gridRecive ,GridValues& allPropvalues ,const COleDateTime& time);

	//判断该时间步是否已读,读重启文件时要用到
	BOOL IsExistTime(const COleDateTime& time);

	//设置数据信息
	void SetDataInfo(CString strPropName , CGridRecive* gridRecive ,CString strFile,const COleDateTime& curTime ,__int64 dataOffset,__int64 dataCount);

	/************************************************************************/
	/* HiSim 模拟数据   设置数据信息                                          */
	/************************************************************************/
	void SetDataInfo(CString strPropName,CGridRecive* gridRecive ,const COleDateTime& curTime);

	//计算数据的回调函数
	BOOL DZNETdata();
	BOOL PORVdata();
	BOOL SOILdata();
	BOOL OGIPdata();
	BOOL OWIPdata();
	BOOL OOIPdata();
	BOOL TSATdata();
	BOOL FIPOIL1data();
	BOOL FIPOIL2data();
	BOOL fwdata();

	// 设置当前属性记录的网格数据是否包括无效网格数据
	void SetTotalGridData(bool bGridTotalData) 
	{ 
		m_bTotalGridData = bGridTotalData; 
	}

public:
	/************************************************************************/
	/* 设置HiSim模拟数据回调函数和读取数据方式                                */
	/************************************************************************/
	void BindHSGridFunction(CGridRecive& gridRev);
	BOOL GetHSDataType() {return m_dataType;};
	void CGridPropUnitPo::CreateHSGridPropData( const CGridRecive& gridRev,const COleDateTime& time,
		const CString& strPropName );

	//修改网格数据
	void CGridPropUnitPo::ModifyHSGridPropData( CGridRecive& gridRev,const COleDateTime& time,
		const GridNo& gridNo,const double& dValue );
	double CGridPropUnitPo::GetHSPropValue( const CGridRecive& gridRev,const COleDateTime& time,const GridNo& gridNo );

	double CGridPropUnitPo::GetHSPropValue( COleDateTime curtime, long gridIndex );

	void CGridPropUnitPo::GetHSMinMaxValue( double& dMaxValue,double& dMinValue );

	void CGridPropUnitPo::ClearHSData();

	//方法:	获取当前时间当前层属性最大最小值
	BOOL CGridPropUnitPo::GetHSKMinMaxValue(CGridRecive& gridRecive , const COleDateTime& time,long layer,double& dMaxValue,double& dMinValue);
	BOOL CGridPropUnitPo::IsExistHSTime(const COleDateTime& time );

	//检查当前数据是否要计算
	BOOL CGridPropUnitPo::CheckHSCurPropData();
	void CGridPropUnitPo::CloseHSFileHandle();
protected:
	//读取数据
	virtual void ReadProp(const COleDateTime& time);

	// 获取当前属性记录所有网格数据时在文件中的偏移量
	long GetTotalGridValueOffset(long lGridIndex);
private:
	//初始化相关参数
	BOOL InitOption(Structure::CALC_PARAMSOPTION& options);
	//用文件名和数据在文件中的起始位置信息来读取数据
	void ReadData(const COleDateTime& time);

	//将数据写入文件
	void WriteData();

	//修改文件中的数据
	void ModifyGridPropData(COleDateTime curtime, long gridIndex, const double& dValue);


	//获取实际网格索引
	void CGridPropUnitPo::GetRealOffset( );


	//计算OOIP
	void CalcData( boost::function<BOOL ()> readFunc );

protected:
	//从文件中取单个属性值
	double GetPropValue(COleDateTime curtime, long gridIndex);

	//计算时取值
	double GetCalcPropValue(COleDateTime curtime, long gridIndex , BOOL btemp);

	//检察是否有数据，没有则计算数据
	BOOL CheckAndCalcProp(double& resVal , GridNo gridNo, COleDateTime curTime ,BOOL bAllCalc = FALSE);

	//文件缓存写入到文件
	void FlushFile();

	//打开相应文件
	BOOL MakeFileOpen(CString fileName);

protected:
	//是否刷新文件缓存
	BOOL m_bFlush;
	//是否新属性
	BOOL m_bNewProp;
	//是否在读新属性
	BOOL m_bReadNewProp;
	//网格数据
	CGridRecive* m_pReciveData;
	//数据写入的文件
	CFile m_file;
	//数据所在文件
	CFile m_dateFile;
	//m_dateFile所对应的时间
	COleDateTime  m_pretime;
	//////////////////////////////////////////////////////////////////////////
	//子类CGridPropUnitPo
	//////////////////////////////////////////////////////////////////////////
	//属性数据的起始偏移
	typedef std::map<COleDateTime, __int64> TimedDataOffset;
	TimedDataOffset m_dataOffsets;
	//属性数据所在文件名
	std::map<COleDateTime , CString> m_strFiles;
	//项目名
	CString m_objName;
	//项目路径
	CString m_objPath;
	//是否已写入文件
	BOOL m_bFile;
	BOOL m_bFileOpen;

	// 当前属性数据是否记录所有网格属性值
	bool m_bTotalGridData;

protected:

	// 当前数据集记录的属性所属的方案
	static CString m_strCurStoreDataCase;

	// 当前数据集记录的属性
	static CString m_strCurStoreDataProp;

	// 注意m_GridProps是static
	// 注意基类中有一个m_namedGridProps
	// 当前数据集记录的属性数据
	static TimedGridProp m_GridProps;

	// 当前时间数据的网格实际偏移
	static std::map<long, long> m_realOffset;

	// 当前时间数据的网格实际索引
	static std::map<long,long> m_realGridIndexOffset;

	// 记录所有属性数据时网格数的偏移量
	static std::map<long, long>	m_totalOffset;

	// 是否为计算属性
	static BOOL m_bCalcState;

	//是否计算全部属性
	static BOOL m_bCalcAllProp;

public:
	static CString& get_m_strCurStoreDataCase();
	static CString& get_m_strCurStoreDataProp();
	static TimedGridProp& get_m_GridProps();
	static std::map<long, long>& get_m_realOffset();
	static std::map<long, long>& get_m_totalOffset();
	static BOOL& get_m_bCalcState();
	static BOOL& get_m_bCalcAllProp();

private:
	//序列化函数
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		//ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CGridPropUnitPo);
		////子类CGridPropUnitPo
		//ar & BOOST_SERIALIZATION_NVP(m_dataOffsets);
		//ar & BOOST_SERIALIZATION_NVP(m_strFiles);
		//ar & BOOST_SERIALIZATION_NVP(m_objName);
		//ar & BOOST_SERIALIZATION_NVP(m_objPath);
		//ar & BOOST_SERIALIZATION_NVP(m_bFile);
	}
};
