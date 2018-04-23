/***************************************
*文件名：DataBuff.cpp
*功能说明：对网络数据的缓存 封装
*作者：
*日期：2112.11.21
*修改记录：
*数据自增长功能(2012.12.12)
****************************************/
#include "DataBuff.h"

#ifdef __linux__
#include <arpa/inet.h>
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include<netinet/in.h> 
#endif

using namespace std;
USING_NS_CC;

CDataBuff::CDataBuff()
{
    memset(m_data, 0, sizeof(m_data));
    m_pData = m_data;
    m_nLength = 0;
    m_maxLength = MAX_DATA_LENGTH;
    m_bIsNetByteOrder = true;
}

CDataBuff:: ~CDataBuff()
{
    if (NULL != m_pData && m_maxLength > MAX_DATA_LENGTH)
    {
        delete []m_pData;
        m_pData = NULL;
    }
}

/************************************************************************************
* 函数原型: bool	PushData(const void* pData, int length);
* 函数功能: 将网络数据包缓存起
* 参数：任意类型数据
*  ［IN]
*  const void* pData 数据
* ［OUT］
* int length: 需要push数据的长度
* 返回值: 返回push数据是否成功
*************************************************************************************/
bool CDataBuff::PushData(const void* pData, int length)
{
    // 1、指针检测
    if (pData == NULL)
    {
        printf("Null pointer error PushData(const void* pData, int length)");
        return false;
    }

    // 2、能否完整地放入
    if (m_nLength + length <= m_maxLength)
    {
        memcpy(&(m_pData[m_nLength]), (char*)pData, length) ;
        m_nLength += length;
        return true;
    }
    else if (length>=MAX_DATA_LENGTH)
    {
        // 当前空间不能完整存储并且需要插入数据长度大于512字节
        m_maxLength+=length;
        char* pTmp = new char[m_maxLength];
        if (NULL == pTmp)
        {
			printf("Null pointer error;new space failedPushData(const void* pData, int length)");
            return false;
        }
        // 新建存储空间置0
        memset(pTmp, 0, m_maxLength);
        // 原始数据拷贝
        memcpy(pTmp, m_pData, m_nLength);
        // 数据指针重置
        m_pData = pTmp;
        // 拷贝当前数据
        memcpy(&(m_pData[m_nLength]), (char*)pData, length);
        // 当前数据长度记录
        m_nLength += length;
        return true;
    }
    else
    {
        // 最大长度增加512
        m_maxLength += MAX_DATA_LENGTH;
        char* pTmp2 = new char[m_maxLength];
        if (NULL == pTmp2)
        {
			printf("Null pointer error;new space failedPushData(const void* pData, int length)");
            return false;
        }
        // 新建存储空间置0
        memset(pTmp2, 0, m_maxLength);
        // 原始数据拷贝
        memcpy(pTmp2, m_pData, m_nLength);
        // 数据指针重置
        m_pData = pTmp2;
        // 拷贝当前数据
        memcpy(&(m_pData[m_nLength]), (char*)pData, length);
        // 当前数据长度记录
        m_nLength += length;
        return true;

    }

}

/************************************************************************************
* 函数原型: bool	PushData(short int data);
* 函数功能: short int data 数据
* 参数：
*  ［IN]
*  short int data 数据
* ［OUT］
* 返回值: 返回push数据是否成功
*************************************************************************************/
bool CDataBuff::PushShort(short int data)
{
    if (m_bIsNetByteOrder)
    {
        data = htons(data);
    }
    return PushData(&data, sizeof(short int));
}

/************************************************************************************
* 函数原型: bool   PushData(int   data);
* 函数功能: push int 行数据
* 参数：
*  ［IN]
*  int   data 数据
* ［OUT］
* 返回值: 返回push数据是否成功
*************************************************************************************/
bool CDataBuff::PushInt(int data )
{
    if (m_bIsNetByteOrder)
    {
        data = htonl(data);
    }
    return PushData(&data , sizeof(int));
}
/************************************************************************************
* 函数原型: bool   PushUInt64(INT_64 data);
* 函数功能: push uint64 行数据
* 参数：
*  ［IN]
*  INT_64   data 数据
* ［OUT］
* 返回值: 返回push数据是否成功
*************************************************************************************/
bool CDataBuff::PushUInt64(INT_64 data)
{
    INT_64 data2 = data;
    if (m_bIsNetByteOrder)
    {
        data2 = m_EndianConverter64.hton64(data);
    }
    return PushData(&data2 , sizeof(INT_64));
}

/************************************************************************************
* 函数原型: bool   PushData(float data);
* 函数功能: push float  数据
* 参数：
*  ［IN]
*  float data 数据
* ［OUT］
* 返回值: 返回push数据是否成功
*************************************************************************************/
bool CDataBuff::PushFloat(float data)
{
    return PushData(&data, sizeof(float));
}

/************************************************************************************
* 函数原型: bool   PushData(double data);
* 函数功能: push double 数据
* 参数：
*  ［IN]
*  double data 数据
* ［OUT］
* 返回值: 返回push数据是否成功
*************************************************************************************/
bool CDataBuff::PushDouble(double data)
{
    return PushData(&data, sizeof(double));
}

/************************************************************************************
* 函数原型: bool   PushData(bool data);
* 函数功能: push bool 数据
* 参数：
*  ［IN]
*  bool data 数据
* ［OUT］
* 返回值: 返回push数据是否成功
*************************************************************************************/
bool CDataBuff::PushBool(bool data)
{
    return PushData(&data, sizeof(bool));
}

/************************************************************************************
* 函数原型: bool   PushData(std::string str);
* 函数功能: push std::string  数据
* 参数：
*  ［IN]
*  std::string str 数据
* 注意：这里先将string类型数据长度push进去，成功后将str 数据push进去
* 返回值: 返回push数据是否成功
*************************************************************************************/
bool CDataBuff::PushString(std::string str)
{
    int len = str.size();

    if (PushInt(len))
    {
        const char* pStr = str.c_str();
        return PushData(pStr, len);
    }
    return false;
}

/************************************************************************************
* 函数原型: bool   PushData(char data);
* 函数功能: push   char  数据
* 参数[IN]：char data 数据
*  
* 返回值: 返回push数据是否成功
*************************************************************************************/
bool CDataBuff::PushChar(char data)
{
    return PushData(&data, sizeof(char));
}

/************************************************************************************
* 函数原型: void*  GetData( void );
* 函数功能: 获取缓存数据
* 参数：
*  
* 
* 返回值: 返回缓存的数据
*************************************************************************************/
void* CDataBuff::GetData()
{

    return m_pData;
}
/************************************************************************************
* 函数原型: int    GetLength( void );
* 函数功能: 获取当前缓存数据长度
* 参数：
*  
* 返回值: 缓存数据的实际长度
*************************************************************************************/
int CDataBuff::GetLength()
{
    return m_nLength;
}
/************************************************************************************
* 函数原型: int        GetMaxLength( void );
* 函数功能: 获取缓存区最大 可 缓存数据的最大长度
* 参数：
*  
* 返回值: 可以缓存最大数据长度
*************************************************************************************/
int CDataBuff::GetMaxLength()
{
    return m_maxLength;
}

/************************************************************************************
* 函数原型: void   ResetData( void );
* 函数功能: 清楚缓存区数据 
* 参数：
* 注意：当最大缓存长度大于默认最大缓存长度，重置后会恢复到默认最大缓存长度
* 返回值: 
*************************************************************************************/
void CDataBuff::ResetData()
{
    memset(m_data, 0, MAX_DATA_LENGTH);
    if (m_maxLength > MAX_DATA_LENGTH)
    {
        // 释放 堆区 缓存，并重置最大缓存区长度
        delete []m_pData;
        m_maxLength = MAX_DATA_LENGTH;
    }
    m_pData = m_data;
    m_nLength = 0;
}

/************************************************************************************
* 函数原型: bool SetDataMaxLength( int length );
* 函数功能: 设置修改能缓存数据的 最大值
* 参数：
*  
* 返回值: 
*************************************************************************************/
bool CDataBuff::SetDataMaxLength(int length)
{
    // 重置长度小于当前最大长度，直接false,考虑到存储空间不足时，
    //应该向上扩展，不能逆向缩小
    if (length < m_maxLength)
    {
		printf("new space failed,must up extend, not reverse scale ,SetDataMaxLength(int length)");
        return false;
    }
    if (length >m_maxLength)
    {

        m_maxLength = length;
        char* pTmp = new char[m_maxLength];
        if (NULL == pTmp)
        {
			printf("SetDataMaxLength(int length) new space failed");
            return false;
        }
        memset(pTmp, 0, length);
        memcpy(pTmp, m_pData, m_nLength);
        m_pData = pTmp;
    }
    return true;
}

//NS_QIN_END