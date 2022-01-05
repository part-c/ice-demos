//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Ice/Ice.h>
#include <HelloI.h>

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

        string proxy_name = "hello";
        Ice::Identity id ;
        id.name = proxy_name;
        //增加一个适配器
        Demo::HelloPtr object = new HelloI("Simple Server Response:");
        ptr_adapter_->add(object, id);
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
