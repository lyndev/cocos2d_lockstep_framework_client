/**************************************************************
* 文件名: CCommunicationUDP.h
* 功能说明: 基于BOOST库的UDP通信客户端
* 作者：
* 日期：
* 修改记录
**************************************************************/
#pragma once

#ifndef _INCLUDE_BOOST_ASIO_HPP
	#define _INCLUDE_BOOST_ASIO_HPP
	#include <boostlibs/boost/asio.hpp>
	#include <boostlibs/boost/thread/thread.hpp>
	#include <boost/array.hpp> 
#endif

#include "CircularQueue.h"
#include "ICommunication.h"
#include "AnalyticPacket.h"

using boost::asio::ip::udp;
using namespace boost::asio;
using namespace std; 
#define UDP_DATA_PACKAGE_MAX_LENGTH 1024
class CCommunicationUDP : public ICommunication, public CObjectBase
{
public:
	CCommunicationUDP(void);
	virtual ~CCommunicationUDP(void);

	int  Connect(const char * IP, unsigned int port);                          // 异步连接服务器接口函数
	void Disconnect();
	int  ConnectByDomainName(const char * pDomainName, const char * pPort);    // 通过域名和端口连接服务器函数
	int  Send(const char * pSendMsg, unsigned int nLen);                       // 异步发送数据接口函数
	int  Close();                                                              // 关闭连接接口函数
	void Destroy();                                                            // 销毁通讯对象
	
	static void EndCommnication();                                             // 结束通信
	static int  StartCommnication();

	void SetRecvHandle(CObjectBase * pObjectBase, Fun_Comm_PCHAR_SIZE_T fun);  // 设置接收数据处理函数
	void SetAnalyticPacket(CAnalyticPacket *pAnalytic);							// 设置包解析扩展对象
protected:
	void _SendUDPDataThreadLogic();                                             // 发送数据线程逻辑
	void _ParseUDPDataThreadLogic();

	void _RequestAsynRead();
	void HandleRead(const boost::system::error_code & error, std::size_t bytes_transferred);
	void _HandleUDPReadLogic(const boost::system::error_code & error, std::size_t bytes_transferred);
	static void IOUDPServeRunThread();                                            // io_service运行线程函数                                // 解析数据线程逻辑
	static void SendUDPDataThread(void *pThis);                                 // 发送数据线程
	static void ParseUDPDataThread(void *pThis);                                // 解析数据包线程
	static void _IOUDPServeRunThreadLogic();                                    // io_service运行线程逻辑

	static boost::thread *			S_pIOUDPRunThread;							// io_service upd运行线程对象指针
	static boost::asio::io_service  S_udp_io_service;                           // BOOST库的io_service
	static boost::asio::io_service  S_io_service;                              // BOOST库的io_service
	static bool                     S_bStart;
	CObjectBase *					m_pObjectDisRecv;                          // 接收数据回调对象指针
	CCircularQueue<SSendDataNode>   m_queueSendUDPData;							// 发送数据队列
	udp::endpoint					m_local_endPoint;
	udp::endpoint					m_romote_endpoint;
	udp::socket*					m_udpSocket;								// udp通信套接字
	Fun_Comm_PCHAR_SIZE_T           m_funRecv;									// 接收数据回调函数
	boost::thread *                 m_pSendUDPDataThread;						// 发送数据线程对象指针
	boost::thread *                 m_pParseUDPDataThread;						// 解析数据包线程
	boost::mutex                    m_mutexSendMsg;								// 发送消息队列互斥体。发送心跳消息、用户Send在不同线程
	CAnalyticPacket*				m_pAnalyticUDPPacket;						// 包解析对象扩展版 支持多线程分包对象
	char*                           m_pReadUDPBuffer;                            // 读取数据的Buff指针
	boost::array<char, UDP_DATA_PACKAGE_MAX_LENGTH> m_recvBuf; //接收数据缓冲区
	bool							m_isUDPBlockAsynRead;		
};