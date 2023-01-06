//
// Created by Hujifa on 4/2/2022.
//

#include "JSONObject.h"
#include "../Utils/obfuscate.h"

__attribute__ ((visibility ("hidden")))
JSONObject::JSONObject(JNIEnv* env) {

    /* JSONObject jsonObject = new JSONObject(); */
    jclass      JSONObjectClass     = env->FindClass("org/json/JSONObject");
    jmethodID   constructJSONObject = env->GetMethodID(JSONObjectClass,"<init>", "()V");
    jobject     jsonObject          = env->NewObject(JSONObjectClass,constructJSONObject);

    /* Initialize variables */
    this->env   = env;
    this->jsonObject    = jsonObject;
    this->JSONObjectClass   = JSONObjectClass;
}

__attribute__ ((visibility ("hidden")))
JSONObject::JSONObject(JNIEnv *env, jstring json) {

    /* JSONObject jsonObject = new JSONObject(json); */
    jclass      JSONObjectClass     = env->FindClass("org/json/JSONObject");
    jmethodID   constructJSONObject = env->GetMethodID(JSONObjectClass,"<init>", "(Ljava/lang/String;)V");
    jobject     jsonObject          = env->NewObject(JSONObjectClass,constructJSONObject, json);

    /* Initialize env variable */
    this->env   = env;
    this->jsonObject    = jsonObject;
    this->JSONObjectClass   = JSONObjectClass;
}

__attribute__ ((visibility ("hidden")))
void JSONObject::put(jstring key, jstring value) {

    /* jsonObject.put( key, value ); */
    jmethodID put  = env->GetMethodID( JSONObjectClass,"put", "(Ljava/lang/String;Ljava/lang/Object;)Lorg/json/JSONObject;");
    env->CallObjectMethod( jsonObject, put, key, value);
}

__attribute__ ((visibility ("hidden")))
jstring JSONObject::getString( jstring name) {

    /* String value =  jsonObject.getString("name" ) */
    jclass      JSONObject  = env->FindClass("org/json/JSONObject");
    jmethodID   getString   = env->GetMethodID(JSONObject,"getString", "(Ljava/lang/String;)Ljava/lang/String;");
    jstring     value       = static_cast<jstring>(env->CallObjectMethod(jsonObject,getString,name));
    return      value;
}

__attribute__ ((visibility ("hidden")))
jstring JSONObject::toString() {

    /* String jsonObjectToString = jsonObject.toString() */
    jmethodID   toString            = env->GetMethodID( JSONObjectClass , "toString", "()Ljava/lang/String;");
    jstring     jsonObjectToString  = static_cast<jstring>(env->CallObjectMethod(jsonObject, toString));
    return      jsonObjectToString;
}


