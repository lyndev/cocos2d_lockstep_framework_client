/****************************************************************************************
文 件 名: ZlibParse.h
作    者: 张祥围
版    本: 
完成日期:
功能描述: 数据解压
其它相关: 
修改记录: 
****************************************************************************************/
#pragma once

class CZlibParse
{
public:
    CZlibParse();
    CZlibParse(unsigned int reserveSize);
    CZlibParse(unsigned int revSize, unsigned int groSize);
    ~CZlibParse();

public:
    void            setReserveBufSize(unsigned int size);
    void            setSelfGrowthSize(unsigned int size);
    void            setReserveAndSelfGrowthSize(unsigned int revSize, unsigned int groSize);
    unsigned char   *getOutputBuf();
    unsigned long   getOutputBufSize();
    bool            Uncompress(const unsigned char *src, unsigned long srcSize);
    void            freeOutputBuf();
    
private:
    enum{ DEFAULT_RESERVE_BUFSIZE = 0xff };
    unsigned char *m_pOutputBuf;
    unsigned long m_OutputBufSize;
    unsigned int  m_reserveBufSize;
    unsigned int  m_selfGrowthSize;
    unsigned int  m_bufSizeMax;
};
