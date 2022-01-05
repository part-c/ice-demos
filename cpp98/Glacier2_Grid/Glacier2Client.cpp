//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Ice/Ice.h>
#include <Glacier2/Glacier2.h>
#include <Hello.h>

using namespace std;
using namespace Demo;


class CloseCallbackI : public Ice::CloseCallback
{
public:

    virtual void
    closed(const Ice::ConnectionPtr&)
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
        Ice::InitializationData initData;
        initData.properties = Ice::createProperties();
        string strRoute = "Ice.Default.Router";
        // 设置Glacier2的路由信息,ChannelGlacier2为Glacier2服务名称, 10031为Glacier2服务监听端口以及ip
        initData.properties->setProperty(strRoute, "ChannelGlacier2/router:tcp -p 10031 -h 192.168.2.128");
        // 设置服务器代理信息, 10000为服务器进程监听端口以及ip
        string strProxy = "Callback.Proxy";
        // glacier2_hello为服务器的identity
        initData.properties->setProperty(strProxy, "glacier2_hello:tcp -h 192.168.2.128 -p 10000");

        Ice::CommunicatorHolder ptr_holder_ = Ice::CommunicatorHolder(initData);
	    Ice::CommunicatorPtr ptr_communicator_ = ptr_holder_.communicator();

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
    // Ice::Identity callbackReceiverIdent;
    // callbackReceiverIdent.name = "callbackReceiver";
    // callbackReceiverIdent.category = router->getCategoryForClient();

    // Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapterWithRouter("", router);
    // adapter->activate();
    // HelloPrx callback = HelloPrx::uncheckedCast(adapter->add(new HelloI(""), callbackReceiverIdent));

    //session->setCallback(callback);

    HelloPrx hello = Ice::checkedCast<HelloPrx>(communicator->propertyToProxy(strProxy));
    if(!hello)
    {
        cerr << "couldn't find a `::Demo::Hello' object." << endl;
        return 1;
    }

    for(int i = 0; i < 20; ++i) {
        cout << hello->getGreeting() << endl;   
        IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(2000));
    }
     
    return 0;
}
