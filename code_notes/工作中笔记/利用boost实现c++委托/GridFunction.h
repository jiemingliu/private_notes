#pragma once
#include <boost/function.hpp>
#include "GridStruct.h"

class CGridRecive;
//#include "GridPropUnit.h"
//#include "GridPropUnitPre.h"
//通过属性取得指定网格在执行时间下的值
typedef boost::function<double(const CGridRecive& ,const COleDateTime& ,const GridNo& )> fun_GetGridValue;
//通过属性和时间获取所有网格数据
typedef boost::function<std::vector<double>(const COleDateTime&,const std::vector<GridNo>&)> fun_GetGridValues;
//设置指定网格在执行时间下的值
typedef boost::function<void(CGridRecive& ,const COleDateTime& ,const GridNo& ,const double& )> fun_SetGridValue;
//通过属性取得所有时间下该属性的最大最小值
typedef boost::function<void(double& ,double& )> fun_GetMinMaxValue;
//获取当前时间当前层属性最大最小值
typedef boost::function<BOOL(CGridRecive&, const COleDateTime& ,long ,double& ,double& )> fun_GetKMinMaxValue;
//增加一个时间步进的数据
typedef boost::function<void(const CGridRecive& ,const COleDateTime& ,const CString& )> fun_createGridPropData;
//清空数据
typedef boost::function<void()> fun_ClearData;
//获取数据类型　TRUE 为后处理，FALSE为前处理
typedef boost::function<BOOL()> fun_DataType;
//属性此时间是否有数据
typedef boost::function<BOOL(const COleDateTime&)> fun_IsExistTime;
////拷贝前处理数据
//typedef boost::function<void(CGridPropUnitPre&)> fun_CopyGridPropUnitPre;
////拷贝后处理数据
//typedef boost::function<void(CGridPropUnit& , CGridRecive*)> fun_CopyGridPropUnit;
//关闭打开的文件句柄，可执行ECL
typedef boost::function<void()> fun_CloseFileHandle;

//定义属性获取方法
struct GridFunction
{
	//通过属性取得指定网格在执行时间下的值
	fun_GetGridValue getValue;
	//通过属性和时间获取所有网格数据
	fun_GetGridValues getValues;
	//设置指定网格在执行时间下的值
	fun_SetGridValue setValue;
	//通过属性取得所有时间下该属性的最大最小值
	fun_GetMinMaxValue getMinMax;
	//获取当前时间当前层属性最大最小值
	fun_GetKMinMaxValue getKMinMaxValue;
	//增加一个时间步进的数据
	fun_createGridPropData createGridPropData;
	//清空数据
	fun_ClearData clearData;
	fun_DataType getDataType;
	fun_IsExistTime isExistTime;
	//检查当前数据是否要计算
	fun_DataType checkCurPropData;
	//fun_CopyGridPropUnitPre copyGridPropUnitPre;
	//fun_CopyGridPropUnit     copyGridPropUnit;
	fun_CloseFileHandle        closeFileHandle;
};
