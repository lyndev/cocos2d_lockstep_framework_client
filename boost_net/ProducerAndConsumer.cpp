#include "ProducerAndConsumer.h"
#include "cocos2d.h"

using namespace cocos2d;
CProducerAndConsumer::CProducerAndConsumer(void)
{
	m_bEndProducer = false;
}

CProducerAndConsumer::~CProducerAndConsumer(void)
{
	Clear();
}

/***********************************************************************************
 * 函数原型: void Producer(void * pProduct, size_t szLen)
 * 函数功能: 生产者接口
 * 参数：
 *  ［IN］
 *       void * pProduct: 产品内容
 *       size_t szLen： 产品长度
 *       isHighPri: 是否最高优先级
 * ［OUT］: 无
 * 返回值: 
 ************************************************************************************/
void CProducerAndConsumer::Producer(void * pProduct, size_t szLen, bool isHighPri)
{
	if (true == m_bEndProducer)
	{  return;	}

	// 生产产品
	SProduct product;
	product.szLen = szLen;
	product.pProduct = new char[product.szLen];

	if (NULL == product.pProduct)
	{
		// 记录日志
		CCLOG("CProducerAndConsumer::Producer: alloc memory failed");

		// 返回
		return;
	}

	::memcpy(product.pProduct, pProduct, product.szLen);

	// 将产品放入待消费队列
    if (isHighPri)
    {
        m_queue.PushFront(product);
    }
    else
    {
        m_queue.Push(product);
    }
}

/***********************************************************************************
 * 函数原型: bool Consumer(void * pProduct, size_t szLen)
 * 函数功能: 生产者接口
 * 参数：
 *  ［IN］: 无
 * ［OUT］: 
 *       void * & pProduct: 待消费产品内容
 *       size_t & szLen： 待消费产品长度
 * 返回值: 如果有产品待消费，返回true, 否则返回false
 ************************************************************************************/
bool CProducerAndConsumer::Consumer(void * & pProduct, size_t & szLen)
{
	if (true == m_queue.Empty())
	{
		pProduct = NULL;
		szLen = 0;

		return false;
	}

	// 获取一个待消费产品
	SProduct product = m_queue.Pop();
	pProduct = product.pProduct;
	szLen = product.szLen;
	
	return true;
}

/***********************************************************************************
 * 函数原型: void GarbageHandle(void * pProduct, size_t szLen)
 * 函数功能: 垃圾处理回收
 * 参数：
 *  ［IN］:同OUT参数
 * ［OUT］: 
 *       void * & pProduct: 产品内容
 *       size_t & szLen: 产品长度
 * 返回值: 
 ************************************************************************************/
void CProducerAndConsumer::GarbageHandle(void * & pProduct, size_t & szLen)
{
	delete [] pProduct;
	pProduct = NULL;
	szLen = 0;
}

/***********************************************************************************
 * 函数原型: void Clear()
 * 函数功能: 清除未处理的产品
 * 参数：
 *  ［IN］:
 * ［OUT］: 
 * 返回值: 
 ************************************************************************************/
void CProducerAndConsumer::Clear()
{
	m_bEndProducer = true;
	char * pMsg = NULL;
	size_t nLen = 0;

	while (true)
	{
		// 消费产品
		Consumer((void * &)pMsg, nLen);
		if (NULL == pMsg)
		{
			break;
		}

		// 回收产品
		GarbageHandle((void * &)pMsg, nLen);
	}
}