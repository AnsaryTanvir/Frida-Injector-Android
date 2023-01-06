// Created by Hujifa on 4/1/2022.


#ifndef DRM_H
#define DRM_H

#include <jni.h>

class DRM {

    public:
        static jstring      getUniqueDeviceID(JNIEnv *env);
        static jstring      getMD5Hash(JNIEnv *env, jstring inputString);
        static jstring      getSignature(JNIEnv *env, jobject context, jstring packageFileName);
        static jstring      readTextFromPackageFile(JNIEnv* env, jobject context, jstring packageFilePath);
        static bool         extractXorOperatedPackageFile(JNIEnv* env, jobject context, jint xorKey, jstring packageFilePath, jstring outputFileName);
        static const char*  getPackageName(JNIEnv* env, jobject context);
        static const char*  getPackageCachePath(JNIEnv* env, jobject context);
private:


};


#endif
