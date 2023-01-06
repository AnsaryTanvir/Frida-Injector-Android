// Created by Hujifa on 4/1/2022.

#include "Base64.h"
#include "../Utils/obfuscate.h"


/** Encode UTF-8 Plan Text to Base64 Encoded String. ( No Wrap  means No Line Break ).
 *
 * Example:
 *          string encoded = Base64::toBase64String(env,env->NewStringUTF("Hello"));
 *
 * @param env
 * @param string The UTF-8 string which is to be encoded in Base64.
 * @return Returns Base64 encoded string.
 */
__attribute__ ((visibility ("hidden")))
jstring Base64::toBase64String(JNIEnv *env, jstring string) {

    /* Charset charset = StandardCharsets.UTF_8; */
    jclass      StandardCharsets    = env->FindClass("java/nio/charset/StandardCharsets");
    jfieldID    UTF_8_FieldId       = env->GetStaticFieldID(StandardCharsets,"UTF_8","Ljava/nio/charset/Charset;");
    jobject     charset             = env->GetStaticObjectField(StandardCharsets,UTF_8_FieldId);

    /* byte[] byteArray = string.getBytes( charset ); */
    jclass      String              = env->FindClass("java/lang/String");
    jmethodID   getBytes            = env->GetMethodID(String,"getBytes","(Ljava/nio/charset/Charset;)[B");
    jbyteArray  byteArray           = static_cast<jbyteArray>(env->CallObjectMethod(string, getBytes, charset));

    /* return Base64.encodeToString( byteArray, 2); */
    jclass      Base64              = env->FindClass(OBFUSCATE("android/util/Base64"));
    jmethodID   encodeToString      = env->GetStaticMethodID(Base64, OBFUSCATE("encodeToString"),"([BI)Ljava/lang/String;");
    jstring     base64Encoded       = static_cast<jstring>( env->CallStaticObjectMethod(Base64, encodeToString,byteArray, Base64_NO_WRAP));

    return base64Encoded;
}


/** Decode Base64 Encoded String to UTF-8 Plan Text.
 *
 * Example:
 *          jstring decoded = Base64::fromBase64String(env, env->NewStringUTF("SGVsbG8="));
 *
 * @param env
 * @param string
 * @return Returns Base64 decoded UTF-8 Plan Text.
 */
__attribute__ ((visibility ("hidden")))
jstring Base64::fromBase64String(JNIEnv *env, jstring string) {

    /* byte decodedByteArray[] = Base64.decode( string, Base64.DEFAULT); */
    jclass      Base64              = env->FindClass(OBFUSCATE("android/util/Base64"));
    jmethodID   decode              = env->GetStaticMethodID(Base64, OBFUSCATE("decode"),"(Ljava/lang/String;I)[B");
    jbyteArray  decodedByteArray    = static_cast<jbyteArray>(env->CallStaticObjectMethod(Base64,decode,string,Base64_DEFAULT));

    /* return new String( decodedByteArray, StandardCharsets.UTF_8); */
    jclass      StandardCharsets    = env->FindClass("java/nio/charset/StandardCharsets");
    jfieldID    UTF_8_FieldId       = env->GetStaticFieldID(StandardCharsets,"UTF_8","Ljava/nio/charset/Charset;");
    jobject     UTF_8               = env->GetStaticObjectField(StandardCharsets, UTF_8_FieldId);

    jclass      String              = env->FindClass("java/lang/String");
    jmethodID   constructString     = env->GetMethodID(String,"<init>","([BLjava/nio/charset/Charset;)V");
    jstring     decodedString       = static_cast<jstring>(env->NewObject(String, constructString,decodedByteArray, UTF_8));

    return decodedString;
}
