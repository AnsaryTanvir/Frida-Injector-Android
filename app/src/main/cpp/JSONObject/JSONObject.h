//
// Created by ansary on 4/2/2022.
//

#ifndef JSONOBJECT_H
#define JSONOBJECT_H

#include <jni.h>

class JSONObject {


    public:

        JSONObject(JNIEnv *env);
        JSONObject(JNIEnv *env, jstring json);

        void    put(jstring key, jstring value);
        jstring getString( jstring name );
        jstring toString();

    private:
        JNIEnv  *env;
        jclass  JSONObjectClass;
        jobject jsonObject;

};


#endif JSONOBJECT_H
