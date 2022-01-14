//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Ice/Ice.h>
#include <Hello.h>

using namespace std;
using namespace Demo;

int run(const Ice::CommunicatorPtr&, const string&);

int
main(int argc, char* argv[])
{
    int status = 0;

    try
    {
        Ice::InitializationData initData;
        initData.properties = Ice::createProperties();
        string strProxy = "Hello.Proxy";
        // 直连服务器
        initData.properties->setProperty(strProxy, "hello:ssl -p 10001");
        initData.properties->setProperty("Ice.Default.Host", "localhost");
        initData.properties->setProperty("Ice.Plugin.IceSSL", "IceSSL:createIceSSL");
        
        /*
        initData.properties->setProperty("IceSSL.DefaultDir", "../../certs");
        initData.properties->setProperty("IceSSL.CAs", "cacert.pem");
        initData.properties->setProperty("IceSSL.CertFile", "client.p12");
        initData.properties->setProperty("IceSSL.Password", "password");
        */

        initData.properties->setProperty("IceSSL.DefaultDir", "/home/luodahui/core/channel-sdk/ssl/openssl");
        initData.properties->setProperty("IceSSL.CAs", "ca.pem");
        initData.properties->setProperty("IceSSL.CertFile", "client.pem");
        initData.properties->setProperty("IceSSL.Password", "123456");
        

        // initData.properties->setProperty("IceSSL.Keychain", "../../certs/client.keychain");
        // initData.properties->setProperty("IceSSL.KeychainPassword", "password");

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
    HelloPrx hello = Ice::checkedCast<HelloPrx>(
		communicator->propertyToProxy(strProxy)->ice_twoway()->ice_timeout(-1)->ice_secure(false));

    if(!hello)
    {
        cerr << "couldn't find a `::Demo::Hello' object." << endl;
        return 1;
    }
    
    cout << hello->getGreeting() << endl;
    
    return 0;
}
