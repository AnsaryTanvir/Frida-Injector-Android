// Created by Hujifa on 4/1/2022.

#ifndef JASYPT_H
#define JASYPT_H

#include <jni.h>

/* Jasypt - http://www.jasypt.org/
 * Dependency: implementation group: 'org.jasypt', name: 'jasypt', Version: '1.9.3'
 */

class Jasypt {

    public:
    static jstring encryptString(JNIEnv *env, jstring encryptionKey, jstring plainText);
    static jstring decryptString(JNIEnv *env, jstring encryptionKey, jstring encryptedText);

};


#endif
