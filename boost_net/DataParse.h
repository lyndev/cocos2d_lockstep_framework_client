/**************************************************************
* 
* 文件名: DataParse.h
* 功能说明: 解析二进制数据
* 作者：赵魁
* 日期：2012.11.26
* --------------------------------------------------------------------------------
* 修改记录 :
 日期       版本    作者    内容
 13-02-21   1.0     Joeson  添加带参构造函数
**************************************************************/
#pragma once


#include "Utility.h"
#include "EndianConverter64.h"
 
//NS_QIN_BEGIN

class CDataParse
{
public:
	CDataParse(void);
    CDataParse(void *pData, unsigned int uDataLen, bool isNetOrder = true);
	~CDataParse(void);

public:
	void                    SetData(void * pData, unsigned int nDataLen, bool bByteOrder = true);       //设置数据
	void *                  GetData(void);                                                              //获得当前读取位置数据		
	unsigned int            GetLength(void){ return m_nReadPos;};                                       //获取已读取数据长度
	bool					GetByteOrder(void);                                                         //获取字节序
	void					SetByteOrder(bool bOrder);                                                  //设置字节序
	void                    SetCurReadPos(int nPos) {m_nReadPos = nPos;}                                // 设置当前读取位置
    unsigned int            GetAllLength(){ return m_nDataLen; }                                        // 获取总长度
public:
	unsigned int            ReadUINT(void);                                                     //读取32位无符号整数
	unsigned char           ReadUINT8(void);                                                    //读取无符号整数
	unsigned short          ReadUINT16(void);                                                   //读取无符号整数
	int                     ReadINT(void);                                                      //读取32位整数
	char					ReadINT8(void);                                                     //读取整数
	short					ReadINT16(void);                                                    //读取整数
	bool					ReadBOOL(void);                                                     //读取布尔值
	float					ReadFLOAT(void);                                                    //读取浮点数
	INT_64					ReadINT_64(void);                                                   //读取64位无符号整数
	std::string             ReadString(void);                                                   //读取字符
	void					ReadString(char * & pPos , int & len);                              //读取字符串
    std::string             ReadStringByUint16();                                               //读取字符串，前2个字节存string的长度

	bool                    End();                                                              //是否解析完

private:
	bool                m_bNetByteOrder;                                                //是否是网络字节序
	char *              m_pData;                                                        //数据
	unsigned int		m_nReadPos;                                                     //当前读取位置
	unsigned int		m_nDataLen;                                                     //数据长度
	CEndianConverter64	m_EndianCvt64;                                                  //64位数据转换器
};


//NS_QIN_END