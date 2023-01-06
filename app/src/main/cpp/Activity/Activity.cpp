// Created by Hujifa on 4/1/2022.

#include "Activity.h"
#include <string>
#include "../Utils/obfuscate.h"

/** Get the Package Name of Caller
 *
 * @param env
 * @param context
 * @return jstring The caller's package name
 */
__attribute__ ((visibility ("hidden")))
jstring Activity::getPackageName( JNIEnv *env, jobject context ){

    /* return this.getPackageName(); */
    jclass      CurrentActivity = env->GetObjectClass(context);
    jmethodID   getPackageName  = env->GetMethodID(CurrentActivity, OBFUSCATE("getPackageName"), OBFUSCATE("()Ljava/lang/String;"));
    jstring     packageName     = (jstring) env->CallObjectMethod(context, getPackageName);

    return packageName;
}


/** Show a Toast on an Activity
 *
 * @param env
 * @param context
 * @param text         The text to show via toast.
 * @param TOAST_LENGTH The length of the toast either 0 for short or 1 for long.
 */
__attribute__ ((visibility ("hidden")))
void Activity::Toast(JNIEnv *env, jobject context, jstring text , jint TOAST_LENGTH ){

    /* Toast toastObject = Toast.makeText(this, "String", Toast.LENGTH_LONG); */
    jclass      Toast       = env->FindClass(OBFUSCATE("android/widget/Toast"));
    jmethodID   makeText    = env->GetStaticMethodID(Toast, OBFUSCATE("makeText"), OBFUSCATE("(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;"));
    jobject     toastObject = env->CallStaticObjectMethod(Toast, makeText, context, text, TOAST_LENGTH);

    /* toastObject.show(); */
    jmethodID   show        = env->GetMethodID(Toast, OBFUSCATE("show"), OBFUSCATE("()V"));
    env->CallVoidMethod(toastObject,show);

}