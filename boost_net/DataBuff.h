/***************************************
*文件名：DataBuff.h
*功能说明：对网络数据的缓存 封装
*作者：
*日期：2112.11.21
*修改记录：
*数据自增长功能(2012.12.12)
****************************************/
#pragma once
#include "EndianConverter64.h"

#define     MAX_DATA_LENGTH     512

class CDataBuff
{
public:
    CDataBuff(void);
    ~CDataBuff(void);
    bool    PushData(const void* pData, int length);    // push数据
    bool    PushShort(short int data);      // push short int数据
    bool    PushInt(int   data);            // push int   数据
    bool    PushFloat(float data);          // push float 数据
    bool    PushDouble(double data);        // push double 数据
    bool    PushBool(bool data);            // push bool   数据
    bool    PushString(std::string str);    // push string 数据
    bool    PushChar(char data);            // push char 数据
    bool    PushUInt64(INT_64 data);        // push uint64 数据

public:
    void*   GetData( void );                    // 获取数据
    int     GetLength( void );                  // 获取数据长度
    int     GetMaxLength( void );               // 获取最大可存储数据长度
    void    ResetData( void );                  // 数据清空
    bool    SetDataMaxLength( int length );     // 设置可存储数据最大长度

    bool    GetIsNetByteOrder(void){ return m_bIsNetByteOrder;}          // 获取是否是网络字节序
    void    SetIsNetByteOrder(bool bOrder){ m_bIsNetByteOrder = bOrder;} // 设置字节序

private:
    char    m_data[MAX_DATA_LENGTH];
    char*   m_pData;
    int     m_nLength;                      //数据实际长度
    int     m_maxLength;                    //数据最大长度

    CEndianConverter64 m_EndianConverter64; // 64位整形转换为网络字节序

    bool    m_bIsNetByteOrder;              // 是否是网络字节序
};

//NS_QIN_END