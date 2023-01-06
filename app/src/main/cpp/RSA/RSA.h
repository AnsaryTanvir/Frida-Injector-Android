// Created by Ansary Tanvir on 4/1/2022.

#ifndef RSA_H
#define RSA_H

#include "jni.h"
#include <cstring>
#include <string>

class RSA {

    public:
        static jstring encryptByPublic(JNIEnv *env, jstring plainText);
        static jstring decryptByPublic(JNIEnv *env, jstring encryptedText);
        static jstring encryptByChunks(JNIEnv *env, jstring plainText);
        static jstring decryptByChunks(JNIEnv *env, jstring data, jstring splitter);

    private:
        static const jint Cipher_ENCRYPT_MODE        = 1;            // Cipher.ENCRYPT_MODE
        static const jint Cipher_DECRYPT_MODE        = 2;            // Cipher.DECRYPT_MODE
        static const jint Base64_DEFAULT             = 0;            // Bas64.DEFAULT
        static const jint Base64_NO_WRAP             = 2;            // Base64.NO_WRAP

        static jobject getPublicKeyFromX509(JNIEnv *env, jstring algorithm, jstring publicKey);

};


#endif
