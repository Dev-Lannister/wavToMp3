//
// Created by lijiwei on 2020/3/18.
//
#include <jni.h>
#include <malloc.h>
#include <string.h>
#include "lamemp3/lame.h"

#include <android/log.h>

#define LOG_TAG    "jiat"
#define LOGD(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


char *Jstring2CStr(JNIEnv *env, jstring jstr) {
    char *rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("GB2312");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

extern "C" JNIEXPORT void JNICALL
Java_com_youdao_lame_util_ConvertUtil_convertmp3(JNIEnv *env, jobject instance, jstring jwav,
                                                 jstring jmp3, jint samplerate, jint channel) {
    char *cwav = Jstring2CStr(env, jwav);
    char *cmp3 = Jstring2CStr(env, jmp3);

    LOGD("wav = %s", cwav);
    LOGD("mp3 = %s", cmp3);
    LOGD("samplerate = %d", samplerate);
    LOGD("channel = %d", channel);

    FILE *fwav = fopen(cwav, "rb");
    FILE *fmp3 = fopen(cmp3, "wb");

    short int wav_buffer[8192 * 2];
    unsigned char mp3_buffer[8192];

    lame_t lame = lame_init();
    lame_set_in_samplerate(lame, samplerate);
    lame_set_num_channels(lame, channel);
    if (channel == 1) {
        lame_set_mode(lame, MONO);
    }
    lame_set_VBR(lame, vbr_default);

    lame_init_params(lame);
    LOGD("lame init finish");

    int read;
    int write; //代表读了多少个次 和写了多少次
    int total = 0; // 当前读的wav文件的byte数目
    do {

        read = fread(wav_buffer, sizeof(short int) * channel, 8192, fwav);
        LOGD("converting ....%d", total);
        total += read * sizeof(short int) * channel;

        if (read != 0) {
            if (channel == 1) {
                write = lame_encode_buffer(lame, wav_buffer, NULL, read, mp3_buffer, 8192);
            } else {
                write = lame_encode_buffer_interleaved(lame, wav_buffer, read, mp3_buffer, 8192);
            }
            fwrite(mp3_buffer, sizeof(unsigned char), write, fmp3);
        }
        if (read == 0) {
            lame_encode_flush(lame, mp3_buffer, 8192);
        }
    } while (read > 0);
    LOGD("convert  finish");
    lame_close(lame);
    fclose(fwav);
    fclose(fmp3);


// 调用java代码，通知转码结束
    jclass jclazz = env->FindClass("com/youdao/lame/util/ConvertUtil");
    jmethodID jmethod = env->GetMethodID(jclazz, "convertCompleted", "()V");
    env->CallVoidMethod(instance, jmethod);

}
