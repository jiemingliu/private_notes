#pragma once
#include <map>
#include "DataObjStruct.h"

class CDataBlock;
class CDataKeyWord;
class CDataPoClass;
class CDataPoEntity;

//数据块类型容器
typedef std::map<CString,void*> MapDataBlock;
//关键词类型容器
typedef std::map<CString,void*> MapKeyWord;
//后处理类型容器
typedef std::map<CString,void*> MapPoClass;
//后处理数据实体容器
typedef std::map<CString,void*> MapPoEntity;


class AFX_EXT_CLASS CDataObjFactory
{
public:
	CDataObjFactory(void);
	~CDataObjFactory(void);

public:
	/**
	 * 方法:	注册对象类型
	 * 全名:	CDataObjFactory::RegisterType
	 * 类型:	public static 
	 * 
	 * @date	2008/03/21
	 * @author	xx
	 * @return	void
	 */
	static void RegisterType();

	/**
	 * 方法:	Reg注册Chemical场数据类型isterChemicalPropType
	 * 全名:	CDataObjFactory::RegisterChemicalPropType
	 * 类型:	public 
	 * 
	 * @date	2010/08/19
	 * @author	HUANGL
	 * @return	void
	 */
	static void RegisterChemicalPropType();

	/**
	 * 方法:	序列化二进制方式注册
	 * 全名:	CDataObjFactory::RegisterType_binary_iarchive
	 * 类型:	public static 
	 * 
	 * @date	2008/07/02
	 * @author	XX
	 * @param	boost::archive::binary_iarchive & Archive
	 * @return	void
	 */
	void RegisterType_archive(boost::archive::binary_iarchive& Archive);

	/**
	 * 方法:	序列化二进制方式注册
	 * 全名:	CDataObjFactory::RegisterType_binary_oarchive
	 * 类型:	public static 
	 * 
	 * @date	2008/07/02
	 * @author	XX
	 * @param	boost::archive::binary_oarchive & Archive
	 * @return	void
	 */
	void RegisterType_archive(boost::archive::binary_oarchive& Archive);

	///**
	// * 方法:	序列化文本方式注册
	// * 全名:	CDataObjFactory::RegisterType_text_iarchive
	// * 类型:	public static 
	// * 
	// * @date	2008/07/02
	// * @author	XX
	// * @param	boost::archive::text_iarchive & Archive
	// * @return	void
	// */
	//void RegisterType_archive(boost::archive::xml_iarchive& Archive);

	///**
	// * 方法:	序列化文本方式注册
	// * 全名:	CDataObjFactory::RegisterType_text_oarchive
	// * 类型:	public static 
	// * 
	// * @date	2008/07/02
	// * @author	XX
	// * @param	boost::archive::text_oarchive & Archive
	// * @return	void
	// */
	//void RegisterType_archive(boost::archive::xml_oarchive& Archive);

public:
	/**
	 * 方法:	创建DataBlock
	 * 全名:	CDataObjFactory::CreateColByType
	 * 类型:	public static 
	 * 
	 * @date	2008/03/21
	 * @author	xx
	 * @param	const CString & strBlockName
	 * @return	CDataBlock*
	 */
	static CDataBlock* CreateDataBlock(const CString& strBlockName);

	/**
	 * 方法:	创建DataKeyWord
	 * 全名:	CDataObjFactory::CreateDataKeyWord
	 * 类型:	public 
	 * 
	 * @date	2008/03/27
	 * @author	xx
	 * @param	const KeyInfo & keyInfo
	 * @return	CDataKeyWord*
	 */
	static CDataKeyWord* CreateDataKeyWord(const KeyInfo& keyInfo);

	/**
	 * 方法:	创建DataPoClass后处理类型
	 * 全名:	CDataObjFactory::CreateDataPoClass
	 * 类型:	public static 
	 * 
	 * @date	2008/04/01
	 * @author	xx
	 * @param	const CString & strClassName
	 * @return	CDataPoClass*
	 */
	static CDataPoClass* CreateDataPoClass(const CString& strClassName);

	/**
	 * 方法:	创建CDataPoEntity后处理数据实体
	 * 全名:	CDataObjFactory::CreateDataPoEntity
	 * 类型:	public static 
	 * 
	 * @date	2008/04/03
	 * @author	xx
	 * @param	const CString & strEntityName
	 * @return	CDataPoEntity*
	 */
	static CDataPoEntity* CreateDataPoEntity(const CString& strEntityName);

	/**
	 * 方法:	查找Chemical后处理场属性名
	 * 全名:	CDataObjFactory::FindChemicalProp
	 * 类型:	public static 
	 * 
	 * @date	2010/08/20
	 * @author	HUANGL
	 * @param	long nFlag
	 * @param	CString & strProp
	 * @return	bool
	 */
	static bool FindChemicalProp(long nFlag, CString& strProp);

private:
	//注册DataBlock类型
	static void RegisterDataBlockType();
	//注册KeyWord类型
	static void RegisterDataKeyWordType();
	//注册PoClass类型
	static void RegisterPoClassType();
	//注册PoEntity类型
	static void RegisterPoEntityType();
	/// <summary>
	/// 注册summary中的关键字
	/// </summary>
	static void RegisterSummaryKeyWord();
private:
	//插入数据块类型
	template<class dataBlkType>
	static void AddDataBlockType()
	{
		ASSERT(m_mapDataBlocks.find(dataBlkType::typeBlockName) == m_mapDataBlocks.end());
		m_mapDataBlocks[dataBlkType::typeBlockName] = &(dataBlkType::CreateDataBlock);
	}

	//插入关键词类型
	template<class dataKeyWord>
	static void AddDataKeyWordType()
	{
		ASSERT(m_mapKeyWords.find(dataKeyWord::typeKeyWord) == m_mapKeyWords.end());
		m_mapKeyWords[dataKeyWord::typeKeyWord] = &(dataKeyWord::CreateDataKeyWord);
	}

	//插入关键词类型-此方法用于批量插入相同类型的一组关键词创建方法
	template<class dataKeyWord>
	static void AddDataKeyWordType(CString strKeyWord)
	{
		ASSERT(m_mapKeyWords.find(strKeyWord) == m_mapKeyWords.end());
		m_mapKeyWords[strKeyWord] = &(dataKeyWord::CreateDataKeyWord);
	}

	//插入DataPoClass后处理类型
	template<class dataPoClassType>
	static void AddPoClassType()
	{
		ASSERT(m_mapPoClass.find(dataPoClassType::typeClassName) == m_mapPoClass.end());
		m_mapPoClass[dataPoClassType::typeClassName] = &(dataPoClassType::CreateDataPoClass);
	}

	//插入DataPoClass后处理类型
	template<class dataPoEntityType>
	static void AddPoEntityType()
	{
		ASSERT(m_mapPoEntity.find(dataPoEntityType::typeEntityName) == m_mapPoEntity.end());
		m_mapPoEntity[dataPoEntityType::typeEntityName] = &(dataPoEntityType::CreateDataPoEntity);
	}
	template<class dataPoEntityType>
	static void AddPoEntityType(CString strKeyword)
	{
		ASSERT(m_mapPoEntity.find(strKeyword) == m_mapPoEntity.end());
		m_mapPoEntity[strKeyword] = &(dataPoEntityType::CreateDataPoEntity);
	}

private:
	//保存创建CDataBlock接口函数指针
	static MapDataBlock m_mapDataBlocks;
	//保存创建CDataKeyWord接口函数指针
	static MapKeyWord m_mapKeyWords;
	//保存创建CDataPoClass接口函数指针
	static MapPoClass m_mapPoClass;
	//后处理数据实体容器
	static MapPoEntity m_mapPoEntity;
};
