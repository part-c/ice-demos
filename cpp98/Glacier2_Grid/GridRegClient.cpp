//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Ice/Ice.h>
#include <IceGrid/IceGrid.h>
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
        string key_endpoints = "Ice.Default.Locator";
        string value_endpoints = "ChannelIceGrid/Locator:tcp -p 10032 -h localhost";
        Ice::InitializationData initData;
        initData.properties = Ice::createProperties();
        // 设置IceGrid的服务信息（用于查询注册到Grid的servant）
        initData.properties->setProperty(key_endpoints, value_endpoints);

        Ice::CommunicatorHolder ptr_holder_ = Ice::CommunicatorHolder(initData);
	    Ice::CommunicatorPtr ptr_communicator_ = ptr_holder_.communicator();

        string taskIceServantAdapterId = "icChannelAdapter1";
        string servantId = "hello_grid";
        // 寻找方式
        string strProxy = servantId+"@"+taskIceServantAdapterId;
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
    HelloPrx hello = HelloPrx::checkedCast(communicator->stringToProxy(strProxy));
    if(!hello)
    {
        cerr << "couldn't find a `::Demo::Hello' object." << endl;
        return 1;
    }

    for(int i = 0; i < 120; ++i) {
        cout << hello->getGreeting() << endl;   
        IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(2000));
    }
    
    return 0;
}
