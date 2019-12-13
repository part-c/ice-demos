//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <jni.h>

#import <Ice/Ice.h>
#import <Hello.h>

using namespace std;
using namespace Demo;

namespace
{

enum DeliveyMode
{
    DeliveryModeTwoway,
    DeliveryModeTwowaySecure,
    DeliveryModeOneway,
    DeliveryModeOnewayBatch,
    DeliveryModeOnewaySecure,
    DeliveryModeOnewaySecureBatch,
    DeliveryModeDatagram,
    DeliveryModeDatagramBatch
};

string toCppString(JNIEnv* env, jstring value)
{
    if(value != NULL)
    {
        auto cstr = env->GetStringUTFChars(value, 0);
        string s(cstr);
        env->ReleaseStringUTFChars(value, cstr);
        return s;
    }
    else
    {
        return string();
    }
}

jstring toJavaString(JNIEnv* env, const string& value)
{
    return env->NewStringUTF(value.c_str());
}

class StatusCallback
{
public:

    StatusCallback(JNIEnv* env, jobject statusCallback) :
        _statusCallback(env->NewGlobalRef(statusCallback))
    {
        jclass cls = env->GetObjectClass(statusCallback);
        _sending = env->GetMethodID(cls, "sending", "()V");
        _sent = env->GetMethodID(cls, "sent", "(Z)V");
        _response = env->GetMethodID(cls, "response", "()V");
        _exception = env->GetMethodID(cls, "exception", "(Ljava/lang/String;)V");
    }

    void sending(JNIEnv* env)
    {
        env->CallVoidMethod(_statusCallback, _sending);
    }

    void sent(JNIEnv* env, bool oneway)
    {
        env->CallVoidMethod(_statusCallback, _sent, oneway);
    }

    void response(JNIEnv* env)
    {
        env->CallVoidMethod(_statusCallback, _response);
    }

    void exception(JNIEnv* env, string message)
    {
        env->CallVoidMethod(_statusCallback, _exception, toJavaString(env, message));
    }

private:

    jobject _statusCallback;
    jmethodID _sending;
    jmethodID _sent;
    jmethodID _response;
    jmethodID _exception;
};

class HelloClient
{
public:

    HelloClient(JNIEnv* env, jobject statusCallback) :
        _callback(env, statusCallback),
        _response(false)
    {
        // IceDiscovery is not supported right now with the Android NDK port
        //Ice::registerIceDiscovery();
        Ice::registerIceUDP();

        _ich = Ice::initialize();

        env->GetJavaVM(&_jvm);
    }

    void updateProxy(const string& hostname, int deliveryMode, int timeout, bool discovery)
    {
        if(_helloPrx &&
           hostname == _hostname &&
           deliveryMode == _deliveryMode &&
           timeout == _timeout &&
           discovery == _discovery)
        {
            return;
        }

        _deliveryMode = deliveryMode;
        _hostname = hostname;
        _timeout = timeout;
        _discovery = discovery;

        if(_hostname.empty() && !_discovery)
        {
            return;
        }

        ostringstream os;
        os << "hello";
        if(!_discovery)
        {
            os << ":tcp -h \"" << _hostname << "\" -p 10000";
            os << ":ssl -h \"" << _hostname << "\" -p 10001";
            os << ":udp -h \"" << _hostname << "\" -p 10000";
        }

        _helloPrx = Ice::uncheckedCast<HelloPrx>(_ich->stringToProxy(os.str()));
        switch(deliveryMode)
        {
            case DeliveryModeTwoway:
            {
                _helloPrx = _helloPrx->ice_twoway();
                break;
            }
            case DeliveryModeTwowaySecure:
            {
                _helloPrx = _helloPrx->ice_twoway()->ice_secure(true);
                break;
            }
            case DeliveryModeOneway:
            {
                _helloPrx = _helloPrx->ice_oneway();
                break;
            }
            case DeliveryModeOnewayBatch:
            {
                _helloPrx = _helloPrx->ice_batchOneway();
                break;
            }
            case DeliveryModeOnewaySecure:
            {
                _helloPrx = _helloPrx->ice_oneway()->ice_secure(true);
                break;
            }
            case DeliveryModeOnewaySecureBatch:
            {
                _helloPrx = _helloPrx->ice_batchOneway()->ice_secure(true);
                break;
            }
            case DeliveryModeDatagram:
            {
                _helloPrx = _helloPrx->ice_datagram();
                break;
            }
            case DeliveryModeDatagramBatch:
            {
                _helloPrx = _helloPrx->ice_batchDatagram();
                break;
            }
        }

        if(_timeout != 0)
        {
            _helloPrx = _helloPrx->ice_invocationTimeout(_timeout);
        }
    }

    void sayHello(JNIEnv* env, int delay)
    {
        try
        {
            if(!_helloPrx)
            {
                return;
            }

            if(_deliveryMode != DeliveryModeOnewayBatch &&
               _deliveryMode != DeliveryModeOnewaySecureBatch &&
               _deliveryMode != DeliveryModeDatagramBatch)
            {
                _response = false;
                _callback.sending(env);
                bool oneway = _deliveryMode == DeliveryModeOneway || _deliveryMode == DeliveryModeDatagram;
                _helloPrx->sayHelloAsync(delay,
                                         [=] { response(); },
                                         [=](exception_ptr eptr) { exception(eptr); },
                                         [=](bool) { sent(oneway); });
            }
            else
            {
                _helloPrx->sayHello(delay);
            }
        }
        catch(const Ice::LocalException& ex)
        {
            exception(env, ex);
        }
    }

    void shutdown(JNIEnv* env)
    {
        try
        {
            if(!_helloPrx)
            {
                return;
            }

            if(_deliveryMode != DeliveryModeOnewayBatch &&
               _deliveryMode != DeliveryModeOnewaySecureBatch &&
               _deliveryMode != DeliveryModeDatagramBatch)
            {
                _response = false;
                _callback.sending(env);

                bool oneway = _deliveryMode == DeliveryModeOneway || _deliveryMode == DeliveryModeDatagram;
                _helloPrx->shutdownAsync([=]() { response(); },
                                         [=](exception_ptr eptr) { exception(eptr); },
                                         [=](bool) { sent(oneway); });
            }
            else
            {
                _helloPrx->shutdown();
            }
        }
        catch(const Ice::LocalException& ex)
        {
            exception(env, ex);
        }
    }

    void flush(JNIEnv* env)
    {
        if(!_helloPrx)
        {
            return;
        }

        try
        {
            _helloPrx->ice_flushBatchRequestsAsync([=](exception_ptr eptr) { exception(eptr); },
                                                   [=](bool) { sent(true); });
        }
        catch(const Ice::LocalException& ex)
        {
            exception(env, ex);
        }
    }

    void destroy(JNIEnv*)
    {
        _ich->destroy();
    }

private:

    void response()
    {
        JNIEnv *env;
        _jvm->AttachCurrentThread(&env, 0);
        _response = true;
        _callback.response(env);
    }

    void exception(exception_ptr eptr)
    {
        try
        {
            rethrow_exception(eptr);
        }
        catch(const Ice::Exception& ex)
        {
            JNIEnv *env;
            _jvm->AttachCurrentThread(&env, 0);
            exception(env, ex);
        }
    }

    void sent(bool oneway)
    {
        if(_response)
        {
            return;
        }
        JNIEnv *env;
        _jvm->AttachCurrentThread(&env, 0);
        _callback.sent(env, oneway);
    }

    void exception(JNIEnv* env, const Ice::Exception& ex)
    {
        _response = true;

        ostringstream os;
        os << ex;
        _callback.exception(env, os.str());
    }

    StatusCallback _callback;
    Ice::CommunicatorHolder _ich;
    JavaVM* _jvm;

    bool _response;
    shared_ptr<HelloPrx> _helloPrx;
    int _deliveryMode;
    string _hostname;
    int _timeout;
    bool _discovery;
};

unique_ptr<HelloClient> client;

}

extern "C"
JNIEXPORT void JNICALL
Java_com_zeroc_hellocpp_HelloApp_createHelloClient(JNIEnv* env, jobject, jobject statusCallback)
{
    client.reset(new HelloClient(env, statusCallback));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zeroc_hellocpp_HelloApp_helloClientUpdateProxy(JNIEnv* env, jobject, jstring host, jint mode, jint timeout,
                                                        jboolean discovery)
{
    client->updateProxy(toCppString(env, host), mode, timeout, discovery);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zeroc_hellocpp_HelloApp_helloClientFlush(JNIEnv* env, jobject)
{
    client->flush(env);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zeroc_hellocpp_HelloApp_helloClientShutdown(JNIEnv* env, jobject)
{
    client->shutdown(env);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zeroc_hellocpp_HelloApp_helloClientSayHello(JNIEnv* env, jobject, jint delay)
{
    client->sayHello(env, delay);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zeroc_hellocpp_HelloApp_helloClientDestroy(JNIEnv* env, jobject)
{
    client->destroy(env);
}
