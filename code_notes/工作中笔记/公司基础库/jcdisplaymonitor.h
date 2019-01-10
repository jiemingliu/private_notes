/******************************************************************************
* 文件名: jcdisplaymonitor.h
* 描述: PC显示器操作
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/07/10 9:38:52, lys 创建文件
*
******************************************************************************/
#ifndef _JCDISPLAYMONITOR_H_
#define _JCDISPLAYMONITOR_H_

#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)

#else /*end linux*/

#include <windows.h>
#include <vector>
#include "jcbase.h"

using namespace std;

struct DLL_EXPORT JCMonitorInfo
{
	MONITORINFOEX m_nMonitorRawInfo;
	HMONITOR      m_hMonitorHandle;
	HDC           m_hMonitorHdc;
};

typedef vector<JCMonitorInfo> JCMonitorInfoSet;
typedef vector<DISPLAY_DEVICE> JCDispDevInfoSet;

class DLL_EXPORT JCDisplayMonitor
{
public:
	enum DisplayMode
	{
		DISPLAY_TYPE_SINGLE, 
		DISPLAY_TYPE_CLONE, 
		DISPLAY_TYPE_EXTEND
	};

	enum ErrorCode
	{
		SUCCESS,     
		NO_MONITOR,  
		ONLYONE_MONITOR,
		DEVINFO_ERROR,
		SET_FAILED
	};

	JCDisplayMonitor();
	virtual ~JCDisplayMonitor();

	static BOOL MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
	static unsigned int EnumMonitors();
	static unsigned int EnumDispDevices();
	static JCMonitorInfoSet &MonitorsInfo();
	static JCDispDevInfoSet &DispDevInfo();
	static ErrorCode SetDisplayMode(DisplayMode eMode, unsigned int nX, unsigned int nY, bool bExChange = false);

private:
	static JCMonitorInfoSet m_tMonitorInfo;
	static JCDispDevInfoSet m_tDispDevInfo;
};


class DLL_EXPORT JCScreenPaint
{
public:
	JCScreenPaint();
	virtual ~JCScreenPaint();

	bool InitDC();
	void DrawBmp(const string &strBmpPath);
	void DrawLine(POINT tStartPoint, POINT tEndPoint, unsigned int nLineType
		, unsigned int nWidth, COLORREF tPenColor);
	void DrawPoint(POINT pt, unsigned int nLineWidth, unsigned int nWidth, COLORREF tPenColor);
	void DrawPolygon(POINT pt[], unsigned int nPtNum, unsigned int nFillMode, COLORREF tFillColor
		, unsigned int nLineType, COLORREF tLineColor);
	void DrawCircle(unsigned int nCenterX, unsigned int nCenterY
		, unsigned int nRadius, unsigned int nLineType, int nWidth, COLORREF tPenColor);
	void RePaint();

private:
	HDC m_hDestDC  ;
	HDC m_hSrcDC ; // source DC - memory device context

	unsigned int m_nDestWidth;
	unsigned int m_nDestHeight;
	unsigned int m_nSrcWidth;
	unsigned int m_nSrcHeight;
};
#endif /*end win32*/

#endif /* _JCDISPLAYMONITOR_H_ */

