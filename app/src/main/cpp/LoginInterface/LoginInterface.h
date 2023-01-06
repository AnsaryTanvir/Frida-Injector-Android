// Created by Hujifa on 4/2/2022.

#ifndef LOGINTEMPLATE_H
#define LOGINTEMPLATE_H

#include <jni.h>

class LoginInterface {

    private:
        const jint MATCH_PARENT         = -1  ;     // LinearLayout.LayoutParams.MATCH_PARENT
        const jint WRAP_CONTENT         = -2  ;     // LinearLayout.LayoutParams.WRAP_CONTENT
        const jint ORIENTATION_VERTICAL =  1  ;     // Orientation Vertical
        const jint GRAVITY_CENTER       = 17  ;     // Gravity.CENTER
        const jint GRAVITY_RIGHT        = 5   ;     // Gravity.RIGHT
        const jint PASSWORD_TYPE        = 129 ;     // Edit Text Input Type Password


    public:

        static const jint VISIBLE       = 0   ;     // VIew.VISIBLE
        static const jint GONE          = 8   ;     // VIew.GONE

        /* Reason of using global refrence:
                * https://developer.android.com/training/articles/perf-jni
                * https://stackoverflow.com/questions/51367339/what-causes-the-jni-error-use-of-deleted-local-reference
        */
        jobject usernameEditTextGlobalRefs;
        jobject passwordEditTextGlobalRefs;
        jobject rememberCheckBoxGlobalRefs;
        jobject loginButtonGlobalRefs;
        jobject progressBarGlobalRefs;

        /* Constructors */
        LoginInterface(JNIEnv *env, jobject context);

};


#endif
