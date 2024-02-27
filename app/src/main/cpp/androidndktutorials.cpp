#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_amandeep_androidndktutorials_MainActivity_getValues(JNIEnv *env, jobject thiz) {
    std::string hello = " String from JNI C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_amandeep_androidndktutorials_MainActivity_processString(JNIEnv *env,
                                                                 jobject mainActivity) {
    const jclass mainActivityClass = env->GetObjectClass(mainActivity);
    const jmethodID methodId = env->GetMethodID(mainActivityClass, "processInJava",
                                                "()Ljava/lang/String;");
    if (methodId == nullptr) {
        return env->NewStringUTF(" ");
    }

    const jobject result = env->CallObjectMethod(mainActivity, methodId);

    const std::string java_msg = env->GetStringUTFChars((jstring) result, JNI_FALSE);

    const std::string c_message = "Result from Java =>";

    const std::string message = java_msg + c_message;

    return env->NewStringUTF(message.c_str());
}