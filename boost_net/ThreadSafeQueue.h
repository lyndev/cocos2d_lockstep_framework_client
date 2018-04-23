/**************************************************************
 * 文件名: ThreadSafeQueue.h
 * 功能说明: 基于STL队列的线程安全队列
 * 作者：
 * 日期：2012.7.9
 * 功能列表：
 * Push()：入队，即插入元素
 * PushFront():入队，插入元素到头
 * Pop()：出队，即删除元素
 * Front()：读取队首元素
 * Back()：读取队尾元素
 * Empty()：判断队列是否为空
 * Size()：队列当前元素
 **************************************************************/

#pragma once

#include "boostlibs/boost/thread/thread.hpp"
#include <list>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

template<typename T>
class CTSQueue
{

public:
	void    Push(T data);                                                // 入队 即插入元素
    void    PushFront(T const&data);                                     // 入队，插入到头节点
	T       Pop();                                                       // 出队，即删除元素
	T       Front();                                                     // 读取队首元素
	T       Back();                                                      // 读取队尾元素
    bool    Empty();                                                     // 判断队列是否为空
    size_t  Size();                                                      // 队列当前元素个数

protected:
	list<T>                   m_queue;                                   // STL 链表
	boost::mutex              m_mutex;                                   // 互斥同步对象
	T                         m_tmpData;                                 // 出现异常后，返回的临时数据对象
};

/***********************************************************************************
 * 函数原型: template<typename T>
 *           void CTSQueue<T>::PushFront(T const&data)
 * 函数功能: 将元素插入到队列
 * 参数：
 *  ［IN］: 
 *    T data: 待插入元素的值
 * ［OUT］: 
 * 返回值: 
 ************************************************************************************/
template<typename T>
void CTSQueue<T>::PushFront( T const&data )
{
	m_mutex.lock();
	m_queue.push_front(data);
	m_mutex.unlock();
}

/***********************************************************************************
 * 函数原型: template<typename T>
 *           void CTSQueue<T>::Push(T data)
 * 函数功能: 将元素插入到队列
 * 参数：
 *  ［IN］: 
 *    T data: 待插入元素的值
 * ［OUT］: 
 * 返回值: 
 ************************************************************************************/
template<typename T>
void CTSQueue<T>::Push(T data)
{
	m_mutex.lock();
	m_queue.push_back(data);
	m_mutex.unlock();
}

/***********************************************************************************
 * 函数原型: template<typename T>
 *           T CTSQueue<T>::Pop()
 * 函数功能: 弹出对头元素
 * 参数：
 *  ［IN］: 无
 * ［OUT］: 无
 * 返回值:  如果队里不为空，返回对头元素的值，否则返回临时对象
 ************************************************************************************/
template<typename T>
T CTSQueue<T>::Pop()
{
	if (true == Empty())
	{
		// 日志记录
		CCLOG("CTSQueue<T>::Pop(): queue is empty");

		return m_tmpData;
	}

	m_mutex.lock();
	T data = Front();
	m_queue.pop_front();
	m_mutex.unlock();

	return data;
}

/***********************************************************************************
 * 函数原型: template<typename T>
 *           T CTSQueue<T>::Front()
 * 函数功能: 读取队首元素
 * 参数：
 *  ［IN］: 无
 * ［OUT］: 无
 * 返回值:  如果队里不为空，返回对头元素的值，否则返回临时对象
 ************************************************************************************/
template<typename T>
T CTSQueue<T>::Front()
{
	if (true == Empty())
	{
		// 日志记录
		CCLOG("CTSQueue<T>::Front: queue is empty");

		return m_tmpData;
	}

	return m_queue.front();
}

/***********************************************************************************
 * 函数原型: template<typename T>
 *           T CTSQueue<T>::Back()
 * 函数功能: 读取队尾元素
 * 参数：
 *  ［IN］: 无
 * ［OUT］: 无
 * 返回值:  如果队里不为空，返回对尾元素的值，否则返回临时对象
 ************************************************************************************/
template<typename T>
T CTSQueue<T>::Back()
{
	if (true == Empty())
	{
		// 日志记录
		CCLOG("CTSQueue<T>::Back(): queue is empty");

		return m_tmpData;
	}

	return m_queue.back();
}

/***********************************************************************************
 * 函数原型: template<typename T>
 *           bool CTSQueue<T>::Empty()
 * 函数功能:  判断队列是否为空
 * 参数：
 *  ［IN］: 无
 * ［OUT］: 无
 * 返回值:  如果队里不为空，返回 true，否则返回false
 ************************************************************************************/
template<typename T>
bool CTSQueue<T>::Empty()
{
	return m_queue.empty();
}

/***********************************************************************************
 * 函数原型: template<typename T>
 *           size_t CTSQueue<T>::Size()
 * 函数功能:  队列当前元素个数
 * 参数：
 *  ［IN］: 无
 * ［OUT］: 无
 * 返回值:  返回队列长度
 ************************************************************************************/
template<typename T>
size_t CTSQueue<T>::Size()
{
	return m_queue.size();
}