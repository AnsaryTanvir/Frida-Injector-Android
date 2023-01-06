//
// Created by ansary on 10/15/22.
//

#ifndef ROOTMANAGER_H
#define ROOTMANAGER_H

#include "jni.h"

class RootManager {
    public:
        static void    runCommandV(JNIEnv* env, jstring command);
        static jstring runCommandS(JNIEnv*env, jstring command);

};

#endif ROOTMANAGER_H
