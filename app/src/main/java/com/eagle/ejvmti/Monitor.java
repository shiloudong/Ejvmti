package com.eagle.ejvmti;

import android.content.Context;
import android.os.Build;
import android.os.Debug;

import java.lang.reflect.Method;

/**
 * @author linquandong
 * @create 2021/10/30
 * @Describe
 **/
public class Monitor {
    // Used to load the 'native-lib' library on application startup.
    public static final String LIB_NAME = "native-lib";
    static {
        System.loadLibrary(LIB_NAME);
    }

    public static void init(Context context){
        try {
            ClassLoader classLoader = context.getClassLoader();
            Method findLibrary = ClassLoader.class.getDeclaredMethod("findLibrary");
            //so 地址
            String soPath = (String) findLibrary.invoke(classLoader,LIB_NAME);
            if(Build.VERSION.SDK_INT >=Build.VERSION_CODES.P){
                Debug.attachJvmtiAgent(soPath,null,classLoader);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    //开启监听，并不日志输入到path路径
    public native void startMonitor(String path);

    //关闭监听
    public native void stopMonitor();
}
