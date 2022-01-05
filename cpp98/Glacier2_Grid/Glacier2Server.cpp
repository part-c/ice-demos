//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Ice/Ice.h>
#include <HelloI.h>
// 自定义Glacier2的鉴权逻辑(默认不鉴权)
/*
#include <Glacier2/Glacier2.h>
class DummyPermissionsVerifierI : public Glacier2::PermissionsVerifier
{
public:

    virtual bool
    checkPermissions(const string& userId, const string& password, string&, const Ice::Current&) const
    {
        cout << "verified user `" << userId << "' with password `" << password << "'" << endl;
        return true;
    }
};
*/

using namespace std;

int main(int argc, char* argv[])
{
    int status = 0;

    try
    {
        // set properties
        string key_endpoints = "Hello.Endpoints";
        string value_endpoints = "tcp -h 192.168.2.128 -p 10000";
        Ice::InitializationData initData;
        initData.properties = Ice::createProperties();
        initData.properties->setProperty(key_endpoints, value_endpoints);
        // There is no limit to the size of the received message
        // initData.properties->setProperty("Ice.MessageSizeMax", "0");
        // initData.properties->setProperty("Ice.Default.Host", "localhost");
        // initData.properties->setProperty("Ice.Trace.Protocol", "1");
        // initData.properties->setProperty("Ice.Trace.ThreadPool", "1");
        
        Ice::CommunicatorHolder ptr_holder_ = Ice::CommunicatorHolder(initData);
        Ice::CommunicatorPtr ptr_communicator_ = ptr_holder_.communicator();
        Ice::ObjectAdapterPtr ptr_adapter_ = ptr_communicator_->createObjectAdapter("Hello");

        string proxy_name = "glacier2_hello";
        Ice::Identity id ;
        id.name = proxy_name;
        //创建servant并添加到适配器
        Demo::HelloPtr object = new HelloI("Glacier2 Server Response:");
        ptr_adapter_->add(object, id);
        // 适配器加入Glacier2鉴权逻辑
        // Glacier2::PermissionsVerifierPtr dpv = new DummyPermissionsVerifierI;
        // ptr_adapter_->add(dpv, Ice::stringToIdentity("ChannelSessionVerifier"));
        // session管理
        // ChatSessionManagerIPtr csm = new ChatSessionManagerI;
        // adapter->add(csm, Ice::stringToIdentity("ChatSessionManager"));

        //激发启动一个通信器
        ptr_adapter_->activate();
        ptr_communicator_->waitForShutdown();
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}
