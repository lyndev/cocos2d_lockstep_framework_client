#pragma once
#include "ObjectBase.h"

#define SAFE_CALL_Fun_Comm_INT(pObjectBase, fun, pComm, nRet) \
	if (pObjectBase && fun) \
{\
	(pObjectBase->*fun)(pComm, (nRet));\
}\

#define SAFE_CALL_Fun_Comm_PCHAR_INT(pObjectBase, fun, pComm, pRecvBuf, nLen) \
	if (pObjectBase && fun) \
{\
	(pObjectBase->*fun)(pComm, pRecvBuf, (nLen));\
}\

#ifndef MIN
#define MIN(a, b)  ((a) > (b) ? (b) : (a))
#endif

#define SAFE_DELETE_BUFF(ptr, len)   if (NULL != ptr) { delete []ptr; ptr = NULL; }

class ICommunication;
typedef void (CObjectBase::*Fun_Comm_INT)(ICommunication *, int);
typedef void (CObjectBase::*Fun_Comm_PCHAR_SIZE_T)(ICommunication *, char *, size_t);

class ICommunication
{
public:
	// 发送数据的节点
	struct SSendDataNode
	{
		char       *pBuff;                  // 数据包
		int         nDataLen;               // 数据长度

		SSendDataNode() : pBuff(0), nDataLen(0) {}
		SSendDataNode(char *buf, int len) : pBuff(buf), nDataLen(len) {}
	};

	virtual int  Connect(const char * IP, unsigned int port) = 0;                          // 异步连接服务器接口函数
	virtual int  ConnectByDomainName(const char * pDomainName, const char * pPort) = 0;    // 通过域名和端口连接服务器函数
	virtual int  Send(const char * pSendMsg, unsigned int nLen) = 0;                       // 异步发送数据接口函数
	virtual int  Close() = 0;                                                              // 关闭连接接口函数
	virtual void Destroy() = 0;							// 销毁通讯对象
};