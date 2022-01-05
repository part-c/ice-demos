//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Ice/Ice.h>
#include <HelloI.h>

using namespace std;

HelloI::HelloI(const string& name) : _name(name)
{
}

std::string HelloI::getGreeting(const Ice::Current&)
{
    cout << "receive message from client." << endl;
    return _name + " says Hello World!";
}

void
HelloI::shutdown(const Ice::Current& c)
{
    cout << _name << " shutting down..." << endl;
    c.adapter->getCommunicator()->shutdown();
}
