#include "stdafx.h"
#include "GstCoordinate.h"

using namespace TGS;
GstCoordinate::GstCoordinate( void )
{
	m_matAct2Scr.reset();
	m_matScr2Act = m_matAct2Scr;
	//
	m_offsetPt.iset( 0,0 );
}

GstCoordinate::~GstCoordinate( void )
{


}

void GstCoordinate::saveCanvasOffset( SkCanvas* canvas ) const
{
	//SkRect rcBnd;
	//getPaintObject()->getBoundingBox(rcBnd);
	//SkRect offsetRect = rcBnd;
	//offsetRect.offset( -m_offsetPt );
	SkMatrix mat;
	mat.reset();
	mat.setTranslate(m_offsetPt);
	SkMatrix matTota = canvas->getTotalMatrix();
	matTota.preConcat(mat);
	canvas->setMatrix( matTota );
}

void GstCoordinate::setXYScaleByDrawRect(SkRect drawRc,bool isUp = true)
{
	drawRc.sort();
	m_origination = SkPoint::Make(drawRc.left(),drawRc.top());
	float xmax,ymax,xmin,ymin;
	getActualRange(xmax,xmin,ymax,ymin);

	float xscale = (xmax-xmin)*1000/drawRc.width();
	float yscale = (ymax-ymin)*1000/drawRc.height();
	setX_Scale(xscale);
	setY_Scale(yscale);

	initMatrix(isUp);
}

void GstCoordinate::getMatrix(SkMatrix&matAct2Scr, SkMatrix& matScr2Act)
{
	matAct2Scr = m_matAct2Scr;
	matScr2Act = m_matScr2Act;
}

void GstCoordinate::setRenderLocalMatrix(const SkMatrix& mat)
{
	m_renderLocalMatrix = mat;
}

void GstCoordinate::getRenderLocalMatrix(SkMatrix& mat)
{
	mat = m_renderLocalMatrix;
}

float  GstCoordinate::getMainTickScaleY()
{
	assert(false);
	return 0;
}

float GstCoordinate::getSubTickScaleY()
{
	assert(false);
	return 0;
}

float  GstCoordinate::getTinyTickScaleY()
{
	assert(false);
	return 0;
}

float  GstCoordinate::actualToScreenX( float actualX )
{
	assert(!m_matAct2Scr.isIdentity());

	SkPoint pt = SkPoint::Make(0,0);
	m_matAct2Scr.mapXY(actualX,0, &pt);
	return pt.fX;
}

float  GstCoordinate::actualToScreenY( float actualY )
{
	assert(!m_matAct2Scr.isIdentity());
	SkPoint pt = SkPoint::Make(0,0);
	m_matAct2Scr.mapXY(0, actualY, &pt);
	return pt.fY;
}

float  GstCoordinate::screenToActualX( float screenX )
{
	assert(!m_matScr2Act.isIdentity());

	SkPoint pt = SkPoint::Make(0,0);
	m_matScr2Act.mapXY(screenX,0, &pt);
	return pt.fX;

}

float  GstCoordinate::screenToActualY( float screenY )
{
	assert(!m_matScr2Act.isIdentity());

	SkPoint pt = SkPoint::Make(0,0);
	m_matScr2Act.mapXY(0,screenY, &pt);
	return pt.fY;
}

float  GstCoordinate::getScreenCoordWidth()
{
	//assert(false);
	return 0;
}

float  GstCoordinate::getScreenCoordHeight()
{
	//assert(false);
	return 0;
}

void  GstCoordinate::initMatrix(bool isUp)
{
	SkPoint ptAct[3];
    ptAct[0] = getLeftTopActualPoint(isUp);
	ptAct[1] = getLeftBottomActualPoint(isUp);
	ptAct[2] = getRightBottomActualPoint(isUp);

	SkPoint ptScr[3];
	ptScr[0] = getLeftTopScrPoint();
	ptScr[1] = getLeftBottomScrPoint();
	ptScr[2] = getRightBottomScrPoint();

	bool bset= m_matAct2Scr.setPolyToPoly(ptAct, ptScr,3);
	assert(bset);
	if (!bset)
	{
		m_matAct2Scr.reset();
	}
	m_matScr2Act = m_matAct2Scr;
	//修正特殊角度造成的误差
	{
		//修正x方向上(0或180度时)的误差
		   //暂不做

		//修正y方向上(0或180度时)的误差
		SkPoint actYvect = ptAct[0] - ptAct[1];
		SkPoint scrYvect = ptScr[0] - ptScr[1];
		if(bIs2VectDirSameOrOpposite(actYvect,scrYvect))
		{
			m_matAct2Scr.setSkewY(0);
		}

	}

	bset=m_matAct2Scr.invert(&m_matScr2Act);
	if (!bset)
	{
		m_matScr2Act.reset();
	}
}
bool  GstCoordinate::bIs2VectDirSameOrOpposite(const SkPoint &firstVect , const SkPoint &secondVect)
{
	//求向量的长度乘积
	float lengthProduct = firstVect.length() * secondVect.length();

	if(0 == lengthProduct )  //有向量的长度为0
	{
		return true;
	}
	float vectorProduct =  firstVect.fX * secondVect.fX + firstVect.fY * secondVect.fY;  //向量乘积

	if(lengthProduct == vectorProduct ||    //夹角为0度
		0 == lengthProduct + vectorProduct)  //夹角为180度
	{
		return true;
	}
	return false;

	//经测试，上述方法在一般情况下也满足要求

	////将所有的值转换为double型，防止float型精度不够造成的误差
	//double fx = static_cast<double>(firstVect.fX);
	//double fy = static_cast<double>(firstVect.fY);
	//double sx = static_cast<double>(secondVect.fX);
	//double sy = static_cast<double>(secondVect.fY);

	////求向量的长度乘积
	////之所以不用 firstVect.length() * secondVect.length() 是因为它用的是float型，精度不够
	//double squareLengthOfFirstVect = fx * fx + fy * fy ;//firstVect.fX * firstVect.fX + firstVect.fY * firstVect.fY;
	//double squareLengthOfSecondVect = sx * sx + sy * sy;//secondVect.fX * secondVect.fX + secondVect.fY * secondVect.fY;
	//double lengthProduct = sqrt(squareLengthOfFirstVect * squareLengthOfSecondVect);

	//if(0 == lengthProduct )  //有向量的长度为0
	//{
	//	return true;
	//}
	//double vectorProduct =  fx * sx + fy * sy;//firstVect.fX * secondVect.fX + firstVect.fY * secondVect.fY;  //向量乘积

	//if(lengthProduct == vectorProduct ||    //夹角为0度
	//   0 == lengthProduct + vectorProduct)  //夹角为180度
	//{
	//   return true;
	//}
	//return false;
}
SkPoint  GstCoordinate::getLeftTopScrPoint()
{
	SkRect tmpRect ;
	getDrawRect(tmpRect);
	return SkPoint::Make(tmpRect.fLeft, tmpRect.fTop);
	//assert(false);
	//return SkPoint::Make(0,0);
}

SkPoint  GstCoordinate::getLeftBottomScrPoint()
{
	SkRect tmpRect;
	getDrawRect(tmpRect);
	return SkPoint::Make(tmpRect.fLeft, tmpRect.fBottom);

	//assert(false);
	//return SkPoint::Make(0,0);
}

SkPoint  GstCoordinate::getRightBottomScrPoint()
{
	SkRect tmpRect;
	getDrawRect(tmpRect);
	return SkPoint::Make(tmpRect.fRight, tmpRect.fBottom);

	//assert(false);
	//return SkPoint::Make(0,0);
}

SkPoint  GstCoordinate::getLeftTopActualPoint(bool isUp)
{
	float xmax, xmin, ymax, ymin;
	getActualRange(xmax, xmin, ymax, ymin);
	if (isUp)
	{
		return SkPoint::Make(xmin, ymax);
	}else
	{
		return SkPoint::Make(xmin, ymin);
	}
	
	//assert(false);
	//return SkPoint::Make(0,0);
}

SkPoint  GstCoordinate::getLeftBottomActualPoint(bool isUp)
{
	float xmax, xmin, ymax, ymin;
	getActualRange(xmax, xmin, ymax, ymin);
	if (isUp)
	{
		return SkPoint::Make(xmin, ymin);
	}else
	{
		return SkPoint::Make(xmin, ymax);
	}
	

	//assert(false);
	//return SkPoint::Make(0,0);
}

SkPoint  GstCoordinate::getRightBottomActualPoint(bool isUp)
{
	float xmax, xmin, ymax, ymin;
	getActualRange(xmax, xmin, ymax, ymin);
	if (isUp)
	{
		return SkPoint::Make(xmax, ymin);
	}else
	{
		return SkPoint::Make(xmax, ymax);
	}
}

SkPoint  GstCoordinate::actualToScreen( SkPoint actualP )
{
	assert(!m_matAct2Scr.isIdentity());
	SkPoint pt = SkPoint::Make(0,0);
	m_matAct2Scr.mapXY(actualP.fX, actualP.fY, &pt);
	return pt;
}

SkPoint  GstCoordinate::screenToActual( SkPoint screenP )
{
	assert(!m_matScr2Act.isIdentity());

	SkPoint pt = SkPoint::Make(0,0);
	m_matScr2Act.mapXY(screenP.fX, screenP.fY, &pt);
	return pt;
}

void  GstCoordinate::setActualRange( float xMax, float xMin, float yMax, float yMin )
{
	//m_fXMax = xMax;
	//m_fXMin = xMin;
	//m_fYMax = yMax;
	//m_fYMin = yMin;
}

void  GstCoordinate::setDrawRect( SkRect rc)
{
	//m_rect = rc;
}

bool  GstCoordinate::valid()
{
	return !m_matScr2Act.isIdentity();
}

bool  GstCoordinate::getDrawRect(SkRect& rcBond)
{
	SkPoint org= getOrigination();
	float xMax;
	float xMin; 
	float yMax;
	float yMin;
	float xScale= getX_Scale();
	float yScale= getY_Scale();

	getActualRange( xMax, xMin, yMax, yMin );
	float fxw=(xMax-xMin)*1000/xScale;
	float fyw=(yMax-yMin)*1000/yScale;
	//;//取得工区的内框绘制范围...todo。。。
	rcBond = SkRect::MakeXYWH(org.x(), org.y(), fxw, fyw);
	//return rcBond;
	return true;
	//return m_rect;
}

void  GstCoordinate::getActualRange( float &xMax, float &xMin, float &yMax, float &yMin )
{
	assert(false);

	//xMax = m_fXMax;
	//xMin = m_fXMin;
	//yMin = m_fYMin;
	//yMax = m_fYMax;
}

/**
 * 获取工区矩形
 */
SkRect GstCoordinate::getCoordinateRect()
{

	return  m_coordinateRect;
}


/**
 * 设置工区矩形大小
 */
void GstCoordinate::setCoordinateRect(SkRect rect)
{
	m_coordinateRect = rect;
}