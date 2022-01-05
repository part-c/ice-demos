//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Ice/Ice.h>
#include <Hello.h>

using namespace std;

int run(const Ice::CommunicatorPtr&);

class Callback : public IceUtil::Shared
{
public:

    void response()
    {
        cout << "this is response" << endl;
    }

    void exception(const Ice::Exception& ex)
    {
        cerr << "sayHello AMI call failed:\n" << ex << endl;
    }
};
typedef IceUtil::Handle<Callback> CallbackPtr;

int
main(int argc, char* argv[])
{
    int status = 0;

    try
    {
        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and its dtor destroys this communicator.
        //
        Ice::CommunicatorHolder ich(argc, argv, "config.client");

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        //
        if(argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            status = run(ich.communicator());
        }
    }
    catch(const std::exception& ex)
    {
        cerr << argv[0] << ": " << ex.what() << endl;
        status = 1;
    }

    return status;
}

void
menu()
{
    cout <<
        "usage:\n"
        "i: send immediate greeting\n"
        "d: send delayed greeting\n"
        "s: shutdown server\n"
        "x: exit\n"
        "?: help\n";
}

int
run(const Ice::CommunicatorPtr& communicator)
{
    Demo::HelloPrx hello = Demo::HelloPrx::checkedCast(communicator->propertyToProxy("Hello.Proxy"));
    if(!hello)
    {
        cerr << "invalid proxy" << endl;
        return 1;
    }

    menu();

    CallbackPtr cb = new Callback();

    char c = 'x';
    do
    {
        try
        {
            cout << "==> " << flush;
            cin >> c;
            if(c == 'i')
            {
                hello->sayHello(0);
            }
            else if(c == 'd')
            {
                hello->begin_sayHello(5000, Demo::newCallback_Hello_sayHello(cb, &Callback::response, &Callback::exception));
            }
            else if(c == 's')
            {
                hello->shutdown();
            }
            else if(c == 'x')
            {
                // Nothing to do
            }
            else if(c == '?')
            {
                menu();
            }
            else
            {
                cout << "unknown command `" << c << "'" << endl;
                menu();
            }
        }
        catch(const Ice::Exception& ex)
        {
            cerr << ex << endl;
        }
    }
    while(cin.good() && c != 'x');

    return 0;
}
