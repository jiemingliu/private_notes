/********************************************************************
	created:	2016/06/29
	created:	29:6:2016   11:03
	file base:	data_convert
	file ext:	hpp
	author:		Huangl
	
	purpose:	数据转换
*********************************************************************/
#pragma once
#include <string>

namespace TGS {
namespace Convert {

/// <summary>
/// Author： Huangl
/// 转换数据类型
/// </summary>
/// <param name="srcDatas"></param>
/// <param name="tarDatas"></param>
/// <param name="count"></param>
/// <returns></returns>
template<typename _SrcType, typename _TarType>
inline void convertDatasToType(const _SrcType& srcDatas, _TarType& tarDatas, size_t count)
{
    for (size_t index = 0; index < count; ++index)
    {
        tarDatas[index] = srcDatas[index];
    }
}

/// <summary>
/// Author： Huangl
/// 逆序转换数据类型
///    适用于当srcDatas与tarDatas内存地址相同，但需要从字节较小的类型转换至字节
/// 更大的类型，例如short*转换至float*
/// </summary>
/// <param name="srcDatas"></param>
/// <param name="tarDatas"></param>
/// <param name="count"></param>
/// <returns></returns>
template<typename _SrcType, typename _TarType>
inline void reverseConvertType(const _SrcType& srcDatas, _TarType& tarDatas, size_t count)
{
    for (size_t index; count > 0; --count)
    {
        index = count - 1;
        tarDatas[index] = srcDatas[index];
    }
}

} // Convert
} // TGS