/******************************************************************************
* 文件名: jcdbusmsgdef.h
* 描述: JCDbus内部控制消息定义
* Copyright(c)2014-2016 JCDZ Co.,Ltd.
*
* 修改记录:
* --------------------
* v1.0   2014/04/04 10:51:9, lys 创建文件
*
******************************************************************************/
#ifndef _JCDBUSMSGDEF_H_
#define _JCDBUSMSGDEF_H_

/*JCDBUS框架内部消息使用高10bit, JCDBUS用户消息使用低22bit*/
#define INNER_MSG_MASK   0x000003FF
#define USER_MSG_MASK    0x003FFFFF
#define INNER_MSG_OFFSET 22

#define INNER_MSG(val) (((val) & INNER_MSG_MASK)<<INNER_MSG_OFFSET)
#define USER_MSG(val) ((val) & USER_MSG_MASK)
#define IS_USER_MSG(val) ((val) & USER_MSG_MASK)

const unsigned int JCDBUS_INVALID_MSG    = 0;
const unsigned int JCDBUS_CTRL_CONNECT   = INNER_MSG(1); 
const unsigned int JCDBUS_CTRL_STOP      = INNER_MSG(2);
const unsigned int JCDBUS_CTRL_QUERY     = INNER_MSG(3);
const unsigned int JCDBUS_UDT_CONNECT    = INNER_MSG(4);
const unsigned int JCDBUS_UDT_DISCONNECT = INNER_MSG(5);
const unsigned int JCDBUS_UDT_UPLOAD_INFO	 = INNER_MSG(6);
const unsigned int JCDBUS_UDT_UPLOAD_SERVERINFO	 = INNER_MSG(7);


#endif /* _JCDBUSMSGDEF_H_ */

