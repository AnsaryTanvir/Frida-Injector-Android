// Created by Hujifa on 4/1/2022.

#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <jni.h>

class Activity {

    public:


        static jstring  getPackageName ( JNIEnv *env, jobject context );
        static void     launchActivity (JNIEnv *env, jobject context, jstring className);
        static void     Toast   (JNIEnv *env, jobject context, jstring text , jint TOAST_LENGTH );
    private:
        static const jint  TOAST_LENGTH_LONG  =  1;     // Toast.LENGTH_LONG
        static const jint  TOAST_LENGTH_SHORT =  0;     // Toast.LENGTH_SHORT

};


#endif
