/********************************************************************
	创建日期:	2008/05/07
	文件路径:	e:\My Task\simtools\CommonData\Tools
	文 件 名:	DataRead_inline
	文件后缀:	h
	作    者:	iuhiao	
	说明:	  读取文件时使用的内联函数
*********************************************************************/
#pragma once
#include "EclHeadStruct.h"
#include "DataPoStruct.h"
#include "..\Data\GridRecive.h"
#include "ProjectManger.h"
typedef std::vector<float> Grids;
#include "DataRoot.h"
#include "ProgressControl/ProgressControl.h"
#include "..\..\CommonTools\Tools\Path.h"

/**
* 方法:	设置读取数据时所应读取的实际数据量
* 全名:	Setndatas
* 类型:	inline 
* 
* @日期 	2008/05/07
* @作者  iuhiao
* @参数 	int ndatas
* @返回类型 	void
*/
inline void Setndatas(int& ndatas)
{
	ndatas = ndatas + (ndatas/1000) * 2;
}
/************************************************************************/
/*      字符串设置　　　                               */
/************************************************************************/
inline void rmSpace(char *ss, int len)
{
	int  i, j1, j2, k;
	char str[256];
	if(len >= 8 && lstrcmpi(ss, ":+:+:+:+") == 0){
		lstrcpy(ss, "");
		return;
	}
	j1 = 0;
	j2 = -1;
	for(i = 0; i < len; i++)
		if(ss[i] != ' '){ j1 = i; break; }
	for(i = ( len - 1 ); i >= 0; i--)
		if(ss[i] != ' ' && ss[i] != '\n' && ss[i] != '\r'){ j2 = i; break; }
	k = 0;
	if(j2 >= 0)
		for(i = j1; i <= j2; i++){ str[k] = ss[i]; k++; }
	str[k] = '\0';
	lstrcpy(ss, str);
}
/*************************************************************
*	读整数+4
************************************************************/
inline int intread(int* nk,char* lpbMapAddress)
{
	unsigned char cc[5], ss[5];
	for (int ic = 0;ic < 4;++ic)///读4个字节数据，赋给一个整型变量
	{
		cc[ic] = *(lpbMapAddress + ic);
	}	
	ss[0] = cc[3]; ss[1] = cc[2]; ss[2] = cc[1]; ss[3] = cc[0];
	ss[4] = '\0';
	*nk = *((int*)ss);
	if(*nk < 0) return(0);
	return(1);
}
/******************************************************
*	读实数+4
*****************************************************/
inline void realread(float *fval,char* lpbMapAddress)
{
	unsigned char cc[5], ss[5];
	for (int ic = 0;ic < 4;++ic)///读4个字节数据，赋给一个整型变量
	{
		cc[ic] = *(lpbMapAddress + ic);
	}
	ss[0] = cc[3]; ss[1] = cc[2]; ss[2] = cc[1]; ss[3] = cc[0];
	ss[4] = '\0';
	*fval = *((float*)ss);
	return;
}

/*************************************************
*	读双精度数+8
************************************************/
inline void doubread(double *fval,char* lpbMapAddress)
{
	int    i;
	unsigned char cc[9], ss[9];
	for (int ic = 0;ic < 8;++ic)///读8个字节数据，赋给一个整型变量
	{
		cc[ic] = *(lpbMapAddress + ic);
	}
	for(i = 0; i < 8; i++) 
	{
		ss[i] = cc[7-i];
	}
	ss[8] = '\0';
	*fval = *((double*)ss);
	return;
}
/*****************************************************
*	读取　EclHead　结构体+24
*****************************************************/
inline  int read_eh(char* lpbMapAddress,EclHead* eh,int* nOffset)
{
	int  iesc, n;
	char ch[9], ct[5];
	//先从文件读一个整数
	if(intread(&iesc,lpbMapAddress) <= 0) 
	{
		lpbMapAddress = lpbMapAddress + 4;	
		*nOffset = 4;
		return(0); 
	}
	*nOffset = 4;
	lpbMapAddress = lpbMapAddress + 4;	
	if(iesc != 16) return(0);
	///读8个字节的字符串
	for (int ic = 0;ic < 8;++ic)
	{
		ch[ic] = *(lpbMapAddress + ic);
	}
	lpbMapAddress = lpbMapAddress + 8;	
	*nOffset = 8;
	ch[8] = '\0';///加上串的结束符
	rmSpace(ch, 8);
	if(intread(&n, lpbMapAddress) <= 0) 
	{
		*nOffset = 16;
		lpbMapAddress = lpbMapAddress + 4;	
		return(0);///读一个整数
	}
	lpbMapAddress = lpbMapAddress + 4;
	////读4个字节的字符串
	for (int ic = 0;ic < 4;++ic)
	{
		ct[ic] = *(lpbMapAddress + ic);
	}
	lpbMapAddress = lpbMapAddress + 4;
	///加上串的结束符
	ct[4] = '\0';
	rmSpace(ct, 4);
	*nOffset = 20;
	if(lstrcmpi(ch, "") == 0) return(0); 
	//对EclHead　结构体赋值
	lstrcpy(eh->ch, ch);
	//对EclHead　结构体赋值
	eh->n = n;         
	//对EclHead　结构体赋值
	lstrcpy(eh->ct, ct);  

	if(intread(&iesc, lpbMapAddress) <= 0) 
	{
		*nOffset = 24;
		lpbMapAddress = lpbMapAddress + 4;
		///读一个整数
		return(0);
	}
	*nOffset = 24;
	lpbMapAddress = lpbMapAddress + 4;	
	return(1);	
}
/****************************************************************
*	读记录
****************************************************************/
inline void read_ecl_record(char* lpbMapAddress,char ct[],double ebuf[],char crt[])
{
	int    idi, ilog;
	float  ral;
	double dou;
	char   cr[10];
	/*根据传入的ct值不同，对ebuf赋不同值 */
	if(lstrcmpi(ct, "INTE") == 0)
	{		
		(void)intread(&idi, lpbMapAddress);
		lpbMapAddress = lpbMapAddress + 4;	
		ebuf[0]=(double)idi;
	}
	else if(lstrcmpi(ct, "REAL") == 0)
	{
		realread(&ral, lpbMapAddress);
		lpbMapAddress = lpbMapAddress + 4;	
		ebuf[1]=(double)ral;
	}
	else if(lstrcmpi(ct, "LOGI") == 0)
	{
		(void)intread(&ilog, lpbMapAddress);
		lpbMapAddress = lpbMapAddress + 4;	
		ebuf[2]=(double)ilog;
	}
	else if(lstrcmpi(ct, "DOUB") == 0)
	{
		doubread(&dou, lpbMapAddress);
		lpbMapAddress = lpbMapAddress + 8;	
		ebuf[3]=(double)dou;
	}
	else if(lstrcmpi(ct, "CHAR") == 0)
	{
		for (int ic = 0;ic < 8;++ic)///读8个字节数据，赋给一个整型变量
		{
			cr[ic] = *(lpbMapAddress + ic);
		}
		lpbMapAddress = lpbMapAddress + 8;	
		cr[8] = '\0';
		rmSpace(cr, 8);
		lstrcpy(crt, cr);		
	}
}

/**
 * 方法:	读文件（关键字信息）
 * 全名:	<PoClass>::ReadFile
 * 类型:	public 
 * 
 * @日期 	2008/09/16
 * @作者  iuhiao
 * @参数 	CString strFile  文件名
 * @参数 	PoClass * pPoClass　文件类对象
 * @参数 	PoGlobalData & poGloablData　全局数据
 * @参数 	CGridRecive & gridRecive　网格数据对象
 * @参数 	int nFileType　　文件类型
 * @返回类型 	void
 */
template<class PoClass>
inline void ReadFile(CString strFile,PoClass* pPoClass ,PoGlobalData& poGloablData  ,
					 CDataRoot* pDataRoot , int nFileType = 0)
{
	//进度条信息
	CString strProgressInfo;
	CPath path(strFile);
	strProgressInfo.Format("正在读取文件：%s",path.GetFileName());
	//try
	{
		//nFileType = 0为后处理文件，=1为重启文件
		int	  nk;//关键字数据量
		int  ifid;//关键字所占空间字节数
		EclHead  eh;
		///关键字名
		CString strTitle;
		//关键字类型
		CString strtype;
		//关键字数据
		CString strdata;
		// 创建文件对象
		HANDLE hFile = ::CreateFile(strFile, GENERIC_READ,FILE_SHARE_READ, NULL, 
			OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			TRACE("创建文件对象失败,错误代码:%d\r\n", GetLastError());
			return;
		}
		// 创建文件映射对象
		HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		if (hFileMap == NULL)
		{
			//关闭文件对象
			CloseHandle(hFile);	
			TRACE("创建文件映射对象失败,错误代码:%d\r\n", GetLastError());		
			return;
		}

		// 得到文件尺寸
		DWORD dwFileSizeHigh;
		__int64 qwFileSize = GetFileSize(hFile, &dwFileSizeHigh);
		qwFileSize |= (((__int64)dwFileSizeHigh) << 32);///MSDN
		// 偏移地址 
		__int64 qwFileOffset = 0;

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		// 得到系统分配粒度
		SYSTEM_INFO SysInfo;
		GetSystemInfo(&SysInfo);
		DWORD dwGran = SysInfo.dwAllocationGranularity;
		__int64 T_newmap = 1200 * dwGran;
		// 块大小
		DWORD dwBlockBytes = 1500 * dwGran;//文件数据分段大小
		if (qwFileSize - qwFileOffset < dwBlockBytes)
		{
			dwBlockBytes = (DWORD)qwFileSize;
		}

		// 映射视图
		char *lpbMapAddress = (char *)MapViewOfFile(hFileMap,FILE_MAP_READ,
			(DWORD)(qwFileOffset >> 32), (DWORD)(qwFileOffset & 0xFFFFFFFF),dwBlockBytes);
		if (lpbMapAddress == NULL)
		{
			//关闭文件对象
			CloseHandle(hFile);	
			// 关闭文件映射对象句柄
			CloseHandle(hFileMap);
			TRACE("映射文件映射失败,错误代码:%d ", GetLastError());
			return;
		} 		
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		///////////读文件数据
		//对于读EchHead时的不同情况，设置偏移
		int eh_offset = 0;
		//当前网格体所对应的gridRecive
		CGridRecive* gridRecive = &(pDataRoot->GetGridData().GetGridRecive());
		//当前读取的块是否加密网格的数据
		BOOL bLgrData = FALSE;
		//LGR标识
		CString strNLGR;
		CString strLgrName;
		//是否文件的最后一次映射
		BOOL bLastBlock;
		try
		{
			while(qwFileOffset < qwFileSize)
			{
				/********************            读文件             ***************************/
				//对于读EchHead时的不同情况，设置偏移
				eh_offset = 0;
				eh.n = 0;
				eh.ch[0] = '\0';
				ASSERT(qwFileSize - qwFileOffset > 0);
				//读取关键词头信息
				read_eh(lpbMapAddress, &eh ,&eh_offset);
				qwFileOffset += eh_offset;		
				lpbMapAddress = lpbMapAddress + eh_offset;
				//根据读取出的ct值不同，对*ifid赋不同值 判断每个数据的所占字节
				if(lstrcmpi(eh.ct, "DOUB") == 0 || lstrcmpi(eh.ct, "CHAR") == 0)
				{
					ifid = 8;
				}
				else 
				{
					ifid = 4;
				}
				//////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////
				if (qwFileOffset + eh.n * ifid + 4 > T_newmap)
				{
					//当数据读到80%时,为防数据溢出,需要映射在其后的数据  T_newmap
					//释放当前映射
					UnmapViewOfFile(lpbMapAddress);
					bLastBlock = FALSE;					
					int nGran = qwFileOffset / dwGran;
					__int64 fileOff = nGran * dwGran;
					if ((DWORD)(qwFileSize - fileOff) < dwBlockBytes)
					{ 
						dwBlockBytes = (DWORD)(qwFileSize - fileOff);
						bLastBlock = TRUE;
					}
					lpbMapAddress = (char *)MapViewOfFile(hFileMap,FILE_MAP_READ,
						(DWORD)(fileOff >> 32), (DWORD)(fileOff & 0xFFFFFFFF),dwBlockBytes);
					if (lpbMapAddress == NULL)
					{
						//关闭文件对象
						CloseHandle(hFile);	
						// 关闭文件映射对象句柄
						CloseHandle(hFileMap);
						TRACE("映射文件映射失败,错误代码:%d ", GetLastError());
						return;
					} 
					// 修正参数
					lpbMapAddress = lpbMapAddress + ( qwFileOffset - nGran * dwGran );
					if (bLastBlock)
					{
						T_newmap = qwFileSize;
					}
					else
					{
						T_newmap = fileOff  + 1200 * dwGran;
					}
				}
				//////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////
				/*
				*	读一个关键字,关键字和它的数据作为一个整体写入一个相应数据结构,加入到m_dataObjs
				*/
				strTitle = eh.ch;
				strtype = eh.ct;
				if (_T("ENDLGR") == strTitle)
				{
					gridRecive = &(pDataRoot->GetGridData().GetGridRecive());
					bLgrData = FALSE;
				}
				///创建后处理数据实体..........................................关键字名
				if (strTitle == "SWAT" && (strFile.Find(".SLN") != -1))
				{
					//流线数据中的SWAT关键字
					strTitle = "LINE_SWAT";
				}
				if(eh.n <= 0) 
				{
					continue;
				}
				//nk为读出的该字段总字节数
				if(intread(&nk, lpbMapAddress) <= 0) 
				{
					qwFileOffset += 4;
					lpbMapAddress = lpbMapAddress + 4;	
					continue;
				}
				qwFileOffset += 4;
				lpbMapAddress = lpbMapAddress + 4;
				//跳过文件中不应该出现的关键字
				if (strTitle == _T("TRANX") || strTitle == _T("TRANY") || strTitle == _T("TRANZ"))
				{
					if (strFile.Find(_T(".UNRST")) != -1 || strFile.Find(_T(".X")) != -1)
					{
						qwFileOffset = qwFileOffset + eh.n * ifid + 4;
						lpbMapAddress = lpbMapAddress + eh.n * ifid + 4;
						continue;
					}
				}
				if (bLgrData)
				{
					//添加LGR标识
					strTitle = strNLGR + strTitle;
				}
				//获取关键字数据容器
				CDataPoEntity* pPoEntity = pPoClass->FindPoEntity(strTitle);
				if (NULL == pPoEntity)
				{
					if (bLgrData)
					{
						//实际关键字名
						strTitle = strTitle.Mid(strNLGR.GetLength() , strTitle.GetLength() - 3);
					}
					//创建当前关键词对象(没有注册的关键词就返回NULL)
					pPoEntity = CDataObjFactory::CreateDataPoEntity(strTitle);
					if (pPoEntity != NULL)
					{
						if (bLgrData)
						{
							if (gridRecive->m_SimTimes.size() < poGloablData.times.size())
							{
								gridRecive->m_SimTimes = poGloablData.times;
							}
							pPoEntity->SetTitle(strNLGR + strTitle);
						}
						///加入到vector
						pPoClass->GetDataObjs ().push_back(pPoEntity);
					}			
				}
				//不要的关键字数据
				if (pPoEntity == NULL )
				{
					qwFileOffset = qwFileOffset + eh.n * ifid + 4;
					lpbMapAddress = lpbMapAddress + eh.n * ifid + 4;
					continue;
				}			
				//读取数据
				//////////////////////////////////////////////////////////////////////////
				//设置进度条
				int curPer = int((qwFileOffset - 4 - eh_offset) * 100/qwFileSize);
				CProgressControl::getSingletonPtr()->StepProgress(curPer ,strProgressInfo,
					_T("正在读取属性：")  + strTitle);
				//////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////
				if (strTitle == _T("LGR"))
				{
					std::vector<char> LgrData;
					LgrData.resize(eh.n * sizeof(char) * 8);	
					memcpy((char*)&LgrData[0],(char*)lpbMapAddress,eh.n * sizeof(char) * 8);
					char keywords[10];
					for (int ic = 0;ic < 8;++ic)
					{
						//读取KEYWORDS数据　数据属性名(生产类型)
						keywords[ic] = LgrData[ic];
					}
					keywords[8] = '\0';
					//将":+:+:+:+"串置为空
					rmSpace(keywords, 8);
					strLgrName = keywords;
					gridRecive = &(pDataRoot->GetLgrGridData(strLgrName).GetGridRecive());
					bLgrData = TRUE;
					strNLGR = _T("LGR") + strLgrName;
				}
				//没有使用CGridPropUnit的属性数据设置值(如：流线)
				//int nData = eh.n;
				//Setndatas(nData);
				////08.10.28修改，以前可能读的数据少了，没有读应该跳过的数据
				//pPoEntity->SetDatas(lpbMapAddress, nData , poGloablData  , gridRecive );
				pPoEntity->SetDatas(lpbMapAddress,eh.n , poGloablData  , *gridRecive );
				pPoEntity->SetDatas(lpbMapAddress,eh.n , poGloablData  , *gridRecive , strNLGR.GetLength());
				//使用CGridPropUnit的属性数据只设置数据信息
				if (bLgrData)
				{
					pPoEntity->SetDatas(strFile , qwFileOffset - 4 - eh_offset , gridRecive ,strNLGR.GetLength());
					pPoEntity->SetDatas(pDataRoot->GetLgrData() , strLgrName);
				}
				else
				{
					pPoEntity->SetDatas(strFile , qwFileOffset - 4 - eh_offset , gridRecive );
				}
				//////////////////////////////////////////////////////////////////////////
				//如果读到重启方案 则直接退出,在外层经行网格拷贝等相关操作
				if (strTitle == "DIMENS" && nFileType == 1)
				{
					//读重启文件时，先只读DIMENS
					//释放最后数据块映射
					UnmapViewOfFile(lpbMapAddress);
					//关闭文件对象
					CloseHandle(hFile);	
					// 关闭文件映射对象句柄
					CloseHandle(hFileMap);
					return;
				}
				//////////////////////////////////////////////////////////////////////////
				//读流线数据
				pPoEntity->SetDataType(eh.ct);
				pPoEntity->SetDatas(strFile , qwFileOffset - 4 - eh_offset , &pDataRoot->GetFlowLineData());
				//修正指针
				qwFileOffset = qwFileOffset + eh.n * ifid + 4;
				lpbMapAddress = lpbMapAddress + eh.n * ifid + 4;
				//设置进度条
				curPer = int(qwFileOffset * 100/qwFileSize);
				CProgressControl::getSingletonPtr()->StepProgress(curPer ,strProgressInfo,
					_T("正在读取属性：")  + strTitle);
			} 
			//释放最后数据块映射
			UnmapViewOfFile(lpbMapAddress);
			//关闭文件对象
			CloseHandle(hFile);	
			// 关闭文件映射对象句柄
			CloseHandle(hFileMap);	
			CProgressControl::getSingletonPtr()->StepProgress(100 ,strProgressInfo,
				_T(" "));
			return;
		}
		catch (...)
		{
			TRACE("错误代码:%d ", GetLastError());
			//释放最后数据块映射
			UnmapViewOfFile(lpbMapAddress);
			// 关闭文件映射对象句柄
			//关闭文件对象
			CloseHandle(hFile);	
			CloseHandle(hFileMap);	
			CProgressControl::getSingletonPtr()->StepProgress(100 ,strProgressInfo,
				_T(" "));
			//if (strTitle == _T("ICON"))
			//{
			//	AfxMessageBox(_T("读取井轨迹数据错误！"));
			//	return;
			//}
			AfxMessageBox(_T("读取后处理网格数据错误！"));
			return;
		}
	}
}

const int 		MAX_SEG_DATA_NUM = 1000;
const int 		MAX_SEG_KEY_NUM = 105;
const int		INT_KEY_HEAD_NUM = 16;
const int		SPACE_HEX = 0x20;

inline int GetTypeSize(CString sType) {

	int nSize = 4;

	if (_T("DOUB") == sType ||
		_T("CHAR") == sType) {
		nSize = 8;
	}

	return nSize;
}

inline void TransNumberByte(char* in, char* out, int nSize)
{
	for (int i = 0; i < nSize; ++i) {
		*(out + i) = *(in + nSize - 1 - i);
	}
}

inline void intwrite(int value, CFile* pFile)
{
	char bytes[4];
	TransNumberByte((char*)(&value), bytes, 4);
	pFile->Write(bytes, 4);
}

inline void realwrite(float value, CFile* pFile)
{
	char bytes[4];
	TransNumberByte((char*)(&value), bytes, 4);
	pFile->Write(bytes, 4);
}

inline void doublewrite(double value, CFile* pFile)
{
	char bytes[8];
	TransNumberByte((char*)(&value), bytes, 8);
	pFile->Write(bytes, 8);
}

inline void stringwrite(char* chars, int nLength, CFile* pFile)
{
	pFile->Write(chars, nLength);
}

inline void stringwrite(CString str, CFile* pFile)
{
	int nLength = str.GetLength();
	char* chars = str.GetBuffer();

	stringwrite(chars, nLength, pFile);

	str.ReleaseBuffer();
}

inline void charStringwrite(CString str, CFile* pFile)
{
	int nLength = str.GetLength();
	ASSERT(nLength <= 8);

	if (nLength == 8) {
		stringwrite(str, pFile);
	}
	else {
		char chars[8];
		memset(chars, SPACE_HEX, 8);
		char* oriChars= str.GetBuffer();
		memcpy(chars, oriChars, nLength);
		stringwrite(chars, 8, pFile);
		str.ReleaseBuffer();
	}

}

inline void keyStringwrite(CString str, CFile* pFile)
{
	charStringwrite(str, pFile);
}

inline void writeEVerNum(CFile* pFile)
{
	intwrite(INT_KEY_HEAD_NUM, pFile);
}

inline void writeDataType(CString sType, CFile* pFile)
{
	char* chars = sType.GetBuffer();
	pFile->Write(chars, sType.GetLength());
	sType.ReleaseBuffer();
}

inline void writeIntegerArray(std::vector<int> datas, CFile* pFile)
{
	int nTypeSize = 4;
	int nTotalSize = (int)datas.size();
	{
		int nSize;
		int nCount = 0;
		for (int i = 0; i < nTotalSize; ++i) {
			if (nCount == 0) {
				int nLeftSize = nTotalSize - i;
				if (nLeftSize > MAX_SEG_DATA_NUM)
					nLeftSize = MAX_SEG_DATA_NUM;

				nSize = nLeftSize * nTypeSize;
				intwrite(nSize, pFile);
			}

			intwrite(datas[i], pFile);

			if (++nCount == MAX_SEG_DATA_NUM || i == nTotalSize-1) {
				intwrite(nSize, pFile);
				nCount = 0;
			}
		}
	}
}

inline void writeRealArray(std::vector<float> datas, CFile* pFile)
{
	int nTypeSize = 4;
	int nTotalSize = (int)datas.size();
	{
		int nSize;
		int nCount = 0;
		for (int i = 0; i < nTotalSize; ++i) {
			if (nCount == 0) {
				int nLeftSize = nTotalSize - i;
				if (nLeftSize > MAX_SEG_DATA_NUM)
					nLeftSize = MAX_SEG_DATA_NUM;

				nSize = nLeftSize * nTypeSize;
				intwrite(nSize, pFile);
			}

			realwrite(datas[i], pFile);

			if (++nCount == MAX_SEG_DATA_NUM || i == nTotalSize-1) {
				intwrite(nSize, pFile);
				nCount = 0;
			}
		}
	}
}

inline void writeDoubleArray(std::vector<double> datas, CFile* pFile)
{
	int nTypeSize = 8;
	int nTotalSize = (int)datas.size();
	{
		int nSize;
		int nCount = 0;
		for (int i = 0; i < nTotalSize; ++i) {
			if (nCount == 0) {
				int nLeftSize = nTotalSize - i;
				if (nLeftSize > MAX_SEG_DATA_NUM)
					nLeftSize = MAX_SEG_DATA_NUM;

				nSize = nLeftSize * nTypeSize;
				intwrite(nSize, pFile);
			}

			doublewrite(datas[i], pFile);

			if (++nCount == MAX_SEG_DATA_NUM || i == nTotalSize-1) {
				intwrite(nSize, pFile);
				nCount = 0;
			}
		}
	}
}

inline void writeCharArray(std::vector<CString> datas, CFile* pFile, int nMaxSegNum)
{
	int nTypeSize = 8;
	int nTotalSize = (int)datas.size();
	{
		int nSize;
		int nCount = 0;
		for (int i = 0; i < nTotalSize; ++i) {
			if (nCount == 0) {
				int nLeftSize = nTotalSize - i;
				if (nLeftSize > nMaxSegNum)
					nLeftSize = nMaxSegNum;

				nSize = nLeftSize * nTypeSize;
				intwrite(nSize, pFile);
			}

			charStringwrite(datas[i], pFile);

			if (++nCount == nMaxSegNum || i == nTotalSize-1) {
				intwrite(nSize, pFile);
				nCount = 0;
			}
		}
	}
}