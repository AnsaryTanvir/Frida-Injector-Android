#ifndef HTTPURLCONNECTION_H
#define HTTPURLCONNECTION_H

#include <jni.h>
#include <string>

class HttpURLConnection {

    public:
        static jstring readInputStream(JNIEnv *env, jobject inputStream );
        static jstring GET(JNIEnv* env, jstring SERVER_URL);
        static jstring POST(JNIEnv* env, jstring SERVER_URL, jstring value);

};


#endif