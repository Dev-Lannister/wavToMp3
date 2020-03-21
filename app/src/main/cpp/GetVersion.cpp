//
// Created by lijiwei on 2020/3/18.
//

#include <jni.h>
#include "lamemp3/lame.h"


extern "C"
JNIEXPORT jstring

JNICALL
Java_com_youdao_lame_util_ConvertUtil_getLameVersion(JNIEnv *env, jobject obj) {
    return env->NewStringUTF(get_lame_version());
}