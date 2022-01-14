//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Ice/Ice.h>
#include <IceGrid/IceGrid.h>
#include <Glacier2/Glacier2.h>
#include <Hello.h>

using namespace std;
using namespace Demo;

class CloseCallbackI : public Ice::CloseCallback
{
public:

    virtual void closed(const Ice::ConnectionPtr&)
    {
        cout << "The Glacier2 session has been destroyed========." << endl;
    }
};

int run(const Ice::CommunicatorPtr&, const string&);

int
main(int argc, char* argv[])
{
    int status = 0;

    try
    {
        string strRoute = "Ice.Default.Router"; 
        Ice::InitializationData initData;
        initData.properties = Ice::createProperties();
        // 设置Glacier2的路由信息,ChannelGlacier2为Glacier2服务名称, 10031为Glacier2服务监听端口以及ip
        string tcp_value = "ChannelGlacier2/router:tcp -p 10031 -h 192.168.2.128";
        // initData.properties->setProperty(strRoute, tcp_value);

        // 设置SSL
        string ssl_value = "ChannelGlacier2/router:ssl -p 10031 -h 192.168.2.128";
        initData.properties->setProperty(strRoute, ssl_value);
        initData.properties->setProperty("Ice.Plugin.IceSSL", "IceSSL:createIceSSL");
        initData.properties->setProperty("IceSSL.DefaultDir", "/home/luodahui/core/channel-sdk/ssl/openssl");
        initData.properties->setProperty("IceSSL.CAs", "ca.pem");
        initData.properties->setProperty("IceSSL.CertFile", "client.pem");
        initData.properties->setProperty("IceSSL.Password", "123456");
        // initData.properties->setProperty("IceSSL.Keychain", "client.key");
        // initData.properties->setProperty("IceSSL.KeychainPassword", "123456");

        Ice::CommunicatorHolder ptr_holder_ = Ice::CommunicatorHolder(initData);
	    Ice::CommunicatorPtr ptr_communicator_ = ptr_holder_.communicator();

        string taskIceServantAdapterId = "icChannelAdapter1";
        string servantId = "hello_glacier2_grid";
        // 寻找方式
        string strProxy = servantId + "@" + taskIceServantAdapterId;
        status = run(ptr_communicator_, strProxy);
    }
    catch(const std::exception& ex)
    {
        cerr << argv[0] << ": " << ex.what() << endl;
        status = 1;
    }

    return status;
}

int run(const Ice::CommunicatorPtr& communicator, const string& strProxy)
{
    Glacier2::RouterPrx router = Glacier2::RouterPrx::checkedCast(communicator->getDefaultRouter());
    Glacier2::SessionPrx session;
    do
    {
        try
        {
            session = router->createSession("", "");
            break;
        }
        catch(const Glacier2::PermissionDeniedException& ex)
        {
            cout << "permission denied:\n" << ex.reason << endl;
        }
        catch(const Glacier2::CannotCreateSessionException& ex)
        {
            cout << "cannot create session:\n" << ex.reason << endl;
        }
    }while(!session);

    Ice::Int acmTimeout = router->getACMTimeout();
    Ice::ConnectionPtr connection = router->ice_getCachedConnection();
    assert(connection);
    connection->setACM(acmTimeout, IceUtil::None, Ice::HeartbeatAlways);
    connection->setCloseCallback(new CloseCallbackI());
    HelloPrx hello = HelloPrx::uncheckedCast(communicator->stringToProxy(strProxy));
    if(!hello)
    {
        cerr << "couldn't find a `::Demo::Hello' object." << endl;
        return 1;
    }

    // 保证全部发送成功
    while(true) {
        // 异常处理
        try {
            for(int i = 0; i < 100; ++i) {
                cout << "i:" << i << "====" << hello->getGreeting() << endl;   
                // IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(2000));
            }
            break; // 全部发送完成
        } catch(const Ice::UnknownLocalException& ex) {
            cout << "ex:" << ex.what() << endl;
            IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(1000));
        }
    }
    
    return 0;
}
