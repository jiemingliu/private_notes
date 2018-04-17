#pragma  once
#include "SkPoint.h"

namespace LineTools
{
	//////////////////////////////////////////////////////////////////////////
	//线段与直线交点（s1_1、s1_2线段端点，l1_1、l1_2直线端点）
	bool lineIntersectSegment(const SkPoint& s1_1,const SkPoint& s1_2,const SkPoint& l1_1,const SkPoint& l1_2,SkPoint& pt)
	{
		double a1 = s1_2.fY - s1_1.fY;
		double b1 = s1_1.fX - s1_2.fX;
		double c1 = a1 * s1_1.fX + b1 * s1_1.fY;

		double a2 = l1_2.fY - l1_1.fY;
		double b2 = l1_1.fX - l1_2.fX;
		double c2 = a2 * l1_1.fX + b2 * l1_1.fY;

		double det = a1 * b2 - a2 * b1;

		if (det == 0)
		{
			if ((s1_1.fX - l1_1.fX) * (l1_2.fY - l1_1.fY) - (s1_1.fY - l1_1.fY) * (l1_2.fX - l1_1.fX) == 0 )
			{
				pt.fX = s1_1.fX;
				pt.fY = s1_1.fY;
				return true;
			}
		}
		else
		{
			pt.fX = (b2 * c1 - b1 * c2) / det;
			pt.fY = (a1 * c2 - a2 * c1) / det;

			if (min(s1_1.fX, s1_2.fX) <= pt.fX && pt.fX <= max(s1_1.fX, s1_2.fX) && 
				min(s1_1.fY, s1_2.fY) <= pt.fY && pt.fY <= max(s1_1.fY, s1_2.fY) )
			{
				return true;
			}
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	//两线段交点
	bool segmentIntersectSegment(const SkPoint& p1_1,const SkPoint& p1_2,const SkPoint& p2_1,const SkPoint& p2_2,SkPoint& pt)
	{
		double a1 = p1_2.fY - p1_1.fY;
		double b1 = p1_1.fX - p1_2.fX;
		double c1 = a1 * p1_1.fX + b1 * p1_1.fY;

		double a2 = p2_2.fY - p2_1.fY;
		double b2 = p2_1.fX - p2_2.fX;
		double c2 = a2 * p2_1.fX + b2 * p2_1.fY;

		double det = a1 * b2 - a2 * b1;

		if (det == 0)
		{
			if ((p1_1.fX - p2_1.fX) * (p2_2.fY - p2_1.fY) - (p1_1.fY - p2_1.fY) * (p2_2.fX - p2_1.fX) == 0 )
			{
				pt.fX = p1_1.fX;
				pt.fY = p1_1.fY;
				return true;
			}
		}
		else
		{
			pt.fX = (b2 * c1 - b1 * c2) / det;
			pt.fY = (a1 * c2 - a2 * c1) / det;

			if (min(p1_1.fX, p1_2.fX) <= pt.fX && pt.fX <= max(p1_1.fX, p1_2.fX) && 
				min(p1_1.fY, p1_2.fY) <= pt.fY && pt.fY <= max(p1_1.fY, p1_2.fY) &&
				min(p2_1.fX, p2_2.fX) <= pt.fX && pt.fX <= max(p2_1.fX, p2_2.fX) && 
				min(p2_1.fY, p2_2.fY) <= pt.fY && pt.fY <= max(p2_1.fY, p2_2.fY) )
			{
				return true;
			}
		}
		return false;
	}
}