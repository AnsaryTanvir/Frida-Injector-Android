//
// Created by ansary on 10/15/22.
//

#include "RootManager.h"
#include "../Utils/obfuscate.h"

__attribute__ ((visibility ("hidden")))
void RootManager::runCommandV(JNIEnv* env, jstring command) {

    /** RootManager instance = RootManager.getInstance(); */
    jclass      RootManager = env->FindClass(OBFUSCATE("com/chrisplus/rootmanager/RootManager"));
    jmethodID   getInstance = env->GetStaticMethodID(RootManager, OBFUSCATE("getInstance"), OBFUSCATE("()Lcom/chrisplus/rootmanager/RootManager;"));
    jobject     instance    = env->CallStaticObjectMethod(RootManager, getInstance);

    /** instance.runCommand(command); */
    jmethodID   runCommand  = env->GetMethodID(RootManager, OBFUSCATE("runCommand"),OBFUSCATE("(Ljava/lang/String;)Lcom/chrisplus/rootmanager/container/Result;"));
    env->CallObjectMethod(instance, runCommand, command);
}


__attribute__ ((visibility ("hidden")))
jstring RootManager::runCommandS(JNIEnv *env, jstring command) {

    /** RootManager instance = RootManager.getInstance(); */
    jclass      RootManager = env->FindClass(OBFUSCATE("com/chrisplus/rootmanager/RootManager"));
    jmethodID   getInstance = env->GetStaticMethodID(RootManager, OBFUSCATE("getInstance"), OBFUSCATE("()Lcom/chrisplus/rootmanager/RootManager;"));
    jobject     instance    = env->CallStaticObjectMethod(RootManager, getInstance);

    /** instance.runCommand(command); */
    jmethodID   runCommand  = env->GetMethodID(RootManager, OBFUSCATE("runCommand"),OBFUSCATE("(Ljava/lang/String;)Lcom/chrisplus/rootmanager/container/Result;"));
    jobject     result      = env->CallObjectMethod(instance, runCommand, command);

    /** return result.getMessage(); */
    jclass      Result      = env->FindClass(OBFUSCATE("com/chrisplus/rootmanager/container/Result"));
    jmethodID   getMessage  = env->GetMethodID(Result, OBFUSCATE("getMessage"), OBFUSCATE("()Ljava/lang/String;"));
    jstring     message     = static_cast<jstring>( env->CallObjectMethod(result, getMessage));

    return  message;
}

