/*
** Lua binding: CCommunicationAgent
** Generated automatically by tolua++-1.0.92 on 03/12/18 15:28:25.
*/

/****************************************************************************
 Copyright (c) 2011 cocos2d-x.org

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

extern "C" {
#include "scripting/lua-bindings/manual/tolua_fix.h"
}

#include <map>
#include <string>
#include "cocos2d.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "audio/include/SimpleAudioEngine.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace CocosDenshion;

/* Exported function */
TOLUA_API int  tolua_CCommunicationAgent_open (lua_State* tolua_S);

#include "Boost_Net/CommunicationAgent.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CCommunicationAgent");
}

/* method: GetInstance of class  CCommunicationAgent */
#ifndef TOLUA_DISABLE_tolua_CCommunicationAgent_CCommunicationAgent_GetInstance00
static int tolua_CCommunicationAgent_CCommunicationAgent_GetInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CCommunicationAgent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CCommunicationAgent* tolua_ret = (CCommunicationAgent*)  CCommunicationAgent::GetInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCommunicationAgent");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendMsg of class  CCommunicationAgent */
#ifndef TOLUA_DISABLE_tolua_CCommunicationAgent_CCommunicationAgent_SendMsg00
static int tolua_CCommunicationAgent_CCommunicationAgent_SendMsg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCommunicationAgent",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCommunicationAgent* self = (CCommunicationAgent*)  tolua_tousertype(tolua_S,1,0);
  unsigned int nMsgId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  const char* pData = ((const char*)  tolua_tostring(tolua_S,3,0));
  unsigned int nLen = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendMsg'", NULL);
#endif
  {
   self->SendMsg(nMsgId,pData,nLen);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendMsg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendMsgPriority of class  CCommunicationAgent */
#ifndef TOLUA_DISABLE_tolua_CCommunicationAgent_CCommunicationAgent_SendMsgPriority00
static int tolua_CCommunicationAgent_CCommunicationAgent_SendMsgPriority00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCommunicationAgent",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCommunicationAgent* self = (CCommunicationAgent*)  tolua_tousertype(tolua_S,1,0);
  unsigned int nMsgId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  const char* pData = ((const char*)  tolua_tostring(tolua_S,3,0));
  unsigned int nLen = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
  bool bHigh = ((bool)  tolua_toboolean(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendMsgPriority'", NULL);
#endif
  {
   self->SendMsgPriority(nMsgId,pData,nLen,bHigh);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendMsgPriority'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendMsgToServer of class  CCommunicationAgent */
#ifndef TOLUA_DISABLE_tolua_CCommunicationAgent_CCommunicationAgent_SendMsgToServer00
static int tolua_CCommunicationAgent_CCommunicationAgent_SendMsgToServer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCommunicationAgent",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCommunicationAgent* self = (CCommunicationAgent*)  tolua_tousertype(tolua_S,1,0);
  unsigned int nMsgId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  const char* pData = ((const char*)  tolua_tostring(tolua_S,3,0));
  unsigned int nLen = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendMsgToServer'", NULL);
#endif
  {
   int tolua_ret = (int)  self->SendMsgToServer(nMsgId,pData,nLen);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendMsgToServer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendUDPMsgToServer of class  CCommunicationAgent */
#ifndef TOLUA_DISABLE_tolua_CCommunicationAgent_CCommunicationAgent_SendUDPMsgToServer00
static int tolua_CCommunicationAgent_CCommunicationAgent_SendUDPMsgToServer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCommunicationAgent",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCommunicationAgent* self = (CCommunicationAgent*)  tolua_tousertype(tolua_S,1,0);
  unsigned int nMsgId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  const char* pData = ((const char*)  tolua_tostring(tolua_S,3,0));
  unsigned int nLen = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendUDPMsgToServer'", NULL);
#endif
  {
   int tolua_ret = (int)  self->SendUDPMsgToServer(nMsgId,pData,nLen);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendUDPMsgToServer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Connect of class  CCommunicationAgent */
#ifndef TOLUA_DISABLE_tolua_CCommunicationAgent_CCommunicationAgent_Connect00
static int tolua_CCommunicationAgent_CCommunicationAgent_Connect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCommunicationAgent",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCommunicationAgent* self = (CCommunicationAgent*)  tolua_tousertype(tolua_S,1,0);
  const char* IP = ((const char*)  tolua_tostring(tolua_S,2,0));
  unsigned int port = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Connect'", NULL);
#endif
  {
   int tolua_ret = (int)  self->Connect(IP,port);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Connect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ConnectUDP of class  CCommunicationAgent */
#ifndef TOLUA_DISABLE_tolua_CCommunicationAgent_CCommunicationAgent_ConnectUDP00
static int tolua_CCommunicationAgent_CCommunicationAgent_ConnectUDP00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCommunicationAgent",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCommunicationAgent* self = (CCommunicationAgent*)  tolua_tousertype(tolua_S,1,0);
  const char* IP = ((const char*)  tolua_tostring(tolua_S,2,0));
  unsigned int port = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ConnectUDP'", NULL);
#endif
  {
   int tolua_ret = (int)  self->ConnectUDP(IP,port);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ConnectUDP'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DisConnect of class  CCommunicationAgent */
#ifndef TOLUA_DISABLE_tolua_CCommunicationAgent_CCommunicationAgent_DisConnect00
static int tolua_CCommunicationAgent_CCommunicationAgent_DisConnect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCommunicationAgent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCommunicationAgent* self = (CCommunicationAgent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DisConnect'", NULL);
#endif
  {
   int tolua_ret = (int)  self->DisConnect();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DisConnect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMsg of class  CCommunicationAgent */
#ifndef TOLUA_DISABLE_tolua_CCommunicationAgent_CCommunicationAgent_GetMsg00
static int tolua_CCommunicationAgent_CCommunicationAgent_GetMsg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCommunicationAgent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCommunicationAgent* self = (CCommunicationAgent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMsg'", NULL);
#endif
  {
   self->GetMsg();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMsg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setStartTime of class  CCommunicationAgent */
#ifndef TOLUA_DISABLE_tolua_CCommunicationAgent_CCommunicationAgent_setStartTime00
static int tolua_CCommunicationAgent_CCommunicationAgent_setStartTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCommunicationAgent",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCommunicationAgent* self = (CCommunicationAgent*)  tolua_tousertype(tolua_S,1,0);
  int startTime = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setStartTime'", NULL);
#endif
  {
   self->setStartTime(startTime);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setStartTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetAnalyticPacket of class  CCommunicationAgent */
#ifndef TOLUA_DISABLE_tolua_CCommunicationAgent_CCommunicationAgent_SetAnalyticPacket00
static int tolua_CCommunicationAgent_CCommunicationAgent_SetAnalyticPacket00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCommunicationAgent",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCommunicationAgent* self = (CCommunicationAgent*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetAnalyticPacket'", NULL);
#endif
  {
   self->SetAnalyticPacket(id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetAnalyticPacket'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Destroy of class  CCommunicationAgent */
#ifndef TOLUA_DISABLE_tolua_CCommunicationAgent_CCommunicationAgent_Destroy00
static int tolua_CCommunicationAgent_CCommunicationAgent_Destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCommunicationAgent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCommunicationAgent* self = (CCommunicationAgent*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Destroy'", NULL);
#endif
  {
   self->Destroy();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_CCommunicationAgent_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"CCommunicationAgent","CCommunicationAgent","",NULL);
  tolua_beginmodule(tolua_S,"CCommunicationAgent");
   tolua_function(tolua_S,"GetInstance",tolua_CCommunicationAgent_CCommunicationAgent_GetInstance00);
   tolua_function(tolua_S,"SendMsg",tolua_CCommunicationAgent_CCommunicationAgent_SendMsg00);
   tolua_function(tolua_S,"SendMsgPriority",tolua_CCommunicationAgent_CCommunicationAgent_SendMsgPriority00);
   tolua_function(tolua_S,"SendMsgToServer",tolua_CCommunicationAgent_CCommunicationAgent_SendMsgToServer00);
   tolua_function(tolua_S,"SendUDPMsgToServer",tolua_CCommunicationAgent_CCommunicationAgent_SendUDPMsgToServer00);
   tolua_function(tolua_S,"Connect",tolua_CCommunicationAgent_CCommunicationAgent_Connect00);
   tolua_function(tolua_S,"ConnectUDP",tolua_CCommunicationAgent_CCommunicationAgent_ConnectUDP00);
   tolua_function(tolua_S,"DisConnect",tolua_CCommunicationAgent_CCommunicationAgent_DisConnect00);
   tolua_function(tolua_S,"GetMsg",tolua_CCommunicationAgent_CCommunicationAgent_GetMsg00);
   tolua_function(tolua_S,"setStartTime",tolua_CCommunicationAgent_CCommunicationAgent_setStartTime00);
   tolua_function(tolua_S,"SetAnalyticPacket",tolua_CCommunicationAgent_CCommunicationAgent_SetAnalyticPacket00);
   tolua_function(tolua_S,"Destroy",tolua_CCommunicationAgent_CCommunicationAgent_Destroy00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_CCommunicationAgent (lua_State* tolua_S) {
 return tolua_CCommunicationAgent_open(tolua_S);
};
#endif

