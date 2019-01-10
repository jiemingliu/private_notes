/******************************************************************************
* 文件名: jcnonecopy.h
* 描述: -
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/03/14 10:36:8, gaopeng 创建文件
*
******************************************************************************/

#ifndef _JCNONECOPY_H
#define _JCNONECOPY_H

#include "jcbase.h"

class DLL_EXPORT JCNonecopy
{
protected:
	JCNonecopy();
	~JCNonecopy();

private:
	JCNonecopy(const JCNonecopy&);
	const JCNonecopy& operator=(const JCNonecopy&);
};

#endif /* _JCNONECOPY_H */

