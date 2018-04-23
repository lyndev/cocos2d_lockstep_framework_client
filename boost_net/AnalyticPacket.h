/********************************************************************************
* 模 块 名: 略
* 文 件 名: AnalyticPacket.h
* 关联文件: 
* 功    能: 提供分包功能，约定调用规范
*           添加数据包过程:
*           1. 调用AllocBuff分配用于读取数据的缓存.
*           2. 调用AddData添加到包中.
*           3. 调用Parse解包分析
*           4. IsCompletePack接口返回是否Parse了个包，在Parse后调用
*           5. GetDataAndResetBuf获得数据包，并重置包的数据缓存。不用手动ResetBuf()
*           
*           组合数据包过程:
*           1. 调用GetAdditionalDataLen函数，可获得通讯额外数据的长度，根据此值和外部发送长度
*              可确定接受组合数据缓存的大小，根据该大小外部分配空间。
*           2. 调用Combination函数，传递待组合的数据缓存和接受组合数据的缓存，以进行组合
* 备    注: 组合数据包的内存由外部提供
*           解析数据包相关的内存由解包类提供。支持异步添加数据和读取数据
--------------------------------------------------------------------------------
* 修改记录 :
 日期       版本    作者    内容
 13-03-11   1.0     Joeson  创建 考虑将内存管理及数据队列提到该类中
*******************************************************************************/
#ifndef _COMMUNICATION_ANALYTICPACKETEX_H_
#define _COMMUNICATION_ANALYTICPACKETEX_H_
#define  NET_BYTE_ORDER
#ifdef NET_BYTE_ORDER

#define  QM_HTONL(x) htonl(x)
#define  QM_HTONS(x) htons(x)
#define  QM_NTOHL(x) ntohl(x)
#define  QM_NTOHS(x) ntohs(x)

#else

#define  QM_HTONL(x) (x)
#define  QM_HTONS(x) (x)
#define  QM_NTOHL(x) (x)
#define  QM_NTOHS(x) (x)

#endif 

#include "ObjectBase.h"

enum EPacketType
{
	E_UNKNOWN_PACKET = 0,                              // 未知类型数据包
	E_USE_PACKET,                                      // 用户数据包
	E_ACTIVE_PACKET,                                   // 心跳检测包
};

class  CAnalyticPacket : public CObjectBase
{
public:
    virtual int     GetAdditionalDataLen(EPacketType eType) = 0;                                           // 获得额外数据长度
	virtual int     Combination(EPacketType eType, const char * pData, int nDataLen,
		                        char * & pSendData, int & nSendDataLen) = 0;                               // 发送数据组合包头接口
    virtual bool    IsCacheFull() = 0;                                                                     // 分包缓存是否已满 true为满
    virtual bool    AllocBuff(char *&pBuff, int nBuffLen) = 0;                                             // 分配内存
	virtual int     AddData(char * pRecvData, int nRecvDataLen) = 0;                                       // 添加接收数据接口
    virtual int     Parse() = 0;                                                                           // 解析数据
	virtual bool    IsCompletePack() = 0;                                                                  // 是否是完成数据包
	virtual void    GetDataAndResetBuf(EPacketType & eType, char * & pRecvData, int & nRecvDataLen) = 0;   // 获得完整数据包并重置接收缓冲区
	virtual int     GetNeedDataLen() = 0;                                                                  // 获得当前还需要接收数据的长度(构成一个包)
    virtual int     GetRequestDataLen() = 0;                                                               // 得到每次请求接受数据长度
	virtual void    ResetBuf() = 0;                                                                        // 重置数据接收缓冲区
    virtual void    Reset() = 0;                                                                           // 重置包解析对象
};
#endif