// Created by Hujifa on 4/1/2022.

#include "Layout.h"
#include "../Utils/obfuscate.h"

/**@brief Hide Action Bar from an Activity
 *
 * @param env
 * @param context
 */
__attribute__ ((visibility ("hidden")))
void Layout::hideActionBar(JNIEnv *env, jobject context) {

    /* requestWindowFeature(Window.FEATURE_NO_TITLE); */
    jclass      CurrentActivity         = env->GetObjectClass(context);
    jmethodID   requestWindowFeature    = env->GetMethodID(CurrentActivity,"requestWindowFeature","(I)Z");
    env->CallBooleanMethod(context, requestWindowFeature, FEATURE_NO_TITLE);

    /* ActionBar actionBar = getSupportActionBar(); */
    jmethodID   getSupportActionBar     = env->GetMethodID(CurrentActivity,"getSupportActionBar", "()Landroidx/appcompat/app/ActionBar;");
    jclass      ActionBar               = env->FindClass("androidx/appcompat/app/ActionBar");
    jobject     actionBar               = env->CallObjectMethod(context, getSupportActionBar);

    /* actionBar.hide(); */
    jmethodID   hide    = env->GetMethodID(ActionBar,"hide","()V");
    env->CallVoidMethod(actionBar,hide);

    env->DeleteLocalRef(CurrentActivity);
    env->DeleteLocalRef(ActionBar);
}

/**@brief Convert Pixel Unit to dp Unit
 *
 * @param env
 * @param context
 * @param dp jfloat The unit in dp
 * @return jint The pixel equivalent of given dp unit
 */
__attribute__ ((visibility ("hidden")))
jint Layout::getPixelFromDp(JNIEnv *env, jobject context, jfloat dp) {

    /*  return (int) ( (f * getResources().getDisplayMetrics().density) + 0.5f); */

    /* Resources loader = getResources(); */
    jclass      CurrentActivity     = env->GetObjectClass(context);
    jmethodID   getResources        = env->GetMethodID(CurrentActivity,"getResources","()Landroid/content/res/Resources;");
    jobject     resources           = env->CallObjectMethod(context, getResources);

    /* DisplayMetrics displayMetrics = loader.getDisplayMetrics(); */
    jclass      Resources           = env->FindClass("android/content/res/Resources");
    jmethodID   getDisplayMetrics   = env->GetMethodID(Resources,"getDisplayMetrics", "()Landroid/util/DisplayMetrics;");
    jobject     displayMetrics      = env->CallObjectMethod(resources,getDisplayMetrics);

    /* float density = getDisplayMetrics().density; */
    jclass      DisplayMetrics      = env->FindClass("android/util/DisplayMetrics");
    jfieldID    densityField        = env->GetFieldID(DisplayMetrics,"density", "F");
    jfloat      density             = env->GetFloatField(displayMetrics,densityField);

    /* adding 0.5f with pixel in to make it accurate in terms of type casting */
    jint pixel = ( density * dp ) + 0.5f;

    env->DeleteLocalRef(CurrentActivity);
    env->DeleteLocalRef(resources);
    env->DeleteLocalRef(Resources);
    env->DeleteLocalRef(displayMetrics);
    env->DeleteLocalRef(DisplayMetrics);

    return pixel;
}