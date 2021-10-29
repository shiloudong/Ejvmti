#include <jni.h>
#include <string>
#include "jvmti.h"

jvmtiEnv *_jvmtiEnv;

JNIEXPORT jint JNICALL
Agent_OnAttach(JavaVM *vm, char *options, void *reserved) {
    //获取jvmti环境
    vm->GetEnv(reinterpret_cast<void **>(&_jvmtiEnv), JVMTI_VERSION_1_2);
    //开启jvmti能力,
    jvmtiCapabilities capabilities; //每个变量代表一个能力
    _jvmtiEnv->GetCapabilities(&capabilities);
    //开启所以能力
    _jvmtiEnv->AddCapabilities(&capabilities);
    return JNI_OK;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_eagle_ejvmti_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

void JNICALL ObjectFree(jvmtiEnv *jvmti_env,
                        jlong tag) {

}

void JNICALL ObjectAlloc(jvmtiEnv *jvmti_env,
                         JNIEnv *jni_env,
                         jthread thread,
                         jobject object,
                         jclass object_klass,
                         jlong size) {

}

extern "C"
JNIEXPORT void JNICALL
Java_com_eagle_ejvmti_Monitor_startMonitor(JNIEnv *env, jobject thiz, jstring path) {
    const char *logPath = env->GetStringUTFChars(path, 0);
    //订阅事件
    jvmtiEventCallbacks callbacks;
    //对象释放事件赋值，有事件产生是触发"ObjectFree"方法
    callbacks.ObjectFree = ObjectFree;
    //对象创建事件
    callbacks.VMObjectAlloc =ObjectAlloc;

    //注册callback
    _jvmtiEnv->SetEventCallbacks(&callbacks, sizeof(callbacks));

    //开启监控
    _jvmtiEnv->SetEventNotificationMode(JVMTI_ENABLE,JVMTI_EVENT_OBJECT_FREE,NULL);
    _jvmtiEnv->SetEventNotificationMode(JVMTI_ENABLE,JVMTI_EVENT_VM_OBJECT_ALLOC,NULL);

    env->ReleaseStringUTFChars(path, logPath);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_eagle_ejvmti_Monitor_stopMonitor(JNIEnv *env, jobject thiz) {
    //停止监控
    _jvmtiEnv->SetEventNotificationMode(JVMTI_DISABLE,JVMTI_EVENT_OBJECT_FREE,NULL);
    _jvmtiEnv->SetEventNotificationMode(JVMTI_DISABLE,JVMTI_EVENT_VM_OBJECT_ALLOC,NULL);
}