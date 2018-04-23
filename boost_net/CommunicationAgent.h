/********************************************************************
文 件 名: Classes\SDK\Boost_Net\CommunicationAgent.h
作    者: 
版    本: 1.0
创建日期: 2014/03/28
功能描述: 通信代理类，公共接口用于导出到lua中使用
修改记录:
*********************************************************************/
#pragma once
#include "AnalyticPacketClient.h"
#include "CommunicationC.h"
#include "DataBuff.h"
#include "ProducerAndConsumer.h"
#include "CommunicationUDP.h"

USING_NS_CC;

class CCommunicationAgent:CObjectBase
{
public:
    static CCommunicationAgent* Create();																// 静态创建方法
    static CCommunicationAgent* GetInstance(){ return m_pInstance; }									// 获取主单例
	void   Destroy();
    CCommunicationAgent(void){m_pInstance = this;};
	virtual		~CCommunicationAgent(void);
    void        SendMsg(unsigned int nMsgId, const char* pData, size_t nLen);							// 发送消息到本地消息队列
	void        SendMsgPriority( unsigned int nMsgId, const char* pData, size_t nLen, bool bHigh);		// 发送消息到本地消息队列,带优先级
    int         SendMsgToServer(unsigned int nMsgId, const char* pData, size_t nLen);					// 发送数据到服务器接口
	int         SendUDPMsgToServer(unsigned int nMsgId, const char* pData, size_t nLen);					// 发送数据到服务器接口
    int         Connect(const char* IP, unsigned int port);												// 连接服务器
	int         ConnectUDP(const char* IP, unsigned int port);												// 连接服务器
    int         DisConnect();																			// 断开连接
    void        GetMsg();																				// 获得消息
    void		setStartTime(int startTime);															// 设置登录时间（服务器对时用）
	void        SetAnalyticPacket(int id);																// 设置分包对象（1、代理，2、游戏服）
protected:
    bool        Init();																					// 初始化函数
    void        ConnectCallBack(CCommunicationC* pComm, int nRet);										// 连接回调函数
    void        RecvTcpDataCallBack(CCommunicationC* pComm, char* pData, size_t nLen);							// 网络数据接收回调函数
	void        RecvUDPDataCallBack(CCommunicationUDP* pComm, char* pData, size_t nLen);							// 网络数据接收回调函数
    void        DistCallBack(CCommunicationC* pComm, int nRet);											// 断开连接的回调函数
    void        SendSuccessCallBack(CCommunicationC* pComm, int nRet);									// 服务器消息发送成功回调函数
    void        MessageProc(unsigned int nMsgId, const char* pData, size_t nLen);						// 消息传入lua
private:
    CCommunicationC                     m_communicationTCP;												// tcp通信对象
	CCommunicationUDP                  m_communicationUDP;												// udp通信对象
	CProducerAndConsumer                m_tcpProducerAndConsumer;										// tcp生产者、消费者模型
	CProducerAndConsumer                m_udpProducerAndConsumer;										// udp生产者、消费者模型

    static CCommunicationAgent*         m_pInstance;													// 单例
    CAnalyticPacketClient_IDCode*       m_tcpAnalyticPacketObject;										// 游戏服务器tcp分包对象
	CAnalyticPacketClient_IDCode*       m_udpAnalyticPacketObject;										// 游戏服务器udp分包对象
	CAnalyticPacketClient*              mProxyAnalyticPacketObj;										// 代理服务器分包对象
};