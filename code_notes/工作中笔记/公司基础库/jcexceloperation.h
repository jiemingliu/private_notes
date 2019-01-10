/******************************************************************************
* 文件名: jcexceloperation.hpp
* 描述: 对LibExcel的引用
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/07/22 20:28:20, lys 创建文件
*
******************************************************************************/
#ifndef _JCEXCELOPERATION_H_
#define _JCEXCELOPERATION_H_

#include <string>
#include "jcbase.h"

namespace xlsFile
{
	class XLSFile;
}
class XLSSheet;
class JCExcelFile;

using namespace std;
using namespace xlsFile;

class DLL_EXPORT JCExcelSheet
{
public:
	enum Color_Type
	{
		COLOR_BLACK = 1,
		COLOR_BROWN,
		COLOR_OLIVE_GREEN,
		COLOR_DARK_GREEN,
		COLOR_DARK_TEAL,
		COLOR_DARK_BLUE,
		COLOR_INDIGO,
		COLOR_GRAY80,

		COLOR_DARK_RED,
		COLOR_ORANGE,
		COLOR_DARK_YELLOW,
		COLOR_GREEN,
		COLOR_TEAL,
		COLOR_BLUE,
		COLOR_BLUE_GRAY,
		COLOR_GRAY50,

		COLOR_RED,
		COLOR_LIGHT_ORANGE,
		COLOR_LIME,
		COLOR_SEA_GREEN,
		COLOR_AQUA,
		COLOR_LIGHT_BLUE,
		COLOR_VIOLET,
		COLOR_GRAY40,

		COLOR_PINK,
		COLOR_GOLD,
		COLOR_YELLOW,
		COLOR_BRIGHT_GREEN,
		COLOR_TURQUOISE,
		COLOR_SKY_BLUE,
		COLOR_PLUM,
		COLOR_GRAY25,

		COLOR_ROSE,
		COLOR_TAN,
		COLOR_LIGHT_YELLOW,
		COLOR_LIGHT_GREEN,
		COLOR_LIGHT_TURQUOISE,
		COLOR_PALEBLUE,
		COLOR_LAVENDER,
		COLOR_WHITE,

		COLOR_PERIWINKLE,
		COLOR_DARK_BLUE2,
		COLOR_PLUM2,
		COLOR_PINK2,
		COLOR_IVORY,
		COLOR_YELLOW2,
		COLOR_LIGHT_TURQUOISE2,
		COLOR_TURQUOISE2,

		COLOR_DARK_PURPLE,
		COLOR_VIOLET2,
		COLOR_CORAL,
		COLOR_DARK_RED2,
		COLOR_OCEAN_BLUE,
		COLOR_TEAL2,
		COLOR_ICE_BLUE,
		COLOR_BLUE2,

		COLOR_SYS_WIND_FG,
		COLOR_SYS_WIND_BG
	};

	enum Border_Style
	{
		BORDER_NONE = 0,
		BORDER_THIN,
		BORDER_MEDIUM,
		BORDER_DASHED,
		BORDER_DOTTED,
		BORDER_THICK,
		BORDER_DOUBLE,
		BORDER_HAIR
	};

	enum Border_Side
	{
		BORDER_BOTTOM = 0,
		BORDER_TOP,
		BORDER_LEFT,
		BORDER_RIGHT
	};

	enum BoldNess_Option
	{
		BOLDNESS_BOLD = 0,
		BOLDNESS_HALF,
		BOLDNESS_NORMAL,
		BOLDNESS_DOUBLE
	};

	enum UnderLine_Option
	{
		UNDERLINE_NONE = 0,
		UNDERLINE_SINGLE,
		UNDERLINE_DOUBLE,
		UNDERLINE_SINGLEACC,
		UNDERLINE_DOUBLEACC
	};

	enum Script_Option
	{
		SCRIPT_NONE = 0,
		SCRIPT_SUPER,
		SCRIPT_SUB
	};

	enum HAlign_Type
	{
		HALIGN_GENERAL = 0,
		HALIGN_LEFT,
		HALIGN_CENTER,
		HALIGN_RIGHT,
		HALIGN_FILL,
		HALIGN_JUSTIFY,
		HALIGN_CENTERACCROSS
	};

	enum VAlign_Type
	{
		VALIGN_TOP = 0,
		VALIGN_CENTER,
		VALIGN_BOTTOM,
		VALIGN_JUSTIFY
	};

	enum TextOrient_Option
	{
		ORI_NONE = 0,
		ORI_TOPBOTTOMTXT,
		ORI_90NOCLOCKTXT,
		ORI_90CLOCKTXT
	};

	enum Fill_Option
	{
		FILL_NONE = 0,
		FILL_SOLID,
		FILL_ATEN75,
		FILL_ATEN50,
		FILL_ATEN25,
		FILL_ATEN12,
		FILL_ATEN06,
		FILL_HORIZ_LIN,
		FILL_VERTICAL_LIN,
		FILL_DIAG,
		FILL_INV_DIAG,
		FILL_INTER_DIAG,
		FILL_DIAG_THICK_INTER,
		FILL_HORIZ_LINES_THIN,
		FILL_VERTICAL_LINES_THIN,
		FILL_DIAG_THIN,
		FILL_INV_DIAG_THIN,
		FILL_HORIZ_INT_THIN,
		FILL_HORIZ_INTER_THICK
	};

	JCExcelSheet(const string &strSheetName, JCExcelFile &tExcelFile);
	JCExcelSheet(XLSSheet &tWorkSheet);
	virtual ~JCExcelSheet();


    /************************************************************************/
    /*	设置属性接口                                                        */
    /************************************************************************/

    /******************************************************************************
    * 函数名:JCExcelSheet.DeleteCellContent
    * 描述: 删除单元格内容
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 17:0:22, lys 创建函数
    *
    ******************************************************************************/
	void DeleteCellContent(unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetCellText
    * 描述: 以字符串设置单元格内容
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 17:1:2, lys 创建函数
    *
    ******************************************************************************/
	void SetCellText(unsigned int nRow, unsigned int nCol, const string &strText);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetCellNumber
    * 描述: 以浮点数值设置单元格内容
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 17:1:45, lys 创建函数
    *
    ******************************************************************************/
	void SetCellNumber(unsigned int nRow, unsigned int nCol, const double dNum);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetCellNumber
    * 描述: 以整形值设置单元格内容
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 17:2:17, lys 创建函数
    *
    ******************************************************************************/
	void SetCellNumber(unsigned int nRow, unsigned int nCol, const int nNum);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetTabNumber
    * 描述: 向表格填充整形数值
    *
    * 输入:tTab数值数组, nRowCount表格行数, nColCount表格列数, nRowStep表格行步进, nColStep表格列步进
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 19:10:1, lys 创建函数
    *
    ******************************************************************************/
	void SetTabNumber(const int *tTab, unsigned int nRowCount ,unsigned int nColCount, unsigned int nRowStep = 0, unsigned int nColStep = 0);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetTabNumber
    * 描述: 向表格填充浮点数值
    *
    * 输入:tTab数值数组, nRowCount表格行数, nColCount表格列数, nRowStep表格行步进, nColStep表格列步进
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 19:12:33, lys 创建函数
    *
    ******************************************************************************/
	void SetTabNumber(const double *tTab, unsigned int nRowCount ,unsigned int nColCount, unsigned int nRowStep = 0, unsigned int nColStep = 0);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetTabText
    * 描述: 向表格填充字符串
    *
    * 输入:tTab字符串数组, nRowCount表格行数, nColCount表格列数, nRowStep表格行步进, nColStep表格列步进
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 19:12:55, lys 创建函数
    *
    ******************************************************************************/
	void SetTabText(const string *tTab, unsigned int nRowCount ,unsigned int nColCount, unsigned int nRowStep = 0, unsigned int nColStep = 0);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetRowHeight
    * 描述: 设置行高
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 17:3:27, lys 创建函数
    *
    ******************************************************************************/
	void SetRowHeight(unsigned int nRow, const unsigned int nHeight);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetColWidth
    * 描述: 设置列宽
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 17:3:53, lys 创建函数
    *
    ******************************************************************************/
	void SetColWidth(unsigned int nCol, const unsigned int nWidth);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetCellMerge
    * 描述: 合并单元格
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 17:4:27, lys 创建函数
    *
    ******************************************************************************/
	void SetCellMerge(unsigned int nStartRow, unsigned int nStartCol,
				unsigned int nEndRow, unsigned int nEndCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetCellFrame
    * 描述: 设置单元格边框
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 17:5:51, lys 创建函数
    *
    ******************************************************************************/
	void SetCellFrame(unsigned int nRow, unsigned int nCol, const Color_Type eColor, const Border_Style eBorder);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetCellBgColor
    * 描述: 设置单元格背景颜色
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/24 17:31:15, lys 创建函数
    *
    ******************************************************************************/
	void SetCellBgColor(const unsigned int nRow, const unsigned int nCol, const Color_Type eColor, const Fill_Option eFill);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetCellBorderStyle
    * 描述: 设置单元格边框风格
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/24 17:36:34, lys 创建函数
    *
    ******************************************************************************/
	void SetCellBorderStyle(const unsigned int nRow, const unsigned int nCol, const Border_Side eBorder,
							const Border_Style eBs, const Color_Type eColor);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetCellLocked
    * 描述: 设置单元格锁定
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/28 9:46:48, lys 创建函数
    *
    ******************************************************************************/
	void SetCellLocked(const unsigned int nRow, const unsigned int nCol, bool bLock);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetFont
    * 描述: 设置单元格字体
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 17:7:3, lys 创建函数
    *
    ******************************************************************************/
	void SetFont(unsigned int nRow, unsigned int nCol, const string strFontName);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetFontColor
    * 描述: 设置单元格字体颜色
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 17:7:37, lys 创建函数
    *
    ******************************************************************************/
	void SetFontColor(unsigned int nRow, unsigned int nCol, const Color_Type eColor);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetFontHeight
    * 描述: 设置字体高度
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 17:8:6, lys 创建函数
    *
    ******************************************************************************/
	void SetFontHeight(unsigned int nRow, unsigned int nCol, const unsigned int nHeight = 10);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetFontBold
    * 描述: 设置单元格字体加粗模式
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 17:29:22, lys 创建函数
    *
    ******************************************************************************/
	void SetFontBold(unsigned int nRow, unsigned int nCol, const BoldNess_Option eBold);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetFontUnderline
    * 描述: -
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 17:30:0, lys 创建函数
    *
    ******************************************************************************/
	void SetFontUnderline(unsigned int nRow, unsigned int nCol, const UnderLine_Option eUnderLine);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetFontScript
    * 描述: 设置单元格脚本字体风格
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:24:6, lys 创建函数
    *
    ******************************************************************************/
	void SetFontScript(unsigned int nRow, unsigned int nCol, const Script_Option eScript);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetFontItalic
    * 描述: 设置单元格字体斜体
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 17:33:22, lys 创建函数
    *
    ******************************************************************************/
	void SetFontItalic(unsigned int nRow, unsigned int nCol, const bool bIt = true);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetFontStrikeout
    * 描述: 设置字体带删除线
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:27:49, lys 创建函数
    *
    ******************************************************************************/
	void SetFontStrikeout(unsigned int nRow, unsigned int nCol, const bool bStrike);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetFontOutline
    * 描述: 设置单元格字体带边框
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:28:14, lys 创建函数
    *
    ******************************************************************************/
	void SetFontOutline(unsigned int nRow, unsigned int nCol, const bool bOutLine);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetFontShadow
    * 描述: 设置单元格字体带阴影
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:28:29, lys 创建函数
    *
    ******************************************************************************/
	void SetFontShadow(unsigned int nRow, unsigned int nCol,const bool bShadow);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetFontHAlign
    * 描述: 设置文字水平对齐方式
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:30:53, lys 创建函数
    *
    ******************************************************************************/
	void SetFontHAlign(unsigned int nRow, unsigned int nCol,const HAlign_Type eHAlign);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetFontVAlign
    * 描述: 设置单元格文字垂直方向对齐方式
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:33:19, lys 创建函数
    *
    ******************************************************************************/
	void SetFontVAlign(unsigned int nRow, unsigned int nCol,const VAlign_Type eVAlign);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetFontOrientation
    * 描述: 设置单元格文字方向
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:35:32, lys 创建函数
    *
    ******************************************************************************/
	void SetFontOrientation(unsigned int nRow, unsigned int nCol, const TextOrient_Option eOrient);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetRowColor
    * 描述: 设置行颜色
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/26 16:8:20, lys 创建函数
    *
    ******************************************************************************/
	void SetRowBgColor(unsigned int nRow, unsigned int nStartCol, unsigned int nEndCol, const Color_Type eColor, const Fill_Option eFill);

    /******************************************************************************
    * 函数名:JCExcelSheet.SetColColor
    * 描述: 设置列颜色
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/26 16:8:26, lys 创建函数
    *
    ******************************************************************************/
	void SetColBgColor(unsigned int nCol, unsigned int nStartRow, unsigned int nEndRow, const Color_Type eColor, const Fill_Option eFill);

	/************************************************************************/
	/*  获取属性接口                                                        */
	/************************************************************************/


    /******************************************************************************
    * 函数名:JCExcelSheet.GetLabel
    * 描述: 获取字符串单元格中的字符串内容
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:41:45, lys 创建函数
    *
    ******************************************************************************/
	string GetLabel(unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.GetNumberDouble
    * 描述: 以浮点型获取数值类型(整形或浮点)单元格中的数值
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:42:11, lys 创建函数
    *
    ******************************************************************************/
	double GetNumberDouble (unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.GetNumberInt
    * 描述: 以整型获取数值类型(整形或浮点)单元格中的数值
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:42:35, lys 创建函数
    *
    ******************************************************************************/
	int GetNumberInt (unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.GetCellType
    * 描述: 获取单元格内容类型
    *
    * 输入:
    * 输出:
    * 返回值: "Blank"  无内容 "String" 字符串 "Double" 浮点数  "Integer"整形
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:42:58, lys 创建函数
    *
    ******************************************************************************/
	string GetCellType(unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.GetMaxCol
    * 描述: 获取最大列号
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:52:15, lys 创建函数
    *
    ******************************************************************************/
	int GetMaxCol();

    /******************************************************************************
    * 函数名:JCExcelSheet.GetMinCol
    * 描述: 获取最小列号
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:52:30, lys 创建函数
    *
    ******************************************************************************/
	int GetMinCol();

    /******************************************************************************
    * 函数名:JCExcelSheet.GetMinRow
    * 描述: 获取最小行号
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:52:42, lys 创建函数
    *
    ******************************************************************************/
	int GetMinRow();

    /******************************************************************************
    * 函数名:JCExcelSheet.GetMaxRow
    * 描述: 获取最大行号
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:52:54, lys 创建函数
    *
    ******************************************************************************/
	int GetMaxRow();

    /******************************************************************************
    * 函数名:JCExcelSheet.HasBorderTop
    * 描述: 单元格是否有上边框
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:54:24, lys 创建函数
    *
    ******************************************************************************/
	bool HasBorderTop(unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.HasBorderRight
    * 描述: 是否有右边框
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:54:49, lys 创建函数
    *
    ******************************************************************************/
	bool HasBorderRight(unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.HasBorderLeft
    * 描述: 是否有左边框
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:55:35, lys 创建函数
    *
    ******************************************************************************/
	bool HasBorderLeft(unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.HasBorderBottom
    * 描述: 是否有下边框
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:55:49, lys 创建函数
    *
    ******************************************************************************/
	bool HasBorderBottom(unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.GetBgColor
    * 描述: 获取单元格背景色
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:56:20, lys 创建函数
    *
    ******************************************************************************/
	Color_Type GetBgColor(unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.GetFontColor
    * 描述: 获取字体颜色
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:56:34, lys 创建函数
    *
    ******************************************************************************/
	Color_Type GetFontColor(unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.GetFontName
    * 描述: 获取字体类型名
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:57:48, lys 创建函数
    *
    ******************************************************************************/
	string GetFontName(unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.GetFontHeight
    * 描述: 获取字体高度
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:58:4, lys 创建函数
    *
    ******************************************************************************/
	int GetFontHeight(unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.GetBold
    * 描述: 获取字体加粗选项
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:58:26, lys 创建函数
    *
    ******************************************************************************/
	BoldNess_Option GetBold(unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.GetUnderLine
    * 描述: 获取字体下划线选项
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:58:54, lys 创建函数
    *
    ******************************************************************************/
	UnderLine_Option GetUnderLine(unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.GetScript
    * 描述: 获取脚本字体选项
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:59:33, lys 创建函数
    *
    ******************************************************************************/
	Script_Option GetScript(unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.IsFontItalic
    * 描述: 字体是否为斜体
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 18:59:56, lys 创建函数
    *
    ******************************************************************************/
	bool IsFontItalic(unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.GetHAlign
    * 描述: 获取单元格内容水平对齐方式
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 19:0:27, lys 创建函数
    *
    ******************************************************************************/
	HAlign_Type GetHAlign(unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.GetVAlign
    * 描述: 获取单元格内容垂直对齐方式
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 19:0:27, lys 创建函数
    *
    ******************************************************************************/
	VAlign_Type GetVAlign(unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.GetOrientation
    * 描述: 获取单元格内容方向
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 19:1:42, lys 创建函数
    *
    ******************************************************************************/
	TextOrient_Option GetOrientation(unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.GetColspan
    * 描述: 获取单元格列延伸长度(合并单元格时即合并多个基础单元格的列数-1)
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 19:3:10, lys 创建函数
    *
    ******************************************************************************/
	int GetColspan(unsigned int nRow, unsigned int nCol);

    /******************************************************************************
    * 函数名:JCExcelSheet.GetRowspan
    * 描述: 获取单元格行延伸长度(合并单元格时即合并多个基础单元格的行数-1)
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 19:4:7, lys 创建函数
    *
    ******************************************************************************/
	int GetRowspan(unsigned int nRow, unsigned int nCol);

private:
	XLSSheet *m_pSheet;
};


class DLL_EXPORT JCExcelFile
{
	friend class JCExcelSheet;
public:
	enum JCExcelMode{CREATE_ONLY, READ_WRITE};

	JCExcelFile(const string &strFile, JCExcelMode eMode = CREATE_ONLY);
	virtual ~JCExcelFile();

	XLSFile *GetXlsFile();

    /******************************************************************************
    * 函数名:JCExcelFile.UpdateExcelFilePath
    * 描述: 更新Excel文件完整路径名
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/23 23:55:43, lys 创建函数
    *
    ******************************************************************************/
	void UpdateExcelFilePath(const string& strFile);

    /******************************************************************************
    * 函数名:JCExcelFile.GetExcelFilePath
    * 描述: 获取文件完整路径名
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/24 0:27:59, lys 创建函数
    *
    ******************************************************************************/
	string GetExcelFilePath();

    /******************************************************************************
    * 函数名:JCExcelFile.GetSheetCount
    * 描述: 获取Excel文件sheet个数
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/24 0:28:37, lys 创建函数
    *
    ******************************************************************************/
	unsigned int GetSheetCount();

    /******************************************************************************
    * 函数名:JCExcelFile.NewSheet
    * 描述: 为Excel文件生成一个新sheet
    *
    * 输入: sheet名
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/24 0:29:7, lys 创建函数
    *
    ******************************************************************************/
	JCExcelSheet NewSheet(const string &strSheetName = "sheet");

    /******************************************************************************
    * 函数名:JCExcelFile.GetSheet
    * 描述: 获取Excel文件的指定sheet对象
    *
    * 输入: nSheetIndex Sheet索引,从0开始
    * 输出: 当Excel中不存在sheet时,会抛出异常
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/24 0:29:46, lys 创建函数
    *
    ******************************************************************************/
	JCExcelSheet GetSheet(const int nSheetIndex);

    /******************************************************************************
    * 函数名:JCExcelFile.Save
    * 描述: 保存EXCEL对象到磁盘文件(Save之后ExcelFile对象本身已失效, 若继续操作此文件需重新载入到对象)
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/24 0:30:28, lys 创建函数
    *
    ******************************************************************************/
	void Save();

    /******************************************************************************
    * 函数名:JCExcelFile.SaveAs
    * 描述: Excel另存为指定文件名的磁盘文件
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/24 0:30:59, lys 创建函数
    *
    ******************************************************************************/
	void SaveAs(const string &strFile);

    /******************************************************************************
    * 函数名:JCExcelFile.OpenCsv
    * 描述: -
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/24 0:31:45, lys 创建函数
    *
    ******************************************************************************/
	//XLSSheet OpenCsv(const string &strCsvName, const char chSeparator=';', const string &strSheetName="sheetCsv");

    /******************************************************************************
    * 函数名:JCExcelFile.OpenCsv2
    * 描述: -
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/24 0:31:53, lys 创建函数
    *
    ******************************************************************************/
	JCExcelSheet OpenCsv2(const string &strCsvName, const string &strSheetName="sheetCsv", const char chSeparator=';');

    /******************************************************************************
    * 函数名:JCExcelFile.SaveToCsv
    * 描述: 保存EXCEL对象到CSV文件
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/24 0:32:9, lys 创建函数
    *
    ******************************************************************************/
	void SaveToCsv(const string &strCsvName);

    /******************************************************************************
    * 函数名:JCExcelFile.CopyXls
    * 描述: 拷贝Excel文件到Excel文件, Copy失败会报异常
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/24 0:32:33, lys 创建函数
    *
    ******************************************************************************/
	static void CopyXls(const string& strXlsName, const string& strCopyName);

    /******************************************************************************
    * 函数名:JCExcelFile.CopyXlstoHtml
    * 描述: 拷贝Excel文件到Html文件, Copy失败会报异常
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/24 0:32:53, lys 创建函数
    *
    ******************************************************************************/
    static void CopyXlstoHtml(const string &strXlsName, const string &strHtmlName);

    /******************************************************************************
    * 函数名:JCExcelFile.CopyXlstoCsv
    * 描述: 拷贝Excel文件到csv文件, Copy失败会报异常
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/07/24 0:33:20, lys 创建函数
    *
    ******************************************************************************/
	static void CopyXlstoCsv(const string &strXlsName, const string &strCsvName);

private:
	XLSFile *m_pXlsFile;
};


#endif /* _JCEXCELOPERATION_H_ */

