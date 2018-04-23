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


	// �������ݽ��ջ�����
	m_pAnalyticUDPPacket->Reset();

	// ����һ���첽�����ݲ���
	_RequestAsynRead();

	//  ����ͨ���߳�
	StartCommnication();

	// �������������߳� 
	if (NULL == m_pSendUDPDataThread)
	{
		m_pSendUDPDataThread = new boost::thread(boost::bind(&CCommunicationUDP::SendUDPDataThread, (void*)this));
		if (NULL == m_pSendUDPDataThread)
		{
			return 5;
		}
	}

	// �����������ݰ��߳�
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
		// ��Ӱ�ͷ�����ڷְ�
		int nHeadAndDataLen = m_pAnalyticUDPPacket->GetAdditionalDataLen(E_USE_PACKET) + nLen;
		char *pHeadAndData = new char[nHeadAndDataLen];
		if (NULL == pHeadAndData)
		{
			CCLOG("CCommunicationUDP::Send Alloc memory failed!");
			return -5;
		}
		// ���
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

	// �ȴ������߳̽��� 
	if (NULL != m_pSendUDPDataThread)
	{
		m_pSendUDPDataThread->interrupt();
		m_pSendUDPDataThread->join();
		delete m_pSendUDPDataThread;
		m_pSendUDPDataThread = NULL;
	}

	// �ȴ��������ݰ��߳̽��� 
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
			// ��¼��־
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

		// �ȴ��߳̽���
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
				//�ͷ��ڴ�            
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

		// δ�������
		if (!m_pAnalyticUDPPacket->IsCompletePack())
		{
			// ˵�������������Ѿ�û�����ݣ��ͷ�һ��cpu��Դ
			boost::this_thread::sleep(boost::posix_time::milliseconds(10));
			continue;
		}

		// ��ȡ���ݰ�
		m_pAnalyticUDPPacket->GetDataAndResetBuf(eType, pBuff, nBuffLen);

		// ���ݲ�ͬ�����ͽ��д���
		switch (eType)
		{
			case E_USE_PACKET:                          // �û����ݰ�            
				SAFE_CALL_Fun_Comm_PCHAR_INT(m_pObjectDisRecv, m_funRecv, this, pBuff, nBuffLen);    // �ص����պ���
				break;

			case E_ACTIVE_PACKET:                       // ��������
				CCLOG("CCommunicationUDP Msg active");
				SAFE_CALL_Fun_Comm_PCHAR_INT(m_pObjectDisRecv, m_funRecv, this, pBuff, nBuffLen);
				break;

			default: break;
		}

		// �Ѿ������첽��ȡ��Ҫ�����첽��ȡ
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
	// ��ȡ�ɹ�
	if (!error)
	{
		// ����µķְ������߼� [13-03-13 Joeson]
		m_pAnalyticUDPPacket->AddData(m_pReadUDPBuffer, bytes_transferred);
		m_pReadUDPBuffer = NULL;

		// ����ڵ�����Ժ󣬻����������������ٴζ�ȡ��������ȡ����
		if (m_pAnalyticUDPPacket->IsCacheFull())
		{
			CCLOG("[_HandleTcpReadLogic] read msg data is full, blocking read");
			m_isUDPBlockAsynRead = true;
		}
		else
		{
			_RequestAsynRead();         // ����һ���첽�����ݲ���
		}
	}
	// ��ȡʧ��
	else
	{
		CCLOG("CCommunicationUDP::_HandleUDPReadLogic read buffer failed!");
		// ��ȡʧ���ͷ��ڴ�
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

// ���ý������ݴ�����                          
void CCommunicationUDP::SetRecvHandle(CObjectBase * pObjectBase, Fun_Comm_PCHAR_SIZE_T fun)
{
	m_funRecv = fun;
	m_pObjectDisRecv = pObjectBase;
}

void CCommunicationUDP::SetAnalyticPacket(CAnalyticPacket *pAnalytic)
{
	m_pAnalyticUDPPacket = pAnalytic;
}