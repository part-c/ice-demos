//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

package com.zeroc.hellocpp;

import android.app.Application;
import android.content.Context;
import android.net.wifi.WifiManager;
import android.os.Build.VERSION;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;

import java.util.LinkedList;
import java.util.List;

public class HelloApp extends Application
{
    public interface StatusCallback
    {
        void sending();
        void sent(boolean oneway);
        void response();
        void exception(String message);
    }

    @Override
    public void onCreate()
    {
        super.onCreate();
        _uiHandler = new Handler(Looper.getMainLooper())
        {
            @Override
            public void handleMessage(Message m)
            {
                if(m.what == MSG_READY)
                {
                    _initialized = true;
                }

                Message copy = new Message();
                copy.copyFrom(m);
                if(_handler != null)
                {
                    _handler.sendMessage(copy);
                }
                else
                {
                    _queue.add(copy);
                }
            }
        };

        _wifiManager = (WifiManager)getApplicationContext().getSystemService(Context.WIFI_SERVICE);

        //
        // On some devices, a multicast lock must be acquired otherwise multicast packets are discarded.
        // The lock is initially unacquired.
        //
        _lock = _wifiManager.createMulticastLock("com.zeroc.demos.android.hellocpp");
        _lock.acquire();

        // Initialization can take some time. To avoid blocking the calling thread, we perform the initialization
        // in a separate thread.
        new Thread(new Runnable()
        {
            public void run()
            {
                createHelloClient(new StatusCallback()
                {
                    public void sending()
                    {
                        _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_SENDING));
                    }

                    public void sent(boolean oneway)
                    {
                        if(oneway)
                        {
                            _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_RESPONSE));
                        }
                        else
                        {
                            _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_SENT));
                        }
                    }

                    public void response()
                    {
                        _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_RESPONSE));
                    }

                    public void exception(String message)
                    {
                        _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_EXCEPTION, message));
                    }
                });
                _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_READY));
            }
        }).start();
    }

    /** Called when the application is stopping. */
    @Override
    public void onTerminate()
    {
        super.onTerminate();
        helloClientDestroy();
    }

    void setHandler(Handler handler)
    {
        // Nothing to do in this case.
        if(_handler != handler)
        {
            _handler = handler;

            if(_handler != null)
            {
                if(!_initialized)
                {
                    _handler.sendMessage(_handler.obtainMessage(MSG_WAIT));
                }
                else
                {
                    // Send all the queued messages.
                    while(!_queue.isEmpty())
                    {
                        _handler.sendMessage(_queue.remove(0));
                    }
                }
            }
        }
    }

    void setHost(String host)
    {
        _host = host;
        helloClientUpdateProxy(_host, _mode, _timeout, _useDiscovery);
    }

    void setUseDiscovery(boolean b)
    {
        _useDiscovery = b;
        helloClientUpdateProxy(_host, _mode, _timeout, _useDiscovery);
    }

    void setTimeout(int timeout)
    {
        _timeout = timeout;
        helloClientUpdateProxy(_host, _mode, _timeout, _useDiscovery);
    }

    void setDeliveryMode(int mode)
    {
        _mode = mode;
        helloClientUpdateProxy(_host, _mode, _timeout, _useDiscovery);
    }

    int getDeliveryMode()
    {
        return _mode;
    }

    void flush()
    {
        helloClientFlush();
    }

    void shutdown()
    {
        helloClientShutdown();
    }

    void sayHello(int delay)
    {
        helloClientSayHello(delay);
    }

    static
    {
        System.loadLibrary("helloclient");
    }

    private native void createHelloClient(StatusCallback callback);
    private native void helloClientUpdateProxy(String host, int mode, int timeout, boolean discovery);
    private native void helloClientFlush();
    private native void helloClientShutdown();
    private native void helloClientSayHello(int delay);
    private native void helloClientDestroy();

    public static final int MSG_WAIT = 0;
    public static final int MSG_READY = 1;
    public static final int MSG_EXCEPTION = 2;
    public static final int MSG_RESPONSE = 3;
    public static final int MSG_SENDING = 4;
    public static final int MSG_SENT = 5;

    public static final int DELIVERY_MODE_TWOWAY = 0;
    public static final int DELIVERY_MODE_TWOWAY_SECURE = 1;
    public static final int DELIVERY_MODE_ONEWAY = 2;
    public static final int DELIVERY_MODE_ONEWAY_BATCH = 3;
    public static final int DELIVERY_MODE_ONEWAY_SECURE = 4;
    public static final int DELIVERY_MODE_ONEWAY_SECURE_BATCH = 5;
    public static final int DELIVERY_MODE_DATAGRAM = 6;
    public static final int DELIVERY_MODE_DATAGRAM_BATCH = 7;

    private WifiManager _wifiManager;
    private WifiManager.MulticastLock _lock;

    private List<Message> _queue = new LinkedList<Message>();
    private Handler _uiHandler;
    private Handler _handler;

    private boolean _initialized;

    // Proxy settings.
    private String _host;
    private boolean _useDiscovery;
    private int _timeout;
    private int _mode;
}
