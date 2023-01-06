#include "SharedPreferences.h"

#include "jni.h"
#include "../Utils/obfuscate.h"

__attribute__ ((visibility ("hidden")))
SharedPreferences::SharedPreferences(JNIEnv* env, jobject context ) {

    /* SharedPreferences sharedPreferences  = getSharedPreferences("Preferences", 0); */
    jclass      ContextWrapper          = env->FindClass(OBFUSCATE("android/content/ContextWrapper"));
    jmethodID   getSharedPreferences    = env->GetMethodID(ContextWrapper, OBFUSCATE("getSharedPreferences"), OBFUSCATE("(Ljava/lang/String;I)Landroid/content/SharedPreferences;"));
    jobject     sharedPreferences       = env->CallObjectMethod(context, getSharedPreferences, env->NewStringUTF(OBFUSCATE("Preferences")), 0);

    this->env   = env;
    this->sharedPreferences = sharedPreferences;
}

__attribute__ ((visibility ("hidden")))
jstring SharedPreferences::getPreference(jstring key) {

    //String preference     = sharedPreferences.getString( key, null);
    jclass      SharedPreferences   = this->env->FindClass(OBFUSCATE("android/content/SharedPreferences"));
    jmethodID   getString           = env->GetMethodID(SharedPreferences, OBFUSCATE("getString"), OBFUSCATE("(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"));
    jstring     preference          = static_cast<jstring>(env->CallObjectMethod(sharedPreferences,getString,key,NULL));

    return preference;
}

__attribute__ ((visibility ("hidden")))
void SharedPreferences::setPreference(jstring key, jstring value) {

    /* SharedPreferences.Editor editor = sharedPreferences.edit() */
    jclass      SharedPreferences   = env->FindClass(OBFUSCATE("android/content/SharedPreferences"));
    jmethodID   edit                = env->GetMethodID(SharedPreferences, OBFUSCATE("edit"), OBFUSCATE("()Landroid/content/SharedPreferences$Editor;"));
    jobject     editor              = env->CallObjectMethod(sharedPreferences, edit);

    /* editor.putString(key, value); */
    jclass      SharedPreferencesEditor = env->FindClass(OBFUSCATE("android/content/SharedPreferences$Editor"));
    jmethodID   putString               = env->GetMethodID(SharedPreferencesEditor, OBFUSCATE("putString"), OBFUSCATE("(Ljava/lang/String;Ljava/lang/String;)Landroid/content/SharedPreferences$Editor;"));
    env->CallObjectMethod(editor, putString, key, value);

    /* editor.apply(); */
    jmethodID   apply = env->GetMethodID(SharedPreferencesEditor, OBFUSCATE("apply"), OBFUSCATE("()V"));
    env->CallVoidMethod(editor, apply);

}