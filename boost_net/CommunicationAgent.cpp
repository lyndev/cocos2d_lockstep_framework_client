#include "CommunicationAgent.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "platform/CCPlatformMacros.h"

CCommunicationAgent* CCommunicationAgent::m_pInstance = NULL;

// 解决LuaEngine include CommunicationAgent.h出错
void CCommunicationAgent_SendMsg(unsigned int nMsgId, const std::string& str)
{
    CCommunicationAgent::GetInstance()->SendMsg(nMsgId, str.c_str(), str.length());
}
 
/***********************************************************************************
* 函 数 名: Created
* 访问权限: public static 
* 函数功能: 静态创建方法
* 参    数: 
* 返 回 值: CCommunicationAgent*
************************************************************************************/
CCommunicationAgent* CCommunicationAgent::Create()
{
    CCommunicationAgent* pAgent = new CCommunicationAgent;
    if (pAgent && pAgent->Init())
    {
        return pAgent;
    }
    SAFE_DELETE(pAgent);
    return NULL;
}

CCommunicationAgent::~CCommunicationAgent(void)
{
	m_pInstance = NULL;
}

/***********************************************************************************
* 函 数 名: SendMsg
* 访问权限: public 
* 函数功能: 发送消息到本地消息队列
* 参    数: 
*    [IN] unsigned int nMsgId: 
*    [IN] const char * pData: 
*    [IN] size_t nLen: 
* 返 回 值: void
************************************************************************************/
void CCommunicationAgent::SendMsg( unsigned int nMsgId, const char* pData, size_t nLen )
{
    CDataBuff buff;
    buff.ResetData();
    buff.SetIsNetByteOrder(true);

    // 添加数据
    buff.PushInt(nMsgId);
    if (NULL != pData && 0 != nLen)
    {
        buff.PushData((void *)pData, nLen);
    }
    m_tcpProducerAndConsumer.Producer(buff.GetData(), buff.GetLength(), false);
}

/***********************************************************************************
* 函 数 名: SendMsg
* 访问权限: public 
* 函数功能: 发送消息到本地消息队列， 带优先级
* 参    数: 
*    [IN] unsigned int nMsgId: 
*    [IN] const char * pData: 
*    [IN] size_t nLen: 
* 返 回 值: void
************************************************************************************/
void CCommunicationAgent::SendMsgPriority( unsigned int nMsgId, const char* pData, size_t nLen, bool bHigh)
{
	CDataBuff buff;
	buff.ResetData();
	buff.SetIsNetByteOrder(true);

	// 添加数据
	buff.PushInt(nMsgId);
	if (NULL != pData && 0 != nLen)
	{
		buff.PushData((void *)pData, nLen);
	}
	m_tcpProducerAndConsumer.Producer(buff.GetData(), buff.GetLength(), bHigh);
}

/***********************************************************************************
* 函 数 名: SendMsgToServer
* 访问权限: public 
* 函数功能: 发送数据到服务器接口
* 参    数: 
*    [IN] const char * pData: 
*    [IN] size_t nLen: 
* 返 回 值: int
************************************************************************************/
int CCommunicationAgent::SendMsgToServer(unsigned int nMsgId, const char* pData, size_t nLen )
{
    CDataBuff buff;
    buff.ResetData();
    buff.SetIsNetByteOrder(true);

    // 添加数据
    buff.PushInt(nMsgId);
    if (NULL != pData && 0 != nLen)
    {
        buff.PushData((void *)pData, nLen);
    }
    return m_communicationTCP.Send((char*)buff.GetData(), buff.GetLength());
}

/***********************************************************************************
* 函 数 名: SendMsgToServer
* 访问权限: public
* 函数功能: 发送数据到服务器接口
* 参    数:
*    [IN] const char * pData:
*    [IN] size_t nLen:
* 返 回 值: int
************************************************************************************/
int CCommunicationAgent::SendUDPMsgToServer(unsigned int nMsgId, const char* pData, size_t nLen)
{
	CDataBuff buff;
	buff.ResetData();
	buff.SetIsNetByteOrder(true);

	// 添加数据
	buff.PushInt(nMsgId);
	if (NULL != pData && 0 != nLen)
	{
		buff.PushData((void *)pData, nLen);
	}
	return m_communicationUDP.Send((char*)buff.GetData(), buff.GetLength());
}

/***********************************************************************************
* 函 数 名: Connect
* 访问权限: public 
* 函数功能: 连接服务器
* 参    数: 
*    [IN] const char * IP: 
*    [IN] unsigned int port: 
* 返 回 值: int
************************************************************************************/
int CCommunicationAgent::Connect( const char* IP, unsigned int port )
{
    // 连接服务器
    return m_communicationTCP.Connect(IP, port);
}

int CCommunicationAgent::ConnectUDP(const char* IP, unsigned int port)
{
	// 连接服务器
	return m_communicationUDP.Connect(IP, port);
}

/***********************************************************************************
* 函 数 名: DisConnect
* 访问权限: public 
* 函数功能: 断开连接
* 参    数: 
* 返 回 值: int
************************************************************************************/
int CCommunicationAgent::DisConnect()
{
    m_communicationTCP.Close();
	m_communicationUDP.Close();
	SendMsg(1403, 0, 0);
	return 0;
}

/***********************************************************************************
* 函 数 名: GetMsg
* 访问权限: public 
* 函数功能: 获得消息
* 参    数: 
* 返 回 值: int
************************************************************************************/
void CCommunicationAgent::GetMsg()
{
    char * pMsg = NULL;				// 数据包
    size_t nLen = 0;				// 数据长度
    unsigned int nMsgID = 0;		// 消息id

    clock_t msgStartTime;           // One Msg消息处理起始时间
    clock_t msgEndTime;             // One Msg消息处理结束时间
    int64  TotalTime = 0;           // 消息处理总共花费时长
    int64  OneMsgTotalTime = 0;     // 单条消息花费时长

#ifdef WIN32
    int     Param = 1;
#else
    int     Param = 100;
#endif

	// 处理udp通信数据
	while (true)
	{
		// 消费产品
		m_udpProducerAndConsumer.Consumer((void * &)pMsg, nLen);
		if (pMsg == NULL)
		{
			break;
		}

		// 传递到lua层
		nMsgID = *(unsigned int *)(pMsg);
		if (nMsgID != 0)
		{
			nMsgID = ntohl(nMsgID);
		}

		if (nLen > sizeof(unsigned int))
		{
			MessageProc(nMsgID, (pMsg + sizeof(nMsgID)), nLen - sizeof(unsigned int));
		}

		// 回收产品
		m_udpProducerAndConsumer.GarbageHandle((void * &)pMsg, nLen);
	}


	// 处理tcp通信数据
    while (true)
    {

        // 消费产品
        m_tcpProducerAndConsumer.Consumer((void * &)pMsg, nLen);
        if (pMsg == NULL)
        {
            break;
        }

        // 获取消息id
        nMsgID = *(unsigned int *)(pMsg);
        if (nMsgID != 0)
        {
            nMsgID = ntohl(nMsgID);
        }
        
        // 计时并开始派发消息
        msgStartTime = clock();
        if (nLen > sizeof(unsigned int))
        {
			MessageProc(nMsgID, (pMsg + sizeof(nMsgID)), nLen - sizeof(unsigned int));
        }
        else if (nLen == sizeof(unsigned int))
        {
            MessageProc(nMsgID, 0, 0);
        }
        msgEndTime = clock();

        OneMsgTotalTime = (msgEndTime - msgStartTime);
        if (OneMsgTotalTime > 100 * Param)
        {
            // 单条消息处理时间过长
            CCLOG("CommunicationAgent::GetMsg ==>Msg handle timer too long id:%d  time: %d", nMsgID, OneMsgTotalTime);
        }

        // 回收产品
        m_tcpProducerAndConsumer.GarbageHandle((void * &)pMsg, nLen);

        // 当前帧处理消息时间过长，到下一帧处理剩余的消息
        TotalTime += OneMsgTotalTime;
        if (TotalTime > 10 * Param)
        {
            break;
        }
    }
}

/***********************************************************************************
* 函 数 名: Init
* 访问权限: protected 
* 函数功能: 初始化函数
* 参    数: 
* 返 回 值: bool
************************************************************************************/
bool CCommunicationAgent::Init()
{
    mProxyAnalyticPacketObj = new CAnalyticPacketClient();
	m_tcpAnalyticPacketObject = new CAnalyticPacketClient_IDCode();
	m_udpAnalyticPacketObject = new CAnalyticPacketClient_IDCode();
    if (m_tcpAnalyticPacketObject == NULL || m_tcpAnalyticPacketObject == NULL || m_udpAnalyticPacketObject == NULL)
    {
        CCLOG("new CAnalyticPacketClient_IDCode, CAnalyticPacketClient failed!");
        return false;
    }

	// tcp
	m_communicationTCP.SetConnectHandle(this, (Fun_Comm_INT)(&CCommunicationAgent::ConnectCallBack));
	m_communicationTCP.SetRecvHandle(this, (Fun_Comm_PCHAR_SIZE_T)(&CCommunicationAgent::RecvTcpDataCallBack));
	m_communicationTCP.SeDistConnectHandle(this, (Fun_Comm_INT)(&CCommunicationAgent::DistCallBack));
    m_communicationTCP.SetSendSuccessHandle(this, (Fun_Comm_INT)(&CCommunicationAgent::SendSuccessCallBack));
    m_communicationTCP.SetAnalyticPacket(mProxyAnalyticPacketObj);

	// udp
	m_communicationUDP.SetAnalyticPacket(m_udpAnalyticPacketObject);
	m_communicationUDP.SetRecvHandle(this, (Fun_Comm_PCHAR_SIZE_T)(&CCommunicationAgent::RecvUDPDataCallBack));

    return true;
}

/***********************************************************************************
* 函 数 名: SetAnalyticPacket
* 函数功能: 设置分包对象（1、代理，2、游戏服）
* 参    数: 
*    [IN] int id: 1、代理，2、游戏服
* 返 回 值: void
************************************************************************************/
void CCommunicationAgent::SetAnalyticPacket(int id)
{
	if (id == 1)
	{
		if(NULL == mProxyAnalyticPacketObj)
		{
			mProxyAnalyticPacketObj = new CAnalyticPacketClient();
		}
		m_communicationTCP.SetAnalyticPacket(mProxyAnalyticPacketObj);

		if(NULL != m_tcpAnalyticPacketObject)
		{
			delete m_tcpAnalyticPacketObject;
			m_tcpAnalyticPacketObject = NULL;
		}
	}
	else if (id == 2)
	{

		if (NULL != mProxyAnalyticPacketObj)
		{
			delete mProxyAnalyticPacketObj;
			mProxyAnalyticPacketObj = NULL;
		}

		if(NULL == m_tcpAnalyticPacketObject)
		{
			m_tcpAnalyticPacketObject = new CAnalyticPacketClient_IDCode();
		}

		if (NULL == m_udpAnalyticPacketObject) 
		{
			m_udpAnalyticPacketObject = new CAnalyticPacketClient_IDCode();
		}

		m_communicationTCP.SetAnalyticPacket(m_tcpAnalyticPacketObject);
		m_communicationUDP.SetAnalyticPacket(m_udpAnalyticPacketObject);

	}
}

/***********************************************************************************
* 函 数 名: ConnectCallBack
* 访问权限: protected 
* 函数功能: 连接回调函数
* 参    数: 
*    [IN] CCommunicationC * pComm: 
*    [IN] int nRet: 
* 返 回 值: void
************************************************************************************/
void CCommunicationAgent::ConnectCallBack( CCommunicationC* pComm, int nRet )
{
    // 消息ID参考MsgRegister.lua
	// 连接服务器成功
	if (0 == nRet)
	{
		SendMsg(1401, 0, 0);
	}
	// 连接服务器失败
	else
	{
		SendMsg(1402, 0, 0);
	}
}

/***********************************************************************************
* 函 数 名: RecvTcpDataCallBack
* 访问权限: protected 
* 函数功能: 网络数据接收回调函数
* 参    数: 
*    [IN] CCommunicationC * pComm: 
*    [IN] char * pData: 
*    [IN] size_t nLen: 
* 返 回 值: void
************************************************************************************/
void CCommunicationAgent::RecvTcpDataCallBack( CCommunicationC* pComm, char* pData, size_t nLen )
{
    // 将网络数据压入到消息队列
    m_tcpProducerAndConsumer.Producer(pData, nLen);
}

void CCommunicationAgent::RecvUDPDataCallBack(CCommunicationUDP* pComm, char* pData, size_t nLen)
{
	// 将网络数据压入到消息队列
	m_udpProducerAndConsumer.Producer(pData, nLen);
}

/***********************************************************************************
* 函 数 名: DistCallBack
* 访问权限: protected 
* 函数功能: 断开连接的回调函数
* 参    数: 
*    [IN] CCommunicationC * pComm: 
*    [IN] int nRet: 
* 返 回 值: void
************************************************************************************/
void CCommunicationAgent::DistCallBack( CCommunicationC* pComm, int nRet )
{
    // 消息ID参考MsgRegister.lua
    SendMsg(1403, 0, 0);
}

/***********************************************************************************
* 函 数 名: SendSuccessCallBack
* 访问权限: protected 
* 函数功能: 服务器消息发送成功回调函数
* 参    数: 
*    [IN] CCommunicationC * pComm: 
*    [IN] int nRet:（无意义，未使用） 
* 返 回 值: void
************************************************************************************/
void CCommunicationAgent::SendSuccessCallBack( CCommunicationC* pComm, int nRet )
{
    // 消息ID参考MsgRegister.lua
    SendMsg(1404, 0, 0);
}

/***********************************************************************************
* 函 数 名: MessageProc
* 访问权限: protected 
* 函数功能: 消息传入lua
* 参    数: 
*    [IN] unsigned int nMsgId: 
*    [IN] const char * pData: 
*    [IN] size_t nLen: 
* 返 回 值: void
************************************************************************************/
void CCommunicationAgent::MessageProc( unsigned int nMsgId, const char* pData, size_t nLen )
{
#define ASSERT_TRY(A, LOG)       \
    if (!(A)) { CCAssert(A && LOG, ""); break; }
    // 取得lua状态机
    LuaEngine *pEngine = LuaEngine::defaultEngine();
    CCAssert(pEngine != NULL, "[CMainLogic]: TouchEventToLua lua引擎为空");
    lua_State *pLuaState = pEngine->getLuaStack()->getLuaState();
    CCAssert(pLuaState != NULL, "[CMainLogic]: TouchEventToLua lua状态机为空");

    // 取得当前栈顶位置(用于检测本函数栈操作是否异常，产生异常则用于还原堆栈)
    int nTop = lua_gettop(pLuaState);
    do 
    {
        // 取得lua中的CMainLogic,压入栈中
        lua_getglobal(pLuaState, "CLuaLogic"); 
        ASSERT_TRY(lua_type(pLuaState, -1) == LUA_TTABLE && lua_gettop(pLuaState) == nTop + 1, "CMainLogic get failed");

        // 取得Update函数压栈
        lua_getfield(pLuaState, -1, "MessageProc");
        ASSERT_TRY(lua_type(pLuaState, -1) == LUA_TFUNCTION, "CMainLogic get Update failed");

        // 压入消息参数
        lua_pushinteger(pLuaState, nMsgId);
        lua_pushlstring(pLuaState, pData, nLen);
        lua_pushinteger(pLuaState, nLen);

        // 调用函数
        int nRet = lua_pcall(pLuaState, 3, 0, 0);
        if (nRet != 0)
        {
            printf("Error:%s\n", lua_tostring(pLuaState, -1));
            break;
        }

        // 弹出栈顶CMainLogic
        lua_pop(pLuaState, 1);

        return;
    }while (0);

    // 异常处理
    lua_settop(pLuaState, nTop);

#undef ASSERT_TRY
}
/***********************************************************************************
* 函 数 名: setStartTime
* 访问权限: public 
* 函数功能: 同步时间
* 参    数: 
*    [IN] int startTime: 
* 返 回 值: void
************************************************************************************/
void CCommunicationAgent::setStartTime( int startTime )
{
    m_tcpAnalyticPacketObject->SetServStartTime(startTime);
}

void CCommunicationAgent::Destroy()
{

	CCommunicationC::EndCommnication();
	CCommunicationUDP::EndCommnication();

	m_communicationTCP.Destroy();
	m_communicationUDP.Destroy();
}