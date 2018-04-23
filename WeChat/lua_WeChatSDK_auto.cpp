#include "lua_WeChatSDK_auto.hpp"
#include "WeChatSDKAPIDelegate.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"



int lua_WeChatSDK_WeChatSDKAPIDelegate_RefreshAccessToken(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"WeChatSDKAPIDelegate",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        const char* arg0;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "WeChatSDKAPIDelegate:RefreshAccessToken"); arg0 = arg0_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_RefreshAccessToken'", nullptr);
            return 0;
        }
        WeChatSDKAPIDelegate::RefreshAccessToken(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "WeChatSDKAPIDelegate:RefreshAccessToken",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_RefreshAccessToken'.",&tolua_err);
#endif
    return 0;
}
int lua_WeChatSDK_WeChatSDKAPIDelegate_ShareToChatScene(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"WeChatSDKAPIDelegate",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "WeChatSDKAPIDelegate:ShareToChatScene");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_ShareToChatScene'", nullptr);
            return 0;
        }
        bool ret = WeChatSDKAPIDelegate::ShareToChatScene(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "WeChatSDKAPIDelegate:ShareToChatScene",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_ShareToChatScene'.",&tolua_err);
#endif
    return 0;
}
int lua_WeChatSDK_WeChatSDKAPIDelegate_GetUserInfo(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"WeChatSDKAPIDelegate",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        const char* arg0;
        const char* arg1;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "WeChatSDKAPIDelegate:GetUserInfo"); arg0 = arg0_tmp.c_str();
        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp, "WeChatSDKAPIDelegate:GetUserInfo"); arg1 = arg1_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_GetUserInfo'", nullptr);
            return 0;
        }
        WeChatSDKAPIDelegate::GetUserInfo(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "WeChatSDKAPIDelegate:GetUserInfo",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_GetUserInfo'.",&tolua_err);
#endif
    return 0;
}
int lua_WeChatSDK_WeChatSDKAPIDelegate_ReqestHeadImg(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"WeChatSDKAPIDelegate",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        const char* arg0;
        const char* arg1;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "WeChatSDKAPIDelegate:ReqestHeadImg"); arg0 = arg0_tmp.c_str();
        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp, "WeChatSDKAPIDelegate:ReqestHeadImg"); arg1 = arg1_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_ReqestHeadImg'", nullptr);
            return 0;
        }
        WeChatSDKAPIDelegate::ReqestHeadImg(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "WeChatSDKAPIDelegate:ReqestHeadImg",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_ReqestHeadImg'.",&tolua_err);
#endif
    return 0;
}
int lua_WeChatSDK_WeChatSDKAPIDelegate_GetIpAddressByDomain(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"WeChatSDKAPIDelegate",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "WeChatSDKAPIDelegate:GetIpAddressByDomain");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_GetIpAddressByDomain'", nullptr);
            return 0;
        }
        const char* ret = WeChatSDKAPIDelegate::GetIpAddressByDomain(arg0);
        tolua_pushstring(tolua_S,(const char*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "WeChatSDKAPIDelegate:GetIpAddressByDomain",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_GetIpAddressByDomain'.",&tolua_err);
#endif
    return 0;
}
int lua_WeChatSDK_WeChatSDKAPIDelegate_CheckAccessTokenValid(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"WeChatSDKAPIDelegate",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        const char* arg0;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "WeChatSDKAPIDelegate:CheckAccessTokenValid"); arg0 = arg0_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_CheckAccessTokenValid'", nullptr);
            return 0;
        }
        WeChatSDKAPIDelegate::CheckAccessTokenValid(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "WeChatSDKAPIDelegate:CheckAccessTokenValid",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_CheckAccessTokenValid'.",&tolua_err);
#endif
    return 0;
}
int lua_WeChatSDK_WeChatSDKAPIDelegate_CheckWXInstalled(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"WeChatSDKAPIDelegate",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_CheckWXInstalled'", nullptr);
            return 0;
        }
        bool ret = WeChatSDKAPIDelegate::CheckWXInstalled();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "WeChatSDKAPIDelegate:CheckWXInstalled",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_CheckWXInstalled'.",&tolua_err);
#endif
    return 0;
}
int lua_WeChatSDK_WeChatSDKAPIDelegate_LocalStringEncrypt(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"WeChatSDKAPIDelegate",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "WeChatSDKAPIDelegate:LocalStringEncrypt");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_LocalStringEncrypt'", nullptr);
            return 0;
        }
        std::string ret = WeChatSDKAPIDelegate::LocalStringEncrypt(arg0);
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "WeChatSDKAPIDelegate:LocalStringEncrypt",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_LocalStringEncrypt'.",&tolua_err);
#endif
    return 0;
}
int lua_WeChatSDK_WeChatSDKAPIDelegate_SendAuthRequest(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"WeChatSDKAPIDelegate",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        const char* arg0;
        const char* arg1;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "WeChatSDKAPIDelegate:SendAuthRequest"); arg0 = arg0_tmp.c_str();
        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp, "WeChatSDKAPIDelegate:SendAuthRequest"); arg1 = arg1_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_SendAuthRequest'", nullptr);
            return 0;
        }
        WeChatSDKAPIDelegate::SendAuthRequest(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "WeChatSDKAPIDelegate:SendAuthRequest",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_SendAuthRequest'.",&tolua_err);
#endif
    return 0;
}
int lua_WeChatSDK_WeChatSDKAPIDelegate_GetHeadImage(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"WeChatSDKAPIDelegate",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_GetHeadImage'", nullptr);
            return 0;
        }
        cocos2d::Sprite* ret = WeChatSDKAPIDelegate::GetHeadImage();
        object_to_luaval<cocos2d::Sprite>(tolua_S, "cc.Sprite",(cocos2d::Sprite*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "WeChatSDKAPIDelegate:GetHeadImage",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_GetHeadImage'.",&tolua_err);
#endif
    return 0;
}
int lua_WeChatSDK_WeChatSDKAPIDelegate_GetAccessTokenByCode(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"WeChatSDKAPIDelegate",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        const char* arg0;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "WeChatSDKAPIDelegate:GetAccessTokenByCode"); arg0 = arg0_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_GetAccessTokenByCode'", nullptr);
            return 0;
        }
        WeChatSDKAPIDelegate::GetAccessTokenByCode(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "WeChatSDKAPIDelegate:GetAccessTokenByCode",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_GetAccessTokenByCode'.",&tolua_err);
#endif
    return 0;
}
int lua_WeChatSDK_WeChatSDKAPIDelegate_OpenWXApp(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"WeChatSDKAPIDelegate",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_OpenWXApp'", nullptr);
            return 0;
        }
        WeChatSDKAPIDelegate::OpenWXApp();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "WeChatSDKAPIDelegate:OpenWXApp",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_OpenWXApp'.",&tolua_err);
#endif
    return 0;
}
int lua_WeChatSDK_WeChatSDKAPIDelegate_RegisterAppID(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"WeChatSDKAPIDelegate",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        std::string arg0;
        std::string arg1;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "WeChatSDKAPIDelegate:RegisterAppID");
        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "WeChatSDKAPIDelegate:RegisterAppID");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_RegisterAppID'", nullptr);
            return 0;
        }
        WeChatSDKAPIDelegate::RegisterAppID(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "WeChatSDKAPIDelegate:RegisterAppID",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_RegisterAppID'.",&tolua_err);
#endif
    return 0;
}
int lua_WeChatSDK_WeChatSDKAPIDelegate_ShareToFriendCircle(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"WeChatSDKAPIDelegate",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "WeChatSDKAPIDelegate:ShareToFriendCircle");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_ShareToFriendCircle'", nullptr);
            return 0;
        }
        bool ret = WeChatSDKAPIDelegate::ShareToFriendCircle(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "WeChatSDKAPIDelegate:ShareToFriendCircle",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_ShareToFriendCircle'.",&tolua_err);
#endif
    return 0;
}
int lua_WeChatSDK_WeChatSDKAPIDelegate_GetHttpResponseData(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"WeChatSDKAPIDelegate",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_GetHttpResponseData'", nullptr);
            return 0;
        }
        std::string ret = WeChatSDKAPIDelegate::GetHttpResponseData();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "WeChatSDKAPIDelegate:GetHttpResponseData",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_WeChatSDK_WeChatSDKAPIDelegate_GetHttpResponseData'.",&tolua_err);
#endif
    return 0;
}
static int lua_WeChatSDK_WeChatSDKAPIDelegate_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (WeChatSDKAPIDelegate)");
    return 0;
}

int lua_register_WeChatSDK_WeChatSDKAPIDelegate(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"WeChatSDKAPIDelegate");
    tolua_cclass(tolua_S,"WeChatSDKAPIDelegate","WeChatSDKAPIDelegate","",nullptr);

    tolua_beginmodule(tolua_S,"WeChatSDKAPIDelegate");
        tolua_function(tolua_S,"RefreshAccessToken", lua_WeChatSDK_WeChatSDKAPIDelegate_RefreshAccessToken);
        tolua_function(tolua_S,"ShareToChatScene", lua_WeChatSDK_WeChatSDKAPIDelegate_ShareToChatScene);
        tolua_function(tolua_S,"GetUserInfo", lua_WeChatSDK_WeChatSDKAPIDelegate_GetUserInfo);
        tolua_function(tolua_S,"ReqestHeadImg", lua_WeChatSDK_WeChatSDKAPIDelegate_ReqestHeadImg);
        tolua_function(tolua_S,"GetIpAddressByDomain", lua_WeChatSDK_WeChatSDKAPIDelegate_GetIpAddressByDomain);
        tolua_function(tolua_S,"CheckAccessTokenValid", lua_WeChatSDK_WeChatSDKAPIDelegate_CheckAccessTokenValid);
        tolua_function(tolua_S,"CheckWXInstalled", lua_WeChatSDK_WeChatSDKAPIDelegate_CheckWXInstalled);
        tolua_function(tolua_S,"LocalStringEncrypt", lua_WeChatSDK_WeChatSDKAPIDelegate_LocalStringEncrypt);
        tolua_function(tolua_S,"SendAuthRequest", lua_WeChatSDK_WeChatSDKAPIDelegate_SendAuthRequest);
        tolua_function(tolua_S,"GetHeadImage", lua_WeChatSDK_WeChatSDKAPIDelegate_GetHeadImage);
        tolua_function(tolua_S,"GetAccessTokenByCode", lua_WeChatSDK_WeChatSDKAPIDelegate_GetAccessTokenByCode);
        tolua_function(tolua_S,"OpenWXApp", lua_WeChatSDK_WeChatSDKAPIDelegate_OpenWXApp);
        tolua_function(tolua_S,"RegisterAppID", lua_WeChatSDK_WeChatSDKAPIDelegate_RegisterAppID);
        tolua_function(tolua_S,"ShareToFriendCircle", lua_WeChatSDK_WeChatSDKAPIDelegate_ShareToFriendCircle);
        tolua_function(tolua_S,"GetHttpResponseData", lua_WeChatSDK_WeChatSDKAPIDelegate_GetHttpResponseData);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(WeChatSDKAPIDelegate).name();
    g_luaType[typeName] = "WeChatSDKAPIDelegate";
    g_typeCast["WeChatSDKAPIDelegate"] = "WeChatSDKAPIDelegate";
    return 1;
}
TOLUA_API int register_all_WeChatSDK(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"WX",0);
	tolua_beginmodule(tolua_S,"WX");

	lua_register_WeChatSDK_WeChatSDKAPIDelegate(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

