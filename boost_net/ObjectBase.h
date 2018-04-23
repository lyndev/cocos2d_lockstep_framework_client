/**************************************************************
 * 文件名: ObjectBase.h
 * 功能说明: 对象基类
 * 作者：
 * 日期：2012.7.10
 * 说明：
 *     1、为了便于程序处理，抽象一个空的基类，我们写的所有类，都从这个类继承
 *     2、本来想命名为CObject,但是被IDE阻止，说CObject是保留类名
 * 修改记录
 *
 **************************************************************/

#pragma once

class CObjectBase
{
public:
	virtual ~CObjectBase() { }
};

class EventBase; 

typedef void (CObjectBase::*CALL_BACK_EVENT)(EventBase & e);

#define EVENT_selector(_SELECTOR) (CALL_BACK_EVENT)(&_SELECTOR)