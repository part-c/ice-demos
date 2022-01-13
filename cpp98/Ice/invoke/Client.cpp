//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Ice/Ice.h>
#include <Printer.h>

using namespace std;
using namespace Demo;

int run(const Ice::CommunicatorPtr&);

static ostream&
operator<<(ostream& out, Demo::Color c)
{
    switch(c)
    {
    case Demo::red:
        out << "red";
        break;
    case Demo::green:
        out << "green";
        break;
    case Demo::blue:
        out << "blue";
        break;
    }
    return out;
}

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif

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
        "1: print string\n"
        "2: print string sequence\n"
        "3: print dictionary\n"
        "4: print enum\n"
        "5: print struct\n"
        "6: print struct sequence\n"
        "7: print class\n"
        "8: get values\n"
        "9: throw exception\n"
        "s: shutdown server\n"
        "i: send bytes to server\n"
        "x: exit\n"
        "?: help\n";
}

int
run(const Ice::CommunicatorPtr& communicator)
{
    Ice::ObjectPrx obj = communicator->propertyToProxy("Printer.Proxy");

    menu();

    char ch = 'x';
    do
    {
        try
        {
        cout << "==> " << flush;
            cin >> ch;
            if(ch == '1')
            {
                //
                // Marshal the in parameter.
                //
                Ice::ByteSeq inParams, outParams;
                Ice::OutputStream out(communicator);
                out.startEncapsulation();
                out.write("The streaming API works!");
                out.endEncapsulation();
                out.finished(inParams);

                //
                // Invoke operation.
                //
                if(!obj->ice_invoke("printString", Ice::Normal, inParams, outParams))
                {
                    cout << "Unknown user exception" << endl;
                }
            }
            else if(ch == '2')
            {
                //
                // Marshal the in parameter.
                //
                Ice::ByteSeq inParams, outParams;
                Ice::OutputStream out(communicator);
                out.startEncapsulation();
                Demo::StringSeq arr;
                arr.push_back("The");
                arr.push_back("streaming");
                arr.push_back("API");
                arr.push_back("works!");
                out.write(arr);
                out.endEncapsulation();
                out.finished(inParams);

                //
                // Invoke operation.
                //
                if(!obj->ice_invoke("printStringSequence", Ice::Normal, inParams, outParams))
                {
                    cout << "Unknown user exception" << endl;
                }
            }
            else if(ch == '3')
            {
                //
                // Marshal the in parameter.
                //
                Ice::ByteSeq inParams, outParams;
                Ice::OutputStream out(communicator);
                out.startEncapsulation();
                Demo::StringDict dict;
                dict["The"] = "streaming";
                dict["API"] = "works!";
                out.write(dict);
                out.endEncapsulation();
                out.finished(inParams);

                //
                // Invoke operation.
                //
                if(!obj->ice_invoke("printDictionary", Ice::Normal, inParams, outParams))
                {
                    cout << "Unknown user exception" << endl;
                }
            }
            else if(ch == '4')
            {
                //
                // Marshal the in parameter.
                //
                Ice::ByteSeq inParams, outParams;
                Ice::OutputStream out(communicator);
                out.startEncapsulation();
                out.write(Demo::green);
                out.endEncapsulation();
                out.finished(inParams);

                //
                // Invoke operation.
                //
                if(!obj->ice_invoke("printEnum", Ice::Normal, inParams, outParams))
                {
                    cout << "Unknown user exception" << endl;
                }
            }
            else if(ch == '5')
            {
                //
                // Marshal the in parameter.
                //
                Ice::ByteSeq inParams, outParams;
                Ice::OutputStream out(communicator);
                out.startEncapsulation();
                Demo::Structure s;
                s.name = "red";
                s.value = Demo::red;
                out.write(s);
                out.endEncapsulation();
                out.finished(inParams);

                //
                // Invoke operation.
                //
                if(!obj->ice_invoke("printStruct", Ice::Normal, inParams, outParams))
                {
                    cout << "Unknown user exception" << endl;
                }
            }
            else if(ch == '6')
            {
                //
                // Marshal the in parameter.
                //
                Ice::ByteSeq inParams, outParams;
                Ice::OutputStream out(communicator);
                out.startEncapsulation();
                Demo::StructureSeq arr;
                arr.push_back(Demo::Structure());
                arr.back().name = "red";
                arr.back().value = Demo::red;
                arr.push_back(Demo::Structure());
                arr.back().name = "green";
                arr.back().value = Demo::green;
                arr.push_back(Demo::Structure());
                arr.back().name = "blue";
                arr.back().value = Demo::blue;
                out.write(arr);
                out.endEncapsulation();
                out.finished(inParams);

                //
                // Invoke operation.
                //
                if(!obj->ice_invoke("printStructSequence", Ice::Normal, inParams, outParams))
                {
                    cout << "Unknown user exception" << endl;
                }
            }
            else if(ch == '7')
            {
                //
                // Marshal the in parameter.
                //
                Ice::ByteSeq inParams, outParams;
                Ice::OutputStream out(communicator);
                out.startEncapsulation();
                Demo::CPtr c = new Demo::C;
                c->s.name = "blue";
                c->s.value = Demo::blue;
                out.write(c);
                out.writePendingValues();
                out.endEncapsulation();
                out.finished(inParams);

                //
                // Invoke operation.
                //
                if(!obj->ice_invoke("printClass", Ice::Normal, inParams, outParams))
                {
                    cout << "Unknown user exception" << endl;
                }
            }
            else if(ch == '8')
            {
                //
                // Invoke operation.
                //
                Ice::ByteSeq inParams, outParams;
                if(!obj->ice_invoke("getValues", Ice::Normal, inParams, outParams))
                {
                    cout << "Unknown user exception" << endl;
                    continue;
                }

                //
                // Unmarshal the results.
                //
                Ice::InputStream in(communicator, outParams);
                in.startEncapsulation();
                Demo::CPtr c;
                in.read(c);
                string str;
                in.read(str);
                in.readPendingValues();
                in.endEncapsulation();
                cout << "Got string `" << str << "' and class: s.name=" << c->s.name
                     << ", s.value=" << c->s.value << endl;
            }
            else if(ch == '9')
            {
                //
                // Invoke operation.
                //
                Ice::ByteSeq inParams, outParams;
                if(obj->ice_invoke("throwPrintFailure", Ice::Normal, inParams, outParams))
                {
                    cout << "Expected exception" << endl;
                    continue;
                }

                Ice::InputStream in(communicator, outParams);
                in.startEncapsulation();
                try
                {
                    in.throwException();
                }
                catch(const Demo::PrintFailure&)
                {
                    // Expected.
                }
                catch(const Ice::UserException&)
                {
                    cout << "Unknown user exception" << endl;
                }
                in.endEncapsulation();
            }
            else if(ch == 's')
            {
                Ice::ByteSeq inParams, outParams;
                obj->ice_invoke("shutdown", Ice::Normal, inParams, outParams);
            }
            else if(ch == 'x')
            {
                // Nothing to do.
            }
            else if(ch == 'i')
            {
                //
                // Marshal the in parameter.
                //
                Ice::ByteSeq inParams, outParams;
                Ice::OutputStream out(communicator);
                out.startEncapsulation();
                Demo::bytes arr;
                arr.push_back('a');
                arr.push_back('b');
                arr.push_back(69);
                arr.push_back(88);
                arr.push_back(0x31);
                out.write(arr);
                out.endEncapsulation();
                out.finished(inParams);

                Ice::Context ctx;
                ctx.insert(std::pair<string, string>("nodeid", "p1"));
                ctx.insert(std::pair<string, string>("msgid", "msgid1"));
                //
                // Invoke operation.
                //
                if(!obj->ice_invoke("send", Ice::Normal, inParams, outParams, ctx))
                {
                    cout << "Unknown user exception" << endl;
                }
            }
            else if(ch == '?')
            {
                menu();
            }
            else
            {
                cout << "unknown command `" << ch << "'" << endl;
                menu();
            }
        }
        catch(const Ice::Exception& ex)
        {
            cerr << ex << endl;
        }
    }
    while(cin.good() && ch != 'x');

    return 0;
}
