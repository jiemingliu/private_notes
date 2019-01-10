#ifndef _JCDSC_H
#define _JCDSC_H

#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)

#else /*end linux*/

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <assert.h>
#include <string>
#include <list>
#include "jcbase.h"

using namespace std;

class DLL_EXPORT JCDscHandler
{
public:
	JCDscHandler(string currentPath_image,string currentPath_dsc);
	virtual ~JCDscHandler();

public:
	char ** allocate_argn (int argc, _TCHAR* argv[]);
	void release_argn(int argc, char ** nargv);
	int wstrlen(_TCHAR * wstr);
	char * wstrdup(_TCHAR * wSrc);
	int	DscHandle(int argc, _TCHAR* argv[]);
	int DivideBmpToLR(string filename);
	int MergeTwoDscToOne(string dsc1, string dsc2);
	int BMP_Convert_DSC(list<string> filelist);

private:
	string        m_strCurrentPath_image;
	string        m_strCurrentPath_dsc;
};

#endif /*end win32*/

#endif /*_JCDSC_H*/
