#include "StdAfx.h"
#include ".\gridpropunit.h"
#include "..\Tools\Datadispose_inline.h"
CGridPropUnit::CGridPropUnit(void)
{
	//默认要重新计算最大最小值
	m_bRecal = TRUE;
	m_dMaxValue = UNACTVALUE;
	m_dMinValue = UNACTVALUE;
	m_strPropName = "";
	m_lgrName = "";
	m_dataType = TRUE;
	m_dataCount = 0;
}

CGridPropUnit::CGridPropUnit( const CString& strPropName )
{
	m_bRecal = TRUE;
	m_dMaxValue = UNACTVALUE;
	m_dMinValue = UNACTVALUE;
	m_strPropName = strPropName;
	m_lgrName = "";
	m_dataType = TRUE;
	m_dataCount = 0;
}
CGridPropUnit::~CGridPropUnit(void)
{
	//AfxMessageBox(m_strPropName);
}

void CGridPropUnit::SetLgrName( CString strName )
{
	m_lgrName = strName;
}

BOOL CGridPropUnit::CheckData( long dataSize )
{
	return (m_namedGridProps.begin()->second).size() == dataSize;
}

CString CGridPropUnit::GetCurGridPropName()
{

	return CProjectManger::getSingletonPtr()->GetActiveCase()->GetGridData().GetCurPropName();
}

CString CGridPropUnit::GetDataCaseName()
{
	return CProjectManger::getSingletonPtr()->GetActiveCase()->GetTitle();
}