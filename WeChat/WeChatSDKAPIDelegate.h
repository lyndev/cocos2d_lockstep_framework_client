//
//  WeChatSDKDelegate.hpp
//  RDTankGame
//
//  Created by 梁彬 on 16/12/9.
//
//

#ifndef WeChatSDKAPIDelegate_H
#define WeChatSDKAPIDelegate_H
#include <iostream>
#include "cocos2d.h"
#include "network/HttpClient.h"
using namespace std;
class WeChatSDKAPIDelegate
{
public:
    
    
    //注册appid
    static void RegisterAppID(string app_id, string app_secret);
    //打开微信应用
    static void OpenWXApp();
    //检查微信是否安装
    static bool CheckWXInstalled();
    //刷新access_token
    static void RefreshAccessToken(const char* refresh_token);
    //检查access_token是否效
    static void CheckAccessTokenValid(const char* access_token);
    //获取用户信息
    static void GetUserInfo(const char* access_token, const char* open_id);
    static void ReqestHeadImg(const char* url, const char* tag);
    //lua调用获取http响应数据
    static string GetHttpResponseData();
    //简单加密处理
    static string LocalStringEncrypt(string encrypt_data);
    
    //根据域名获取ip地址
    static const char* GetIpAddressByDomain(string domain);
    //获取头像图片
    static cocos2d::Sprite* GetHeadImage();
    
    //分享到好友聊天
    static bool ShareToChatScene(std::string content);
    //分享到朋友圈
    static bool ShareToFriendCircle(std::string content);
    
    //java call
    static void GetAccessTokenByCode(const char* code);
    
    //privte
    static string APP_SECRET;
    static string APP_ID;
    static cocos2d::Vector<cocos2d::__String*> ResponseDatas;
    static cocos2d::Vector<cocos2d::Sprite*> HeadImages;
    static void SendAuthRequest(const char* scope, const char* state);
    static void SendHttpRequest(const char* url, const char* tag, const cocos2d::network::ccHttpRequestCallback& callback);
    static void HttpRequestCallBack(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);
};

#endif /* WeChatSDKDelegate_hpp */
