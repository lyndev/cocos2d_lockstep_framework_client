#include "CommunicationC.h"
#include "AnalyticPacket.h"
#include <boost/thread/locks.hpp>
#include "cocos2d.h"

using namespace cocos2d;

bool CCommunicationC::S_bStart = false;
boost::asio::io_service  CCommunicationC::S_io_service;
boost::thread * CCommunicationC::S_pIOTCPRunThread = NULL; 

CCommunicationC::CCommunicationC(void): 
m_socket(S_io_service), 
m_timer(S_io_service, boost::posix_time::seconds(10)),
m_pReadTcpBuffer(0), 
m_isTcpBlockAsynRead(false), 
m_pSendTCPDataThread(NULL), 
m_pParseTCPDataThread(NULL)
{
	// 说明 由于时间关系，这里硬编码为CQiMenPacket，
	// 从良好的设计角度讲，应该用工厂方法修改这里。
	m_funConnect = NULL;
	m_funDisConnect = NULL;
	m_funRecv = NULL;
    m_funSendSuccess = NULL;
	m_pObjectConnect = NULL;
	m_pObjectDisConnect = NULL;
	m_pObjectDisRecv = NULL;
    m_pObjectSendSuccess = NULL;
	m_eNetState = E_NET_NO_INIT;
	m_nActiveTime = 0;
	m_nRecvActivPacket = 1;
	m_lastRecvTime = 0;

    m_queueSendTCPData.SetLength(LEN_SENDQUEUE);
	SetAnalyticPacket(NULL);
}

CCommunicationC::~CCommunicationC(void)
{
    Destroy();
}

/***********************************************************************************
* 函数原型: int CCommunicationC::Connect(const char * IP, int port)
* 函数功能: 异步连接服务器接口函数
* 参数：
*  ［IN］
*       char * IP: 服务器IP
*       unsigned int port： 服务器端口
* ［OUT］: 无
* 返回值: 
*        0: 标示连接指定IP和端口成功
*        1: 连接已经建立，且是指定的IP和端口
*        2: 连接已经建立，但不是指定的IP或端口
*        3: 正在建立连接，且是指定的IP和端口
*        4: 正在建立连接，但不是指定的IP或端口
*        5: 创建发送线程失败
*        6: 创建数据解析包线程失败
************************************************************************************/
int CCommunicationC::Connect(const char * IP, unsigned int port)
{
    //boost::asio::ip::address ip_address = boost::asio::ip::address::from_string(IP);
//    if (ip_address.is_v4())
//    {
//        printf("----This Is IPV4---");
//    }
//    if (ip_address.is_v6())
//    {
//        printf("----This Is IPV6---");
//        
//    }
	//tcp::endpoint endpoint(ip_address, port);

	tcp::endpoint endpoint(boost::asio::ip::address_v4::from_string(IP), port);

	// 状态判定
	if (E_NET_CONECT == m_eNetState)
	{
		// 如果当前连接是指定IP和端口
		if (m_endPoint == endpoint)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else if (E_NET_CONECTING == m_eNetState)
	{
		// 如果当前连接是指定IP和端口
		if (m_endPoint == endpoint)
		{
			return 3;
		}
		else
		{
			return 4;
		}
	}
	
	m_endPoint = endpoint;
	m_eNetState = E_NET_CONECTING;
    m_pAnalyticTcpPacket->Reset();

	// 异步连接服务器
	m_socket.async_connect(m_endPoint, boost::bind(&CCommunicationC::HandleConnect, this, boost::asio::placeholders::error));


    //  开启通信线程
    StartCommnication();

    // 创建发送数据线程 [13-03-13 Joeson]
    if (NULL == m_pSendTCPDataThread)
    {
        m_pSendTCPDataThread = new boost::thread(boost::bind(&CCommunicationC::SendTCPDataThread, (void*)this));
        if (NULL == m_pSendTCPDataThread)
        {
            return 5;
        }
    }

    // 创建解析数据包线程 [13-03-13 Joeson]
    if (NULL == m_pParseTCPDataThread)
    {
        m_pParseTCPDataThread = new boost::thread(boost::bind(&CCommunicationC::ParseTCPDataThread, (void*)this));
        if (NULL == m_pParseTCPDataThread)
        {
            return 6;
        }
    }

	return 0;
}

/***********************************************************************************
* 函数原型: int  CCommunicationC::ConnectByDomainName(const char * pDomainName, const char * pPort)
* 函数功能: 通过域名和端口连接服务器函数
* 参数：
*  ［IN］
*       const char * pDomainName: 服务器域名
*       const char * pPort： 服务器端口
* ［OUT］: 无
* 返回值: 
*        0: 标示连接指定IP和端口成功
*        1: 连接已经建立
*        2: 未解析到关联的地址信息 (无网络)
*        3: 域名不存在
*        4: 未知的域名解析错误
************************************************************************************/
int  CCommunicationC::ConnectByDomainName(const char * pDomainName, const char * pPort)
{
	// 状态判定
	if (E_NET_CONECT == m_eNetState)
	{
		return 1;
	}

	ip::tcp::resolver res(S_io_service);
	ip::tcp::resolver::query query(pDomainName, pPort);
    boost::system::error_code error;
    ip::tcp::resolver::iterator itr_endpoint;
    // try resolve函数,避免崩溃
    try
    {
	    itr_endpoint = res.resolve(query, error);
    }
    catch (...) {}

    if (!error) 
    { /* 无错误*/ }
    // 未解析到关联的地址信息
    else if (boost::asio::error::no_data == error)
    {
        return 2;
    }    
    // 域名不存在
    else if (boost::asio::error::host_not_found == error)
    {
        return 3;
    }
    else
    {
       CCLOG("unknow realm name error:[%d]", error.value());
        return 4;
    }

	ip::tcp::resolver::iterator itr_end; 
	for(; itr_endpoint!=itr_end; ++itr_endpoint)
	{
		// m_socket.close();
		m_endPoint = *itr_endpoint;
		m_eNetState = E_NET_CONECTING;

		// 异步连接服务器
		m_socket.async_connect(m_endPoint, boost::bind(&CCommunicationC::HandleConnect, this, boost::asio::placeholders::error));
	}

	// 开启通信线程
	StartCommnication();

    // 创建发送数据线程 [13-03-13 Joeson]
    if (NULL == m_pSendTCPDataThread)
    {
        m_pSendTCPDataThread = new boost::thread(boost::bind(&CCommunicationC::SendTCPDataThread, (void*)this));
        if (NULL == m_pSendTCPDataThread)
        {
            return 5;
        }
    }

        // 创建解析数据包线程 [13-03-13 Joeson]
    if (NULL == m_pParseTCPDataThread)
    {
        m_pParseTCPDataThread = new boost::thread(boost::bind(&CCommunicationC::ParseTCPDataThread, (void*)this));
        if (NULL == m_pParseTCPDataThread)
        {
            return 6;
        }
    }

	int nError = 0;
	
	return nError;
}

/***********************************************************************************
* 函数原型: int CCommunicationC::Close()
* 函数功能: 关闭连接接口函数
* 参数：
*  ［IN］: 无
* ［OUT］: 无
* 返回值: 
*        0: 永远返回0，标示关闭成功
************************************************************************************/
int  CCommunicationC::Close()
{
	if (E_NET_CLOSED != m_eNetState && E_NET_NO_INIT != m_eNetState)
	{
		// 关闭连接
		m_eNetState = E_NET_CLOSED;
		m_socket.close();
	}

	return 0;
}

/********************************************************************************
* 功  能: 销毁通讯对象
* 参  数: 无
* 返回值: 无
--------------------------------------------------------------------------------
* 修改记录 :
 日期       版本    作者    内容
 13-03-19   1.0     Joeson  创建
*******************************************************************************/
void  CCommunicationC::Destroy()
{
    Close();

    // 等待发送线程结束 [13-03-13 Joeson]
    if (NULL != m_pSendTCPDataThread)
    {
		m_pSendTCPDataThread->interrupt();
        m_pSendTCPDataThread->join();
        //delete m_pSendTCPDataThread;
        //m_pSendTCPDataThread = NULL;
    }    

    // 等待解析数据包线程结束 [13-03-13 Joeson]
    if (NULL != m_pParseTCPDataThread)
    {
		m_pParseTCPDataThread->interrupt();
        m_pParseTCPDataThread->join();
        //delete m_pParseTCPDataThread;
        //m_pParseTCPDataThread = NULL;
    }
    
    // 释放各内存
    if (NULL != m_pReadTcpBuffer && NULL != m_pAnalyticTcpPacket)
    {
        SAFE_DELETE_BUFF(m_pReadTcpBuffer, m_pAnalyticTcpPacket->GetRequestDataLen());
    }
	if (NULL!= m_pAnalyticTcpPacket)
	{
		delete m_pAnalyticTcpPacket;
		m_pAnalyticTcpPacket = NULL;
	}
    SSendDataNode node;
    while (!m_queueSendTCPData.IsEmpty())
    {
        node = m_queueSendTCPData.GetFront();
        SAFE_DELETE_BUFF(node.pBuff, node.nDataLen);
        m_queueSendTCPData.Pop();
    }
}

/**********************************************************************************
* 函数原型: void HandleConnect(const boost::system::error_code& error)
* 函数功能: 异步连接完成的回调处理函数
* 参数：
*  ［IN］
*       const boost::system::error_code& error: 错误类型
* ［OUT］: 无
* 返回值: 
************************************************************************************/
void CCommunicationC::HandleConnect(const boost::system::error_code & error)
{
	_HandleConnectLogic(error);
}

/********************************************************************************
* 功  能: 异步连接完成逻辑
* 参  数: [IN] error: 错误码
* 返回值: 无
--------------------------------------------------------------------------------
* 修改记录 :
 日期       版本    作者    内容
 13-04-01   1.0     Joeson  创建
*******************************************************************************/
void  CCommunicationC::_HandleConnectLogic(const boost::system::error_code & error)
{
    // 连接成功
    if (!error)
    {
		CCLOG("connect c# server success!");
        m_nRecvActivPacket = 0;
        m_eNetState = E_NET_CONECT;

        // 重置数据接收缓冲区
        m_pAnalyticTcpPacket->Reset();

        // 回调连接成功函数
        SAFE_CALL_Fun_Comm_INT(m_pObjectConnect, m_funConnect, this, 0);

        // 请求一次异步读数据操作
        _RequestAsynRead();

        // 如果当前的心跳检测时间不为0，则表示需要开启心跳检测
        if (m_nActiveTime == 0)
        { return; }

        // 如果已经建立连接，则设置开启心跳检测
        if (E_NET_CONECT == m_eNetState)
        {
            m_timer.expires_from_now(boost::posix_time::seconds(m_nActiveTime));
            m_timer.async_wait(boost::bind(&CCommunicationC::ActiveCheck, this, boost::asio::placeholders::error));
        }
    }
    // 连接发生错误
    else
    {
		//关闭连接 by soko　2013-3-16　当连接不了服务器时，要先关闭
        Close();

        m_eNetState = E_NET_NO_INIT;
        int nError = 1;

        // 找不到指定IP或端口的主机
        if (error == boost::asio::error::host_not_found)
        {
            nError = 1;
        }
        // 连接对方拒绝
        else if (error == boost::asio::error::connection_refused)
        {
            nError = 2;
        }
        // The service is not supported for the given socket type.
        else if (error == boost::asio::error::service_not_found)
        {
            nError = 3;
        }
        // The service is not supported for the given socket type.
        else if (error == boost::asio::error::service_not_found)
        {
            nError = 4;
        }
        // 网络不可用
        else if (error == boost::asio::error::network_unreachable)
        {
            nError = 5;
        }
        else
        {
            // 记录未知失败原因
            boost::system::system_error syserror(error);
            CCLOG("CCommunicationC::HandleConnect: connect failed, it's info is:%s", syserror.what());
        }


        SAFE_CALL_Fun_Comm_INT(m_pObjectConnect, m_funConnect, this, nError);
    }
}

/***********************************************************************************
* 函数原型: void HandleRead(const boost::system::error_code& error)
* 函数功能: 异步读取数据完成的回调处理函数
* 参数：
*  ［IN］
*       const boost::system::error_code& error: 错误类型
* ［OUT］: 无
* 返回值: 
*
* 注意：有解析包决定接收数据长度，该函数设置为每次只接收指定长度的数据。
*       这样的设计应该可以改进，但是由于时间关系，暂时不做处理。顾红波 2012.7.6
************************************************************************************/
void CCommunicationC::HandleRead(const boost::system::error_code & error, std::size_t bytes_transferred)
{
	_HandleTcpReadLogic(error, bytes_transferred);
}

/********************************************************************************
* 功  能: 异步读取完成逻辑
* 参  数: [IN] error: 错误码
*         [IN] bytes_transferred: 传输的大小
* 返回值: 无
--------------------------------------------------------------------------------
* 修改记录 :
 日期       版本    作者    内容
 13-04-01   1.0     Joeson  创建
*******************************************************************************/
void  CCommunicationC::_HandleTcpReadLogic(const boost::system::error_code & error, std::size_t bytes_transferred)
{
	// 读取成功
	if (!error)
	{
        // 添加新的分包对象逻辑 [13-03-13 Joeson]
        m_pAnalyticTcpPacket->AddData(m_pReadTcpBuffer, bytes_transferred);
        m_pReadTcpBuffer = NULL;

        // 这个节点添加以后，缓存已满，不触发再次读取，阻塞读取数据
        if (m_pAnalyticTcpPacket->IsCacheFull())
        {
			CCLOG("[_HandleTcpReadLogic] read msg data is full, blocking read");
            m_isTcpBlockAsynRead = true;
        }
        else
        {
            _RequestAsynRead();         // 请求一次异步读数据操作
        }
	}
	// 读取失败
	else
	{
        SAFE_DELETE_BUFF(m_pReadTcpBuffer, m_pAnalyticTcpPacket->GetRequestDataLen());           // 读取失败释放内存

		if (E_NET_CONECT == m_eNetState)
		{
			// 关闭连接
			Close();

			SAFE_CALL_Fun_Comm_INT(m_pObjectDisConnect, m_funDisConnect, this, 0);

			CCLOG("[_HandleTcpReadLogic] read msg data failed, close network");
		}
	} 
}

/***********************************************************************************
* 函数原型: void CCommunicationC::Send(const char * pSendMsg, unsigned int nLen)
* 函数功能: 异步发送数据接口函数
* 参数：
*  ［IN］
*       char * pSendMsg： 待发送数据缓冲区
*       unsigned int nLen： 待发送数据长度
* ［OUT］: 无
* 返回值: 
*          0: 发送成功
*         -1: 如果传入参数失败，则返回-1，
*         -2: 连接已经断开，不能发送数据
*         -4: 发送队列已满
*         -5: 分配内存失败
*         其他：返回Combination函数的返回值
************************************************************************************/
int CCommunicationC::Send(const char * pSendMsg, unsigned int nLen)
{
	// 网络状态检测
	if (E_NET_CONECT != m_eNetState)
	{
		return -2;
	}

	// 异常检测
	if (NULL == pSendMsg || nLen <= 0)
	{
		// 记录错误日志
		//CLog::GetInstance().Log("Net.log", "CCommunicationC::Send: input parameters are not legitimate, buf is %d, length is %d", pSendMsg, nLen);

		return -1;
	}

	// 添加包头，用于分包
	int nHeadAndDataLen = m_pAnalyticTcpPacket->GetAdditionalDataLen(E_USE_PACKET) + nLen;
	char *pHeadAndData = new char[nHeadAndDataLen];
	if (NULL == pHeadAndData)
	{
		CCLOG("CCommunicationC::Send Alloc memory failed!");
		return -5;
	}
	// 组包
	boost::unique_lock<boost::mutex> lock(m_mutexSendMsg);
	int nRet = m_pAnalyticTcpPacket->Combination(E_USE_PACKET, pSendMsg, nLen, pHeadAndData, nHeadAndDataLen);
	if (0 == nRet)
	{
		// 压入到发送队列中 [13-03-13 Joeson]
		SSendDataNode node(pHeadAndData, nHeadAndDataLen);
		bool bRet = m_queueSendTCPData.Push(node);
		if (!bRet)
		{
			SAFE_DELETE_BUFF(pHeadAndData, nHeadAndDataLen);
			CCLOG("Push node failed!");
			return -4;
		}
	}
	else
	{
		delete[]pHeadAndData;
	}

	return nRet;
}

/***********************************************************************************
* 函数原型: int CCommunicationC::StartCommnication()
* 函数功能: 开启通信
* 参数：
*  ［IN］: 无
* ［OUT］: 如果开启成功，返回非0，否则返回0
* 返回值: 
************************************************************************************/
int CCommunicationC::StartCommnication()
{
	if (false == S_bStart)
	{
		S_pIOTCPRunThread = new boost::thread(&CCommunicationC::IOServeRunThread);
		if (NULL == S_pIOTCPRunThread)
		{
			// 记录日志
			CCLOG("CCommunicationC::StartCommnication: S_pIOTCPRunThread start IOrun thread failed");

			return 0;
		}

		S_bStart = true;
	}

	return 1;
}

/********************************************************************************
* 功  能: 发送数据线程
* 参  数: [IN] pThis: 通讯对象指针
* 返回值: 无
* 备  注: 从数据队列中读取数据，打包发送
--------------------------------------------------------------------------------
* 修改记录 :
 日期       版本    作者    内容
 13-04-01   1.0     Joeson  创建
*******************************************************************************/
void  CCommunicationC::SendTCPDataThread(void *pThis)
{
	((CCommunicationC*)pThis)->_SendTCPDataThreadLogic();
}

/********************************************************************************
* 功  能: 发送数据线程逻辑
* 参  数: 无
* 返回值: 无
--------------------------------------------------------------------------------
* 修改记录 :
 日期       版本    作者    内容
 13-03-13   1.0     Joeson  创建
*******************************************************************************/
void  CCommunicationC::_SendTCPDataThreadLogic()
{
    SSendDataNode node;
    int nSendBuffUsed   = 0;            // 发送缓存已使用量
    int nNodeBuffOffset = 0;            // 当前数据节点缓存偏移
    int nLenCanPushToSendBuff = 0;      // 每次能放入到发送缓存的长度
    int nTemp = 0;
    while (S_bStart)
    {
        // 将队列中数据组装到sendbuffer中去
        if (NULL == node.pBuff && !m_queueSendTCPData.IsEmpty())
        {
            node = m_queueSendTCPData.GetFront();
            m_queueSendTCPData.Pop();
            nNodeBuffOffset = 0;
        }
        while (NULL != node.pBuff && nSendBuffUsed < MAX_SENDBUFF_LEN)
        {
            nLenCanPushToSendBuff = node.nDataLen - nNodeBuffOffset;
            nTemp                 = MAX_SENDBUFF_LEN - nSendBuffUsed;
            nLenCanPushToSendBuff = MIN(nLenCanPushToSendBuff, nTemp);
            memcpy(m_szTCPSendBuffer + nSendBuffUsed, node.pBuff + nNodeBuffOffset, nLenCanPushToSendBuff);
            nSendBuffUsed   += nLenCanPushToSendBuff;
            nNodeBuffOffset += nLenCanPushToSendBuff;

            // 数据节点未读完，说明缓存已满
            if (nNodeBuffOffset < node.nDataLen)
            {
                if (nSendBuffUsed < MAX_SENDBUFF_LEN)
                {
                    CCLOG("CCommunicationC::SendTCPDataThread.The send buff not full(residual:[%d])."
                                            " but the node still has data.(length:[%d])",
                                            MAX_SENDBUFF_LEN - nSendBuffUsed, node.nDataLen - nNodeBuffOffset);
                }
                
                break;
            }
            // 已经读完，释放内存            
            SAFE_DELETE_BUFF(node.pBuff, node.nDataLen);
            // 继续弹出数据节点
            if (!m_queueSendTCPData.IsEmpty())
            {
                node = m_queueSendTCPData.GetFront();
                m_queueSendTCPData.Pop();
                nNodeBuffOffset = 0;
            }
        }

        if (0 != nSendBuffUsed)
        {
            // 弹出数据并发送
            boost::system::error_code errCode;
            //std::size_t sended = m_socket.send(boost::asio::buffer(m_szTCPSendBuffer, nSendBuffUsed), 0, errCode);
            // boost官方资料上描述：tcp::socket::send函数并不保证会投递整个数据到底层，建议使用write函数。 [13-07-17 Joeson]
            std::size_t sended = boost::asio::write(m_socket, boost::asio::buffer(m_szTCPSendBuffer, nSendBuffUsed), boost::asio::transfer_all(), errCode);
            nSendBuffUsed = 0;

            // 发送成功
            if (!errCode)
            {
                SAFE_CALL_Fun_Comm_INT(m_pObjectSendSuccess, m_funSendSuccess, this, 0);        
            }     
            // 发送失败   
            else if (E_NET_CONECT == m_eNetState)
            {
				CCLOG("[_SendTCPDataThreadLogic] send msg failed, close network");

                // 关闭连接
                Close();

                SAFE_CALL_Fun_Comm_INT(m_pObjectDisConnect, m_funDisConnect, this, 1);
                break;          // 结束该线程
            }
        }  // end if (0 != nSendBuffUsed)
        else
        {
            // 队列中已经没有需发送的资源，释放1次cpu资源
            boost::this_thread::sleep(boost::posix_time::milliseconds(10));
        }
        
    } // end while (S_bStart)

    SAFE_DELETE_BUFF(node.pBuff, node.nDataLen);           // 数据还未发送完 就断了
}

/********************************************************************************
* 功  能: 解析数据包线程
* 参  数: [IN] pThis: 通讯对象指针
* 返回值: 无
--------------------------------------------------------------------------------
* 修改记录 :
 日期       版本    作者    内容
 13-04-01   1.0     Joeson  创建
*******************************************************************************/
void CCommunicationC::ParseTCPDataThread(void *pThis)
{
	((CCommunicationC*)pThis)->_ParseTCPDataThreadLogic();
}

/********************************************************************************
* 功  能: 解析数据线程逻辑
* 参  数: 无
* 返回值: 无
--------------------------------------------------------------------------------
* 修改记录 :
 日期       版本    作者    内容
 13-03-13   1.0     Joeson  创建
*******************************************************************************/
void  CCommunicationC::_ParseTCPDataThreadLogic()
{
    EPacketType eType;
    char *pBuff = NULL;
    int   nBuffLen = 0;
    while (S_bStart)
    {
        m_pAnalyticTcpPacket->Parse();
        
        // 未解析完成
        if (!m_pAnalyticTcpPacket->IsCompletePack())
        {
            // 说明解析对象中已经没有数据，释放一次cpu资源
            boost::this_thread::sleep(boost::posix_time::milliseconds(30));
            continue;
        }

        // 读取数据包
        m_pAnalyticTcpPacket->GetDataAndResetBuf(eType, pBuff, nBuffLen);
        // 根据不同的类型进行处理
        switch (eType)
        {
        case E_USE_PACKET:                          // 用户数据包            
            SAFE_CALL_Fun_Comm_PCHAR_INT(m_pObjectDisRecv, m_funRecv, this, pBuff, nBuffLen);    // 回调接收函数
            break;

        case E_ACTIVE_PACKET:                       // 心跳检测包
            m_nRecvActivPacket++;
			CCLOG("msg active");
            SAFE_CALL_Fun_Comm_PCHAR_INT(m_pObjectDisRecv, m_funRecv, this, pBuff, nBuffLen);
            break;

        default: break;
        }

        // 已经阻塞异步读取，要触发异步读取
        if (m_isTcpBlockAsynRead && !m_pAnalyticTcpPacket->IsCacheFull())
        {
            m_isTcpBlockAsynRead = false;
            _RequestAsynRead();
        }        
    }  
}

/***********************************************************************************
* 函数原型: void CCommunicationC::EndCommnication()
* 函数功能: 结束通信
* 参数：
*  ［IN］: 无
* ［OUT］: 无
* 返回值: 
************************************************************************************/
void CCommunicationC::EndCommnication()
{
	if (true == S_bStart)
	{
		S_bStart = false;
		S_io_service.stop();

		// 等待线程结束
		S_pIOTCPRunThread->interrupt();
		S_pIOTCPRunThread->join();

		delete S_pIOTCPRunThread;
		S_pIOTCPRunThread = NULL;
	}
}

/***********************************************************************************
* 函数原型: void CCommunicationC::IOServeRunThread()
* 函数功能: io_service运行线程函数
* 参数：
*  ［IN］: 无
* ［OUT］: 无
* 返回值: 
************************************************************************************/
void CCommunicationC::IOServeRunThread()
{
	_IOServeRunThreadLogic();
}

/********************************************************************************
* 功  能: io_service运行线程逻辑
* 参  数: 无
* 返回值: 无
--------------------------------------------------------------------------------
* 修改记录 :
 日期       版本    作者    内容
 13-04-01   1.0     Joeson  创建
*******************************************************************************/
void  CCommunicationC::_IOServeRunThreadLogic()
{
    boost::asio::io_service::work work(S_io_service);
    S_io_service.run();
}

/***********************************************************************************
* 函数原型: void CCommunicationC::ActiveCheck(const boost::system::error_code& error)
* 函数功能: 心跳检测函数
* 参数：
*  ［IN］: 无
* ［OUT］: 无
* 返回值: 
************************************************************************************/
void CCommunicationC::ActiveCheck(const boost::system::error_code& error)
{
	// 如果是因为计时时间到，则进行心跳检测
	if (!error)
	{
		// 如果当前不是处于正常连接状态，则返回
		if (E_NET_CONECT != m_eNetState)
		{
			return;
		}
		
		// 如果连接已经断开
		if (m_nRecvActivPacket <= 0)
		{
			// 关闭连接
			Close();

			CCLOG("Close connect by heart checking");
			
			// 回调通知应用程序连接已经断开
			SAFE_CALL_Fun_Comm_INT(m_pObjectDisConnect, m_funDisConnect, this, 0);
		} 
		else
		{
			m_nRecvActivPacket--;
			m_timer.expires_from_now(boost::posix_time::seconds(m_nActiveTime));
			m_timer.async_wait(boost::bind(&CCommunicationC::ActiveCheck, this, boost::asio::placeholders::error));

			// 回执心跳包
			int nHeadAndDataLen = m_pAnalyticTcpPacket->GetAdditionalDataLen(E_ACTIVE_PACKET);
            char *pHeadAndData = new char[nHeadAndDataLen];
            if (NULL == pHeadAndData)
            {
                CCLOG("alloc memeroy failed");
                return;
            }            
            
            boost::unique_lock<boost::mutex> lock(m_mutexSendMsg);
            int nRet = m_pAnalyticTcpPacket->Combination(E_ACTIVE_PACKET, NULL, 0, pHeadAndData, nHeadAndDataLen);
			if (0 == nRet)
			{
                // 压入到发送队列中 [13-03-13 Joeson]
                SSendDataNode node(pHeadAndData, nHeadAndDataLen);
                bool isSuc = m_queueSendTCPData.Push(node);
                if (!isSuc)
                {
                    SAFE_DELETE_BUFF(pHeadAndData, nHeadAndDataLen);
                   CCLOG("Push node failed! ActiveCheck");
                }                
			}
            else
            {
                SAFE_DELETE_BUFF(pHeadAndData, nHeadAndDataLen);
            }
		}
	}
	// 暂时不做任何处理
	else
	{

	}
}

/********************************************************************************
* 功  能: 请求一次异步读取
* 参  数: 无
* 返回值: 无
--------------------------------------------------------------------------------
* 修改记录 :
 日期       版本    作者    内容
 13-03-14   1.0     Joeson  创建
*******************************************************************************/
void  CCommunicationC::_RequestAsynRead()
{
    int nBuffLen = m_pAnalyticTcpPacket->GetRequestDataLen();
    if (!m_pAnalyticTcpPacket->AllocBuff(m_pReadTcpBuffer, nBuffLen))
    {
        CCLOG("CCommunicationC::HandleRead AllocBuff failed!");
        return;
    }
    //async_read(m_socket, boost::asio::buffer(m_pReadTcpBuffer, nBuffLen), 
    //           boost::bind(&CCommunicationC::HandleRead, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	// 小心亢，async_read会阻塞socket直到读取了指定长度的数据
    m_socket.async_receive(boost::asio::buffer(m_pReadTcpBuffer, nBuffLen),
                           boost::bind(&CCommunicationC::HandleRead, this, boost::asio::placeholders::error, 
                                       boost::asio::placeholders::bytes_transferred));
}

/***********************************************************************************
* 函数原型: void SetActiveTime(unsigned  int seconds)
* 函数功能: 开启并设置心跳检测时间
* 参数：
*  ［IN］: 
*   unsigned  int seconds： 心跳检测时间，单位秒。
* ［OUT］: 无
* 返回值: 
************************************************************************************/
// 
void CCommunicationC::SetActiveTime(unsigned  int seconds)
{
	m_nActiveTime = seconds;
	m_nRecvActivPacket = 1;

	if (m_nActiveTime == 0)
	{ return; }

	// 如果已经建立连接，则设置开启心跳检测
	if (E_NET_CONECT == m_eNetState)
	{
		m_timer.cancel();

		m_timer.expires_from_now(boost::posix_time::seconds(m_nActiveTime));
		m_timer.async_wait(boost::bind(&CCommunicationC::ActiveCheck, this, boost::asio::placeholders::error));
	}
}

// 设置连接完成回调函数
void CCommunicationC::SetConnectHandle(CObjectBase * pObjectBase, Fun_Comm_INT fun)
{
	m_funConnect = fun;
	m_pObjectConnect = pObjectBase;
}

// 设置断开连接回调处理函数                               
void CCommunicationC::SeDistConnectHandle(CObjectBase * pObjectBase,  Fun_Comm_INT fun)
{
	m_funDisConnect = fun;
	m_pObjectDisConnect = pObjectBase;
}

// 设置接收数据处理函数                          
void CCommunicationC::SetRecvHandle(CObjectBase * pObjectBase, Fun_Comm_PCHAR_SIZE_T fun)
{
	m_funRecv = fun;
	m_pObjectDisRecv = pObjectBase;
}

// 设置数据发送成功回调处理函数
void CCommunicationC::SetSendSuccessHandle(CObjectBase * pObjectBase,  Fun_Comm_INT fun)
{
    m_funSendSuccess = fun;
    m_pObjectSendSuccess = pObjectBase;
}

/********************************************************************************
* 功  能: 设置包解析扩展对象
* 参  数: [IN] pAnalytic: 解析包对象
* 返回值: 无
--------------------------------------------------------------------------------
* 修改记录 :
 日期       版本    作者    内容
 13-03-13   1.0     Joeson  创建
*******************************************************************************/
void CCommunicationC::SetAnalyticPacket( CAnalyticPacket *pAnalytic )
{
    m_pAnalyticTcpPacket = pAnalytic;
}