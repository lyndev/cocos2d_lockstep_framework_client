/****************************************************************************************
Copyright (C), 2012，Haowan123 Tech. Co., Ltd.
文 件 名: ZlibParse.cpp
作    者: 张祥围
版    本: 
完成日期:
功能描述: 数据解压
其它相关: 
修改记录: 
****************************************************************************************/
#include "ZlibParse.h"
#include "zlib.h"

#define NULL    0

CZlibParse::CZlibParse()
{
    m_pOutputBuf     = NULL;
    m_reserveBufSize = DEFAULT_RESERVE_BUFSIZE;
    m_bufSizeMax     = m_reserveBufSize;
    m_OutputBufSize  = m_bufSizeMax;
}

CZlibParse::CZlibParse(unsigned int reserveSize)
{
    setReserveAndSelfGrowthSize(reserveSize, reserveSize);
    m_pOutputBuf    = NULL;
    m_bufSizeMax    = m_reserveBufSize;
    m_OutputBufSize = m_bufSizeMax;
}

CZlibParse::CZlibParse(unsigned int revSize, unsigned int groSize)
{
    setReserveAndSelfGrowthSize(revSize, groSize);
    m_pOutputBuf    = NULL;
    m_bufSizeMax    = m_reserveBufSize;
    m_OutputBufSize = m_bufSizeMax;
}

CZlibParse::~CZlibParse()
{
    freeOutputBuf();
}


/************************************************************************
* 功能：获取输入缓冲区的buf（数据解压后的内存位置）
* 参数：无
* 返回：无
************************************************************************/
unsigned char *CZlibParse::getOutputBuf()
{
    return m_pOutputBuf;
}


/************************************************************************
* 功能：获取输入缓冲区长度（数据解压后的长度）
* 参数：无
* 返回：无
************************************************************************/
unsigned long CZlibParse::getOutputBufSize()
{
    return m_OutputBufSize;
}


/************************************************************************
* 功能：设置预留空间大小
* 参数：size: 预留空间大小
* 返回：无
************************************************************************/
void CZlibParse::setReserveBufSize(unsigned int size)
{
    if(size != 0)
    {
        m_reserveBufSize = size;
    }
}


/************************************************************************
* 功能：设置空间不足时自增长空间大小                               
************************************************************************/
void CZlibParse::setSelfGrowthSize(unsigned int size)
{
    if(size != 0)
    {
        m_selfGrowthSize = size;
    }
}


/************************************************************************
* 功能：设置预留空间和空间不足时自增长空间大小
************************************************************************/
void CZlibParse::setReserveAndSelfGrowthSize(unsigned int revSize, unsigned int groSize)
{
    setReserveBufSize(revSize);
    setSelfGrowthSize(groSize);
}


/************************************************************************
* 功能：解压数据
* 参数：src:数据源
*       sreSize: 源数据大小
* 返回：true： 解压成功
*       false: 解压失败
************************************************************************/
bool CZlibParse::Uncompress(const unsigned char *src, unsigned long srcSize)
{
    int ret = Z_BUF_ERROR;

    //如果预留空间比现在的最大空间大，则重设最大空间大小
    if(m_reserveBufSize > m_bufSizeMax || srcSize > m_bufSizeMax)
    {
        if(m_pOutputBuf != NULL)
        {
            delete []m_pOutputBuf;
            m_pOutputBuf = NULL;
        }

        m_bufSizeMax = m_reserveBufSize > srcSize ? m_reserveBufSize : srcSize;
    }
    
    //如果输出缓冲区未分配内存空间，则分配
    if(m_pOutputBuf == NULL)
    {
        m_pOutputBuf = new unsigned char[m_bufSizeMax];
    }

    //解压
    if(m_pOutputBuf != NULL)
    {
        m_OutputBufSize = m_bufSizeMax;
        ret = uncompress(m_pOutputBuf, &m_OutputBufSize, src, srcSize);
        
        while(ret == Z_BUF_ERROR)       //Z_BUF_ERROR解压输出缓冲区内存不够
        {
            delete []m_pOutputBuf;
            m_pOutputBuf = NULL;
            m_bufSizeMax    += m_selfGrowthSize;//m_reserveBufSize;
            m_pOutputBuf    = new unsigned char[m_bufSizeMax];
            m_OutputBufSize = m_bufSizeMax;
            
            if(m_pOutputBuf != NULL)
            {
                ret = uncompress(m_pOutputBuf, &m_OutputBufSize, src, srcSize);
            }
            else
            {
                break;
            }
        }
    }

    if(ret != Z_OK)
    {
        m_OutputBufSize = 0;
        if(m_pOutputBuf != NULL)
        {
            delete []m_pOutputBuf;
            m_pOutputBuf = NULL;
        }
    }

    return ret == Z_OK;
}


/************************************************************************
* 功能：释放输出缓冲区
* 参数：无
* 返回：无
************************************************************************/
void CZlibParse::freeOutputBuf()
{
    if(m_pOutputBuf != NULL)
    {
        delete []m_pOutputBuf;
        m_pOutputBuf = NULL;
    }

    m_OutputBufSize = 0;
}
