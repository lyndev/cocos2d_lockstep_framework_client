//
//  WeChatSDKDelegate.cpp
//  RDTankGame
//
//  Created by 梁彬 on 16/12/9.
//
//
#include "WeChatSDKAPIDelegate.h"
#include "Boost_Net/CommunicationAgent.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "cocos/platform/android/jni/JniHelper.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "WXApiOCDelegate.h"
#endif
USING_NS_CC;
using namespace network;

string WeChatSDKAPIDelegate::APP_SECRET = "";
string WeChatSDKAPIDelegate::APP_ID = "";
cocos2d::Vector<__String*> WeChatSDKAPIDelegate::ResponseDatas = cocos2d::Vector<__String*>();
cocos2d::Vector<Sprite*> WeChatSDKAPIDelegate::HeadImages = cocos2d::Vector<Sprite*>();
void WeChatSDKAPIDelegate::RegisterAppID(string app_id, string app_secret)
{
    WeChatSDKAPIDelegate::APP_SECRET = app_secret;
    WeChatSDKAPIDelegate::APP_ID = app_id;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo info;
    bool isHave = JniHelper::getStaticMethodInfo(info, "org/cocos/unkchess/wxapi/WXEntryActivity", "RegisterAppID", "(Ljava/lang/String;)V");
    if (isHave)
    {
        jstring js_appid = info.env->NewStringUTF(app_id.c_str());
        info.env->CallStaticVoidMethod(info.classID, info.methodID, js_appid);
    }
    CCLOG("-----RegisterAppID_Android----%s------secret=", app_id.c_str(), app_secret.c_str());

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //CCLOG("-----RegisterAppID_IOS----%s", app_id);
    OC_RegisterAppID(app_id);

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	
#endif
}


void WeChatSDKAPIDelegate::OpenWXApp()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo info;
    bool isHave = JniHelper::getStaticMethodInfo(info, "org/cocos/unkchess/wxapi/WXEntryActivity", "OpenWXApp", "()V");
    if (isHave)
    {
        info.env->CallStaticVoidMethod(info.classID, info.methodID);
    }
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //TODO:IOS
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#endif
}

bool WeChatSDKAPIDelegate::CheckWXInstalled()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo info;
    bool isHave = JniHelper::getStaticMethodInfo(info, "org/cocos/unkchess/wxapi/WXEntryActivity", "CheckWXInstalled", "()Z");
    if (isHave)
    {
        jboolean is_installed = (jboolean)info.env->CallStaticBooleanMethod(info.classID, info.methodID);
        return (bool)is_installed;
    }
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //TODO:IOS
    return false;
#endif
    return false;
}

void WeChatSDKAPIDelegate::SendAuthRequest(const char* scope, const char* state)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo info;
    bool isHave = JniHelper::getStaticMethodInfo(info, "org/cocos/unkchess/wxapi/WXEntryActivity", "SendAuthRequest", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (isHave)
    {
        jstring js_scope = info.env->NewStringUTF(scope);
        jstring js_state = info.env->NewStringUTF(state);
        info.env->CallStaticVoidMethod(info.classID, info.methodID, js_scope, js_state);
    }
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    OC_SendAuthRequest(scope, state);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#endif
}

void WeChatSDKAPIDelegate::SendHttpRequest(const char* url, const char* tag, const cocos2d::network::ccHttpRequestCallback& callback)
{
    HttpRequest* http_req = new HttpRequest();
    http_req->setRequestType(HttpRequest::Type::GET);
    http_req->setUrl(url);
    http_req->setResponseCallback(callback);
    http_req->setTag(tag);
    HttpClient::getInstance()->send(http_req);
    http_req->release();
}

void WeChatSDKAPIDelegate::GetAccessTokenByCode(const char* code)
{
    __String* url =  __String::createWithFormat("https://api.weixin.qq.com/sns/oauth2/access_token?appid=%s&secret=%s&code=%s&grant_type=authorization_code",APP_ID.c_str(), APP_SECRET.c_str(), code);
    const char* c_url = url->getCString();
    CCLOG("url = %s",c_url);
    SendHttpRequest(url->getCString(), "get_access_token", std::bind(WeChatSDKAPIDelegate::HttpRequestCallBack, std::placeholders::_1, std::placeholders::_2));
}


void WeChatSDKAPIDelegate::RefreshAccessToken(const char* refresh_token)
{
    __String* url =  __String::createWithFormat("https://api.weixin.qq.com/sns/oauth2/refresh_token?appid=%s&grant_type=refresh_token&refresh_token=%s",APP_ID.c_str(), refresh_token);
    const char* c_url = url->getCString();
    CCLOG("url = %s",c_url);
    SendHttpRequest(url->getCString(), "refresh_access_token", std::bind(WeChatSDKAPIDelegate::HttpRequestCallBack, std::placeholders::_1, std::placeholders::_2));
}

//暂时不实现，因为用户每次登录都会去刷新access_token，以保证每次拿到的access_token都是有效的
void WeChatSDKAPIDelegate::CheckAccessTokenValid(const char* access_token)
{

}

void WeChatSDKAPIDelegate::GetUserInfo(const char* access_token, const char* open_id)
{
    __String* url =  __String::createWithFormat("https://api.weixin.qq.com/sns/userinfo?access_token=%s&openid=%s",access_token, open_id);
    const char* c_url = url->getCString();
    CCLOG("url = %s",c_url);
    SendHttpRequest(url->getCString(), "get_user_info", std::bind(WeChatSDKAPIDelegate::HttpRequestCallBack, std::placeholders::_1, std::placeholders::_2));
}

void WeChatSDKAPIDelegate::ReqestHeadImg(const char* url, const char* tag)
{
    SendHttpRequest(url, tag, std::bind([=](HttpClient* client, HttpResponse* response){
        if (response == nullptr)
        {
            return;
        }
        const char* head_tag = response->getHttpRequest()->getTag();
        CCLOG("tag=%s", head_tag);
        if (!response->isSucceed())
        {
            CCLOG("error buffer: %s", response->getErrorBuffer());
            cout<<"------error="<<response->getErrorBuffer()<<endl;
            return;
        }
        vector<char> *buffer = response->getResponseData();
        Image* img = new Image();
        img->initWithImageData((const unsigned char*)buffer->data(), buffer->size());
        Texture2D* texture = new Texture2D();
        texture->initWithImage(img);
        Sprite* sprite = Sprite::createWithTexture(texture);
        if (sprite != nullptr)
        {
            sprite->setName(head_tag);
            sprite->retain();
            WeChatSDKAPIDelegate::HeadImages.pushBack(sprite);
            CCommunicationAgent::GetInstance()->SendMsg(1604, 0, 0);
        }}, std::placeholders::_1, std::placeholders::_2));
}

void WeChatSDKAPIDelegate::HttpRequestCallBack(HttpClient* client, HttpResponse* response)
{
    if (response == nullptr)
    {
        return;
    }
    const char* tag = response->getHttpRequest()->getTag();
    CCLOG("tag=%s", tag);
    if (!response->isSucceed())
    {
        CCLOG("error buffer: %s", response->getErrorBuffer());
        return;
    }
    vector<char> *buffer = response->getResponseData();
    string resp_data ; // 输出数据
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        resp_data.append(1, (*buffer)[i]);
    }
    CCLOG("response data= %s", resp_data.c_str());
    ResponseDatas.pushBack(__String::create(resp_data));
    if (string(tag).compare("get_access_token") == 0)
    {
            CCommunicationAgent::GetInstance()->SendMsg(1601, 0, 0);
    }else if (string(tag).compare("refresh_access_token") == 0)
    {
        CCommunicationAgent::GetInstance()->SendMsg(1602, 0, 0);
    }else if (string(tag).compare("get_user_info") == 0)
    {
        CCommunicationAgent::GetInstance()->SendMsg(1603, 0, 0);
    }
    
}

string WeChatSDKAPIDelegate::GetHttpResponseData()
{
    if (ResponseDatas.size() > 0)
    {
        __String* temp = ResponseDatas.front()->clone();
        ResponseDatas.erase(0);
        return string(temp->getCString());
    }
    return "None data!";
    
}

bool WeChatSDKAPIDelegate::ShareToChatScene(std::string content)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return false;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return OC_ShareToChatScene(content);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return false;
#endif
    
}
bool WeChatSDKAPIDelegate::ShareToFriendCircle(std::string content)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return false;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return OC_ShareToFriendCircle(content);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return false;
#endif
}

const char* WeChatSDKAPIDelegate::GetIpAddressByDomain(string domain)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return nullptr;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return OC_GetIpAddressByDomain(domain);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return nullptr;
#endif
}

Sprite* WeChatSDKAPIDelegate::GetHeadImage()
{
    if (HeadImages.size() > 0)
    {
        Sprite* sp = HeadImages.front();
        HeadImages.erase(0);
        return sp;
    }
    return nullptr;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#ifdef __cplusplus
extern "C" {
#endif
    void Java_org_cocos_unkchess_wxapi_WXEntryActivity_SetAccessTokenCode(JNIEnv *env, jobject thiz, jstring code) {
        WeChatSDKAPIDelegate::GetAccessTokenByCode(env->GetStringUTFChars(code, JNI_FALSE));
    }
#ifdef __cplusplus
}
#endif
#endif





string WeChatSDKAPIDelegate::LocalStringEncrypt(string encrypt_data)
{
    unsigned long str_len = encrypt_data.length();
    for(int i = 0; i < str_len; ++i)
    {
        encrypt_data[i] = encrypt_data[i] ^ 0x0b;
    }
    return encrypt_data;
}
