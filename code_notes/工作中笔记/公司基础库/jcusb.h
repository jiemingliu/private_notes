/******************************************************************************
* 文件名: jcusb.h
* 描述: -
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/12/11 09:09:33, gaopeng 创建文件
*
******************************************************************************/
#ifndef _JCUSB_H
#define _JCUSB_H

#include <map>
#include <string>

#include <list>
#include "jcbase.h"
#include "..\..\..\JCbase\thirdparty\libusb\inc\lusb0_usb.h"
#include "jcnonecopy.h"
#include "jcLock.hpp"


typedef struct _CmdMap_
{
    int m_nCmdValue;
    std::string m_strCmdName;
    int m_nCmdMoudle;
} CmdMap, *pCmdMap;

typedef std::map<std::string, std::string> AttrMap;

typedef std::map<int , int> LibUsbListMap;

struct usb_dev_handle;

class DLL_EXPORT JCUsbWrapper : public JCNonecopy
{
public:
    JCUsbWrapper();
    ~JCUsbWrapper();

public:
    /******************************************************************************
    * 函数名:JCUsbWrapper.UsbInit
    * 描述: -使用前，对Usb的初始化
    *
    * 输入:
    * 输出:
    * 返回值:false:初始化失败， true:初始化成功
    *
    * 修改记录:
    * --------------------
    *    2014/12/11 09:09:33, gaopeng 创建函数
    *
    ******************************************************************************/
    bool UsbInit();

	/******************************************************************************
    * 函数名:JCUsbWrapper.UsbExit
    * 描述: -使用完，销毁Usb
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/12/11 09:09:33, gaopeng 创建函数
    *
    ******************************************************************************/
    void UsbExit();

	/******************************************************************************
    * 函数名:JCUsbWrapper.OpenDevice
    * 描述: -打开指定vid,pid的usb设备
    *
    * 输入:
    * 输出:
    * 返回值:false:打开失败， true:打开成功
    *
    * 修改记录:
    * --------------------
    *    2014/12/11 09:09:33, gaopeng 创建函数
    *
    ******************************************************************************/
    bool OpenDevice(int nVid, int nPid,int nDevNum);

	/******************************************************************************
    * 函数名:JCUsbWrapper.CloseDevice
    * 描述: -关闭打开的设备
    *
    * 输入:
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/12/11 09:09:33, gaopeng 创建函数
    *
    ******************************************************************************/
    void CloseDevice();

	//中断接口,收发, 支持CA310 USB口读写
	int InterruptWrite(int nEp, char* buff, int nLen, int nTimeOut = 1000);

	int InterruptRead(int nEp, char* buff, int nLen, int nTimeOut = 1000);

	//获取所有的usb口信息
	int GetAllUsbInformation(LibUsbListMap* m_tLibUsbListMap, int nVid, int nPid);

	//void OutputLog(std::string strFmt, ...);

	/****************************同步接口******************************************
	/******************************************************************************
    * 函数名:JCUsbWrapper.SyncControlTransfer
    * 描述: -usb Control Transactions
    *
    * 输入:
	*	bmRequestType	the request type field for the setup packet
	*	bRequest		the request field for the setup packet
	*	wValue			the value field for the setup packet
	*	wIndex			the index field for the setup packet
	*	data			a suitably-sized data buffer for either input or output (depending on direction bits within bmRequestType)
	*	wLength			the length field for the setup packet. The data buffer should be at least this size.
	*	timeout			timeout (in millseconds) that this function should wait before giving up due to no response being received. For an unlimited timeout, use value 0.
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/12/17 09:09:33, gaopeng 创建函数
    *
    ******************************************************************************/
    int SyncControlTransfer(unsigned char bmRequestType, 
							unsigned char bRequest, 
							unsigned short wValue, 
							unsigned short wIndex, 
							unsigned char *data, 
							unsigned short wLength, 
							unsigned int timeout);

	/****************************同步接口******************************************
	/******************************************************************************
    * 函数名:JCUsbWrapper.SyncBulkTransfer
    * 描述: -usb Bulk Transactions
    *
    * 输入:
	*	endpoint	the address of a valid endpoint to communicate with
	*	data		a suitably-sized data buffer for either input or output (depending on endpoint)
	*	length		for bulk writes, the number of bytes from data to be sent. for bulk reads, the maximum number of bytes to receive into the data buffer.
	*	transferred	output location for the number of bytes actually transferred.
	*	timeout		timeout (in millseconds) that this function should wait before giving up due to no response being received. For an unlimited timeout, use value 0.
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/12/17 09:09:33, gaopeng 创建函数
    *
    ******************************************************************************/
//     int SyncBulkTransfer(unsigned char endpoint, 
// 						unsigned char *data, 
// 						int length, 
// 						int *transferred, 
// 						unsigned int timeout);

	/****************************同步接口******************************************
	/******************************************************************************
    * 函数名:JCUsbWrapper.SyncInterruptTransfer
    * 描述: -usb Interrupt Transactions
    *
    * 输入:
	*	endpoint	the address of a valid endpoint to communicate with
	*	data		a suitably-sized data buffer for either input or output (depending on endpoint)
	*	length		for bulk writes, the number of bytes from data to be sent. for bulk reads, the maximum number of bytes to receive into the data buffer.
	*	transferred	output location for the number of bytes actually transferred.
	*	timeout		timeout (in millseconds) that this function should wait before giving up due to no response being received. For an unlimited timeout, use value 0.
    * 输出:
    * 返回值:
    *
    * 修改记录:
    * --------------------
    *    2014/12/17 09:09:33, gaopeng 创建函数
    *
    ******************************************************************************/
// 	int SyncInterruptTransfer(unsigned char endpoint, 
// 							unsigned char *data, 
// 							int length, 
// 							int *transferred, 
// 							unsigned int timeout);

private:
	usb_dev_handle *m_tHandle;
	JCCriticalSection m_tCsUsbPort;
};

#endif /* _JCUSB_H */

