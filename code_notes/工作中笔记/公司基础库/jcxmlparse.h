/******************************************************************************
* 文件名: jcxmlparse.h
* 描述: -
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/03/13 18:40:33, gaopeng 创建文件
* v1.1	 2015/11/27 09:40:33, gaopeng 完善封装，屏蔽对tiXml的头文件引用	
******************************************************************************/
#ifndef _JCXMLPARSE_H
#define _JCXMLPARSE_H

#include <map>
#include <string>

#include <list>
#include "jcbase.h"
#include "jcnonecopy.h"

using namespace std;

typedef std::map<std::string, std::string> AttrMap;

class TiXmlElement ;
class TiXmlNode;
class TiXmlAttribute;
class TiXmlText;
class TiXmlDocument;
class TiXmlDeclaration;

/************************************************************************/
/*对应于XML中的申明部分	                                                */
/************************************************************************/
class DLL_EXPORT JCXmlDeclaration 
{
public:
	JCXmlDeclaration(string strVersion, string strEncoding, string strStandalone)
	{
		m_strVersion = strVersion;
		m_strEncoding = strEncoding;
		m_strStandalone = strStandalone;
	}

public:
	string m_strVersion;
	string m_strEncoding;
	string m_strStandalone;
};

/************************************************************************/
/*对应于XML的元素属性	                                                */
/************************************************************************/
class DLL_EXPORT JCXmlAttribute
{ 
public:
	JCXmlAttribute(string strAttribute, string strValue)
	{
		m_strAttribute = strAttribute;
		m_strValue = strValue;
	}

public:
	string m_strAttribute;
	string m_strValue;
};

/************************************************************************/
/*对应于XML的元素		                                                */
/************************************************************************/
class DLL_EXPORT JCXmlElement
{
public:
	JCXmlElement():m_strValue(""), m_ptElem(NULL)
	{
	}

	JCXmlElement(string strValue):m_strValue(strValue), m_ptElem(NULL)
	{
	}

	typedef std::map<std::string, std::string> AttrMap;

public:
	bool GetNextElement(JCXmlElement& tElem);

	bool GetAttributeValue(string strAttrName, string& strAttrValueRet);

	bool GetAttributes(AttrMap& tAttrMap);

	bool FindAttribute(string strAttrName);

	bool InsertAttribute(string strAttrName, string strAttrValue);

	bool DelAttribute(string strAttrName);

public:
	string m_strValue;
	TiXmlElement* m_ptElem;
};

/************************************************************************/
/* 对应于DOM结构中的节点                                                */
/************************************************************************/
class DLL_EXPORT JCXmlNode
{
public:
	JCXmlNode():m_ptNote(NULL)
	{}
	JCXmlNode(string strNodeName):m_strNodeName(strNodeName),m_ptNote(NULL)
	{}

public:
	bool InsertElement(JCXmlElement& tElement);
	bool GetFirstChildNode(JCXmlNode& tNode);
	bool GetNextNode(JCXmlNode& tNode);
	JCXmlElement ToElement();

public:
	string m_strNodeName;
	TiXmlNode* m_ptNote;
};

/************************************************************************/
/*XML文档处理			                                                */
/************************************************************************/
class DLL_EXPORT JCXMLparser : public JCNonecopy
{
public:
    JCXMLparser();
    ~JCXMLparser();

public:
    /******************************************************************************
    * 函数名:JCXMLparser.CreateXmlFile
    * 描述: -根据输入文件名创建xml文件。***需要调用SaveXmlFile才保存到磁盘***
    *
    * 输入:fullName:完整文件名，包含路径；overWrite:是否覆盖已存在文件
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/13 18:39:27, gaopeng 创建函数
    *
    ******************************************************************************/
    bool CreateXmlFile(std::string fullName, bool overWrite=false);

    /******************************************************************************
    * 函数名:JCXMLparser.LoadXmlFile
    * 描述: -根据文件名，加载xml文件
    *
    * 输入:fileName:xml完整文件名，包含路径
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/13 18:39:51, gaopeng 创建函数
    *
    ******************************************************************************/
    bool LoadXmlFile(std::string fileName);

    /******************************************************************************
    * 函数名:JCXMLparser.SaveXmlFile
    * 描述: -保存创建的xml文件，**当参数fileName为空时，以创建时传入的文件名保存。**
    *
    * 输入:fileName 文件名
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/13 18:40:50, gaopeng 创建函数
    *
    ******************************************************************************/
    bool SaveXmlFile(std::string fileName="");

	/******************************************************************************
    * 函数名:JCXMLparser.GetRootNode
    * 描述: -获取xml文档根节点
    *
    * 输入:tNode返回节点
    * 输出:
    * 返回值:成功：True， 失败：False
    *
    * 修改记录:
    * --------------------
    *    2015/11/27 10:40:59, gaopeng 创建函数
    *
    ******************************************************************************/
	bool GetRootNode(JCXmlNode& tNode);

	/******************************************************************************
    * 函数名:JCXMLparser.InsertDeclaration
    * 描述: -xml文档中插入申明
    *
    * 输入:tDel 申明
    * 输出:
    * 返回值:成功：True， 失败：False
    *
    * 修改记录:
    * --------------------
    *    2015/11/27 10:40:59, gaopeng 创建函数
    *
    ******************************************************************************/
	bool InsertDeclaration(JCXmlDeclaration& tDel);

	/******************************************************************************
    * 函数名:JCXMLparser.FindTextNodeByPath
    * 描述: -按路径查找指定text节点，路径格式：level1/level2/destNode
    *
    * 输入:path 路径
    * 输出:
    * 返回值:成功：返回文件内容， 失败：返回空字符串
    *
    * 修改记录:
    * --------------------
    *    2015/11/27 10:40:59, gaopeng 创建函数
    *
    ******************************************************************************/
    string FindTextNodeByPath(std::string path);

	/******************************************************************************
    * 函数名:JCXMLparser.InsertTextNodeByPath
    * 描述: -全路径插入text节点， 路径格式：level1/level2/destNode
    *		如果路径不存在，会依次创建节点路径
    * 输入:path: 路径, strValue: destNode值
    * 输出:
    * 返回值:成功：返回节点指针， 失败：返回NULL
    *
    * 修改记录:
    * --------------------
    *    2015/11/27 10:40:59, gaopeng 创建函数
    *
    ******************************************************************************/
	bool InsertTextNodeByPath(std::string path, std::string strValue);

	/******************************************************************************
    * 函数名:JCXMLparser.InsertCommentNodeByPath
    * 描述: -全路径插入text节点， 路径格式：level1/level2/destNode
    *		如果路径不存在，会依次创建节点路径
    * 输入:path: 路径, strValue: destNode值
    * 输出:
    * 返回值:成功：返回节点指针， 失败：返回NULL
    *
    * 修改记录:
    * --------------------
    *    2017/02/11 18:49:59, gaopeng 创建函数
    *
    ******************************************************************************/
	bool InsertCommentNodeByPath(std::string path, std::string strValue);

	/******************************************************************************
    * 函数名:JCXMLparser.FindXmlNodeByPath
    * 描述: -按路径查找element节点， 路径格式：level1/level2/elementName
    *
    * 输入:path: 路径， tNode返回结点
    * 输出:
    * 返回值:成功：True， 失败：False
    *
    * 修改记录:
    * --------------------
    *   2015/11/27 10:40:59, gaopeng 创建函数
    *
    ******************************************************************************/
	bool FindXmlNodeByPath(std::string path, JCXmlNode& tNode);

	/******************************************************************************
    * 函数名:JCXMLparser.InsertXmlNodeByPath
    * 描述: -全路径插入节点， 路径格式：level1/level2/elementName
    *		如果路径不存在，会依次创建节点路径
    * 输入:path: 路径
    * 输出:
    * 返回值:成功：True， 失败：False
    *
    * 修改记录:
    * --------------------
    *    2015/11/27 10:40:59, gaopeng 创建函数
    *
    ******************************************************************************/
	bool InsertXmlNodeByPath(std::string path);

	/******************************************************************************
    * 函数名:JCXMLparser.DeleteXmlNodeByPath
    * 描述: -删除指定节点， 路径格式：level1/level2/elementName
    *		
    * 输入:path: 路径
    * 输出:
    * 返回值:成功：True， 失败：False
    *
    * 修改记录:
    * --------------------
    *    2015/11/27 10:40:59, gaopeng 创建函数
    *
    ******************************************************************************/
	bool DeleteXmlNodeByPath(std::string path);

private:
	/******************************************************************************
    * 函数名:JCXMLparser.FindNodeByPath
    * 描述: -按路径查找element节点， 路径格式：level1/level2/elementName
    *
    * 输入:path: 路径
    * 输出:
    * 返回值:成功：返回节点指针， 失败：返回NULL
    *
    * 修改记录:
    * --------------------
    *    2014/03/13 18:42:39, gaopeng 创建函数
    *
    ******************************************************************************/
	TiXmlElement* FindNodeByPath(std::string path);

	/******************************************************************************
    * 函数名:JCXMLparser.InsertNodeByPath
    * 描述: -全路径插入节点， 路径格式：level1/level2/elementName
    *		如果路径不存在，会依次创建节点路径
    * 输入:path: 路径
    * 输出:
    * 返回值:成功：返回节点指针， 失败：返回NULL
    *
    * 修改记录:
    * --------------------
    *    2014/03/13 18:42:39, gaopeng 创建函数
    *
    ******************************************************************************/
	TiXmlElement* InsertNodeByPath(std::string path);

	//重置成员指针
    void ResetFilePorinter();

private:
    TiXmlDocument* m_ptDocument;
    std::string m_strFileName;
};

#endif /* _JCXMLPARSE_H */

