/******************************************************************************
* 文件名: jcconfigdatabase.h
* 描述: 配置类型基类, 可方便遍历成员及加载, 导入导出操作
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/10/17 17:7:19, lys 创建文件
*
******************************************************************************/

#ifndef _JCCONFIGDATABASE_H_
#define _JCCONFIGDATABASE_H_

#include <sstream>
#include <string>
#include "jcbase.h"

using namespace std;

class JCLogWrapper;
class JCXMLparser;

enum InfoType
{
	MEMBTYPE,
	MEMBNAME,
	MEMBVALUE
};

/**************************************************************
  用于类成员遍历的宏定义, 目前不支持复合类型成员使用此宏

  即为每个成员定义一个位置索引, 一个获取信息接口和一个设置接口
**************************************************************/
#define DECLARE_MEMBER_ORDER(MembName, MembIndex) \
	static const unsigned char const##MembName = MembIndex;

#define MEMBER_ORDER(MembName) const##MembName

#define END_DECLARE_MEMBER(MembCount)\
	static const unsigned char constMaxMembCount = MembCount

#define MEMBER_COUNT constMaxMembCount

 /*获取成员变量名字, 去掉成员变量的m_前缀*/
#define DECFUNC_MEMBER_NAME(MembName)\
	string Get##MembName##Name(void)\
	{\
		string strName = #MembName;\
		return strName.substr(2);\
	}

#define DECFUNC_MEMBER_VALUE(MembName)\
	string Get##MembName##Value(void)\
	{\
		stringstream& stream = CustomizedStream();\
		stream << MembName;\
		return stream.str();\
	}

#define DECFUNC_SET_MEMBER_VALUE(MembName)\
	void Set##MembName##Value(const string &strValue)\
	{\
		stringstream& stream = CustomizedStream();\
		stream << strValue;\
		stream >> MembName;\
	}

#define DECFUNC_MEMBER_TYPE(MembType, MembName)\
	string Get##MembName##Type()\
	{\
		return #MembType;\
	}

#define MEMBER_GETFUNC_DISPATCH(MembInfoType, MembName) \
	case MEMBER_ORDER(MembName): \
	{\
		if (MembInfoType == MEMBTYPE)  return Get##MembName##Type(); \
		if (MembInfoType == MEMBNAME)  return Get##MembName##Name(); \
		if (MembInfoType == MEMBVALUE) return Get##MembName##Value(); \
		break;\
	}

#define MEMBER_SETFUNC_DISPATCH(MembName, SetValue) \
	case MEMBER_ORDER(MembName): \
	{\
		return Set##MembName##Value(SetValue); \
	}

#define DECLARE_MEMBER(MembType, MembName, MembIndex)\
		DECFUNC_MEMBER_TYPE(MembType, MembName)\
		DECFUNC_MEMBER_NAME(MembName)\
		DECFUNC_MEMBER_VALUE(MembName)\
		DECFUNC_SET_MEMBER_VALUE(MembName)\
		DECLARE_MEMBER_ORDER(MembName, MembIndex)\
		MembType MembName


/******************************************************************************
* 函数名:JCParaBase
* 描述: 参数类型基类型, 为类型提供了类成员变量的值遍历, 值设置及打印接口
*       注意: 鉴于DECFUNC_MEMBER_NAME对成员名的处理规则, 类成员名都以m_为前缀
*       , 且成员类型应为C++支持数据格式化的内建类型
* 输入:
* 输出:
* 返回值:
*
* 修改记录:
* --------------------
*    2014/10/20 16:4:16, lys 创建函数
*
******************************************************************************/
class DLL_EXPORT JCParaBase
{
public:
	JCParaBase();
	virtual ~JCParaBase();

	static stringstream& CustomizedStream();

    /******************************************************************************
    * 函数名:JCParaBase.PrintPara
    * 描述: 打印所有成员信息(输出格式:  成员变量名: 成员变量值)
    *
    * 输入: tLog日志类型对象, nMemberCnt参数应为该类型的非复合结构成员数目, 建议使用 类名::MEMBER_COUNT
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/10/20 16:0:24, lys 创建函数
    *
    ******************************************************************************/
	virtual void PrintPara(JCLogWrapper &tLog, unsigned int nMemberCnt);

	/******************************************************************************
    * 函数名:JCParaBase.DumpParaToXml
    * 描述: 将类成员信息写入XML文件(输出格式:  <成员变量名>成员变量值</成员变量名>)
    *
    * 输入: tXml XML文件处理对象 strPath 类成员写入的XML根节点, 以aaa/bbb/ccc路径形式表示
	*        nMemberCnt 同上
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/10/20 16:0:24, lys 创建函数
    *
    ******************************************************************************/
	virtual void DumpParaToXml(JCXMLparser &tXml, const string &strPath, unsigned int nMemberCnt);

	/******************************************************************************
    * 函数名:JCParaBase.LoadParaFromXml
    * 描述: 从XML文件将值导入对应类成员
    *
    * 输入: tXml XML文件处理对象 strPath 类成员写入的XML根节点, 以aaa/bbb/ccc路径形式表示
	*        nMemberCnt 同上
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/10/20 16:0:24, lys 创建函数
    *
    ******************************************************************************/
	virtual void LoadParaFromXml(JCXMLparser &tXml, const string &strPath, unsigned int nMemberCnt);

    /******************************************************************************
    * 函数名:JCParaBase.MemberInfo
    * 描述: 获取类成员相关信息
    *
    * 输入: nMemberIndex 类成员的内部编号 eInfoType 获取信息类型(类成员类型名, 类成员名字, 类成员值)
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/10/20 16:16:49, lys 创建函数
    *
    ******************************************************************************/
	/************************************************************************/
	/* 
	以如下方式实现函数, 且需要根据实际类成员定义情况重载定制该接
	, 不需要获取信息的类成员可不列举
	switch(nMemberIndex)
	{
		MEMBER_GETFUNC_DISPATCH(eInfoType, 成员1);      
		MEMBER_GETFUNC_DISPATCH(eInfoType, 成员2);
		...
	}
	return string();
	*/
	/************************************************************************/
	virtual string MemberInfo(unsigned int nMemberIndex, InfoType eInfoType);

    /******************************************************************************
    * 函数名:JCParaBase.SetMemberValue
    * 描述: 设置类成员值
    *
    * 输入:nMemberIndex类成员序号, strValue 需要设置的值
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/10/20 16:17:10, lys 创建函数
    *
    ******************************************************************************/
	/************************************************************************/
	/* 	以如下方式实现函数, 且需要根据实际类成员定义情况重载定制该接
		, 不需要获取信息的类成员可不列举

	switch(nMemberIndex)
	{
		MEMBER_SETFUNC_DISPATCH(成员1, strValue);      
		MEMBER_SETFUNC_DISPATCH(成员2, strValue);
		...
	}
	*/
	/************************************************************************/
	virtual void SetMemberValue(unsigned int nMemberIndex, const string &strValue);

};

#endif /* _JCCONFIGDATABASE_H_ */


