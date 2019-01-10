/******************************************************************************
* 文件名: jcversionitem.h
* 描述: 版本号类型(参考软件版本管理规范上层软件适用)
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/04/08 23:37:12, lys 创建文件
*
******************************************************************************/
#ifndef _JCVERSIONITEM_H_
#define _JCVERSIONITEM_H_

#include <string>
#include "jcbase.h"

using namespace std;
/******************************************************************************
* 函数名:JCVersionItem
* 描述: 版本号类型
*
* 输入:
* 输出:
* 返回值:
*
* 修改记录:
* --------------------
*    2014/04/09 14:47:21, lys 创建函数
*
******************************************************************************/
class DLL_EXPORT JCVersionItem
{
public:
	JCVersionItem();

    /******************************************************************************
    * 函数名:JCVersionItem.JCVersionItem
    * 描述: -
    *
    * 输入:平台编号/特性集合编号/厂商或设备类型编号/build编号
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/09 14:47:46, lys 创建函数
    *
    ******************************************************************************/
	JCVersionItem(unsigned int nPlatForm, unsigned int nFeature, unsigned int nLabel, unsigned int nBuild);
	JCVersionItem(const JCVersionItem &tv);
	JCVersionItem &operator = (const JCVersionItem &tv);
	virtual ~JCVersionItem();

    /******************************************************************************
    * 函数名:JCVersionItem.Compare
    * 描述: 版本号比较函数,可继承重写,定制比较规则
    *
    * 输入:
    * 输出:
    * 返回值: 相等 0/ 小于 -1/ 大于 1
    *
    * 修改记录:
    * --------------------
    *    2014/04/09 14:49:16, lys 创建函数
    *
    ******************************************************************************/
	virtual unsigned int Compare(const JCVersionItem &tv);

    /******************************************************************************
    * 函数名:JCVersionItem.operator==
    * 描述: 等号操作符重载
    *
    * 输入:
    * 输出:
    * 返回值:各字段相等则为真
    *
    * 修改记录:
    * --------------------
    *    2014/04/09 14:51:0, lys 创建函数
    *
    ******************************************************************************/
	friend bool DLL_EXPORT operator == (const JCVersionItem &lhs, const JCVersionItem &rhs);
	friend bool DLL_EXPORT operator != (const JCVersionItem &lhs, const JCVersionItem &rhs);

    /******************************************************************************
    * 函数名:JCVersionItem.operator<
    * 描述: 小于操作符重载
    *
    * 输入:
    * 输出:
    * 返回值:按字段权重比较,m_nPlatForm权重最高,依次递减
    *
    * 修改记录:
    * --------------------
    *    2014/04/09 14:52:16, lys 创建函数
    *
    ******************************************************************************/
	friend bool DLL_EXPORT operator < (const JCVersionItem &lhs, const JCVersionItem &rhs);

    /******************************************************************************
    * 函数名:JCVersionItem.Version
    * 描述: 获取*.*.*.*格式版本号
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/04/09 14:54:24, lys 创建函数
    *
    ******************************************************************************/
	string Version();

	unsigned int GetPlatForm();
	unsigned int GetFeature();
	unsigned int GetLabel();
	unsigned int GetBuild();

protected:
	unsigned int m_nPlatForm;
	unsigned int m_nFeature;
	unsigned int m_nLable;
	unsigned int m_nBuild;
};

#endif /* _JCVERSIONITEM_H_ */

