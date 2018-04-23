/**************************************************************
 * 文件名: ProducerAndConsumer.h
 * 功能说明: 生产者、消费者模型封装
 * 作者：
 * 日期：2012.7.9
 * 修改记录
 **************************************************************/
#pragma once

#include "ThreadSafeQueue.h"

class CProducerAndConsumer
{
private:
	// 生产者生产的产品
	struct SProduct
	{
		void *     pProduct;                                         // 产品数据
		size_t     szLen;                                            // 产品长度
	};

public:
	CProducerAndConsumer(void);
	virtual ~CProducerAndConsumer(void);

public:
	void Producer(void * pProduct, size_t szLen, bool isHighPri = false);               // 生产者接口
	bool Consumer(void * & pProduct, size_t & szLen);           // 消费者接口
	void GarbageHandle(void * & pProduct, size_t & szLen);      // 垃圾处理接口

protected:
	void Clear();                                               // 清除未处理的产品

protected:
	CTSQueue<SProduct>            m_queue;                      // 产品待消费队列
	bool                          m_bEndProducer;               // 是否停止生产产品
};
