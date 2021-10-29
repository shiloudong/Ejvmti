package com.eagle.ejvmti;

import android.app.Application;

/**
 * @author linquandong
 * @create 2021/10/30
 * @Describe
 **/
public class EApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
        Monitor.init(this);
    }
}
