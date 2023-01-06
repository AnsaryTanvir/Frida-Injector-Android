// Created by Hujifa on 4/1/2022.

#ifndef BASE64_H
#define BASE64_H

#include "jni.h"

class Base64 {

    public:
    static jstring toBase64String( JNIEnv *env, jstring string );
    static jstring fromBase64String( JNIEnv *env, jstring string );

    private:
    static const jint Base64_DEFAULT = 0;   // Bas64.DEFAULT
    static const jint Base64_NO_WRAP = 2;   // Base64.NO_WRAP ( No Line Break , Ideal For Data Sending )
};

#endif
