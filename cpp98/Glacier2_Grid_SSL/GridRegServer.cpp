//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Ice/Ice.h>
#include <HelloI.h>

using namespace std;
/*
Grid描述：
1. 功能：使用IceGrid，并将服务器的servant注册到IceGrid

2. 过程：
（1）服务器自动分配端口，并启动服务；
（2）将服务器的ip和端口绑定到AdapterId和ServantId上;
（3）客户端通过服务器的AdapterId和ServantId找到服务器的ip和端口，并直接和服务器创建连接;
（4）客户端直接和服务器进行通信;
*/


int main(int argc, char* argv[])
{
    int status = 0;

    try
    {
        // set properties
        string key_endpoints = "Ice.Default.Locator";
        string value_endpoints = "ChannelIceGrid/Locator:tcp -p 10032 -h localhost";
        Ice::InitializationData initData;
        initData.properties = Ice::createProperties();
        // 设置IceGrid的服务信息（用于servant信息的注册）
        initData.properties->setProperty(key_endpoints, value_endpoints);
        string taskIceServantAdapterName = "DefaultTCPAdapter";
        string taskIceServantAdapterId = "icChannelAdapter1";
        initData.properties->setProperty(taskIceServantAdapterName + ".Endpoints", "tcp -h localhost");
        initData.properties->setProperty(taskIceServantAdapterName + ".AdapterId", taskIceServantAdapterId);

        Ice::CommunicatorHolder ptr_holder_ = Ice::CommunicatorHolder(initData);
        Ice::CommunicatorPtr ptr_communicator_ = ptr_holder_.communicator();
        Ice::ObjectAdapterPtr ptr_adapter_ = ptr_communicator_->createObjectAdapter(taskIceServantAdapterName);

        string proxy_name = "hello_grid";
        Ice::Identity id ;
        id.name = proxy_name;
        //创建一个servant并加载到适配器
        Demo::HelloPtr object = new HelloI("Grid Server Response:");
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
