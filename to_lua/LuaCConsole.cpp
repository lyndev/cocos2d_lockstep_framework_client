/*
** Lua binding: CConsole
** Generated automatically by tolua++-1.0.92 on 02/12/15 12:10:06.
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
#include "console/Console.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace CocosDenshion;

/* Exported function */
TOLUA_API int  tolua_CConsole_open (lua_State* tolua_S);


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CConsole");
}

/* method: GetInstance of class  CConsole */
#ifndef TOLUA_DISABLE_tolua_CConsole_CConsole_GetInstance00
static int tolua_CConsole_CConsole_GetInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CConsole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CConsole* tolua_ret = (CConsole*)  CConsole::GetInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CConsole");
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

/* method: SetColor of class  CConsole */
#ifndef TOLUA_DISABLE_tolua_CConsole_CConsole_SetColor00
static int tolua_CConsole_CConsole_SetColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CConsole",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CConsole* self = (CConsole*)  tolua_tousertype(tolua_S,1,0);
  CConsole::E_FORECOLOR clr = ((CConsole::E_FORECOLOR) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetColor'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->SetColor(clr);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_CConsole_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"CConsole","CConsole","",NULL);
  tolua_beginmodule(tolua_S,"CConsole");
   tolua_constant(tolua_S,"RED",CConsole::RED);
   tolua_constant(tolua_S,"GREEN",CConsole::GREEN);
   tolua_constant(tolua_S,"WHITE",CConsole::WHITE);
   tolua_constant(tolua_S,"YELLOW",CConsole::YELLOW);
   tolua_function(tolua_S,"GetInstance",tolua_CConsole_CConsole_GetInstance00);
   tolua_function(tolua_S,"SetColor",tolua_CConsole_CConsole_SetColor00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_CConsole (lua_State* tolua_S) {
 return tolua_CConsole_open(tolua_S);
};
#endif

