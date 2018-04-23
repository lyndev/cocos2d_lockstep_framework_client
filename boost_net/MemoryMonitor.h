/****************************************************************************
Copyright (c) 2012, companyaddress
作者: 
功能: 内存使用情况
时间: 
其他: 
--------------------------------------------------------------------------------
* 修改记录 :
 日期       版本    作者    内容
 13-05-09   1.0     Joeson  安卓下，在单独的线程检测内存使用情况
****************************************************************************/
#pragma once
#include "Utility.h"

class CMemoryMonitor
{
public:
	CMemoryMonitor(){s_pSharedInstance  = this;}
	~CMemoryMonitor(){s_pSharedInstance = NULL;}
    static CMemoryMonitor* GetInstance(){return s_pSharedInstance;}
	double GameUsedMemory();
	double AvailableMemory();
	void   PrintMemory(char* msg);
private:
    static CMemoryMonitor* s_pSharedInstance;
};