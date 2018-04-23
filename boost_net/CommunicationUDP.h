/**************************************************************
* �ļ���: CCommunicationUDP.h
* ����˵��: ����BOOST���UDPͨ�ſͻ���
* ���ߣ�
* ���ڣ�
* �޸ļ�¼
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

	int  Connect(const char * IP, unsigned int port);                          // �첽���ӷ������ӿں���
	void Disconnect();
	int  ConnectByDomainName(const char * pDomainName, const char * pPort);    // ͨ�������Ͷ˿����ӷ���������
	int  Send(const char * pSendMsg, unsigned int nLen);                       // �첽�������ݽӿں���
	int  Close();                                                              // �ر����ӽӿں���
	void Destroy();                                                            // ����ͨѶ����
	
	static void EndCommnication();                                             // ����ͨ��
	static int  StartCommnication();

	void SetRecvHandle(CObjectBase * pObjectBase, Fun_Comm_PCHAR_SIZE_T fun);  // ���ý������ݴ�����
	void SetAnalyticPacket(CAnalyticPacket *pAnalytic);							// ���ð�������չ����
protected:
	void _SendUDPDataThreadLogic();                                             // ���������߳��߼�
	void _ParseUDPDataThreadLogic();

	void _RequestAsynRead();
	void HandleRead(const boost::system::error_code & error, std::size_t bytes_transferred);
	void _HandleUDPReadLogic(const boost::system::error_code & error, std::size_t bytes_transferred);
	static void IOUDPServeRunThread();                                            // io_service�����̺߳���                                // ���������߳��߼�
	static void SendUDPDataThread(void *pThis);                                 // ���������߳�
	static void ParseUDPDataThread(void *pThis);                                // �������ݰ��߳�
	static void _IOUDPServeRunThreadLogic();                                    // io_service�����߳��߼�

	static boost::thread *			S_pIOUDPRunThread;							// io_service upd�����̶߳���ָ��
	static boost::asio::io_service  S_udp_io_service;                           // BOOST���io_service
	static boost::asio::io_service  S_io_service;                              // BOOST���io_service
	static bool                     S_bStart;
	CObjectBase *					m_pObjectDisRecv;                          // �������ݻص�����ָ��
	CCircularQueue<SSendDataNode>   m_queueSendUDPData;							// �������ݶ���
	udp::endpoint					m_local_endPoint;
	udp::endpoint					m_romote_endpoint;
	udp::socket*					m_udpSocket;								// udpͨ���׽���
	Fun_Comm_PCHAR_SIZE_T           m_funRecv;									// �������ݻص�����
	boost::thread *                 m_pSendUDPDataThread;						// ���������̶߳���ָ��
	boost::thread *                 m_pParseUDPDataThread;						// �������ݰ��߳�
	boost::mutex                    m_mutexSendMsg;								// ������Ϣ���л����塣����������Ϣ���û�Send�ڲ�ͬ�߳�
	CAnalyticPacket*				m_pAnalyticUDPPacket;						// ������������չ�� ֧�ֶ��̷ְ߳�����
	char*                           m_pReadUDPBuffer;                            // ��ȡ���ݵ�Buffָ��
	boost::array<char, UDP_DATA_PACKAGE_MAX_LENGTH> m_recvBuf; //�������ݻ�����
	bool							m_isUDPBlockAsynRead;		
};