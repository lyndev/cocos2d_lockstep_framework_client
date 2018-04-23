/*
** Lua binding: CMemoryMonitor
** Generated automatically by tolua++-1.0.92 on 06/28/14 16:33:40.
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
TOLUA_API int  tolua_CMemoryMonitor_open (lua_State* tolua_S);

#include "boost_net/MemoryMonitor.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CMemoryMonitor");
}

/* method: GetInstance of class  CMemoryMonitor */
#ifndef TOLUA_DISABLE_tolua_CMemoryMonitor_CMemoryMonitor_GetInstance00
static int tolua_CMemoryMonitor_CMemoryMonitor_GetInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CMemoryMonitor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CMemoryMonitor* tolua_ret = (CMemoryMonitor*)  CMemoryMonitor::GetInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CMemoryMonitor");
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

/* method: GameUsedMemory of class  CMemoryMonitor */
#ifndef TOLUA_DISABLE_tolua_CMemoryMonitor_CMemoryMonitor_GameUsedMemory00
static int tolua_CMemoryMonitor_CMemoryMonitor_GameUsedMemory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CMemoryMonitor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CMemoryMonitor* self = (CMemoryMonitor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GameUsedMemory'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GameUsedMemory();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GameUsedMemory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AvailableMemory of class  CMemoryMonitor */
#ifndef TOLUA_DISABLE_tolua_CMemoryMonitor_CMemoryMonitor_AvailableMemory00
static int tolua_CMemoryMonitor_CMemoryMonitor_AvailableMemory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CMemoryMonitor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CMemoryMonitor* self = (CMemoryMonitor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AvailableMemory'", NULL);
#endif
  {
   double tolua_ret = (double)  self->AvailableMemory();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AvailableMemory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_CMemoryMonitor_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"CMemoryMonitor","CMemoryMonitor","",NULL);
  tolua_beginmodule(tolua_S,"CMemoryMonitor");
   tolua_function(tolua_S,"GetInstance",tolua_CMemoryMonitor_CMemoryMonitor_GetInstance00);
   tolua_function(tolua_S,"GameUsedMemory",tolua_CMemoryMonitor_CMemoryMonitor_GameUsedMemory00);
   tolua_function(tolua_S,"AvailableMemory",tolua_CMemoryMonitor_CMemoryMonitor_AvailableMemory00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_CMemoryMonitor (lua_State* tolua_S) {
 return tolua_CMemoryMonitor_open(tolua_S);
};
#endif

