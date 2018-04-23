/**************************************************************
 * 文件名: CommunicationC.h
 * 功能说明: 基于BOOST库的通信客户端
 * 作者：
 * 日期：2012.7.5
 * 修改记录
 * --------------------------------------------------------------------------------
 * 修改记录 :
  日期       版本    作者    内容
  13-03-13   2.0     Joeson  发送消息和解析消息在单独的2个线程完成
  13-03-14   2.0     Joeson  当解包对象中的缓存已满时，不请求异步读取，待缓存缓解后再读取
  13-04-01   2.0     Joeson  添加__try __except异常检测。及相关函数调整
 **************************************************************/

#pragma once

// 由于window平台下，asio.hpp和windows.h文件冲突，所以需要将
// <boost/thread/thread.hpp>包含在#include <windows.h>之前,故有以下特殊修改

#ifndef _INCLUDE_BOOST_ASIO_HPP
#define _INCLUDE_BOOST_ASIO_HPP
#include <boostlibs/boost/asio.hpp>
#include <boostlibs/boost/thread/thread.hpp>
#endif

#include <iostream>
#include "CircularQueue.h"
#include "ICommunication.h"
#include "ObjectBase.h"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;
using namespace boost::asio;
using namespace std;

class CCommunicationC;

int const LEN_SENDQUEUE      = 40000;           // 发送数据队列长度
int const MAX_SENDBUFF_LEN   = 8192;            // 单次发送数据缓存最大长度

class CAnalyticPacket;
class CCommunicationC : public ICommunication, public CObjectBase
{
private:
	enum ENetState
	{
		E_NET_NO_INIT = 0,                                  // 连接未初始化状态
		E_NET_CONECTING,                                    // 正在建立建立状态
		E_NET_CONECT,                                       // 连接状态
		E_NET_CLOSED,                                       // 连接断开状态
	};

public:
	CCommunicationC(void);
	virtual ~CCommunicationC(void);
    
public:
	static void EndCommnication();                                             // 结束通信
	static int  StartCommnication();                                           // 启用通信

	int  Connect(const char * IP, unsigned int port);                          // 异步连接服务器接口函数
	int  ConnectByDomainName(const char * pDomainName, const char * pPort);    // 通过域名和端口连接服务器函数
	int  Send(const char * pSendMsg, unsigned int nLen);                       // 异步发送数据接口函数
	int  Close();                                                              // 关闭连接接口函数
    void Destroy();                                                            // 销毁通讯对象

	void SetActiveTime(unsigned int seconds);                                  // 设置心跳检测时间
	void SetConnectHandle(CObjectBase * pObjectBase, Fun_Comm_INT fun);        // 设置连接完成回调函数
	void SeDistConnectHandle(CObjectBase * pObjectBase, Fun_Comm_INT fun);     // 设置断开连接回调处理函数
	void SetRecvHandle(CObjectBase * pObjectBase, Fun_Comm_PCHAR_SIZE_T fun);  // 设置接收数据处理函数
    void SetSendSuccessHandle(CObjectBase * pObjectBase, Fun_Comm_INT fun);    // 设置数据发送成功回调处理函数
    void SetAnalyticPacket(CAnalyticPacket *pAnalytic);							// 设置包解析扩展对象

protected:
	static void IOServeRunThread();                                            // io_service运行线程函数
    static void SendTCPDataThread(void *pThis);                                   // 发送数据线程
    static void ParseTCPDataThread(void *pThis);                                  // 解析数据包线程


protected:
	void HandleConnect(const boost::system::error_code & error);               // 异步连接完成的回调处理函数
	void HandleRead(const boost::system::error_code & error, 
		            std::size_t bytes_transferred);                            // 异步读取数据完成的回调处理函数
	void ActiveCheck(const boost::system::error_code& error);                  // 心跳检测函数
    void _RequestAsynRead();                                                   // 请求一次异步读取
    void _SendTCPDataThreadLogic();                                               // 发送数据线程逻辑
    void _ParseTCPDataThreadLogic();                                              // 解析数据线程逻辑

    static void _IOServeRunThreadLogic();                                      // io_service运行线程逻辑
    void _HandleConnectLogic(const boost::system::error_code & error);         // 异步连接完成逻辑
    void _HandleTcpReadLogic(const boost::system::error_code & error, 
		                  std::size_t bytes_transferred);                      // 异步读取完成逻辑
	

protected:
    static boost::asio::io_service  S_io_service;                              // BOOST库的io_service
	static bool                     S_bStart;                                  // 启动通信标志
	static boost::thread *          S_pIOTCPRunThread;                            // io_service运行线程对象指针]

protected:
	tcp::socket                     m_socket;                                  // 通信套接字
	Fun_Comm_INT                    m_funConnect;                              // 连接完成回调函数
	Fun_Comm_INT                    m_funDisConnect;                           // 连接断开回调函数
	Fun_Comm_PCHAR_SIZE_T           m_funRecv;                                 // 接收数据回调函数
    Fun_Comm_INT                    m_funSendSuccess;                          // 发送数据成功回调函数
	CObjectBase *					m_pObjectConnect;                          // 连接完成回调对象指针
	CObjectBase *					m_pObjectDisConnect;                       // 连接断开回调对象指针
	CObjectBase *					m_pObjectDisRecv;                          // 接收数据回调对象指针
	CObjectBase *					m_pObjectSendSuccess;                      // 发送数据成功回调对象指针
	boost::asio::deadline_timer     m_timer;                                   // 计时器
	ENetState                       m_eNetState;                               // 网络状态
	tcp::endpoint                   m_endPoint;                                // 服务端IP和端口
	unsigned  int                   m_nActiveTime;                             // 心跳检测间隔时间
	unsigned  short                 m_nRecvActivPacket;                        // 收到的心跳检测包数
    boost::thread *                 m_pSendTCPDataThread;                         // 发送数据线程对象指针
    boost::thread *                 m_pParseTCPDataThread;                        // 解析数据包线程
    boost::mutex                    m_mutexSendMsg;                            // 发送消息队列互斥体。发送心跳消息、用户Send在不同线程

    // 发送数据相关变量
    CCircularQueue<SSendDataNode>   m_queueSendTCPData;                        // 发送数据队列

    char                            m_szTCPSendBuffer[MAX_SENDBUFF_LEN];          // 发送数据缓存


    CAnalyticPacket*              m_pAnalyticTcpPacket;                       // 包解析对象扩展版 支持多线程分包对象
    char                            *m_pReadTcpBuffer;                            // 读取数据的Buff指针
    bool                            m_isTcpBlockAsynRead;                         // 是否阻塞了异步读取。背景:在读取数据时发现队列
																				  // 满了，要阻塞起来，待解析了再恢复

	// add by unda.k 2016-5-26
	clock_t							m_lastRecvTime;
};
