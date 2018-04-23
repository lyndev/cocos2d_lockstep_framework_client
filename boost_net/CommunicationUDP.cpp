#include "CommunicationUDP.h"
#include "cocos2d.h"

using namespace cocos2d;
using namespace boost::asio;

bool CCommunicationUDP::S_bStart = false;
boost::asio::io_service  CCommunicationUDP::S_udp_io_service;
boost::thread * CCommunicationUDP::S_pIOUDPRunThread = NULL;

CCommunicationUDP::CCommunicationUDP(void):
m_udpSocket(NULL),
m_pSendUDPDataThread(NULL),
m_pParseUDPDataThread(NULL)
{
	m_funRecv = NULL;
	m_pObjectDisRecv = NULL;
}

CCommunicationUDP::~CCommunicationUDP(void)
{
	Destroy();
}

int CCommunicationUDP::Connect(const char * IP, unsigned int port)
{
	try
	{
		udp::endpoint local_endpoint(ip::udp::v4(), port);
		m_udpSocket = new udp::socket(S_udp_io_service, local_endpoint);
		udp::endpoint remote_endpoint(ip::address_v4::from_string("127.0.0.1"), 7777);
		m_local_endPoint = local_endpoint;
		m_romote_endpoint = remote_endpoint;;

	}
	catch (std::exception& e)//to get the error when sending 
	{
		std::cerr << e.what() << std::endl;
	}


	// 重置数据接收缓冲区
	m_pAnalyticUDPPacket->Reset();

	// 请求一次异步读数据操作
	_RequestAsynRead();

	//  开启通信线程
	StartCommnication();

	// 创建发送数据线程 
	if (NULL == m_pSendUDPDataThread)
	{
		m_pSendUDPDataThread = new boost::thread(boost::bind(&CCommunicationUDP::SendUDPDataThread, (void*)this));
		if (NULL == m_pSendUDPDataThread)
		{
			return 5;
		}
	}

	// 创建解析数据包线程
	if (NULL == m_pParseUDPDataThread)
	{
		m_pParseUDPDataThread = new boost::thread(boost::bind(&CCommunicationUDP::ParseUDPDataThread, (void*)this));
		if (NULL == m_pParseUDPDataThread)
		{
			return 6;
		}
	}
	
	return 0;
}

void Disconnect()
{

}

int CCommunicationUDP::ConnectByDomainName(const char * pDomainName, const char * pPort)
{
	return 0;
}

int CCommunicationUDP::Send(const char * pSendMsg, unsigned int nLen)
{
	try
	{
		// 添加包头，用于分包
		int nHeadAndDataLen = m_pAnalyticUDPPacket->GetAdditionalDataLen(E_USE_PACKET) + nLen;
		char *pHeadAndData = new char[nHeadAndDataLen];
		if (NULL == pHeadAndData)
		{
			CCLOG("CCommunicationUDP::Send Alloc memory failed!");
			return -5;
		}
		// 组包
		boost::unique_lock<boost::mutex> lock(m_mutexSendMsg);
		int nRet = m_pAnalyticUDPPacket->Combination(E_USE_PACKET, pSendMsg, nLen, pHeadAndData, nHeadAndDataLen);
		if (0 == nRet)
		{
			SSendDataNode node(pHeadAndData, nHeadAndDataLen);
			bool bRet = m_queueSendUDPData.Push(node);
			if (!bRet)
			{
				SAFE_DELETE_BUFF(pHeadAndData, nHeadAndDataLen);
				CCLOG(" CCommunicationUDP::Send Push node failed!");
				return -4;
			}
		}
		else
		{
			delete[]pHeadAndData;
		}
	}
	catch (std::exception& e)//to get the error when sending 
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}

int CCommunicationUDP::Close()
{
	m_udpSocket->close();
	return 0;
}

void CCommunicationUDP::Destroy()
{
	Close();

	// 等待发送线程结束 
	if (NULL != m_pSendUDPDataThread)
	{
		m_pSendUDPDataThread->interrupt();
		m_pSendUDPDataThread->join();
		delete m_pSendUDPDataThread;
		m_pSendUDPDataThread = NULL;
	}

	// 等待解析数据包线程结束 
	if (NULL != m_pParseUDPDataThread)
	{
		m_pParseUDPDataThread->interrupt();
		m_pParseUDPDataThread->join();
		delete m_pParseUDPDataThread;
		m_pParseUDPDataThread = NULL;
	}
}


int CCommunicationUDP::StartCommnication()
{
	if (false == S_bStart)
	{
		S_pIOUDPRunThread = new boost::thread(&CCommunicationUDP::IOUDPServeRunThread);
		if (NULL == S_pIOUDPRunThread)
		{
			// 记录日志
			CCLOG("CCommunicationUDP::StartCommnication: S_pIOUDPRunThread start IOrun thread failed");

			return 0;
		}
		S_bStart = true;
	}

	return 1;
}

void CCommunicationUDP::EndCommnication()
{
	if (true == S_bStart)
	{
		S_bStart = false;
		S_udp_io_service.stop();

		// 等待线程结束
		S_pIOUDPRunThread->interrupt();
		S_pIOUDPRunThread->join();

		delete S_pIOUDPRunThread;
		S_pIOUDPRunThread = NULL;
	}
}


void CCommunicationUDP::SendUDPDataThread(void * pThis)
{
	((CCommunicationUDP*)pThis)->_SendUDPDataThreadLogic();
}

void CCommunicationUDP::ParseUDPDataThread(void * pThis)
{
	((CCommunicationUDP*)pThis)->_ParseUDPDataThreadLogic();
}

void CCommunicationUDP::_SendUDPDataThreadLogic()
{
	
	while (S_bStart) {
		while (!m_queueSendUDPData.IsEmpty()) {
			SSendDataNode node = m_queueSendUDPData.GetFront();
			m_queueSendUDPData.Pop();
			try
			{
				m_udpSocket->send_to(buffer(node.pBuff, node.nDataLen), m_romote_endpoint);
				CCLOG("CCommunicationUDP::_SendUDPDataThreadLogic -> send udp data  data len:%d", node.nDataLen);
				//释放内存            
				SAFE_DELETE_BUFF(node.pBuff, node.nDataLen);
			}
			catch (std::exception& e)//to get the error when sending 
			{
				std::cerr << e.what() << std::endl;
			}
			
			break;
		}
		if (m_queueSendUDPData.IsEmpty()) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(10));
		}
	}
	
}

void CCommunicationUDP::_ParseUDPDataThreadLogic()
{
	EPacketType eType;
	char *pBuff = NULL;
	int   nBuffLen = 0;
	while (S_bStart)
	{
		m_pAnalyticUDPPacket->Parse();

		// 未解析完成
		if (!m_pAnalyticUDPPacket->IsCompletePack())
		{
			// 说明解析对象中已经没有数据，释放一次cpu资源
			boost::this_thread::sleep(boost::posix_time::milliseconds(10));
			continue;
		}

		// 读取数据包
		m_pAnalyticUDPPacket->GetDataAndResetBuf(eType, pBuff, nBuffLen);

		// 根据不同的类型进行处理
		switch (eType)
		{
			case E_USE_PACKET:                          // 用户数据包            
				SAFE_CALL_Fun_Comm_PCHAR_INT(m_pObjectDisRecv, m_funRecv, this, pBuff, nBuffLen);    // 回调接收函数
				break;

			case E_ACTIVE_PACKET:                       // 心跳检测包
				CCLOG("CCommunicationUDP Msg active");
				SAFE_CALL_Fun_Comm_PCHAR_INT(m_pObjectDisRecv, m_funRecv, this, pBuff, nBuffLen);
				break;

			default: break;
		}

		// 已经阻塞异步读取，要触发异步读取
		if (m_isUDPBlockAsynRead && !m_pAnalyticUDPPacket->IsCacheFull())
		{
			m_isUDPBlockAsynRead = false;
			_RequestAsynRead();
		}
	}
}

void CCommunicationUDP::HandleRead(const boost::system::error_code & error, std::size_t bytes_transferred) {
	_HandleUDPReadLogic(error, bytes_transferred);
}

void CCommunicationUDP::_HandleUDPReadLogic(const boost::system::error_code & error, std::size_t bytes_transferred)
{
	// 读取成功
	if (!error)
	{
		// 添加新的分包对象逻辑 [13-03-13 Joeson]
		m_pAnalyticUDPPacket->AddData(m_pReadUDPBuffer, bytes_transferred);
		m_pReadUDPBuffer = NULL;

		// 这个节点添加以后，缓存已满，不触发再次读取，阻塞读取数据
		if (m_pAnalyticUDPPacket->IsCacheFull())
		{
			CCLOG("[_HandleTcpReadLogic] read msg data is full, blocking read");
			m_isUDPBlockAsynRead = true;
		}
		else
		{
			_RequestAsynRead();         // 请求一次异步读数据操作
		}
	}
	// 读取失败
	else
	{
		CCLOG("CCommunicationUDP::_HandleUDPReadLogic read buffer failed!");
		// 读取失败释放内存
		SAFE_DELETE_BUFF(m_pReadUDPBuffer, m_pAnalyticUDPPacket->GetRequestDataLen());
	}
}

void  CCommunicationUDP::_RequestAsynRead()
{
	int nBuffLen = m_pAnalyticUDPPacket->GetRequestDataLen();
	if (!m_pAnalyticUDPPacket->AllocBuff(m_pReadUDPBuffer, nBuffLen))
	{
		CCLOG("CCommunicationUDP::_RequestAsynRead AllocBuff failed!");
		return;
	}

	m_udpSocket->async_receive_from(
			boost::asio::buffer(m_pReadUDPBuffer, nBuffLen),
			m_romote_endpoint,
			boost::bind(&CCommunicationUDP::HandleRead,
			this, 
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
	);
}

void CCommunicationUDP::IOUDPServeRunThread()
{
	_IOUDPServeRunThreadLogic();
}

void CCommunicationUDP::_IOUDPServeRunThreadLogic()
{
	boost::asio::io_service::work work(S_udp_io_service);
	S_udp_io_service.run();
}

// 设置接收数据处理函数                          
void CCommunicationUDP::SetRecvHandle(CObjectBase * pObjectBase, Fun_Comm_PCHAR_SIZE_T fun)
{
	m_funRecv = fun;
	m_pObjectDisRecv = pObjectBase;
}

void CCommunicationUDP::SetAnalyticPacket(CAnalyticPacket *pAnalytic)
{
	m_pAnalyticUDPPacket = pAnalytic;
}