/********************************************************************************
* 版    权: 
* 模 块 名: 略
* 文 件 名: AnalyticPacketClient.h
* 关联文件: AnalyticPacketClient.cpp
* 功    能: 实现通讯客户端包解析
* 备    注: 名词约定:
*           > 数据段: 每次AddData添加的数据被定义为一个数据段
*           > 数据包: 一个完整的通讯数据包
--------------------------------------------------------------------------------
* 修改记录 :
 日期       版本    作者    内容
 13-03-11   1.0     Joeson  创建
*******************************************************************************/
#ifndef _COMMUNICATION_QINMEIANALYTICPACKET_H_
#define _COMMUNICATION_QINMEIANALYTICPACKET_H_

#include <list>
#include <map>
#include "AnalyticPacket.h"
#include "CircularQueue.h"
#include "ZlibParse.h"
#include "boostlibs/boost/date_time/posix_time/posix_time.hpp"

#define LEN_PACK_QUEUE      40000                       // 数据包循环队列长度
#define LEN_BUFF_QUEUE      500                         // 缓存区循环队列长度
#define LEN_REQUEST         8192                        // 每次请求接受数据包长度。和客户端单个包的大小一致
#define LEN_COMMON_CACHE    10240                       // 通用缓存大小


class  CAnalyticPacketClient : public CAnalyticPacket
{
public:
    // 数据包节点
    struct SDataPackNode
    {
        char   *pBuff;                      // 缓存地址
        int     nLen;                       // 数据包长度

        SDataPackNode() : pBuff(NULL), nLen() {}
        SDataPackNode(char *buf, int len) : pBuff(buf), nLen(len) {}
    };
    typedef SDataPackNode SBuffQueueNode;

    // 解析数据段
    struct SParseDataSeg
    {
        char       *pData;                  // 数据地址
        int         nOffset;                // 相对于首地址(pData)偏移
        int         nResidualLen;           // 剩余长度

        SParseDataSeg() : pData(0), nOffset(0), nResidualLen(0) {}
    };

public:
    CAnalyticPacketClient();
    virtual ~CAnalyticPacketClient();

    virtual int     GetAdditionalDataLen(EPacketType eType);                                            // 获得额外数据长度
	virtual int     Combination(EPacketType eType, const char * pData, int nDataLen,
		                        char * & pSendData, int & nSendDataLen);                                // 发送数据组合包头
    virtual bool    IsCacheFull();                                                                      // 分包缓存是否已满 true为满
    virtual bool    AllocBuff(char *&pBuff, int nBuffLen);                                              // 分配内存
	virtual int     AddData(char * pRecvData, int nRecvDataLen);                                        // 添加接收数据
    virtual int     Parse();                                                                            // 解析数据
	virtual bool    IsCompletePack();                                                                   // 是否是完成数据包
	virtual void    GetDataAndResetBuf(EPacketType & eType, char * & pRecvData, int & nRecvDataLen);    // 获得完整数据包并重置接收缓冲区
	virtual int     GetNeedDataLen();                                                                   // 获得当前还需要接收数据的长度(构成一个包)
    virtual int     GetRequestDataLen();                                                                // 得到每次请求接受数据长度

	virtual void    ResetBuf();                                                                         // 重置数据接收缓冲区
    virtual void    Reset() { ResetBuf(); }                                     // 重置包解析

protected:
    bool            _AllocMemoryStrategy(int &nBuffLen, char *&pBuff);          // 分配内存策略
    void            _RetrieveMemoryStrategy(char *pBuff);                       // 回收内存策略

protected:
    typedef CCircularQueue<SDataPackNode, LEN_PACK_QUEUE>   DataQueue;          // 数据段队列
    DataQueue           m_queueData;                                    // 数据段队列

    char               *m_pCurPackCache;                                // 当前包. 通讯头(本地字节顺序)+用户层数据包
    int                 m_nCurPackCacheLen;                             // 当前包内存大小
    int                 m_nReadedLen;                                   // 已从数据段队列中读取的长度
    char                m_szCommonCache[LEN_COMMON_CACHE];              // 通用缓存 m_pCurPackCache默认使用它
    int                 m_nZip;                                         // 是否zip解压
	CZlibParse          m_zlib;                                         // Zlib解压对象

    SParseDataSeg       m_ParseDataSeg;                                 // 当前解析的数据段
};

/* 
 * [15-12-22 Unda.k]
 * 新的包解析对象。增加验证码
 * 和服务器对接新的协议
 * 新增加记录: 增加一个简单的组包接口 CombinaionSimple() 总长度 + 用户长度 + （用于存在用户长的的字节+用户数据字节）长度
 */
class CAnalyticPacketClient_IDCode : public CAnalyticPacketClient
{
public:
    CAnalyticPacketClient_IDCode() : m_uSendMsgCounter(0), m_nServStartTime(0) {}

    virtual int     GetAdditionalDataLen(EPacketType eType);                                            // 获得额外数据长度
	virtual int     Combination(EPacketType eType, const char * pData, int nDataLen,
		                        char * & pSendData, int & nSendDataLen);                                // 发送数据组合包头
	virtual int	   CombinationSimple( EPacketType eType, char * & pSendData,  const char * pData, int nDataLen);				// 发送数据包（简单版本接口）
	virtual void    GetDataAndResetBuf(EPacketType & eType, char * & pRecvData, int & nRecvDataLen);    // 获得完整数据包并重置接收缓冲区

    virtual void    Reset();                                    // 重置包解析

    void            SetServStartTime(int startTime) { if(m_timeServSyncTime.is_not_a_date_time()) m_timeServSyncTime = boost::posix_time::from_time_t(startTime); } // 同步时间

private:
    unsigned  int   _GenIdentifyCode(unsigned int counter, unsigned int packLen);     // 生成认证码

private:
    unsigned int                m_uSendMsgCounter;                          // 客户端发送包计数
    int                         m_nServStartTime;                           // 记录服务器同步过来的服务器启动时间 毫秒
    boost::posix_time::ptime    m_timeServSyncTime;                         // 服务器同步时间时的时间
};

#endif