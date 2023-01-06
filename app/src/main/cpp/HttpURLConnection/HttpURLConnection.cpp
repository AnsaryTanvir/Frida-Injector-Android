#include "HttpURLConnection.h"
#include "../Utils/obfuscate.h"


/**@brief Read String from InputStream
 *
 * @param env
 * @param inputStream
 * @return
 */
__attribute__ ((visibility ("hidden")))
jstring HttpURLConnection::readInputStream(JNIEnv *env, jobject inputStream ) {

    /* ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream(); */
    jclass      ByteArrayOutputStream           = env->FindClass(OBFUSCATE("java/io/ByteArrayOutputStream"));
    jmethodID   constructByteArrayOutputStream  = env->GetMethodID(ByteArrayOutputStream, OBFUSCATE("<init>"), OBFUSCATE("()V"));
    jobject     byteArrayOutputStream           = env->NewObject(ByteArrayOutputStream,constructByteArrayOutputStream);

    /* int readLine = inputStream.read(); */
    jclass      InputStream     = env->FindClass(OBFUSCATE("java/io/InputStream"));
    jmethodID   read            = env->GetMethodID(InputStream,OBFUSCATE("read"),OBFUSCATE("()I"));
    int         readLine        = env->CallIntMethod(inputStream,read);

    jmethodID   write           = env->GetMethodID(ByteArrayOutputStream, OBFUSCATE("write"),OBFUSCATE("(I)V"));

    while ( readLine != -1 ){
        /* byteArrayOutputStream.write(readLine); */
        env->CallVoidMethod(byteArrayOutputStream, write, readLine);
        /* readLine = inputStream.read(); */
        readLine = env->CallIntMethod(inputStream,read);
    }

    /* return byteArrayOutputStream.toString(); */
    jmethodID   toString    = env->GetMethodID(ByteArrayOutputStream, OBFUSCATE("toString"),OBFUSCATE("()Ljava/lang/String;"));
    jstring     str         = static_cast<jstring>(env->CallObjectMethod(byteArrayOutputStream, toString));

    /* Freeing Memory */
    env->DeleteLocalRef(ByteArrayOutputStream);
    env->DeleteLocalRef(byteArrayOutputStream);
    env->DeleteLocalRef(InputStream);

    return str;
}


/**@brief Make HTTP Get Request using HttpURLConnection class
 *
 * @param env
 * @param SERVER_URL
 * @return The response received from server.
 */
__attribute__ ((visibility ("hidden")))
jstring HttpURLConnection::GET(JNIEnv *env, jstring SERVER_URL) {

    /* URL url = new URL("https://api.gamingbd.pro/api/fixinject.php"); */
    jclass      URL             = env->FindClass(OBFUSCATE("java/net/URL"));
    jmethodID   constructURL    = env->GetMethodID(URL,OBFUSCATE("<init>"), OBFUSCATE("(Ljava/lang/String;)V"));
    jobject     url             = env->NewObject(URL, constructURL, SERVER_URL );

    /* In Java We need to cast object of one type to another type, but in native code the casting is automatically handled by JNI */
    /* https://stackoverflow.com/questions/50019308/jni-casting-jobject-from-one-class-to-another */
    /* HttpURLConnection urlConnection = (HttpURLConnection) url.openConnection(); */
    jmethodID   openConnection      = env->GetMethodID(URL,OBFUSCATE("openConnection"), OBFUSCATE("()Ljava/net/URLConnection;"));
    jobject     httpURLConnection   = env->CallObjectMethod(url, openConnection) ;

    /* urlConnection.disconnect(); */
    jclass      HttpURLConnection   = env->FindClass(OBFUSCATE("java/net/HttpURLConnection"));
    jmethodID   disconnect          = env->GetMethodID(HttpURLConnection, OBFUSCATE("disconnect"), OBFUSCATE("()V"));
    env->CallVoidMethod(httpURLConnection,disconnect);

    /* InputStream inputStream = urlConnection.getInputStream();  */
    jclass      URLConnection   = env->FindClass(OBFUSCATE("java/net/URLConnection"));
    jmethodID   getInputStream  = env->GetMethodID(URLConnection, OBFUSCATE("getInputStream"), OBFUSCATE("()Ljava/io/InputStream;"));
    jobject     inputStream     = env->CallObjectMethod(httpURLConnection, getInputStream );
    if(env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
        return env->NewStringUTF(OBFUSCATE("Internet not available | Server error"));
    }


    /* String response =  readInputStream( inputStream ); */
    jstring response = HttpURLConnection::readInputStream(env,inputStream);

    /* Freeing Memory */
    env->DeleteLocalRef(URL);
    env->DeleteLocalRef(url);
    env->DeleteLocalRef(httpURLConnection);
    env->DeleteLocalRef(HttpURLConnection);
    env->DeleteLocalRef(URLConnection);
    env->DeleteLocalRef(inputStream);

    /* return response; */
    return response;
}

/** Make HTTP Post Request Using HttpURLConnection Class
 *
 * @param env
 * @param SERVER_URL
 * @param parameters
 * @return The response received from server.
 */
__attribute__ ((visibility ("hidden")))
jstring HttpURLConnection::POST(JNIEnv *env, jstring SERVER_URL, jstring parameters) {

    /* URL url = new URL("https://api.gamingbd.pro/api/native.php"); */
    jclass      URL             = env->FindClass(OBFUSCATE("java/net/URL"));
    jmethodID   constructURL    = env->GetMethodID(URL,OBFUSCATE("<init>"), OBFUSCATE("(Ljava/lang/String;)V"));
    jobject     url             = env->NewObject(URL,constructURL, SERVER_URL );

    /* In Java We need to cast object of one type to another type, but in native code the casting is automatically handled by JNI */
    /* https://stackoverflow.com/questions/50019308/jni-casting-jobject-from-one-class-to-another */
    /* HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection(); */
    jclass HttpURLConnection  = env->FindClass(OBFUSCATE("java/net/HttpURLConnection"));
    jmethodID openConnection  = env->GetMethodID(URL,OBFUSCATE("openConnection"), OBFUSCATE("()Ljava/net/URLConnection;"));
    jobject httpURLConnection = env->CallObjectMethod(url, openConnection) ;


    /* httpURLConnection.setRequestMethod("POST"); */
    jmethodID   setRequestMethod    = env->GetMethodID(HttpURLConnection,OBFUSCATE("setRequestMethod"), OBFUSCATE("(Ljava/lang/String;)V"));
    env->CallVoidMethod(httpURLConnection,setRequestMethod, env->NewStringUTF(OBFUSCATE("POST")));


    /* httpURLConnection.setDoInput(true); */
    jclass      URLConnection   = env->FindClass(OBFUSCATE("java/net/URLConnection"));
    jmethodID   setDoInput      = env->GetMethodID(URLConnection, OBFUSCATE("setDoInput"), OBFUSCATE("(Z)V"));
    env->CallVoidMethod(httpURLConnection, setDoInput,true );

    /* httpURLConnection.setDoOutput(true); */
    jmethodID   setDoOutput     = env->GetMethodID(URLConnection, OBFUSCATE("setDoOutput"), OBFUSCATE("(Z)V"));
    env->CallVoidMethod(httpURLConnection, setDoOutput, true );


    /* OutputStream outputStream = httpURLConnection.getOutputStream(); */
    jmethodID   getOutputStream     = env->GetMethodID(URLConnection,OBFUSCATE("getOutputStream"), OBFUSCATE("()Ljava/io/OutputStream;"));
    jobject     outputStream        = env->CallObjectMethod(httpURLConnection, getOutputStream);
    if(env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
        return env->NewStringUTF(OBFUSCATE("Internet not available | Server error"));
    }

    /* Charset charset = StandardCharsets.UTF_8; */
    jclass      StandardCharsets    = env->FindClass(OBFUSCATE("java/nio/charset/StandardCharsets"));
    jfieldID    UTF_8_FieldId       = env->GetStaticFieldID(StandardCharsets,OBFUSCATE("UTF_8"),OBFUSCATE("Ljava/nio/charset/Charset;"));
    jobject     charset             = env->GetStaticObjectField(StandardCharsets,UTF_8_FieldId);


    /*----------------Starts Adding Post Parameter --------------------------*/
    /* byte[] byteArray = parameters.getBytes(charset); */
    jclass      String      = env->FindClass(OBFUSCATE("java/lang/String"));
    jmethodID   getBytes    = env->GetMethodID(String, OBFUSCATE("getBytes"), OBFUSCATE("(Ljava/nio/charset/Charset;)[B"));
    jbyteArray  byteArr     = static_cast<jbyteArray>(env->CallObjectMethod(parameters, getBytes,charset));

    /* outputStream.write(byteArray); */
    jclass      OutputStream    = env->FindClass(OBFUSCATE("java/io/OutputStream"));
    jmethodID   write           = env->GetMethodID(OutputStream,OBFUSCATE("write"), OBFUSCATE("([B)V"));
    env->CallVoidMethod(outputStream,write,byteArr);

    /* outputStream.flush(); */
    jmethodID   flush   =   env->GetMethodID(OutputStream, OBFUSCATE("flush"), OBFUSCATE("()V"));
    env->CallVoidMethod(outputStream,flush);

    /* outputStream.close(); */
    jmethodID   close   =   env->GetMethodID(OutputStream, OBFUSCATE("close"), OBFUSCATE("()V"));
    env->CallVoidMethod(outputStream, close);

    /* InputStream inputStream = httpURLConnection.getInputStream();  */
    jmethodID   getInputStream  = env->GetMethodID(URLConnection, OBFUSCATE("getInputStream"), OBFUSCATE("()Ljava/io/InputStream;"));
    jobject     inputStream     = env->CallObjectMethod(httpURLConnection, getInputStream );
    if(env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
        return env->NewStringUTF(OBFUSCATE("Internet not available | Server error"));
    }

    /* String response =  readInputStream( inputStream ); */
    jstring response = HttpURLConnection::readInputStream(env, inputStream);

    env->DeleteLocalRef(URL);
    env->DeleteLocalRef(url);
    env->DeleteLocalRef(httpURLConnection);
    env->DeleteLocalRef(HttpURLConnection);
    env->DeleteLocalRef(URLConnection);
    env->DeleteLocalRef(outputStream);
    env->DeleteLocalRef(inputStream);
    env->DeleteLocalRef(StandardCharsets);
    env->DeleteLocalRef(charset);
    env->DeleteLocalRef(String);
    env->DeleteLocalRef(OutputStream);

    /* return response; */
    return response;

}