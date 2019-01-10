/******************************************************************************
* 文件名: jcfileencrypt.h
* 描述: 文件加解密
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2016/10/26 19:18:48, lys 创建文件
*
******************************************************************************/

#ifndef _JCFILEENCRYPT_H_
#define _JCFILEENCRYPT_H_

#include "jcbase.h"
#include <string>

using namespace std;

class DLL_EXPORT JCFileEncrypt
{
public:
	JCFileEncrypt();
	virtual ~JCFileEncrypt();


    /******************************************************************************
    * 函数名:EncryptFileEx
    * 描述: 使用windows CSP服务进行文件加密(支持中文路径)
    *
    * 输入: 源文件-加密后文件-加密密码(可以为空,解密时也为空即可,密码不支持中文)-加密后是否自动删除源文件
    * 输出:
    * 返回值: 0表示成功, 非零值均为错误码
    *
    * 修改记录:
    * --------------------
    *    2016/10/27 11:11:50, lys 创建函数
    *
    ******************************************************************************/
	static long EncryptFileEx(string &strSourceFile, string &strEncryptFile, string &strPassword, bool bAutoDelSource = true);/*和系统函数EncryptFile重名,增加_EX*/

    /******************************************************************************
    * 函数名:EncryptFileEx
    * 描述: 使用windows CSP服务进行文件解密(支持中文路径)
    *
    * 输入: 加密文件-解密后文件-加密密码(可以为空,解密时也为空即可,密码不支持中文)
    * 输出:
    * 返回值: 0表示成功, 非零值均为错误码
    *
    * 修改记录:
    * --------------------
    *    2016/10/27 11:11:50, lys 创建函数
    *
    ******************************************************************************/
    static long DecryptFileEx(string &strEncryptFile, string &strDecryptFile, string &strPassword);
};



#endif /* _JCFILEENCRYPT_H_ */

