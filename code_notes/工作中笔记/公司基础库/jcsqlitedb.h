/******************************************************************************
* 文件名: jcsqlitedb.h
* 描述: -
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/03/18 20:45:29, gaopeng 创建文件
*
******************************************************************************/
#ifndef _JCSQLITEDB_H
#define _JCSQLITEDB_H

#include <string>
#include <vector>
#include <algorithm>
#include "jcbase.h"

/* 创建table时，查询table时，输入列vector */
typedef std::vector<std::string> StrVector;

typedef struct
{
	std::string    m_strKey ; 
	char          *m_szBuf ;
	unsigned int   m_nBufSize ; 
}TKeyBuff;
struct sqlite3_stmt;
struct sqlite3;
/* sqlite3_stmt包装类，用于绑定列和值，查询列类型 */
class DLL_EXPORT JCSqlStmt
{
public:
    JCSqlStmt();
    ~JCSqlStmt();

public:
    /******************************************************************************
    * 类型名:JCSqlStmt
    * 描述: -sql支持插入的数据类型

    * 修改记录:
    * --------------------
    *    2014/03/18 21:0:42, gaopeng 创建函数
    *
    ******************************************************************************/
    enum DATATYPE
    {
		/*
		#define SQLITE_INTEGER  1
		#define SQLITE_FLOAT    2
		#define SQLITE_BLOB     4
		#define SQLITE_NULL     5
		#ifdef SQLITE_TEXT
		# undef SQLITE_TEXT     3
		#else
		# define SQLITE_TEXT    3
		与以上定义对应
		*/
        INT = 1,
        FLT = 2,
        TXT = 3,
        BLB = 4,
		NUL = 5,
    };
#define  MAX_MEM_BUF_LEN   1000
    /******************************************************************************
    * 函数名:JCSqlStmt.InitStmt
    * 描述: -初始化JCSqlStmt,使用JCSqlStmt操作数据库前，必须先初始化
    *
    * 输入:statement: 要执行的sql语句，db:数据库指针，不能为空
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/18 21:7:33, gaopeng 创建函数
    *
    ******************************************************************************/
    void InitStmt(std::string const& statement, sqlite3* db);

    /******************************************************************************
    * 函数名:JCSqlStmt.GetColumnType
    * 描述: -返回指定列的数据类型
    *
    * 输入:columnIndex:列序号(***从1开始***)
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/18 21:11:35, gaopeng 创建函数
    *
    ******************************************************************************/
    DATATYPE GetColumnType(int columnIndex);

    /******************************************************************************
    * 函数名:JCSqlStmt.ValueInt
    * 描述: -返回指定列的整型数据,应先判断是否是为整型
    *
    * 输入:columnIndex:列序号(***从1开始***)
    * 输出:
    * 返回值:整型结果
    *
    * 修改记录:
    * --------------------
    *    2014/03/18 21:14:36, gaopeng 创建函数
    *
    ******************************************************************************/
    int         ValueInt   (int columnIndex);

    /******************************************************************************
    * 函数名:JCSqlStmt.ValueString
    * 描述: -返回指定列的string数据,应先判断是否是为string类型
    *
    * 输入:columnIndex:列序号(***从1开始***)
    * 输出:
    * 返回值:string结果
    *
    * 修改记录:
    * --------------------
    *    2014/03/18 21:16:36, gaopeng 创建函数
    *
    ******************************************************************************/
    std::string ValueString(int columnIndex);

	const void*     ValueBlob(int columnIndex,int &nSize);


    /******************************************************************************
    * 函数名:JCSqlStmt.Bind
    * 描述: -绑定插入到表里的值到相应在列(目前全绑定为string类型)
    *
    * 输入:columnIndex:列序号(***从1开始***), value:相应值
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/18 21:17:43, gaopeng 创建函数
    *
    ******************************************************************************/
    bool Bind(int columnIndex, std::string const& value);
    bool Bind(int columnIndex, double value);
    bool Bind(int columnIndex, int value);
	bool Bind(int columnIndex,char *buf ,int nBufSize) ; 
    bool BindNull(int columnIndex);
	bool ClearBinds();


    /******************************************************************************
    * 函数名:JCSqlStmt.Execute
    * 描述: -执行初始化JCSqlStmt时，传入的sql语句
    *
    * 输入:
    * 输出:
    * 返回值:成功，失败
    *
    * 修改记录:
    * --------------------
    *    2014/03/18 21:20:30, gaopeng 创建函数
    *
    ******************************************************************************/
    bool Execute();

    /******************************************************************************
    * 函数名:JCSqlStmt.NextRow
    * 描述: -select返回值，下一条数据，供一条条数据处理时调用，数据放在m_stmt
    *
    * 输入:
    * 输出:
    * 返回值:true:返回一条可读数据, false:已到达最后一条数据
    *
    * 修改记录:
    * --------------------
    *    2014/03/18 21:21:33, gaopeng 创建函数
    *
    ******************************************************************************/
    bool NextRow();


    /******************************************************************************
    * 函数名:JCSqlStmt.Reset
    * 描述: -   /*   Call Reset if not depending on the NextRow cleaning up.
        For example for select count(*) statements. (和NextRow关联使用)
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/18 21:31:21, gaopeng 创建函数
    *
    ******************************************************************************/
    bool Reset();

    /******************************************************************************
    * 函数名:JCSqlStmt.RestartSelect
    * 描述: -重置JCSqlStmt
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/18 21:31:48, gaopeng 创建函数
    *
    ******************************************************************************/
    bool RestartSelect();

private:
    sqlite3_stmt* m_ptStmt;
};

/* select操作返回结果集 */

class DLL_EXPORT RecordField
{
public:
	std::string strKey;
	std::string strValue;
};

class DLL_EXPORT ResultRecord
{
public:
    std::vector<RecordField> m_tFields;
};

class DLL_EXPORT ResultTable
{
    friend class JCSqliteWrapper;
public:
    ResultTable() : m_nCurRecord(0) {}

    std::vector<ResultRecord> m_tRecords;

    ResultRecord* next()
    {
        if (m_nCurRecord < m_tRecords.size())
        {
            return &(m_tRecords[m_nCurRecord++]);
        }
        return 0;
    }

	ResultRecord* at(int nIndex)
	{
		if (nIndex < 0 || (size_t)nIndex >= m_tRecords.size())
			return 0;
		m_nCurRecord = nIndex;

		return &(m_tRecords[m_nCurRecord++]);
	}

	void Append(ResultTable table)
	{
		m_tRecords.insert(m_tRecords.end(), 
							table.m_tRecords.begin(),
							table.m_tRecords.end());
	}

	unsigned int TableSize()
	{
		return (unsigned int)m_tRecords.size();
	}

	void Reverse()
	{
		std::reverse(m_tRecords.begin(), m_tRecords.end());
	}

    void reset()
    {
        m_tRecords.clear();
        m_nCurRecord=0;
    }

private:
    unsigned int m_nCurRecord;
};

class DLL_EXPORT JCSqliteWrapper
{
public:
    JCSqliteWrapper();
    ~JCSqliteWrapper();

public:
    /******************************************************************************
    * 函数名:JCSqliteWrapper.OpenDb
    * 描述: -打开已存在的db,如果db不存在，返回false
    *
    * 输入:db名称
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/19 9:18:47, gaopeng 创建函数
    *
    ******************************************************************************/
    bool OpenDb(std::string const& dbPath);

    /******************************************************************************
    * 函数名:JCSqliteWrapper.CreateDb
    * 描述: -创建db
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/19 9:20:7, gaopeng 创建函数
    *
    ******************************************************************************/
    bool CreateDb(std::string const& dbName, bool overWrite=false);

public:
    /******************************************************************************
    * 函数名:JCSqliteWrapper.CreateTable
    * 描述: -创建table,只在Opendb或CreateDb成功调用
    *
    * 输入:tableName:表名，columns:列名数组
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/19 9:20:40, gaopeng 创建函数
    *
    ******************************************************************************/
    bool CreateTable(std::string tableName, StrVector& columns);
	bool CreateTable(std::string tableName,std::string strKeyName, std::string strValueName);

    /******************************************************************************
    * 函数名:JCSqliteWrapper.InsertItems
    * 描述: -指定表中，插入内容
    *
    * 输入:tableName:表名，columns:列值数组(***保证值列数与数据表列数相同***)
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/19 9:22:33, gaopeng 创建函数
    *
    ******************************************************************************/
    bool InsertItems(std::string tableName, StrVector& columns);
	
    /******************************************************************************
    * 函数名:JCSqliteWrapper.InsertItems
    * 描述: -指定表中，插入内容
    *
    * 输入:tableName:表名，keyBuf: key与buf数据主要用来存储文件数据
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/19 9:22:33, gaopeng 创建函数
    *
    ******************************************************************************/
    bool InsertItems(std::string tableName, TKeyBuff& keyBuf);

    /******************************************************************************
    * 函数名:JCSqliteWrapper.DeleteItems
    * 描述: -指定表中，插入内容
    *
    * 输入:tableName:表名，删除items 根据 keyBuf 中的m_strKey
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/03/19 9:22:33, gaopeng 创建函数
    *
    ******************************************************************************/
	bool DeleteItems(std::string tableName,const std::string &strKeyName,const TKeyBuff& keyBuf);

	bool DeleteItemsByCondition(std::string tableName, std::string strCondition);

	/******************************************************************************
    * 函数名:JCSqliteWrapper.SelectFromTable
    * 描述: -查询指定表，指定列数据
    *
    * 输入:tableName:表名，columns:列名数组(***列为空，默认返回所有列***), res:返回结果集
    * 输出:
    * 返回值: true: 查询成功，false:查询失败
    *
    * 修改记录:
    * --------------------
    *    2014/06/19 10:24:38, gaopeng 创建函数
    *
    ******************************************************************************/
	bool SelectCountFromTable(std::string tableName, ResultTable& res, std::string condition, std::string& errMsg);

    /******************************************************************************
    * 函数名:JCSqliteWrapper.SelectFromTable
    * 描述: -查询指定表，指定列数据
    *
    * 输入:tableName:表名，columns:列名数组(***列为空，默认返回所有列***), res:返回结果集
    * 输出:
    * 返回值: true: 查询成功，false:查询失败
    *
    * 修改记录:
    * --------------------
    *    2014/03/19 9:24:38, gaopeng 创建函数
    *
    ******************************************************************************/
	bool SelectFromTable(std::string tableName, 
		StrVector columns, 
		ResultTable& res, 
		std::string condition, 
		std::string& errMsg);

	bool SelectFromTable(std::string tableName,
		std::string  keyName,
		std::string  valueName,
		std::string  keyValue ,
		const void * &pBuf,
		int  &nLen,
		std::string& errMsg);
	bool UpdateTable(std::string tableName,
		std::string  keyName,
		std::string  valueName,
		TKeyBuff& keyBuf,
		std::string& errMsg
		);
	void BuildUpCondition(std::string& sql, std::string condition);

	bool SelectStmt(std::string const& stmt, ResultTable& res, std::string& errMsg);

	/******************************************************************************
    * 函数名:JCSqliteWrapper.ResetDbPointer
    * 描述: -重置db指针，方便连续创建db文件。
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/05/16 18:24:38, gaopeng 创建函数
    *
    ******************************************************************************/
	bool ResetDbPointer();

    /******************************************************************************
    * 函数名:JCSqliteWrapper.CreateIndex
    * 描述: -创建指定列的索引
    *
    * 输入:tableName:表名，columnName:列名，indexName:索引名
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/12/06 15:20:40, gaopeng 创建函数
    *
    ******************************************************************************/
	bool CreateIndex(std::string tableName, std::string columnName, std::string indexName);

    bool ExecSql(std::string const& stmt);
    std::string LastError();

    //数据库事务操作
    bool Begin   ();
    bool Commit  ();
    bool Rollback();

	//数据库locked解决方案
	bool ResolveDatabaseLocked(std::string strFile);

private:
    static int SelectCallback(void *p_data, int num_fields, char **p_fields, char **p_col_names);

    sqlite3* m_ptDb;
    JCSqlStmt m_tJCSqlStmt;
	std::string m_strTableName;
	int	m_nValueSize;
	std::string m_strTempDbName;
};

#endif /* _JCSQLITEDB_H */

