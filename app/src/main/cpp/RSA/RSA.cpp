// Created by Ansary Tanvir on 4/1/2022.

#include "RSA.h"
#include "../Utils/obfuscate.h"


__attribute__ ((visibility ("hidden")))
jobject RSA::getPublicKeyFromX509(JNIEnv *env, jstring algorithm, jstring publicKey){

    /* byte[] decodedKey       = Base64.decode( publicKey, Base64.DEFAULT); */
    jclass          Base64     = env->FindClass(OBFUSCATE("android/util/Base64"));
    jmethodID       decode     = env->GetStaticMethodID(Base64,OBFUSCATE("decode"), OBFUSCATE("(Ljava/lang/String;I)[B"));
    jobjectArray    decodedKey = static_cast<jobjectArray>(env->CallStaticObjectMethod(Base64, decode,publicKey, Base64_DEFAULT));

    /* X509EncodedKeySpec x509              = new X509EncodedKeySpec(decodedKey); */
    jclass      X509EncodedKeySpec          = env->FindClass(OBFUSCATE("java/security/spec/X509EncodedKeySpec"));
    jmethodID   constructX509EncodedKeySpec = env->GetMethodID(X509EncodedKeySpec, OBFUSCATE("<init>"), OBFUSCATE("([B)V"));
    jobject     x509                        = env->NewObject(X509EncodedKeySpec, constructX509EncodedKeySpec, decodedKey);

    /* KeyFactory keyFactory    = KeyFactory.getInstance(algorithm); */
    jclass      KeyFactory      = env->FindClass(OBFUSCATE("java/security/KeyFactory"));
    jmethodID   getInstance     = env->GetStaticMethodID(KeyFactory, OBFUSCATE("getInstance"), OBFUSCATE("(Ljava/lang/String;)Ljava/security/KeyFactory;"));
    jobject     keyFactory      = env->CallStaticObjectMethod(KeyFactory, getInstance, algorithm);

    /* return keyFactory.generatePublic(x509); */
    jmethodID   generatePublic  = env->GetMethodID(KeyFactory, OBFUSCATE("generatePublic"), OBFUSCATE("(Ljava/security/spec/KeySpec;)Ljava/security/PublicKey;"));
    return      env->CallObjectMethod(keyFactory, generatePublic, x509 );
}


/* Parameters: Current JNIEnv , String
 * Returns  RSA Encrypted String
 * Example:
    jstring encoded = RSA::encryptByPublic(env, env->NewStringUTF("Hi"));
    LOGD("%s", env->GetStringUTFChars(encoded, NULL));
 */

__attribute__ ((visibility ("hidden")))
jstring RSA::encryptByPublic(JNIEnv *env, jstring plainText){

    jstring ALGORITHM   = env->NewStringUTF(OBFUSCATE("RSA"));
    jstring RSA_PUBLIC  = env->NewStringUTF(OBFUSCATE("MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCYDDsYxade5YPE89FCYSL72TAOa1nvzVzWkcmi0fx/59X5KHQKrcyLlU4+5aYAJQUtIXhifk7gCj00tNcMma7B+SFNy+1KoFQhaajyRNtHfYyjZRmzX2V5QshRaF+x/aB9e9s2Ra4IRXoiUPGEAQIL2J9stCxwtOJersIGiapvGwIDAQAB" ));

    /* PublicKey publicKey = getPublicKeyFromX509(ALGORITHM, RSA_PUBLIC); */
    jobject publicKey   = getPublicKeyFromX509(env, ALGORITHM , RSA_PUBLIC );

    /* byte plainTextToBytes []     = plainText.getBytes(); */
    jclass       String             = env->FindClass(OBFUSCATE("java/lang/String"));
    jmethodID    getBytes           = env->GetMethodID(String, OBFUSCATE("getBytes"), OBFUSCATE("()[B"));
    jobjectArray plainTextToBytes   = static_cast<jobjectArray>(env->CallObjectMethod( plainText, getBytes));

    /* Cipher cipher        = Cipher.getInstance("RSA/ECB/PKCS1Padding"); */
    jclass      Cipher      = env->FindClass(OBFUSCATE("javax/crypto/Cipher"));
    jmethodID   getInstance = env->GetStaticMethodID(Cipher, OBFUSCATE("getInstance"), OBFUSCATE("(Ljava/lang/String;)Ljavax/crypto/Cipher;"));
    jobject     cipher      = env->CallStaticObjectMethod(Cipher,getInstance, env->NewStringUTF(OBFUSCATE("RSA/ECB/PKCS1Padding")));

    /* cipher.init(Cipher.ENCRYPT_MODE, publicKey); */
    jmethodID init = env->GetMethodID(Cipher, OBFUSCATE("init"), OBFUSCATE("(ILjava/security/Key;)V"));
    env->CallVoidMethod(cipher, init, Cipher_ENCRYPT_MODE, publicKey );

    /* byte[] encryptedBytes  = cipher.doFinal(plaintext); */
    jmethodID   doFinal         = env->GetMethodID(Cipher, OBFUSCATE("doFinal"), OBFUSCATE("([B)[B"));
    jbyteArray  encryptedBytes  = static_cast<jbyteArray>(env->CallObjectMethod(cipher, doFinal, plainTextToBytes ));

    /* return Base64.encodeToString( encryptedBytes, Base64.NO_WRAP); */
    jclass      Base64          = env->FindClass(OBFUSCATE("android/util/Base64"));
    jmethodID   encodeToString  = env->GetStaticMethodID(Base64, OBFUSCATE("encodeToString"), OBFUSCATE("([BI)Ljava/lang/String;"));
    jstring     encodedString   = static_cast<jstring>(env->CallStaticObjectMethod(Base64, encodeToString, encryptedBytes , Base64_NO_WRAP ));

    return encodedString;
}


/* Parameters: Current JNIEnv , Encrypted String
 * Returns  RSA Decrypted String
 * Example:
    jstring encryptedMessage = env->NewStringUTF("Whcv1CFP15HK40quwBWUaEZO0B4UA0AxMqCRL9fVibWNX+Rxv+mDHjeCjtRoj5zx0P6EKxSNWA4KBvr7p1gURKMmpS95jcmnBIRAHJCKNQJrE68Mjicf13fwvRgr0xbSY/TYxwlT0KuMH+a0c9vNwXLsKzAahAmjOiTT6G93LsE=");
    jstring decryptedMessage = RSA::decryptByPublic(env, encryptedMessage);
    LOGD("%s", env->GetStringUTFChars(decryptedMessage, NULL));
 */

__attribute__ ((visibility ("hidden")))
jstring RSA::decryptByPublic(JNIEnv *env, jstring encryptedText){

    jstring ALGORITHM   = env->NewStringUTF(OBFUSCATE("RSA"));
    jstring RSA_PUBLIC = env->NewStringUTF(OBFUSCATE("MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCYDDsYxade5YPE89FCYSL72TAOa1nvzVzWkcmi0fx/59X5KHQKrcyLlU4+5aYAJQUtIXhifk7gCj00tNcMma7B+SFNy+1KoFQhaajyRNtHfYyjZRmzX2V5QshRaF+x/aB9e9s2Ra4IRXoiUPGEAQIL2J9stCxwtOJersIGiapvGwIDAQAB" ));

    /* PublicKey publicKey  = getPublicKeyFromX509(ALGORITHM, RSA_PUBLIC); */
    jobject publicKey       = getPublicKeyFromX509(env, ALGORITHM , RSA_PUBLIC );

    /* byte encryptedBytes[]    = Base64.decode( encrypted_text, Base64.DEFAULT); */
    jclass      Base64          = env->FindClass(OBFUSCATE("android/util/Base64"));
    jmethodID   decode          = env->GetStaticMethodID(Base64,OBFUSCATE("decode"), OBFUSCATE("(Ljava/lang/String;I)[B"));
    jbyteArray  encryptedBytes  = static_cast<jbyteArray>(env->CallStaticObjectMethod(Base64, decode,encryptedText,Base64_DEFAULT));

    /* Cipher cipher = Cipher.getInstance("RSA/ECB/PKCS1Padding"); */
    jclass      Cipher      = env->FindClass(OBFUSCATE("javax/crypto/Cipher"));
    jmethodID   getInstance = env->GetStaticMethodID(Cipher, OBFUSCATE("getInstance"), OBFUSCATE("(Ljava/lang/String;)Ljavax/crypto/Cipher;"));
    jobject     cipher      = env->CallStaticObjectMethod(Cipher,getInstance, env->NewStringUTF(OBFUSCATE("RSA/ECB/PKCS1Padding")));

    /* cipher.init(Cipher.DECRYPT_MODE, publicKey); */
    jmethodID init = env->GetMethodID(Cipher, OBFUSCATE("init"), OBFUSCATE("(ILjava/security/Key;)V"));
    env->CallVoidMethod(cipher, init, Cipher_DECRYPT_MODE, publicKey );

    /* byte decryptedMessage[] = cipher.doFinal(encryptedBytes); */
    jmethodID   doFinal             = env->GetMethodID(Cipher, OBFUSCATE("doFinal"), OBFUSCATE("([B)[B"));
    jbyteArray  decryptedMessage    = static_cast<jbyteArray>(env->CallObjectMethod(cipher, doFinal, encryptedBytes));

    /* String plainText  =  new String( decryptedMessage,"UTF8"); */
    jclass      String          = env->FindClass(OBFUSCATE("java/lang/String"));
    jmethodID   constructString = env->GetMethodID(String, OBFUSCATE("<init>"), OBFUSCATE("([BLjava/lang/String;)V"));
    jstring     plainText       = static_cast<jstring>(env->NewObject(String, constructString,decryptedMessage,env->NewStringUTF(OBFUSCATE("UTF8"))));

    return plainText;
}

__attribute__ ((visibility ("hidden")))
jstring RSA::encryptByChunks(JNIEnv *env, jstring largePlainText) {

    const char* string  = env->GetStringUTFChars(largePlainText, NULL);
    int len             = strlen(string);

    int chunksSize      = 117;
    std::string encryptedChunkSum;
    for ( int i = 0 ; i < len; i += chunksSize ){

        char* chunk = (char*) calloc(chunksSize, sizeof (char));
        for ( int j = i, k = 0 ; j < chunksSize+i;  ){
            if ( j == len ) break;
            chunk[k++] = string[j++];
        }

        jstring encryptedChunk = RSA::encryptByPublic(env, env->NewStringUTF(chunk));
        encryptedChunkSum.append(env->GetStringUTFChars(encryptedChunk, NULL));
        encryptedChunkSum.append("#");

        env->DeleteLocalRef(encryptedChunk);
        free(chunk);
    }

    return env->NewStringUTF(encryptedChunkSum.c_str());

}

__attribute__ ((visibility ("hidden")))
jstring RSA::decryptByChunks(JNIEnv *env, jstring data, jstring splitter) {

    /* StringBuilder stringBuilder = new StringBuilder(); */
    jclass      StringBuilder           = env->FindClass(OBFUSCATE("java/lang/StringBuilder"));
    jmethodID   constructStringBuilder  = env->GetMethodID(StringBuilder, OBFUSCATE("<init>"), OBFUSCATE("()V"));
    jobject     stringBuilder           = env->NewObject(StringBuilder, constructStringBuilder);
    jmethodID   append                  = env->GetMethodID(StringBuilder, OBFUSCATE("append"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/StringBuilder;"));
    jmethodID   toString                = env->GetMethodID(StringBuilder, OBFUSCATE("toString"), OBFUSCATE("()Ljava/lang/String;"));

    /*  String[] chunks             = data.split(splitter); */
    jclass          String  = env->FindClass(OBFUSCATE("java/lang/String"));
    jmethodID       split   = env->GetMethodID(String, OBFUSCATE("split"), OBFUSCATE("(Ljava/lang/String;)[Ljava/lang/String;"));
    jobjectArray    chunks  = static_cast<jobjectArray>(env->CallObjectMethod(data, split, splitter));

    int len = env->GetArrayLength(chunks);
    for ( int i = 0; i < len; i++ ){
        jstring chunk           = static_cast<jstring>(env->GetObjectArrayElement(chunks, i));
        jstring decryptedChunk  = RSA::decryptByPublic(env,chunk);
        env->CallObjectMethod(stringBuilder, append, decryptedChunk);
    }

    jstring decryptedString = static_cast<jstring>(env->CallObjectMethod(stringBuilder, toString));
    return  decryptedString;
}