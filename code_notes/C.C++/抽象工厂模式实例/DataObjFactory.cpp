#include "StdAfx.h"
#include ".\dataobjfactory.h"
#include "DataObjFactory_Inline.h"
#include "DataAqufetp.h"
#include "DataAquancon.h"
#include "DataAquct.h"
#include "DataEquil.h"
#include ".\Ado\KeywordAdo.h"
#include "DataEdit\CDataSPECGRID.h"
#include "DataEdit\CDataRSVD.h"
#include "DataEdit\CDATAMINPORV.h"
#include "DataEdit\CDataWRFTPLT.h"
#include "DataEdit\CDataMINPV.h"

//////////////////////////////////////////////////////////////////////////
MapDataBlock CDataObjFactory::m_mapDataBlocks;
MapKeyWord CDataObjFactory::m_mapKeyWords;
MapPoClass CDataObjFactory::m_mapPoClass;
MapPoEntity CDataObjFactory::m_mapPoEntity;

CDataObjFactory::CDataObjFactory(void)
{
}

CDataObjFactory::~CDataObjFactory(void)
{
}

CDataBlock* CDataObjFactory::CreateDataBlock(const CString& strBlockName)
{
	//定义回调函数
	typedef CDataBlock* (* ApllyCreate_FUN)(void);
	ApllyCreate_FUN  ApllyCreate = (ApllyCreate_FUN)m_mapDataBlocks[strBlockName];

	ASSERT(ApllyCreate !=NULL);
	//创建对象
	return ApllyCreate();
}

CDataKeyWord* CDataObjFactory::CreateDataKeyWord(const KeyInfo& keyInfo)
{
	ASSERT(!keyInfo.strDataKey.IsEmpty());

	//定义回调函数
	typedef CDataKeyWord* (* ApllyCreate_FUN)(void);
	ApllyCreate_FUN  ApllyCreate = (ApllyCreate_FUN)m_mapKeyWords[keyInfo.strDataKey];

	//当前可以找到创建函数，则创建已知关键词
	if (ApllyCreate != NULL)
	{
		CDataKeyWord* pKeyWord = ApllyCreate();
		ASSERT(pKeyWord != NULL);
		pKeyWord->SetTittle(keyInfo.strDataKey);
		//创建对象
		return pKeyWord;
	}

	////当前为时间无关数据，且为网格数据
	//if (!keyInfo.bIsGridData)
	//{
	//	return new CDataText(keyInfo.strDataKey);
	//}
	//else
	//{
	//	return new CDataAngleGrid(keyInfo.strDataKey);
	//}

	//ASSERT(FALSE);
	return new CDataText(keyInfo.strDataKey);
}

CDataPoClass* CDataObjFactory::CreateDataPoClass(const CString& strClassName)
{
	//定义回调函数
	typedef CDataPoClass* (* ApllyCreate_FUN)(void);
	ApllyCreate_FUN  ApllyCreate = (ApllyCreate_FUN)m_mapPoClass[strClassName];

	ASSERT(ApllyCreate !=NULL);
	//创建对象
	return ApllyCreate();
}

CDataPoEntity* CDataObjFactory::CreateDataPoEntity(const CString& strEntityName)
{
	//定义回调函数
	typedef CDataPoEntity* (* ApllyCreate_FUN)(void);
	ApllyCreate_FUN  ApllyCreate = (ApllyCreate_FUN)m_mapPoEntity[strEntityName];

	if (ApllyCreate !=NULL)
	{
		CDataPoEntity* pEntity = ApllyCreate();
		pEntity->SetTitle(strEntityName);
		//创建对象
		return pEntity;
	}

	return NULL;
}

void CDataObjFactory::RegisterType()
{
	//注册SUMMARY中各关键字
	RegisterSummaryKeyWord();

	//注册DataBlock类型
	RegisterDataBlockType();
	//注册KeyWord类型
	RegisterDataKeyWordType();
	//注册PoClass类型
	RegisterPoClassType();
	//注册PoEntity类型
	RegisterPoEntityType();
}

void CDataObjFactory::RegisterDataBlockType()
{
	//注册CDataRunspec类型
	AddDataBlockType<CDataRunspec>();
	//注册CDataGrid类型
	AddDataBlockType<CDataGrid>();
	//注册CDataEdit类型
	AddDataBlockType<CDataEdit>();
	//注册CDataProps类型
	AddDataBlockType<CDataProps>();
	//注册CDataRegions类型
	AddDataBlockType<CDataRegions>();
	//注册CDataSolution类型
	AddDataBlockType<CDataSolution>();
	//注册CDataSummary类型
	AddDataBlockType<CDataSummary>();
	//注册CDataSchedule类型
	AddDataBlockType<CDataSchedule>();
	//注册CDataOptimize类型
	AddDataBlockType<CDataOptimize>();
}

void CDataObjFactory::RegisterDataKeyWordType()
{
	//注册CDataGefac类型
	AddDataKeyWordType<CDataGefac>();

	//注册CDataTstep类型
	AddDataKeyWordType<CDataTstep>();
	//注册CDataNumres类型
	AddDataKeyWordType<CDataNumres>();
	//注册CDataCoordsys类型
	AddDataKeyWordType<CDataCoordsys>();
	//注册CDataStart类型
	AddDataKeyWordType<CDataStart>();
	//注册CDataTitle类型
	AddDataKeyWordType<CDataTitle>();
	//注册CDataFaults类型
	AddDataKeyWordType<CDataFaults>();
	//注册CDataLGR类型
	AddDataKeyWordType<CDataLGR>();
	//注册CDataDimens类型
	AddDataKeyWordType<CDataDimens>();
	//注册CDataEqldims类型
	AddDataKeyWordType<CDataEqldims>();
	//注册CDataWellDims类型
	AddDataKeyWordType<CDataWelldims>();
	//注册CDataTabdims类型
	AddDataKeyWordType<CDataTabdims>();
	//注册CDataRegdims类型
	AddDataKeyWordType<CDataRegdims>();
	//注册CDataPvto类型
	AddDataKeyWordType<CDataPvto>();
	//注册CDataUnit类型
	AddDataKeyWordType<CDataUnit>();	
	//注册CDataSimulateState类型
	AddDataKeyWordType<CDataSimulateState>();
	//注册CDataPvdo类型
	AddDataKeyWordType<CDataPvdo>();
	//注册CDataPvtw类型
	AddDataKeyWordType<CDataPvtw>();
	//注册CDataPvtg类型
	AddDataKeyWordType<CDataPvtg>();
	//注册CDataPvdg类型
	AddDataKeyWordType<CDataPvdg>();
	//注册CDataSwof类型
	AddDataKeyWordType<CDataSwof>();
	//注册CDataSgof类型
	AddDataKeyWordType<CDataSgof>();
	//注册CDataSlgof类型
	AddDataKeyWordType<CDataSlgof>();
	//注册CDataSof3类型
	AddDataKeyWordType<CDataSof3>();
	//注册CDataSof2类型
	AddDataKeyWordType<CDataSof2>();
	//注册CDataSwfn类型
	AddDataKeyWordType<CDataSwfn>();
	//注册CDataSgfn类型
	AddDataKeyWordType<CDataSgfn>();
	//注册CDataPvco类型
	AddDataKeyWordType<CDataPvco>();
	//注册CDataPvzg类型
	AddDataKeyWordType<CDataPvzg>();
	//注册CDataWelspecs类型
	AddDataKeyWordType<CDataWelspecs>();
	//注册CDataWelspecl类型
	AddDataKeyWordType<CDataWelspecl>();
	//注册CDataCompdat类型
	AddDataKeyWordType<CDataCompdat>();
	//注册CDataCompdatl类型
	AddDataKeyWordType<CDataCompdatl>();
	//注册CDataWconhist类型
	AddDataKeyWordType<CDataWconhist>();
	//注册CDataWconinj类型
	AddDataKeyWordType<CDataWconinj>();
	//注册CDataDates类型
	AddDataKeyWordType<CDataDates>();
	//注册CDataCoord类型
	AddDataKeyWordType<CDataCoord>();
	//注册CDataZcorn类型
	AddDataKeyWordType<CDataZcorn>();
	//注册CDataAqufetp类型
	AddDataKeyWordType<CDataAqufetp>();
	//注册CDataAqufetp类型
	AddDataKeyWordType<CDataAquct>();
	//注册CDataAqufetp类型
	AddDataKeyWordType<CDataAquancon>();
	//注册CDataEquil类型
	AddDataKeyWordType<CDataEquil>();
	//注册CDataPoro类型
	AddDataKeyWordType<CDataAngleGrid>(_T("PORO"));
	//注册CDataDznet类型
	AddDataKeyWordType<CDataAngleGrid>(_T("DZNET"));
	//注册CDataNtg类型
	AddDataKeyWordType<CDataAngleGrid>(_T("NTG"));
	//注册CDataPermx类型
	AddDataKeyWordType<CDataAngleGrid>(_T("PERMX"));
	//注册CDataPermy类型
	AddDataKeyWordType<CDataAngleGrid>(_T("PERMY"));
	//注册CDataPermz类型
	AddDataKeyWordType<CDataAngleGrid>(_T("PERMZ"));
	//注册CDataGravity类型
	AddDataKeyWordType<CDataGravity>();
	//注册CDataPvds类型
	AddDataKeyWordType<CDataPvds>();
	//注册CDataSgwfn类型
	AddDataKeyWordType<CDataSgwfn>();
	//注册CDataWconinjh类型
	AddDataKeyWordType<CDataWconinjh>();
	//注册CDataWconinje类型
	AddDataKeyWordType<CDataWconinje>();
	//注册CDataWelopen类型
	AddDataKeyWordType<CDataWelopen>();
	//注册CDataWecon类型
	AddDataKeyWordType<CDataWecon>();
	//注册CDataWefac类型
	AddDataKeyWordType<CDataWefac>();
	//注册CDataWelpi类型
	AddDataKeyWordType<CDataWelpi>();
	//注册CDataWpimult类型
	AddDataKeyWordType<CDataWpimult>();
	//注册CDataCompve类型
	AddDataKeyWordType<CDataCompve>();	
	//注册CDataDx类型
	AddDataKeyWordType<CDataBlockGrid>(_T("DX"));
	//注册CDataDy类型
	AddDataKeyWordType<CDataBlockGrid>(_T("DY"));
	//注册CDataDz类型
	AddDataKeyWordType<CDataBlockGrid>(_T("DZ"));
	//注册CDataDx类型
	AddDataKeyWordType<CDataBlockGrid>(_T("DXV"));
	//注册CDataDy类型
	AddDataKeyWordType<CDataBlockGrid>(_T("DYV"));
	//注册CDataDz类型
	AddDataKeyWordType<CDataBlockGrid>(_T("DZV"));
	//注册CDataTops类型
	AddDataKeyWordType<CDataBlockGrid>(_T("TOPS"));
	//注册CDataPvcdo类型
	AddDataKeyWordType<CDataPvcdo>();
	//注册CDataPorv类型
	AddDataKeyWordType<CDataAngleGrid>(_T("PORV"));
	//注册CDataDepth类型
	AddDataKeyWordType<CDataAngleGrid>(_T("DEPTH"));
	//注册CDataPressure类型
	AddDataKeyWordType<CDataAngleGrid>(_T("PRESSURE"));
	//注册CDataSoil类型
	AddDataKeyWordType<CDataAngleGrid>(_T("SOIL"));
	//注册CDataSwat类型
	AddDataKeyWordType<CDataAngleGrid>(_T("SWAT"));
	//注册CDataSgas类型
	AddDataKeyWordType<CDataAngleGrid>(_T("SGAS"));
	//注册CDataGruptree类型
	AddDataKeyWordType<CDataGruptree>();
	//注册CDataWconprod类型
	AddDataKeyWordType<CDataWconprod>();
	//注册CDataMultflt类型
	AddDataKeyWordType<CDataMultflt>();
	//注册CDataCarfin类型
	AddDataKeyWordType<CDataCarfin>();
	//注册CDataBox类型
	AddDataKeyWordType<CDataBox>(_T("BOX"));
	////注册CDataBox类型
	//AddDataKeyWordType<CDataBox>(_T("ENDBOX"));
	//注册CDataField类型
	AddDataKeyWordType<CDataField>();
	//注册CDataMeter类型
	AddDataKeyWordType<CDataMetric>();
	//注册CDataCompsegs类型
	AddDataKeyWordType<CDataCompsegs>();
	//注册CDataWsegdims类型
	AddDataKeyWordType<CDataWsegdims>();
	//注册CDataWelsegs类型
	AddDataKeyWordType<CDataWelsegs>();
	//
	AddDataKeyWordType<CDataMapaxes>();
	//
	AddDataKeyWordType<CDataRsconst>();
	//
	AddDataKeyWordType<CDataRsconstt>();
	//
	AddDataKeyWordType<CDataDensity>();
	//
	AddDataKeyWordType<DataEditNnc>();
	AddDataKeyWordType<CDataGridunit>();
	//直接拷贝的关键字
	AddDataKeyWordType<CDataSameCopy>(_T("ROCK"));
	AddDataKeyWordType<CDataSameCopy>(_T("RPTSCHED"));
	AddDataKeyWordType<CDataSameCopy>(_T("RPTRST"));
	AddDataKeyWordType<CDataSameCopy>(_T("NEXTSTEP"));
	AddDataKeyWordType<CDataSameCopy>(_T("WELTARG"));
	//AddDataKeyWordType<CDataSameCopy>(_T("WCONPROD"));
	AddDataKeyWordType<CDataSameCopy>(_T("RPTSOL"));
	AddDataKeyWordType<CDataSameCopy>(_T("PMAX"));
	AddDataKeyWordType<CDataSameCopy>(_T("AQUTAB"));
	AddDataKeyWordType<CDataSameCopy>(_T("TUNING"));
	AddDataKeyWordType<CDataSameCopy>(_T("AQUDIMS"));
	AddDataKeyWordType<CDataSameCopy>(_T("AQUALIST"));
	//AddDataKeyWordType<CDataSameCopy>(_T("GRIDUNIT"));
	AddDataKeyWordType<CDataSameCopy>(_T("MAPUNITS"));
	AddDataKeyWordType<CDataSameCopy>(_T("PBVD"));
	AddDataKeyWordType<CDataSameCopy>(_T("DATUM"));
	AddDataKeyWordType<CDataSameCopy>(_T("MSGFILE"));
	AddDataKeyWordType<CDataSameCopy>(_T("SMRYDIMS"));
	AddDataKeyWordType<CDataSameCopy>(_T("DISPDIMS"));
	AddDataKeyWordType<CDataSameCopy>(_T("SCDPDIMS"));
	AddDataKeyWordType<CDataSameCopy>(_T("VFPPDIMS"));
	AddDataKeyWordType<CDataSameCopy>(_T("MESSAGES"));
	AddDataKeyWordType<CDataSameCopy>(_T("NSTACK"));
	AddDataKeyWordType<CDataSameCopy>(_T("EQUIL"));
	AddDataKeyWordType<CDataSameCopy>(_T("PINCH"));
	AddDataKeyWordType<CDataSameCopy>(_T("RTEMP"));

	AddDataKeyWordType<CDataSPECGRID>(_T("SPECGRID")); //周纵横
	AddDataKeyWordType<CDataRSVD>(_T("RSVD"));         //周纵横
	AddDataKeyWordType<CDATAMINPORV>(_T("MINPORV"));   //周纵横
	AddDataKeyWordType<CDataWRFTPLT>(_T("WRFTPLT"));   //周纵横
	AddDataKeyWordType<CDataMINPV>(_T("MINPV"));       //周纵横

	//////////////////////////////////////////////////////////////////////////
	//注册CDataModifyKey(ADD)类型
	AddDataKeyWordType<CDataModifyKey>(_T("ADD"));//AddDataKeyWordType<CDataAdd>();
	//注册CDataModifyKey(MULTIPLY)类型
	AddDataKeyWordType<CDataModifyKey>(_T("MULTIPLY"));//AddDataKeyWordType<CDataMultiply>();
	//注册CDataModifyKey(EQUALS)类型
	AddDataKeyWordType<CDataModifyKey>(_T("EQUALS"));
	//注册CDataModifyKey(COPY)类型
	AddDataKeyWordType<CDataModifyKey>(_T("COPY"));
	//注册CDataModifyKey(COPYBOX)类型
	AddDataKeyWordType<CDataModifyKey>(_T("COPYBOX"));
	//注册CDataModifyKey(ADDREG)类型
	AddDataKeyWordType<CDataModifyKey>(_T("ADDREG"));
	//注册CDataModifyKey(MULTIREG)类型
	AddDataKeyWordType<CDataModifyKey>(_T("MULTIREG"));
	//注册CDataModifyKey(MINVALUE)类型
	AddDataKeyWordType<CDataModifyKey>(_T("MINVALUE"));
	//注册CDataModifyKey(MAXVALUE)类型
	AddDataKeyWordType<CDataModifyKey>(_T("MAXVALUE"));
	//注册CDataModifyKey(EQUALREG)类型
	AddDataKeyWordType<CDataModifyKey>(_T("EQUALREG"));
	//注册CDataModifyKey(COPREG)类型
	AddDataKeyWordType<CDataModifyKey>(_T("COPYREG"));
	////注册CDataModifyKey(RPTSOL)类型
	//AddDataKeyWordType<CDataModifyKey>(_T("RPTSOL"));
	//注册CDataModifyKey(PRTSCHED)类型
	AddDataKeyWordType<CDataModifyKey>(_T("PRTSCHED"));
	//注册CDataModifyKey(PRTRST)类型
	AddDataKeyWordType<CDataModifyKey>(_T("PRTRST"));
	//注册CDataModifyKey(RPTREGS)类型
	AddDataKeyWordType<CDataModifyKey>(_T("RPTREGS"));
	//注册CDataModifyKey(RPTPOPS)类型
	AddDataKeyWordType<CDataModifyKey>(_T("RPTPOPS"));
	//注册CDataModifyKey(RPTOPT)类型
	AddDataKeyWordType<CDataModifyKey>(_T("RPTOPT"));
	//注册CDataModifyKey(RPTHMW)类型
	AddDataKeyWordType<CDataModifyKey>(_T("RPTHMW"));
	//注册CDataModifyKey(RPTHMG)类型
	AddDataKeyWordType<CDataModifyKey>(_T("RPTHMG"));
	//注册CDataModifyKey(RPTGRID)类型
	AddDataKeyWordType<CDataSameCopy>(_T("RPTGRID")); //周纵横
	//////////////////////////////////////////////////////////////////////////
	//注册常用区域关键词
	AddDataKeyWordType<CDataCommonGrid>(_T("EQLNUM"));
	AddDataKeyWordType<CDataCommonGrid>(_T("SATNUM"));
	AddDataKeyWordType<CDataCommonGrid>(_T("PVTNUM"));
	AddDataKeyWordType<CDataCommonGrid>(_T("FIPNUM"));
	AddDataKeyWordType<CDataCommonGrid>(_T("EOSNUM"));
	AddDataKeyWordType<CDataCommonGrid>(_T("ACTNUM"));	
	//////////////////////////////////////////////////////////////////////////
}

//注册PoClass类型
void CDataObjFactory::RegisterPoClassType()
{
	//注册CDataEGrid类型
	AddPoClassType<CDataEGrid>();
	//注册CDataINIT类型
	AddPoClassType<CDataINIT>();
	//注册CDataRsspec类型
	AddPoClassType<CDataRsspec>();
	//注册CDataSnnnn类型
	AddPoClassType<CDataSnnnn>();
	//注册CDataXnnnn类型
	AddPoClassType<CDataXnnnn>();
	//注册CDataSmspec类型
	AddPoClassType<CDataSmspec>();
	//注册CDataSLNSPEC类型
	AddPoClassType<CDataSLNSPEC>();	
	//注册CDataSLNnnnn类型
	AddPoClassType<CDataSLNnnnn>();	
	//注册HiSim Pro Plot Grd类型
	AddPoClassType<CDataHiSimPro>();	
	AddPoClassType<CDataHiSimPlot>();	
	AddPoClassType<CDataHiSimGrd>();
}

//注册PoEntity类型
void CDataObjFactory::RegisterPoEntityType()
{
	//注册CDataPoDIMENS类型
	AddPoEntityType<CDataPoDIMENS>();
	//注册CDataPoStartDat类型
	AddPoEntityType<CDataPoStartDat>();
	//注册CDataPoACTNUM类型
	AddPoEntityType<CDataPoACTNUM>();
	//注册CDataPoGRIDHEAD类型	
	AddPoEntityType<CDataPoGRIDHEAD>();	
	//注册CDataPoITIME类型
	AddPoEntityType<CDataPoITIME>();
	//注册CDataPoCOORDSYS类型
	AddPoEntityType<CDataPoCOORDSYS>();
	//注册CDataPoCOORD类型
	AddPoEntityType<CDataPoCOORD>();
	//注册CDataPoZCORN类型
	AddPoEntityType<CDataPoZCORN>();	
	//////////////////////////////////////////////////////////////////////////
	//井轨迹相关
	//////////////////////////////////////////////////////////////////////////
	//注册CDataPoIWell类型
	AddPoEntityType<CDataPoIWell>();
	//注册CDataPoICon类型
	AddPoEntityType<CDataPoICon>();
	//////////////////////////////////////////////////////////////////////////
	//生产相关
	//////////////////////////////////////////////////////////////////////////
	//注册CDataPoParams类型
	AddPoEntityType<CDataPoParams>();
	//注册CDataPoKeywords类型
	AddPoEntityType<CDataPoStringParams>( _T("KEYWORDS"));
	//注册CDataPoWgnames类型
	AddPoEntityType<CDataPoStringParams>( _T("WGNAMES"));
	//注册CDataPoNums类型
	AddPoEntityType<CDataPoNums>();
	//单位
	AddPoEntityType<CDataPoStringParams>( _T("UNITS"));
	//后处理数据与井轨迹相关当前时间所有的井名
	AddPoEntityType<CDataPoStringParams>( _T("ZWEL"));
	//注册CDataPoSEQHDR类型
	AddPoEntityType<CDataPoSEQHDR>();
	
	//////////////////////////////////////////////////////////////////////////
	//网格属性相关
	//////////////////////////////////////////////////////////////////////////
	//注册不随时间变化的关键字
	AddPoEntityType<CDataPoFloatTableValue>( _T("DX"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("DY"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("DZ"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("TOPS"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("PORV"));	
	AddPoEntityType<CDataPoFloatTableValue>( _T("PERMX"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("PERMY"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("PERMZ"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("TRANX"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("TRANY"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("TRANZ"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("PORO"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("DZNET"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("NTG"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("DEPTH"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("SATNUM"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("PVTNUM"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("EQLNUM"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("FIPNUM"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("FLUXNUM"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("MULTNUM"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("ENDNUM"));


	AddPoEntityType<CDataPoFloatTableValue>( _T("DIFFX"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("DIFFY"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("DIFFZ"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("MULTX"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("MULTY"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("MULTZ"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("MULTX-"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("MULTY-"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("MULTZ-"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("MULTPV"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("MINPVV"));       //周纵横


	//注册随时间变化的关键字
	AddPoEntityType<CDataPoFloatTableValue>( _T("PRESSURE"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("FIPGAS"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("FIPWAT"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("FIPOIL"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("SWAT"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("SOIL"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("SGAS"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("OOIP"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("OWIP"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("OGIP"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("RS"));
	//注册随时间变化的关键字扩展
	AddPoEntityType<CDataPoFloatTableValue>( _T("SWCR"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("SGCR"));


	//不确定是否读得到的数据
	AddPoEntityType<CDataPoFloatTableValue>( _T("SWL"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("SWU"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("SGL"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("SGU"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("KRW"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("KRO"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("KRG"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("PCG"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("PCW"));

	AddPoEntityType<CDataPoFloatTableValue>( _T("OILKR"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("OIL_VISC"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("WATKR"));
	AddPoEntityType<CDataPoFloatTableValue>( _T("WAT_VISC"));

	AddPoEntityType<CDataPoFloatTableValue>( _T("Facies")); //加载沉积微相分布数据


	//注册加密网格
	//注册CDataPoLGRPARNT类型
	AddPoEntityType<CDataPoLGRPARNT>();
	//注册CDataPoLGR类型
	AddPoEntityType<CDataPoLGR>();
	//注册CDataPoLGRSGRID类型
	AddPoEntityType<CDataPoLGRSGRID>();
	//注册CDataPoLGRNAME类型
	AddPoEntityType<CDataPoLGRNAME>();
	//注册CDataPoGRIDUNIT类型
	AddPoEntityType<CDataPoGRIDUNIT>();
	//////////////////////////////////////////////////////////////////////////
	//流线相关
	//注册CDataPoINTEHEAD类型	
	AddPoEntityType<CDataPoINTEHEAD>();
	//注册CDataPoNAMES类型
	AddPoEntityType<CDataPoNAMES>();

	//流线方向数据
	AddPoEntityType<CDataStreamLineBase>(_T("ID_BEG"));
	AddPoEntityType<CDataStreamLineBase>(_T("ID_END"));
	//线坐标和分段数据
	AddPoEntityType<CDataStreamLineBase>(_T("GEOMETRY"));
	AddPoEntityType<CDataStreamLineBase>(_T("GEOMINDX"));

	//每条线一个值的属性
	AddPoEntityType<CDataStreamLineBase>(_T("PFRACRES"));
	AddPoEntityType<CDataStreamLineBase>(_T("PFRACOIL"));
	AddPoEntityType<CDataStreamLineBase>(_T("PFRACWAT"));
	AddPoEntityType<CDataStreamLineBase>(_T("PFRACGAS"));

	AddPoEntityType<CDataStreamLineBase>(_T("IFRACRES"));
	AddPoEntityType<CDataStreamLineBase>(_T("IFRACOIL"));
	AddPoEntityType<CDataStreamLineBase>(_T("IFRACWAT"));
	AddPoEntityType<CDataStreamLineBase>(_T("IFRACGAS"));

	//每条线段(两个点的连线)一个值的属性
	AddPoEntityType<CDataStreamLineBase>(_T("LINE_SWAT"));
	AddPoEntityType<CDataStreamLineBase>(_T("ID_CELL"));
	//每个点一个值的属性
	AddPoEntityType<CDataStreamLineBase>(_T("TIME_BEG"));	
}

void CDataObjFactory::RegisterSummaryKeyWord()
{
	CKeywordAdo ado;
	CADORecordset* dataset;

	try
	{
		dataset = ado.GetKeyword_SUMMARY();
	}
	catch(std::runtime_error e)
	{
		AfxMessageBox(e.what());
	}

	//无数据
	if (dataset == NULL)
	{
		return;
	}

	int nRowsCount = dataset->GetRecordCount();

	CString strValue;

	for (int nRow = 0; nRow < nRowsCount; nRow++)
	{
		//关键词名称
		dataset->GetFieldValue(0, strValue);

		if (strValue.Left(1) == "W")
		{
			AddDataKeyWordType<CDataSummaryW>(strValue);
		}
		else if (strValue.Left(1) == "C")
		{
			AddDataKeyWordType<CDataSummaryC>(strValue);
		}
		else
		{
			AddDataKeyWordType<CDataSameCopy>(strValue);
		}
		
		dataset->MoveNext();
	}
}