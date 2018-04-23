#include "AnalyticPacketClient.h"
#include <boost/date_time/posix_time/posix_time.hpp> 
#include <boost/asio/detail/socket_ops.hpp>
#include <memory.h>
#include "cocos2d.h"
using namespace cocos2d;
// 用到的宏
#define QINMEI_AP_LOG               QiMen::CLog::GetInstance().Log						// 日志宏
#define QINMEI_AP_EXCEP																	// 异常处理宏
#define QINMEI_AP_DELETE_ARR(ptr)   if (NULL != ptr) { delete []ptr; ptr = NULL; }		// 释放数组
#define HEARTCHK_TOCLIENT           100108												// 心跳检测. S->C
#define HEARTCHK_TOSERV             100206												// 心跳检测. C->S
#define MEMORY_ALLOC_DIS            16													// 内存分配粒度(BYTE)
#define MEMORY_ALLOC_MGR_MAX        10240												// 内存管理最大尺寸(BYTE)，大于该尺寸的直接分配、释放，并不管理

// 内存尺寸策略 根据内存分配粒度，向上计算真正应该分配的大小
#define MEMORY_SIZE_STRATEGY(resultSize, factSize)  \ (resultSize = factSize + (MEMORY_ALLOC_DIS - ((factSize) % MEMORY_ALLOC_DIS)))

// 最小值宏
#ifndef MIN
    #define MIN(x, y)   ((x) > (y) ? (y) : (x))             
#endif

// 包头封装。放在cpp中，该结构仅本类可用
#pragma pack(push, 1)
struct SClientPackeHead
{    
	// 值. 目前通讯中,一个int的高位存zip压缩标示,低3位存用户包长度。m_pCurPackCache该值为用户包长度
    unsigned int nValue;        
};
#pragma pack(pop)

static int const g_LenClientPackeHead = sizeof(SClientPackeHead);
static int const g_LenParseDataSeg    = sizeof(CAnalyticPacketClient::SParseDataSeg);

CAnalyticPacketClient::CAnalyticPacketClient() 
: m_nReadedLen(0), m_nZip(0)
{    
    m_pCurPackCache = m_szCommonCache;
    m_nCurPackCacheLen = LEN_COMMON_CACHE;
    m_zlib.setReserveAndSelfGrowthSize(512*1024, 256*1024);
}

CAnalyticPacketClient::~CAnalyticPacketClient()
{
    while (!m_queueData.IsEmpty())
    {
        delete []m_queueData.GetFront().pBuff;
        m_queueData.Pop();
    }
    QINMEI_AP_DELETE_ARR(m_ParseDataSeg.pData);
    if (m_szCommonCache != m_pCurPackCache)
    {
        QINMEI_AP_DELETE_ARR(m_pCurPackCache);
    }
}

/********************************************************************************
* 功  能: 获得额外数据长度
* 参  数: [IN] eType: 包类型
* 返回值: 额外数据长度
*******************************************************************************/
int CAnalyticPacketClient::GetAdditionalDataLen(EPacketType eType)
{
	if (E_ACTIVE_PACKET == eType)
	{
		return g_LenClientPackeHead + 4;                // 消息编号4字节
	}
	else
	{
		return g_LenClientPackeHead;
	}
}

/********************************************************************************
* 功  能: 发送数据组合包头接口
* 参  数: [IN] eType: 包类型
*         [IN] pData: 原始数据包
*         [IN] nDataLen: 原始数据包长度
*         [IN] pSendData: 组合的数据包
*         [IN] nSendDataLen: 组合的数据包长度
* 返回值: 0 组合成功
*         1 组合数据参数异常
*         2 接受组合数据缓存异常
*******************************************************************************/
int CAnalyticPacketClient::Combination( EPacketType eType, const char * pData, int nDataLen, char * & pSendData, int & nSendDataLen )
{
#ifdef QINMEI_AP_EXCEP
    if (NULL == pSendData || nDataLen + GetAdditionalDataLen(eType) > nSendDataLen)
    {
        CCLOG("CAnalyticPacketClient::Combination: recive param invalid."
                      "Outbuf[%u] Outbuflen[%d] Needed size[%d]",
                      pSendData, nSendDataLen, nDataLen + GetAdditionalDataLen(eType));
        return 2;
    }
#endif

    // 根据不同的包做不同的处理
    switch (eType)
    {
    case E_USE_PACKET:                      // 用户包
        {            
#ifdef QINMEI_AP_EXCEP
            if (NULL == pData || 0 == nDataLen)
            {
                CCLOG("CAnalyticPacketClient::Combination: param invalid dataPtr[%u] len[%d]",pData, nDataLen);
                return 1;
            }
#endif

            SClientPackeHead head = {};
            head.nValue   = nDataLen;
            head.nValue   = htonl(head.nValue);
            memcpy(pSendData, &head, g_LenClientPackeHead);
            if (0 < nDataLen)
            {
                memcpy(pSendData + g_LenClientPackeHead, pData, nDataLen);
            }
        }
        break;

    case E_ACTIVE_PACKET:                   // 心跳检测包
        {
            // 消息头
            SClientPackeHead head = {};
            head.nValue = 4;
            head.nValue = htonl(head.nValue);
            memcpy(pSendData, &head, g_LenClientPackeHead);

            // 消息编号
            int nTmp = htonl(HEARTCHK_TOSERV);        
            memcpy(pSendData + g_LenClientPackeHead, &nTmp, 4);
        }
        break;

    default: 
#ifdef QINMEI_AP_EXCEP
        CCLOG("CAnalyticPacketClient::Combination. Unknown PacketType[%d]", eType);
#endif
        return 1;
    }
	
	return 0;
}

/********************************************************************************
* 功  能: 分包缓存是否已满 true为满
* 参  数: 无
* 返回值: true  缓存已满
*         false 缓存未满
--------------------------------------------------------------------------------
* 修改记录 :
 日期       版本    作者    内容
 13-03-14   1.0     Joeson  创建
*******************************************************************************/
bool CAnalyticPacketClient::IsCacheFull()
{
    return m_queueData.IsFull();
}

/********************************************************************************
* 功  能: 分配内存
* 参  数: [OUT] pBuff: 分配的内存空间
*         [OUT] nBuffLen: 分配的内存空间长度
* 返回值: true  分配成功
*         false 分配失败
*******************************************************************************/
bool CAnalyticPacketClient::AllocBuff( char *&pBuff, int nBuffLen )
{
    return _AllocMemoryStrategy(nBuffLen, pBuff);
}

/********************************************************************************
* 功  能: 添加接收数据
* 参  数: [IN] pRecvData: 添加的数据包
*         [IN] nRecvDataLen: 添加的数据包长度
* 返回值: 0 添加成功
*         1 队列已满
*         2 参数异常
* 异  常: 无
* 备  注: 数据包内存(pRecvData)必须调用AllocBuff分配，且外部AddData后不应该再继续使用
*         该内存
*******************************************************************************/
int CAnalyticPacketClient::AddData( char * pRecvData, int nRecvDataLen )
{
#ifdef QINMEI_AP_EXCEP
    if (NULL == pRecvData || 0 == nRecvDataLen )
    {
        CCLOG("CAnalyticPacketClient::AddData: Param invalid. dataPtr[%u], len[%n]", pRecvData, nRecvDataLen);
        return 2;
    }
#endif

    if (m_queueData.IsFull())
    {
        CCLOG("CAnalyticPacketClient::AddData: emergent!!! the data queue is full. size:%d", LEN_PACK_QUEUE);
        return 1;
    }
    SDataPackNode node(pRecvData, nRecvDataLen);
    m_queueData.Push(node);

    return 0;
}

/********************************************************************************
* 功  能: 解析数据
* 参  数: 无
* 返回值: 0 解析成功
*         1 分配内存失败
*         2 解析数据异常
* 异  常: 无
* 备  注: 无
*******************************************************************************/
int CAnalyticPacketClient::Parse()
{
    // 未读取又进行解析
    if (IsCompletePack())
    {
        return 0;
    }

    // 无数据可解析
    if (NULL == m_ParseDataSeg.pData && m_queueData.IsEmpty())
    {
        return 0;
    }

    // 重新分配的缓存，释放掉。 
    // 原来该代码在Reset中，而Reset函数会在GetDataAndResetBuf中调用，进而可能导致返回的指针成野指针[13-03-13 Joeson]
    if (0 == m_nReadedLen && m_pCurPackCache != m_szCommonCache)
    {
        QINMEI_AP_DELETE_ARR(m_pCurPackCache);
        m_pCurPackCache = m_szCommonCache;
        m_nCurPackCacheLen = LEN_COMMON_CACHE;
    }

    // 若当前解析包无数据，从数据队列中弹出一个
    SDataPackNode node;
    if (NULL == m_ParseDataSeg.pData)
    {
        node = m_queueData.GetFront();
        m_queueData.Pop();
        m_ParseDataSeg.pData   = node.pBuff;
        m_ParseDataSeg.nResidualLen = node.nLen;
        m_ParseDataSeg.nOffset = 0;
    }
    // 解析包
    int nPerReadLen = 0;                // 本次读取文件长度
    SClientPackeHead *pHead   = NULL;
    while (NULL != m_ParseDataSeg.pData)
    {
        // 解析通讯包头
		
        if (m_nReadedLen < g_LenClientPackeHead)
        {
            nPerReadLen = GetNeedDataLen();
            nPerReadLen = MIN(nPerReadLen, m_ParseDataSeg.nResidualLen);
            memcpy(m_pCurPackCache + m_nReadedLen, m_ParseDataSeg.pData + m_ParseDataSeg.nOffset, nPerReadLen);
            m_nReadedLen                += nPerReadLen;
            m_ParseDataSeg.nOffset      += nPerReadLen;
            m_ParseDataSeg.nResidualLen -= nPerReadLen;

            // 通讯包头读取完成，转换成本地字节顺序
            if (m_nReadedLen == g_LenClientPackeHead)
            {
				pHead = (SClientPackeHead *)m_pCurPackCache;
				pHead->nValue = ntohl(pHead->nValue);
				m_nZip = (pHead->nValue >> 24);
				pHead->nValue = (pHead->nValue & 0x00FFFFFF);

                // 对数据长度进行校验
                if (0 == pHead->nValue)
                {
#ifdef QINMEI_AP_EXCEP
                    CCLOG("CAnalyticPacketClient::Parse. the use-data length is invalid [%d]", 
                                  pHead->nValue);
#endif
                    return 2;
                }

                // 判断缓存是否足够
                if (g_LenClientPackeHead + (int)pHead->nValue > m_nCurPackCacheLen)
                {
                    char *pTemp = m_pCurPackCache;
                    m_pCurPackCache = new char[g_LenClientPackeHead + pHead->nValue];
                    if (NULL == m_pCurPackCache)
                    {
#ifdef QINMEI_AP_EXCEP
                        CCLOG("CAnalyticPacketClient::Parse. alloc memory failed![%d]", g_LenClientPackeHead + pHead->nValue);
#endif
                        // 分配缓存失败, 恢复到默认缓存
                        m_pCurPackCache = pTemp;
                        return 1;
                    }
                    memcpy(m_pCurPackCache, pTemp, m_nReadedLen);
                    m_nCurPackCacheLen = g_LenClientPackeHead + pHead->nValue;
                    // pTemp 是m_szCommonCache, 不必释放
                } // end if (缓存不够)
            } // end if (通讯包头读取完成)
        }

        // 通讯包头已读取完成，读取数据
        if (m_nReadedLen >= g_LenClientPackeHead && 0 != m_ParseDataSeg.nResidualLen)
        {
            nPerReadLen = GetNeedDataLen();
            nPerReadLen = MIN(m_ParseDataSeg.nResidualLen, nPerReadLen);
            memcpy(m_pCurPackCache + m_nReadedLen, m_ParseDataSeg.pData + m_ParseDataSeg.nOffset, nPerReadLen);
            m_nReadedLen                += nPerReadLen;
            m_ParseDataSeg.nOffset      += nPerReadLen;
            m_ParseDataSeg.nResidualLen -= nPerReadLen;
        }

        // 当前段已经读完了，释放掉
        if (0 == m_ParseDataSeg.nResidualLen)
        {
            _RetrieveMemoryStrategy(m_ParseDataSeg.pData);
            memset(&m_ParseDataSeg, 0, g_LenParseDataSeg);
        }

        // 当前用户包已经读取完成，读取成功
        if (IsCompletePack())
        {
            return 0;
        }

        // 当前队列已无数据
        if (m_queueData.IsEmpty())
        {
            return 0;
        }

        // 当前用户包未完成。再弹出一个节点
        node = m_queueData.GetFront();
        m_queueData.Pop();
        m_ParseDataSeg.pData   = node.pBuff;
        m_ParseDataSeg.nResidualLen = node.nLen;
        m_ParseDataSeg.nOffset = 0;
    } // end while

    return 0;
}

/********************************************************************************
* 功  能: 是否是完成数据包
* 参  数: 无
* 返回值: true: 已经解析完一个包
*         false:未解析完一个包
*******************************************************************************/
bool CAnalyticPacketClient::IsCompletePack()
{
	/*
	//重装前行
    if (m_nReadedLen > g_LenClientPackeHead && (m_nReadedLen - g_LenClientPackeHead) == ((SClientPackeHead*)m_pCurPackCache)->nValue)
    {
        return true;
    }
    else
    {
        return false;
    }
	*/

	
	//觉醒之战
	if (m_nReadedLen > g_LenClientPackeHead && m_nReadedLen == ((SClientPackeHead*)m_pCurPackCache)->nValue + g_LenClientPackeHead)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

/********************************************************************************
* 功  能: 获得完整数据包并重置接收缓冲区
* 参  数: [OUT] eType: 数据包类型
*         [OUT] pRecvData: 数据包
*         [OUT] nRecvDataLen: 数据包长度
* 返回值: 获得失败，eType值为E_UNKNOWN_PACKET
* 备  注: pRecvData返回的是内部数据缓存指针，避免分配、拷贝内存。应该在下次Parse前停止使用
*         该函数会自动ResetBuf
*******************************************************************************/
void CAnalyticPacketClient::GetDataAndResetBuf( EPacketType & eType, char * & pRecvData, int & nRecvDataLen )
{    
    pRecvData = NULL;
    nRecvDataLen = 0;
    eType = E_UNKNOWN_PACKET;

    // 未解析完成一个包就读取
	if (false == IsCompletePack())
	{		
#ifdef QINMEI_AP_EXCEP
        CCLOG("CAnalyticPacketClient::GetDataAndResetBuf. Reading data but it's not completed!");
#endif
		return;
	}

    // 分配的内存要释放，继续使用默认缓存。避免单次分配了大内存，然后就一直驻留在内存中
    if (0 == m_nReadedLen && m_szCommonCache != m_pCurPackCache)
    { 
        delete[]m_pCurPackCache;
        m_pCurPackCache    = m_szCommonCache;
        m_nCurPackCacheLen = LEN_COMMON_CACHE;
    }

    SClientPackeHead *pHead = (SClientPackeHead*)m_pCurPackCache;
#ifdef QINMEI_AP_EXCEP
    if (0 == pHead->nValue)
    {
        CCLOG("CAnalyticPacketClient::GetDataAndResetBuf. Reading data but it's length is zero!");
        return;
    }
#endif

    // 需要解压
    if (0 != m_nZip)
    {     
		// 和程序沟通，消息ID未进行压缩。故需要偏移 头+消息编号
        int const LEN_NON_UNCOMPRESS = g_LenClientPackeHead + 4;            // 不需要解压的长度
		m_zlib.Uncompress((const unsigned char *)(m_pCurPackCache + LEN_NON_UNCOMPRESS), pHead->nValue - 4);
		// 解压失败，则返回                                  


		if (NULL == m_zlib.getOutputBuf())
		{
            unsigned int nProtecolID = *(unsigned int *)(m_pCurPackCache + g_LenClientPackeHead);
            CCLOG("network msg data parse failed msgID:%d", ntohl(nProtecolID));

            eType = E_UNKNOWN_PACKET;
            ResetBuf();
            return;
		}

        // 缓存不足以容下解压后的包
        if ((int)m_zlib.getOutputBufSize() > m_nCurPackCacheLen - LEN_NON_UNCOMPRESS)
        {
            // 分配缓存，拷贝数据
            char *pOrgin = m_pCurPackCache;
            m_pCurPackCache = new char[m_zlib.getOutputBufSize() + LEN_NON_UNCOMPRESS];
            if (NULL == m_pCurPackCache)
            {
                CCLOG("CAnalyticPacketClient::GetDataAndResetBuf. Alloc memory failed! size[%d]", 
                              m_zlib.getOutputBufSize() + LEN_NON_UNCOMPRESS);
                eType = E_UNKNOWN_PACKET;
                ResetBuf();
                m_pCurPackCache = pOrgin;
                return;
            }
            memcpy(m_pCurPackCache, pOrgin, LEN_NON_UNCOMPRESS);
            memcpy(m_pCurPackCache + LEN_NON_UNCOMPRESS, m_zlib.getOutputBuf(), m_zlib.getOutputBufSize());

            // 重定位pHead
            pHead = (SClientPackeHead *)m_pCurPackCache;            
            pHead->nValue = m_zlib.getOutputBufSize() + 4;

            // 维护缓存
            if (m_szCommonCache != pOrgin)
            {
                QINMEI_AP_DELETE_ARR(pOrgin);
            }
            m_nCurPackCacheLen = m_zlib.getOutputBufSize() + LEN_NON_UNCOMPRESS;
        }
        else
        {
            // 重组接收数据
            memcpy(m_pCurPackCache + LEN_NON_UNCOMPRESS, m_zlib.getOutputBuf(), m_zlib.getOutputBufSize());
            pHead->nValue = m_zlib.getOutputBufSize() + 4;
        }
    }

	pRecvData = (m_pCurPackCache + g_LenClientPackeHead);
	nRecvDataLen = pHead->nValue;
	// 应用层4字节为消息编号..额,貌似关联有点强.
	int nMsg = *((int*)pRecvData);
	switch (ntohl(nMsg))
	{
	case HEARTCHK_TOCLIENT:      // 心跳检测包
		eType = E_ACTIVE_PACKET;
		break;
	default:                    // 用户数据包
		eType = E_USE_PACKET;
		break;
	}

    ResetBuf();
}

/********************************************************************************
* 功  能: 获得当前还需要接收数据的长度(构成一个包)
* 参  数: 无
* 返回值: 需要接受的字节数
* 备  注: 若包头都未读取完，那么返回还要多少字节才完成包头
*         包头完成，返回还要多少字节完成整个包
*******************************************************************************/
int CAnalyticPacketClient::GetNeedDataLen()
{
    if (m_nReadedLen < g_LenClientPackeHead)
    {
        return g_LenClientPackeHead - m_nReadedLen;
    }
    else
    {
		// 剩余多少长度需要读取 = 数据包长 - 已读数据包（当前读取的长度-包头长度）
        return ((SClientPackeHead*)m_pCurPackCache)->nValue - (m_nReadedLen - g_LenClientPackeHead);
    }
}

/********************************************************************************
* 功  能: 得到每次请求接受数据长度
* 参  数: 无
* 返回值: 每次请求数据的长度
* 备  注: 这个值因为根据实际测试情况进行调整，以使网络吞吐和解包效率达到最大
*******************************************************************************/
int CAnalyticPacketClient::GetRequestDataLen()
{
    return LEN_REQUEST;
}

/********************************************************************************
* 功  能: 重置数据接收缓冲区
* 参  数: 无
* 返回值: 无
*******************************************************************************/
void CAnalyticPacketClient::ResetBuf()
{
    m_nReadedLen = 0;
}

/********************************************************************************
* 功  能: 分配内存策略
* 参  数: [IN/OUT] nBuffLen: 传入要分配的缓存长度. 传出分配的长度
*         [OUT] pBuff: 传出分配的内存地址
* 返回值: true:  分配成功
*         false: 分配失败
*******************************************************************************/
bool CAnalyticPacketClient::_AllocMemoryStrategy( int &nBuffLen, char *&pBuff )
{
    pBuff = new char[nBuffLen];
    if (NULL == pBuff)
    {
#ifdef QINMEI_AP_EXCEP
        CCLOG("CAnalyticPacketClient::_AllocMemoryStrategy alloc memory failed! size[%d]", nBuffLen);
#endif
        return false;
    }
    return true;
}

/********************************************************************************
* 功  能: 回收内存策略
* 参  数: 无
* 返回值: 无
*******************************************************************************/
void CAnalyticPacketClient::_RetrieveMemoryStrategy( char *pBuff )
{
	try {
		QINMEI_AP_DELETE_ARR(pBuff);
	}
	catch (const char* msg) {
		std::cout << msg << std::endl;
	}
}


////////////////////////////////////////////////////////////////////////// region:CAnalyticPacketClientMask
#define LEN_ADD_DATA_ACTIVE         16              // 心跳包额外增加字段长度验证码4个字节+消息编号4字节 + 时间4个字节 + 时间4个字节
#define LEN_ADD_DATA_USER           8               // 验证码4个字节 + 时间4字节
/********************************************************************************
* 功  能: 获得发送数据包额外数据长度
* 参  数: [IN] eType: 包类型
* 返回值: 发送数据包增加的额外长度
*******************************************************************************/
int CAnalyticPacketClient_IDCode::GetAdditionalDataLen( EPacketType eType )
{
    if (E_ACTIVE_PACKET == eType)
    {
        return g_LenClientPackeHead + LEN_ADD_DATA_ACTIVE;            // 验证码4个字节+消息编号4字节 + 时间4个字节 + 时间4个字节
    }
    else
    {
        return g_LenClientPackeHead + LEN_ADD_DATA_USER;            // 验证码4个字节 + 时间4字节
    }
}

/********************************************************************************
* 功  能: 发送数据组合包头接口
* 参  数: [IN] eType: 包类型
*         [IN] pData: 原始数据包
*         [IN] nDataLen: 原始数据包长度
*         [IN] pSendData: 组合的数据包
*         [IN] nSendDataLen: 组合的数据包长度
* 返回值: 0 组合成功
*         1 组合数据参数异常
*         2 接受组合数据缓存异常
*******************************************************************************/
int CAnalyticPacketClient_IDCode::Combination( EPacketType eType, const char * pData, int nDataLen, char * & pSendData, int & nSendDataLen )
{
    int nAdditionalDataLen = GetAdditionalDataLen(eType);
#ifdef QINMEI_AP_EXCEP
    if (NULL == pSendData || nDataLen + nAdditionalDataLen > nSendDataLen)
    {
        CCLOG("CAnalyticPacketClient_IDCode::Combination: recive param invalid."
                      "Outbuf[%u] Outbuflen[%d] Needed size[%d]",
                      pSendData, nSendDataLen, nDataLen + nAdditionalDataLen);
        return 2;
    }
#endif


    // 根据不同的包做不同的处理
    switch (eType)
    {
	// 用户包
    case E_USE_PACKET:                      
        {            
			// 至少4字节的协议编号
            if (NULL == pData || 4 > nDataLen)
            {
#ifdef QINMEI_AP_EXCEP
                CCLOG("CAnalyticPacketClient_IDCode::Combination: param invalid dataPtr[%u] len[%d]",
                              pData, nDataLen);
#endif
                return 1;
            }

            // 压入头
			int nOffset = 0;
            SClientPackeHead head = {};
            head.nValue   = nDataLen + LEN_ADD_DATA_USER;
            head.nValue   = htonl(head.nValue);
            memcpy(pSendData, &head, g_LenClientPackeHead);
			nOffset += g_LenClientPackeHead;

            // 压入验证码
            unsigned int nIDCode = _GenIdentifyCode(m_uSendMsgCounter++, nDataLen + LEN_ADD_DATA_USER);
            nIDCode = htonl(nIDCode);
            memcpy(pSendData + nOffset, &nIDCode, 4);
			nOffset += 4;

            // 压入时间戳
            int nTmp = 0;
            if (m_timeServSyncTime.is_not_a_date_time())
            {
                nTmp = htonl(0x80000000);
            }
            else
            {
                boost::posix_time::ptime end = boost::posix_time::second_clock::local_time();
                boost::posix_time::time_duration dur = end - m_timeServSyncTime;
                nTmp = htonl(dur.total_seconds());
            }
#if 0
            if (0 != m_nServStartTime)
            {
                boost::posix_time::ptime end = boost::posix_time::second_clock::local_time();
                boost::posix_time::time_duration dur = end - m_timeServSyncTime;
                nTmp = m_nServStartTime + dur.total_milliseconds();             // 服务器同步的时间+偏移
                nTmp = htonl(nTmp);
            }
            else
            {
                nTmp = 0;                       // 第一次时间为0
            }
#endif
            memcpy(pSendData + nOffset, &nTmp, 4);
            nOffset += 4;

			//
            // 压入数据
			// 5.5版本的服务器在协议编号和数据间加了时间戳，所以这里需要进行拆包 Joeson
			//
			// 压入协议编号
			char const *pDataIdx = pData;
			int         nDataLenIdx = nDataLen;
			memcpy(pSendData + nOffset, pDataIdx, 4);
			nOffset += 4;
			pDataIdx += 4;
			nDataLenIdx -= 4;
            
			// 压入剩余的数据
			if (0 < nDataLenIdx)
			{
				memcpy(pSendData + nOffset, pDataIdx, nDataLenIdx);
			}            
        }
        break;

    case E_ACTIVE_PACKET:                   // 心跳检测包
        {
            // 消息头
            int nOffset = 0;
            SClientPackeHead head = {};
            head.nValue = LEN_ADD_DATA_ACTIVE;
            head.nValue = htonl(head.nValue);
            memcpy(pSendData, &head, g_LenClientPackeHead);
            nOffset += g_LenClientPackeHead;

            // 压入验证码
            unsigned int nIDCode = _GenIdentifyCode(m_uSendMsgCounter++, LEN_ADD_DATA_ACTIVE);
            nIDCode = htonl(nIDCode);
            memcpy(pSendData + nOffset, &nIDCode, 4);
            nOffset += 4;

            int nTmp = 0;
            // 告诉服务器时间

            if (0 != m_nServStartTime)
            {
                boost::posix_time::ptime end = boost::posix_time::second_clock::local_time();
                boost::posix_time::time_duration dur = end - m_timeServSyncTime;
                nTmp = m_nServStartTime + dur.total_milliseconds();             // 服务器同步的时间+偏移
                nTmp = htonl(nTmp);
            }
            else
            {
                nTmp = 0;                       // 第一次时间为0
            }
            memcpy(pSendData + nOffset, &nTmp, 4);
            nOffset += 4;
            //memcpy(pSendData + nOffset, &nTmp, 4); // ???

            // 消息编号
            nTmp = htonl(HEARTCHK_TOSERV);        
            memcpy(pSendData + nOffset, &nTmp, 4);
            nOffset += 4;

        }
        break;

    default: 
        CCLOG("CAnalyticPacketClient::Combination. Unknown PacketType[%d]", eType);
        return 1;
    }
	
	return 0;
}

/********************************************************************************
* 功  能: 发送数据组合包头接口
* 参  数: [IN] eType: 包类型
		 [IN] pSendData: 组合后的数据包
*         [IN] pData: 原始数据包
*         [IN] nDataLen: 原始数据包长度
* 返回值: 0 组合成功
*         1 组合数据参数异常
*         2 接受组合数据缓存异常
* author：Unda.K
*******************************************************************************/
int CAnalyticPacketClient_IDCode::CombinationSimple( EPacketType eType, char * & pSendData, const char * pData, int nDataLen )
{
	switch (eType)
	{
	case E_UNKNOWN_PACKET:			// 未知数据包类型
		break;
	case E_USE_PACKET:				// 用户数据包
		{

			// 至少4字节的协议编号
			if (NULL == pData)
			{
				CCLOG("CAnalyticPacketClient_IDCode::CombinaionSimple: param invalid dataPtr[%u] len[%d]",pData, nDataLen);
				return 1;
			}
		
			//BEGIN
			///////////////////// 组包 //////////////////////////

			// 偏移
			int nOffset = 0;
			int _nDataLen = nDataLen;
			// 由于服务器解析的是多消息一个包，所以总包头还需要增加4个字节的长度
			
			// 压入 复合总包头（子包头长+用户数据长） 压入总包头
			unsigned int _header = _nDataLen;
			_header = htonl(_header);
			memcpy(pSendData, &_header, g_LenClientPackeHead);
			nOffset += g_LenClientPackeHead;

			// 压入 (实际数据长度）nDataLen 在前面的函数已经加入了消息ID的长度，所以要减去
			memcpy(pSendData + nOffset, pData, nDataLen);

			//ENDEND
			///////////////////////////////////////////////////
		}
		break;
	case E_ACTIVE_PACKET:				// 心跳包
		break;
	default:
		CCLOG("CAnalyticPacketClient_IDCode::Combination. Unknown PacketType[%d]", eType);
		return 1;
	}
	return 0;
}

/********************************************************************************
* 功  能: 获得完整数据包并重置接收缓冲区
* 参  数: [OUT] eType: 数据包类型
*         [OUT] pRecvData: 数据包
*         [OUT] nRecvDataLen: 数据包长度
* 返回值: 获得失败，eType值为E_UNKNOWN_PACKET
* 备  注: pRecvData返回的是内部数据缓存指针，避免分配、拷贝内存。应该在下次Parse前停止使用
*         该函数会自动ResetBuf
*******************************************************************************/
void CAnalyticPacketClient_IDCode::GetDataAndResetBuf(EPacketType & eType, char * & pRecvData, int & nRecvDataLen)
{
    CAnalyticPacketClient::GetDataAndResetBuf(eType, pRecvData, nRecvDataLen);

    // 若是心跳消息
    if (E_ACTIVE_PACKET == eType)
    {

		// 解数据
		int *pDataIdx = (int*)pRecvData;
		int nMsgNo    = ntohl(*(pDataIdx));				// 消息编号
		int nUTC      = ntohl(*(pDataIdx + 1));			// UTC
		int nStartTime= ntohl(*(pDataIdx + 2));		    // 启动时间
		int nSign     = ntohl(*(pDataIdx + 3));			// 标志
		/*标志用法: 1--需要重置客户端和服务器 0且服务器时间快于客户端时间--需要重置客户端时间*/

		// 第一次 或 标记为1 重置客户端时间(服务器时间重置在上层处理)
		if (0 == m_nServStartTime || 1 == nSign)
		{
			m_timeServSyncTime = boost::posix_time::second_clock::local_time();
			m_nServStartTime = nStartTime;
		}
		// 标记为0
		else if (0 == nSign)
		{
            boost::posix_time::ptime end = boost::posix_time::second_clock::local_time();
            boost::posix_time::time_duration dur = end - m_timeServSyncTime;
			// 时间慢于服务器 重置时间
			if ( nStartTime > (m_nServStartTime + dur.total_milliseconds()) )
			{
				m_timeServSyncTime = boost::posix_time::second_clock::local_time();
				m_nServStartTime = nStartTime;
			}
		}
    }
}

/********************************************************************************
* 功  能: 重置包解析
* 参  数: 无
* 返回值: 无
*******************************************************************************/
void CAnalyticPacketClient_IDCode::Reset()
{
    m_uSendMsgCounter = 0;
    m_nServStartTime  = 0;
    m_timeServSyncTime = boost::date_time::not_a_date_time;
    ResetBuf();
}

/********************************************************************************
* 功  能: 生成认证码
* 参  数: [IN] counter: 发送包计数
*         [IN] packLen: 包长度
* 返回值: 认证码
* 备  注: 算法是:客户端发送数据包个数异或当前包的长度，结果再异或(0x77<<10)，即为验证码
*******************************************************************************/
unsigned  int CAnalyticPacketClient_IDCode::_GenIdentifyCode( unsigned int counter, unsigned int packLen )
{
    unsigned int nCode = counter ^ packLen;
    nCode ^= (0x77 << 10);
    return nCode;
}
