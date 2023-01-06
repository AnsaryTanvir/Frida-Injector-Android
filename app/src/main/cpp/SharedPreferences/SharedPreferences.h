#ifndef ULTRA_X_TEAM_SHAREDPREFERENCES_H
#define ULTRA_X_TEAM_SHAREDPREFERENCES_H

#include <jni.h>

class SharedPreferences {

    public:
        jobject sharedPreferences;


        jstring getPreference(jstring key);
        void    setPreference(jstring key, jstring value);
        SharedPreferences(JNIEnv* env, jobject context);


    private:
        JNIEnv* env;
        jobject thiz;
};


#endif