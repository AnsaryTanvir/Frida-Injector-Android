// Created by Hujifa on 4/1/2022.

#include "Jasypt.h"
#include "../Utils/obfuscate.h"

/* Parameters: Current JNIEnv , encryptionKey , plainText
 * Return String Encrypted by BasicTextEncryptor
 * Example:
    jstring key = env->NewStringUTF("myKey");
    jstring text = env->NewStringUTF("Hello World");
    LOGD("%s", env->GetStringUTFChars( Jasypt::encryptString( env , key, text  ), NULL ));
 */
__attribute__ ((visibility ("hidden")))
jstring Jasypt::encryptString(JNIEnv *env, jstring encryptionKey, jstring plainText) {

    /* BasicTextEncryptor basicTextEncryptor = new BasicTextEncryptor(); */
    jclass BasicTextEncryptor = env->FindClass("org/jasypt/util/text/BasicTextEncryptor");
    jmethodID constructBasicTextEncryptor = env->GetMethodID(BasicTextEncryptor,"<init>", "()V");
    jobject  basicTextEncryptor = env->NewObject(BasicTextEncryptor,constructBasicTextEncryptor);

    /* basicTextEncryptor.setPassword(encryptionKey); */
    jmethodID setPassword = env->GetMethodID(BasicTextEncryptor,"setPassword", "(Ljava/lang/String;)V");
    env->CallVoidMethod(basicTextEncryptor,setPassword, encryptionKey );

    /* String encryptedText = basicTextEncryptor.encrypt(plainText); */
    jmethodID encrypt = env->GetMethodID(BasicTextEncryptor,"encrypt", "(Ljava/lang/String;)Ljava/lang/String;");
    jstring encryptedText = (jstring) env->CallObjectMethod(basicTextEncryptor,encrypt, plainText );

    return encryptedText;

}

/* Parameters: Current JNIEnv , Encryption Key , Encrypted Text
 * Return String Decrypted by BasicTextEncryptor
 * Example:
    jstring key = env->NewStringUTF("myKey");
    jstring encryptedText = env->NewStringUTF("aZYPbCi16SaDcyPSrJ4vl0D/xfTEvIuX");
    LOGD("%s", env->GetStringUTFChars( Jasypt::decryptString(env , key, encryptedText), NULL ));
 */
__attribute__ ((visibility ("hidden")))
jstring Jasypt::decryptString(JNIEnv *env, jstring encryptionKey, jstring encryptedText) {

    /* BasicTextEncryptor basicTextEncryptor = new BasicTextEncryptor(); */
    jclass BasicTextEncryptor = env->FindClass("org/jasypt/util/text/BasicTextEncryptor");
    jmethodID constructBasicTextEncryptor = env->GetMethodID(BasicTextEncryptor,"<init>", "()V");
    jobject  basicTextEncryptor = env->NewObject(BasicTextEncryptor,constructBasicTextEncryptor);

    /* basicTextEncryptor.setPassword(encryptionKey); */
    jmethodID setPassword = env->GetMethodID(BasicTextEncryptor,"setPassword", "(Ljava/lang/String;)V");
    env->CallVoidMethod(basicTextEncryptor,setPassword, encryptionKey );

    /* String encryptedText = basicTextEncryptor.encrypt(plainText); */
    jmethodID decrypt = env->GetMethodID(BasicTextEncryptor,"decrypt", "(Ljava/lang/String;)Ljava/lang/String;");
    jstring decryptedText = (jstring) env->CallObjectMethod(basicTextEncryptor,decrypt, encryptedText );

    return decryptedText;
}