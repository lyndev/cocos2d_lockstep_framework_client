/**************************************************************
 * 文件名: CircularQueue.h
 * 功能说明: 顺序存储结构实现的循环队列操作
 * 作者：
 * 日期：2013.3.10
 * 功能列表：
 * Push()：入队，即插入元素
 * Pop()：出队，即删除元素
 * GetFront()：读取队首元素
 * IsEmpty()：判断队列是否为空
 * IsFull(): 判断队列是否已满
 * GetLength()：队列当前元素
 * SetLength(): 设置队列长度
 **************************************************************/
#pragma once

#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#ifdef __cplusplus
#ifndef NULL
#define NULL    0
#endif
#else
#ifndef NULL
#define NULL    ((void *)0)
#endif
#endif
#endif
using namespace cocos2d;

template <typename DataType, unsigned int uiNum = 10>
class CCircularQueue
{
public:
    CCircularQueue(void);
    ~CCircularQueue(void);

public:
    int GetLength();					// 循环队列的长度
    bool IsEmpty();						// 循环队列是否为空
    bool IsFull();						// 循环队列是否装满
    bool SetLength(unsigned int uiLen); // 设置队列长度
    bool Push(DataType const& data);	// 进队
    DataType &GetFront();				// 得到队首元素
    void Pop();							// 出队

private:
    DataType*       m_pData;			// 队列元素
    int            m_iFront;			// 队首
    int            m_iRear;				// 队尾
    unsigned int   m_uiNum;
};

/***********************************************************************************
 * 函数原型: template <typename DataType, unsigned int ucNum>
            CCircularQueue::CCircularQueue(void)
 * 函数功能: 构造函数
 * 参数：
 *  ［IN］: 无
 * ［OUT］: 无
 * 返回值: 
 ************************************************************************************/
template <typename DataType, unsigned int uiNum>
CCircularQueue<DataType, uiNum>::CCircularQueue(void)
{
    m_iFront = 0;
    m_iRear  = 0;
    m_pData = NULL;
    m_uiNum = uiNum + 1;
    m_pData = new DataType[m_uiNum];
    if (NULL == m_pData)
    {
        CCLOG("malloc memory failed!!!");
    }
}

/***********************************************************************************
 * 函数原型: template <typename DataType, unsigned int ucNum>
            bool CCircularQueue::SetLength(unsigned int uiLen)
 * 函数功能: 设置队列长度
 * 参数：
 *  ［IN］: 
 *   unsigned int uiLen:新的队列长度值
 * ［OUT］: 无
 * 返回值: 
 *    true:设置成功
 *   false:设置失败
 ************************************************************************************/
template <typename DataType, unsigned int uiNum>
bool CCircularQueue<DataType, uiNum>::SetLength(unsigned int uiLen)
{
    if (true == IsEmpty())
    {
        if (NULL != m_pData)
        {
            delete[] m_pData;
            m_pData = NULL;
        }
        m_pData = new DataType[uiLen+1];
        if (NULL == m_pData)
        {
            CCLOG("malloc memory failed!!!");
            return false;
        }
        else
        {
            m_uiNum = uiLen+1;
        }
    }
    else
    {
        return false;
    }
    return true;
}

/***********************************************************************************
 * 函数原型: template <typename DataType, unsigned int ucNum>
 *          CCircularQueue::~CCircularQueue(void)
 * 函数功能: 删除所有队列数据
 * 参数：
 *  ［IN］: 无
 * ［OUT］: 无
 * 返回值: 
 ************************************************************************************/
template <typename DataType, unsigned int uiNum>
CCircularQueue<DataType, uiNum>::~CCircularQueue(void)
{
    if (NULL != m_pData)
    {
        delete[] m_pData;
    }
}

/***********************************************************************************
 * 函数原型: template <typename DataType, unsigned int uiNum>
 *          int CCircularQueue::GetLength()
 * 函数功能: 获取队列中元素个数
 * 参数：
 *  ［IN］: 无
 * ［OUT］: 无
 * 返回值: 当前队列元素个数
 ************************************************************************************/
template <typename DataType, unsigned int uiNum>
int CCircularQueue<DataType, uiNum>::GetLength()
{
    return ((m_iRear - m_iFront + m_uiNum) % m_uiNum);
}

/***********************************************************************************
 * 函数原型: template <typename DataType, unsigned int uiNum>
 *          bool CCircularQueue::IsEmpty()
 * 函数功能: 判断队列是否为空
 * 参数：
 *  ［IN］: 无
 * ［OUT］: 无
 * 返回值: 
 *       true：为空
 *       false：不为空
 ************************************************************************************/
template <typename DataType, unsigned int uiNum>
bool CCircularQueue<DataType, uiNum>::IsEmpty()
{
    if (m_iFront == m_iRear)
    {
        return true;
    }
    return false;
}

/***********************************************************************************
 * 函数原型: template <typename DataType, unsigned int uiNum>
 *          bool CCircularQueue::IsFull()
 * 函数功能: 判断队列是否装满
 * 参数：
 *  ［IN］: 
 * ［OUT］: 
 * 返回值: 
 *       true：装满
 *       false：没装满
 ************************************************************************************/
template <typename DataType, unsigned int uiNum>
bool CCircularQueue<DataType, uiNum>::IsFull()
{
    if (m_iFront == (m_iRear + 1) % m_uiNum)
    {
        return true;
    }
    return false;
}

/***********************************************************************************
 * 函数原型: template <typename DataType, unsigned int uiNum>
 *          void CCircularQueue::Push(DataType const& data)
 * 函数功能: 将元素插入到队列
 * 参数：
 *  ［IN］: 
 *    DataType data: 待插入元素
 * ［OUT］: 
 * 返回值: 
 ************************************************************************************/
template <typename DataType, unsigned int uiNum>
bool CCircularQueue<DataType, uiNum>::Push(DataType const& data)
{
    if (true == IsFull())
    {
        CCLOG("CCircularQueue::Push the queue is full!!!");
        return false;
    }
    if (NULL == m_pData)
    {
        CCLOG("CCircularQueue::Push the queue memory is null!!!");
        return false;
    }
    int nTmp = ((m_iRear + 1) % m_uiNum);
    m_pData[nTmp] = data;
    m_iRear = nTmp;
    return true;
}

/***********************************************************************************
 * 函数原型: template <typename DataType, unsigned int uiNum>
 *          DataType const& CCircularQueue::GetFront()
 * 函数功能: 读取队首元素
 * 参数：
 *  ［IN］: 
 * ［OUT］: 
 * 返回值: 如果队里不为空，返回队首元素的值，否则返回临时对象
 ************************************************************************************/
template <typename DataType, unsigned int uiNum>
DataType & CCircularQueue<DataType, uiNum>::GetFront()
{
    if (true == IsEmpty())
    {
        printf("CCircularQueue::GetFront the queue is empty!!!");
    	static DataType temp;
        return temp;
    }
    if (NULL == m_pData)
    {
        CCLOG("CCircularQueue::GetFront the queue memory is null!!!");
        static DataType temp;
        return temp;
    }
    return m_pData[(m_iFront + 1) % m_uiNum];
}


/***********************************************************************************
 * 函数原型: template <typename DataType, unsigned int uiNum>
 *          void CCircularQueue::Pop()
 * 函数功能: 弹出队首元素
 * 参数：
 *  ［IN］: 无
 * ［OUT］: 无
 * 返回值:  如果队里不为空，返回队首元素的值并删除队首元素，否则返回临时对象
 ************************************************************************************/
template <typename DataType, unsigned int uiNum>
void CCircularQueue<DataType, uiNum>::Pop()
{
    static DataType temp;
    if (true == IsEmpty())
    {
        CCLOG("CCircularQueue::Pop the queue is empty!!!");
        return ;
    }
    m_iFront = ((m_iFront + 1) % m_uiNum);
}