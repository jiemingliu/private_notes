/********************************************************************
创建日期:	2008/10/15
文件路径:	\CommonData\Data
文 件 名:	GridPropUnitBase
文件后缀:	h
作    者:	iuhiao	
说明:	数据管理类的基类
*********************************************************************/
#pragma once
#include "GridStruct.h"
#include <boost/bind.hpp>
class CGridRecive;


class AFX_EXT_CLASS CGridPropUnit
{
public:
	CGridPropUnit( const CString& strPropName);
	CGridPropUnit(void);
	virtual  ~CGridPropUnit(void);

public:
	/**
	 * 方法:	检查当前数据正确性
	 * 全名:	CGridPropUnit::CheckData
	 * 类型:	virtual public 
	 * 
	 * @date	2009/01/14
	 * @author	flf
	 * @param	long dataSize
	 * @return	BOOL
	 */
	virtual BOOL CheckData(long dataSize);
	/**
	* 方法:	清空数据
	* 全名:	CGridPropUnit::ClearData
	* 类型:	public 
	* 
	* @date	2008/08/21
	* @author	XX
	* @return	void
	*/
	virtual void ClearData() = 0;

	/**
	* 方法:	创建网格属性数据
	* 全名:	CGridPropUnit::CreateGridPropData
	* 类型:	public 
	* 
	* @date	2008/08/18
	* @author	XX
	* @param	const CGridRecive & gridRev
	* @param	const COleDateTime & time
	* @param	const CString & strPropName
	* @return	void
	*/
	virtual void CreateGridPropData(const CGridRecive& gridRev,const COleDateTime& time,
		const CString& strPropName) = 0;

	/**
	* 方法:	加入网格数据
	* 全名:	CGridPropUnit::AddGridPropData
	* 类型:	public 
	* 
	* @date	2008/08/18
	* @author	XX
	* @param	const CGridRecive & gridRev
	* @param	const COleDateTime & time
	* @param	const GridNo & gridNo
	* @param	const double & dValue
	* @return	void
	*/
	virtual void AddGridPropData(const CGridRecive& gridRev,const COleDateTime& time,
		const GridNo& gridNo,const double& dValue) = 0;

	/**
	* 方法:	修改网格数据
	* 全名:	CGridPropUnit::ModifyGridPropData
	* 类型:	public 
	* 
	* @date	2008/08/18
	* @author	XX
	* @param	const CGridRecive & gridRev
	* @param	const COleDateTime & time
	* @param	const GridNo & gridNo
	* @param	const double & dValue
	* @return	void
	*/
	virtual void ModifyGridPropData(CGridRecive& gridRev,const COleDateTime& time,
		const GridNo& gridNo,const double& dValue) = 0;

	/**
	* 方法:	获取指定时间下对应网格属性的值
	* 全名:	CGridPropUnit::GetPropValue
	* 类型:	public 
	* 
	* @date	2008/08/18
	* @author	XX
	* @param	const CGridRecive & gridRev
	* @param	const COleDateTime & time
	* @param	const GridNo & gridNo
	* @return	double
	*/
	virtual double GetPropValue(const CGridRecive& gridRev,const COleDateTime& time,const GridNo& gridNo) = 0;

	/**
	* 方法:	计算所有时间最大最小值
	* 全名:	CGridPropUnit::ReCalcPropMaxmin
	* 类型:	public 
	* 
	* @日期 	2008/08/18
	* @作者  iuhiao
	* @返回类型 	void
	*/
	virtual void ReCalcPropMaxmin() = 0;

	/**
	* 方法:	绑定网格调用函数
	* 全名:	CGridPropUnit::BindGridFunction
	* 类型:	public 
	* 
	* @date	2008/08/18
	* @author	XX
	* @param	CGridRecive & gridRev
	* @return	void
	*/
	virtual void BindGridFunction(CGridRecive& gridRev /*, int nDataType = 0*/) = 0;

	/**
	* 方法:	获取当前属性最大最小值
	* 全名:	CDataPoFloatTableValue::GetMinMaxValue
	* 类型:	public 
	* 
	* @日期 	2008/08/12
	* @作者  iuhiao
	* @参数 	double & dMaxValue
	* @参数 	double & dMinValue
	* @返回类型 	void
	*/
	virtual void GetMinMaxValue(double& dMaxValue,double& dMinValue) = 0;

	/**
	* 方法:	获取当前时间当前层属性最大最小值
	* 全名:	CGridPropUnit::GetKMinMaxValue
	* 类型:	virtual public 
	* 
	* @日期 	2008/10/15
	* @作者  iuhiao
	* @参数 	CGridRecive & gridRecive
	* @参数 	const COleDateTime & time
	* @参数 	long layer
	* @参数 	double & dMaxValue
	* @参数 	double & dMinValue
	* @返回类型 	BOOL
	*/
	virtual BOOL GetKMinMaxValue(CGridRecive& gridRecive , const COleDateTime& time,long layer,
		double& dMaxValue,double& dMinValue) = 0;

	/**
	* 方法:	设置属性数据
	* 全名:	CGridPropUnit::SetTimePropDatas
	* 类型:	public 
	* 
	* @日期 	2008/08/18
	* @作者  iuhiao
	* @参数 	CGridRecive & gridRecive
	* @参数 	GridValues & allPropvalues
	* @参数 	COleDateTime & time
	* @返回类型 	void
	*/
	virtual void SetTimePropDatas(CGridRecive& gridRecive ,GridValues& allPropvalues ,const COleDateTime& time) = 0;

	/**
	* 方法:	判断该时间步是否已读,读重启文件时要用到
	* 全名:	CGridPropUnit::IsExistTime
	* 类型:	public 
	* 
	* @日期 	2008/08/24
	* @作者  iuhiao
	* @参数 	COleDateTime & time
	* @返回类型 	BOOL  /TRUE已有
	*/
	virtual BOOL IsExistTime(const COleDateTime& time) = 0;

	/**
	* 方法:	检查当前数据是否要计算
	* 全名:	CGridPropUnit::CheckCurPropData
	* 类型:	public 
	* @日期 	2008-12-22
	* @作者  iuhiao
	* @返回类型 	BOOL
	*/
	virtual BOOL CheckCurPropData() = 0;

	/**
	 * 方法:	设置属性所属的加密区域名
	 * 全名:	CGridPropUnit::SetLgrName
	 * 类型:	virtual public 
	 * @日期 	2008-12-31
	 * @作者  iuhiao
	 * @参数 	CString strName
	 * @返回类型 	void
	 */
	virtual void SetLgrName(CString strName);

	/**
	 * 方法:	设置读取到的数据的类型 "INTE" "CHAR"  "DOUB" "REAL"
	 * 全名:	CGridPropUnit::SetDataType
	 * 类型:	virtual public 
	 * 
	 * @date	2010/06/30
	 * @author	Administrator
	 * @param	CString dataType
	 * @return	void
	 */
	virtual void SetDataType(CString dataType = _T("REAL")){m_dataRealType = dataType;};

protected:
	/**
	 * 方法:	获取当前选中的网格属性名称
	 * 全名:	CGridPropUnit::GetCurGridProp
	 * 类型:	protected 
	 * 
	 * @date	2010/06/30
	 * @author	Administrator
	 * @return	CString
	 */
	CString GetCurGridPropName();

	/**
	 * 方法:	获取当前方案名称
	 * 全名:	CGridPropUnit::GetDataCaseName
	 * 类型:	protected 
	 * 
	 * @date	2010/06/30
	 * @author	Administrator
	 * @return	CString
	 */
	CString GetDataCaseName();

protected:
	//是否要重新计算最大最小值
	BOOL m_bRecal;

	//属性最大最小值
	double  m_dMaxValue;
	double  m_dMinValue;
	//TRUE 为后处理，FALSE为前处理
	BOOL m_dataType;
	//属性所属的加密区域名
	CString m_lgrName;

	//当前记录属性名称
	CString m_strPropName;

	//属性数据
	TimedGridProp m_namedGridProps;

	//数据类型
	CString m_dataRealType;

	//有效网格数据个数
	__int64 m_dataCount;
private:
	//序列化函数
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		//ar & BOOST_SERIALIZATION_NVP(m_namedGridProps);
		//ar & BOOST_SERIALIZATION_NVP(m_strPropName);
		//ar & BOOST_SERIALIZATION_NVP(m_dMaxValue);
		//ar & BOOST_SERIALIZATION_NVP(m_dMinValue);
		//ar & BOOST_SERIALIZATION_NVP(m_bRecal);
		//ar & BOOST_SERIALIZATION_NVP(m_dataType);
		//ar & BOOST_SERIALIZATION_NVP(m_dataRealType);
	}
};