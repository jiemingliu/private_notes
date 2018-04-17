#pragma once

/**
* 方法:	获取每月天数
* 全名:	GetDayAtMonth
* 类型:	public 
* 
* @date	2008/08/14
* @author	flf
* @param	const COleDateTime & time
* @return	int
*/
inline int GetDayAtMonth(const COleDateTime* time )
{
	static COleDateTime newTime;
	ASSERT(time->GetStatus() == COleDateTime::valid);

	if (time->GetMonth() < 12)
	{
		//设为下月第一天
		newTime.SetDate(time->GetYear(),time->GetMonth()+1,1);
	}
	else
	{
		//设为下月第一天
		newTime.SetDate(time->GetYear()+1,1,1);
	}

	//减一天
	COleDateTimeSpan span;
	span.SetDateTimeSpan(1,0,0,0);
	newTime -= span;

	return newTime.GetDay();
}

inline int GetDayAtSeason(const COleDateTime* time )
{
	static COleDateTime newTime;
	static int nMonthDay = 0;
	nMonthDay = GetDayAtMonth(time);

	int nMonth = time->GetMonth() % 3;
	switch(nMonth)
	{
	case 0://3,6,9,12
		{
			//上月
			newTime.SetDate(time->GetYear(),time->GetMonth()-1,1);
			nMonthDay += GetDayAtMonth(&newTime);
			
			//上上月
			newTime.SetDate(time->GetYear(),time->GetMonth()-2,1);
			nMonthDay += GetDayAtMonth(&newTime);
			break;
		}
	case 1://1,4,7,10
		{
			//下月
			newTime.SetDate(time->GetYear(),time->GetMonth()+1,1);
			nMonthDay += GetDayAtMonth(&newTime);

			//下下月
			newTime.SetDate(time->GetYear(),time->GetMonth()+2,1);
			nMonthDay += GetDayAtMonth(&newTime);
			break;
		}
	default://2,5,8,11
		{
			//下月
			newTime.SetDate(time->GetYear(),time->GetMonth()+1,1);
			nMonthDay += GetDayAtMonth(&newTime);
			
			//上月
			newTime.SetDate(time->GetYear(),time->GetMonth()-1,1);
			nMonthDay += GetDayAtMonth(&newTime);
			break;
		}
	}

	return nMonthDay;
}

inline int ConvertMonth(const CString&strMonth)
{
	if (strMonth == "JAN")
	{
		return 1;
	}
	if (strMonth == "FEB")
	{
		return 2;
	}

	if (strMonth == "MAR")
	{
		return 3;
	}

	if (strMonth == "APR")
	{
		return 4;
	}

	if (strMonth == "MAY")
	{
		return 5;
	}

	if (strMonth == "JUN")
	{
		return 6;
	}

	if (strMonth == "JUL" || strMonth == "JLY")
	{
		return 7;
	}

	if (strMonth == "AUG")
	{
		return 8;
	}

	if (strMonth == "SEP")
	{
		return 9;
	}

	if (strMonth == "COT" || strMonth == "OCT")
	{
		return 10;
	}

	if (strMonth == "NOV")
	{
		return 11;
	}

	if (strMonth == "DEC")
	{
		return 12;
	}

	return -1;
}
/**
 * 方法:	字符串转时间（Y-m-d格式：1992-8-07）
 * 全名:	StringCastTime
 * 类型:	public 
 * 
 * @date	2008/08/14
 * @author	flf
 * @param	const CString & strDateTime
 * @param	COleDateTime * time
 * @return	BOOL
 */
inline BOOL StringCastTime(const CString& strDateTime,COleDateTime* time )
{
	try
	{
		//年
		CString strYear = strDateTime.SpanExcluding("-./");
		int nYear = atoi(strYear);
		if (nYear < 1900)
		{
			nYear+= 1900;
		}

		//月
		int nTemp = strDateTime.GetLength() - strYear.GetLength() - 1;
		CString strMonth = strDateTime.Right(nTemp).SpanExcluding("-./");
		strMonth.MakeUpper();
		int nMonth = ConvertMonth(strMonth);
		if (nMonth == -1)
		{
			nMonth = atoi(strMonth);
		}

		//日
		nTemp = nTemp - strMonth.GetLength() - 1;
		CString strDay = strDateTime.Right(nTemp);
		int nDay = atoi(strDay);

		time->SetDate(nYear,nMonth,nDay);
		if (time->GetStatus() != COleDateTime::valid)
		{
			return FALSE;
		}
	}
	catch (CException* )
	{
		time->SetDate(1900,1,1);
		return FALSE;
	}
	return TRUE;
}

/**
* 方法:	字符串转时间（d-m-Y格式：07-8-1992）
* 全名:	StringCastTime1
* 类型:	public 
* 
* @date	2008/08/14
* @author	flf
* @param	const CString & strDateTime
* @param	COleDateTime * time
* @return	BOOL
*/
inline BOOL StringCastTime1(const CString& strDateTime,COleDateTime* time )
{
	try
	{
		//日
		CString strDay = strDateTime.SpanExcluding(_T("-./"));
		int nDay = atoi(strDay);

		//月
		int nTemp = strDateTime.GetLength() - strDay.GetLength() - 1;
		CString strMonth = strDateTime.Right(nTemp).SpanExcluding(_T("-./"));
		strMonth.MakeUpper();
		int nMonth = ConvertMonth(strMonth);
		if (nMonth == -1)
		{
			nMonth = atoi(strMonth);
		}

		//年
		nTemp = nTemp - strMonth.GetLength() - 1;
		CString strYear = strDateTime.Right(nTemp);
		int nYear = atoi(strYear);
		if (nYear < 1900)
		{
			nYear+= 1900;
		}

		time->SetDate(nYear,nMonth,nDay);
		if (time->GetStatus() != COleDateTime::valid)
		{
			return FALSE;
		}
	}
	catch (CException* )
	{
		time->SetDate(1900,1,1);
		return FALSE;
	}
	return TRUE;
}

/**
* 方法:	字符串转时间（Y-m格式：1992-8）
* 全名:	StringCastTime
* 类型:	public 
* 
* @date	2008/08/14
* @author	flf
* @param	const CString & strDateTime
* @param	COleDateTime * time
* @return	BOOL
*/
inline BOOL StringCastTime(const CString& strDateTime,COleDateTime* time ,int nDay)
{
	try
	{
		//年
		CString strYear = strDateTime.SpanExcluding("-./");
		int nYear = atoi(strYear);
		if (nYear < 1900)
		{
			nYear+= 1900;
		}

		//月
		int nTemp = strDateTime.GetLength() - strYear.GetLength() - 1;
		CString strMonth = strDateTime.Right(nTemp).SpanExcluding("-./");
		strMonth.MakeUpper();
		int nMonth = ConvertMonth(strMonth);
		if (nMonth == -1)
		{
			nMonth = atoi(strMonth);
		}

		if (nDay > 28)
		{
			time->SetDate(nYear,nMonth,1);
			nDay = GetDayAtMonth(time);
		}

		time->SetDate(nYear,nMonth,nDay);
		if (time->GetStatus() != COleDateTime::valid)
		{
			return FALSE;
		}
	}
	catch (CException* )
	{
		time->SetDate(1900,1,1);
		return FALSE;
	}
	return TRUE;
}

/**
* 方法:	字符串转时间（m-Y格式：8-1992）
* 全名:	StringCastTime2
* 类型:	public 
* 
* @date	2008/08/14
* @author	flf
* @param	const CString & strDateTime
* @param	COleDateTime * time
* @return	BOOL
*/
inline BOOL StringCastTime1(const CString& strDateTime,COleDateTime* time,int nDay)
{
	try
	{
		//月
		CString strMonth = strDateTime.SpanExcluding(_T("-./"));
		strMonth.MakeUpper();
		int nMonth = ConvertMonth(strMonth);
		if (nMonth == -1)
		{
			nMonth = atoi(strMonth);
		}

		//年
		int nTemp = strDateTime.GetLength() - strMonth.GetLength() - 1;
		CString strYear = strDateTime.Right(nTemp);
		int nYear = atoi(strYear);
		if (nYear < 1900)
		{
			nYear+= 1900;
		}

		if (nDay > 28)
		{
			time->SetDate(nYear,nMonth,1);
			nDay = GetDayAtMonth(time);
		}

		time->SetDate(nYear,nMonth,nDay);
		if (time->GetStatus() != COleDateTime::valid)
		{
			return FALSE;
		}
	}
	catch (CException* )
	{
		time->SetDate(1900,1,1);
		return FALSE;
	}
	return TRUE;
}

/**
 * 方法:	字符串是否数字
 * 全名:	IsNumeric
 * 类型:	public 
 * 
 * @date	2008/08/14
 * @author	flf
 * @param	LPCTSTR lpszStr
 * @return	BOOL
 */
inline BOOL IsNumeric(LPCTSTR lpszStr) 
{ 
	char szTmp[MAX_PATH]=""; 
	const char *pTmp=lpszStr; 
	if(!*lpszStr) 
		return FALSE; 
	*pTmp=='-'?pTmp++:pTmp; 
	sscanf(pTmp," %[.,0-9]",szTmp); 
	if(strcmp(pTmp,szTmp) || !strcmp(pTmp,".")) 
		return FALSE; 
	if(pTmp=strstr(szTmp,".")) 
		if(pTmp=strstr(pTmp+1 , ".")) 
			return FALSE; 
	return TRUE; 
}