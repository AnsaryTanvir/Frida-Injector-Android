// Created by Hujifa on 4/1/2022.

#ifndef LAYOUT_H
#define LAYOUT_H

#include <jni.h>

class Layout {

    public:
        static void hideActionBar(JNIEnv *env, jobject context);
        static jint getPixelFromDp(JNIEnv *env, jobject context, jfloat dp );

    private:
        static const jint FEATURE_NO_TITLE = 1;  // [Package: android.view] [Class: Window] [Field: FEATURE_NO_TITLE]
};
#endif
