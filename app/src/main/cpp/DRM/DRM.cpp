// Created by Hujifa on 4/1/2022.

#include "DRM.h"
#include <stdio.h>
#include <string>
#include "../Utils/obfuscate.h"

#include <android/log.h>
#define LOG_TAG "Debug"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

/**@brief Get Unique Device Identifier that Works in Virtual
 *
 * Device identifiers such as build information, android_id or IMEI are non consistent
 * specially if the information is collected from within a virtual device.
 * But the device's internal storage size is consistent across the device and the virtual devices.
 *
 * Example:
 *         jstring uniqueDeviceID = DRM::getUniqueDeviceID(env);
 *
 * @param env
 * @return jstring Returns the internal storage size as unique device identifier.
 */
__attribute__ ((visibility ("hidden")))
jstring DRM::getUniqueDeviceID(JNIEnv *env) {

    /* File path = Environment.getDataDirectory(); */
    jclass      Environment         = env->FindClass(OBFUSCATE("android/os/Environment" ));
    jmethodID   getDataDirectory    = env->GetStaticMethodID(Environment,OBFUSCATE("getDataDirectory"),OBFUSCATE("()Ljava/io/File;"));

    jclass      File        = env->FindClass(OBFUSCATE("java/io/File" ));
    jobject     file        = env->CallStaticObjectMethod(Environment,getDataDirectory);

    /* StatFs stat = new StatFs(path.getPath()); */
    jmethodID   getPath     = env->GetMethodID(File,OBFUSCATE("getPath"), OBFUSCATE("()Ljava/lang/String;"));
    jobject     path        = env->CallObjectMethod(file,getPath);

    jclass      StatFs              = env->FindClass(OBFUSCATE("android/os/StatFs"));
    jmethodID   constructStatFs     = env->GetMethodID(StatFs,OBFUSCATE("<init>"), OBFUSCATE("(Ljava/lang/String;)V"));
    jobject     statFs              = env->NewObject(StatFs,constructStatFs,path);

    /* long blockSize = stat.getBlockSizeLong(); */
    jmethodID   getBlockSizeLong    = env->GetMethodID(StatFs,OBFUSCATE("getBlockSizeLong"), OBFUSCATE("()J"));
    jlong       blockSize           = env->CallLongMethod(statFs,getBlockSizeLong);

    /* long totalBlocks = stat.getBlockCountLong(); */
    jmethodID   getBlockCountLong   = env->GetMethodID(StatFs,OBFUSCATE("getBlockCountLong"), OBFUSCATE("()J"));
    jlong       totalBlocks         = env->CallLongMethod(statFs,getBlockCountLong);

    /* totalSize = totalBlocks * blockSize */
    long long totalSize = (long long) blockSize * (long long) totalBlocks ;

    {
        env->DeleteLocalRef(Environment);
        env->DeleteLocalRef(File);
        env->DeleteLocalRef(file);
        env->DeleteLocalRef(path);
        env->DeleteLocalRef(StatFs);
        env->DeleteLocalRef(statFs);
    }

    /* Converting long long to jstring.
     * Total digit in long long is about 20, so array size is chosen over 20.
     */
    char totalSizeInChar[32];
    sprintf( totalSizeInChar, OBFUSCATE("%lld"), totalSize );
    jstring totalSizeInJstring = env->NewStringUTF(totalSizeInChar);
    return totalSizeInJstring;
}



/**@brief Get Message Digest 5 (MD5) Hash of a String
 *
 * Example:
 *          jstring MDHash = DRM::getMD5Hash(env, env->NewStringUTF("Hello") );
 *
 * @param env
 * @param inputString The string whose MD5 hash is to be determined
 * @return jstring The MD5 hash of given string
 */
__attribute__ ((visibility ("hidden")))
jstring DRM::getMD5Hash(JNIEnv *env, jstring inputString) {

    /* MessageDigest digest = MessageDigest.getInstance("MD5"); */
    jclass    MessageDigest = env->FindClass(OBFUSCATE("java/security/MessageDigest"));
    jmethodID getInstance   = env->GetStaticMethodID(MessageDigest, OBFUSCATE("getInstance"), OBFUSCATE("(Ljava/lang/String;)Ljava/security/MessageDigest;"));
    jobject   digest        = env->CallStaticObjectMethod(MessageDigest, getInstance, env->NewStringUTF(OBFUSCATE("MD5")));

    /* digest.update(bytes); */
    jclass     String   = env->FindClass(OBFUSCATE("java/lang/String"));
    jmethodID  getBytes = env->GetMethodID(String,OBFUSCATE("getBytes"), OBFUSCATE("()[B"));
    jbyteArray bytes    = (jbyteArray) env->CallObjectMethod( inputString, getBytes);
    jmethodID  update   = env->GetMethodID(MessageDigest,OBFUSCATE("update"), OBFUSCATE("([B)V"));
    env->CallVoidMethod(digest,update, bytes);

    /* byte messageDigest[] = digest.digest(); */
    jmethodID  mdigest       = env->GetMethodID(MessageDigest,OBFUSCATE("digest"), OBFUSCATE("()[B"));
    jbyteArray messageDigest = (jbyteArray) env->CallObjectMethod(digest,mdigest);

    /* StringBuffer hexString = new StringBuffer(); */
    jclass      StringBuffer            = env->FindClass(OBFUSCATE("java/lang/StringBuffer"));
    jmethodID   constructStringBuffer   = env->GetMethodID(StringBuffer,OBFUSCATE("<init>"), OBFUSCATE("()V"));
    jobject     hexString               = env->NewObject(StringBuffer,constructStringBuffer);

    /*
      for (int i = 0; i < messageDigest.length; i++ )
        hexString.append( Integer.toHexString( 0xFF & messageDigest[i]) );
     */

    int   messageDigestLength = env->GetArrayLength(messageDigest);
    /** Getting Byte Array Length */
    /** Obtain a pointer to the elements of the array. Ref: https://www.iitk.ac.in/esc101/05Aug/tutorial/native1.1/implementing/array.html */
    jbyte*      body        = env->GetByteArrayElements(messageDigest,NULL);
    jclass      Integer     = env->FindClass(OBFUSCATE("java/lang/Integer"));
    jmethodID   toHexString = env->GetStaticMethodID(Integer,OBFUSCATE("toHexString"), OBFUSCATE("(I)Ljava/lang/String;"));

    jmethodID   append      = env->GetMethodID(StringBuffer,OBFUSCATE("append"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/StringBuffer;"));
    for ( int i = 0 ; i < messageDigestLength ; i++ ){
        jbyte   _byte            = body[i];
        jint    intFromByte      = 0xFF & _byte;
        jstring hexStringFromInt = (jstring) env->CallStaticObjectMethod(Integer,toHexString,intFromByte);
        env->CallObjectMethod(hexString,append,hexStringFromInt);
    }

    /* return hexString.toString(); */
    jmethodID toString = env->GetMethodID(StringBuffer,OBFUSCATE("toString"), OBFUSCATE("()Ljava/lang/String;")) ;
    jstring   md5      = (jstring) env->CallObjectMethod(hexString,toString);


    {
        env->DeleteLocalRef(MessageDigest);
        env->DeleteLocalRef(String);
        env->DeleteLocalRef(StringBuffer);
        env->DeleteLocalRef(Integer);

        env->DeleteLocalRef(digest);
        env->DeleteLocalRef(hexString);
    }

    return md5;
}



/**@brief Get CRC32 or Signature of a Package File to Detect Tempering.
 *
 * Open the caller's own Application Package (APK) file as Zip file using ZipFile class.
 * Using the ZipEntry class, it attempts to grabs the requested package file from the APK file.
 * If the package file is not found, then it returns NULL.
 * Otherwise, it returns the CRC32 of that package file.
 *
 * Examples":
 *              jstring dexSignature = DRM::getSignature( env , context, env->NewStringUTF("classes.dex"));
 *              jstring manifestSignatue = DRM::getSignature( env , context, env->NewStringUTF("AndroidManifest.xml"));
 *              jstring resourceSignatue = DRM::getSignature( env , context, env->NewStringUTF("loader.arsc"));
 * @param env
 * @param context
 * @param packageFileName  The name of the package file whose CRC32 is to be determined.
 * @return jstring The CRC32 of the package file | NULL if package file is not found.
 */
__attribute__ ((visibility ("hidden")))
jstring DRM::getSignature(JNIEnv *env, jobject context, jstring packageFileName) {

    jstring apkPath;

    /* String apkPath = getPackageCodePath( ); */
    jclass     Activity            = env->GetObjectClass(context);
    jmethodID  getPackageCodePath  = env->GetMethodID(Activity,OBFUSCATE("getPackageCodePath"),OBFUSCATE("()Ljava/lang/String;"));
    apkPath                        = (jstring)  env->CallObjectMethod(context, getPackageCodePath);

    /* ZipFile zipFile = new ZipFile(apkPath); */
    jclass ZipFile              = env->FindClass(OBFUSCATE("java/util/zip/ZipFile"));
    jmethodID constructZipFile  = env->GetMethodID(ZipFile, OBFUSCATE("<init>"), OBFUSCATE("(Ljava/lang/String;)V"));
    jobject zipFile             = env->NewObject(ZipFile,constructZipFile, apkPath);

    /* ZipEntry zipEntry = zipFile.getEntry( fileName ); */
    jmethodID getEntry    = env->GetMethodID(ZipFile,OBFUSCATE("getEntry"),OBFUSCATE("(Ljava/lang/String;)Ljava/util/zip/ZipEntry;"));
    jclass ZipEntry       = env->FindClass(OBFUSCATE("java/util/zip/ZipEntry"));
    jobject zipEntry      = env->CallObjectMethod(zipFile, getEntry, packageFileName );
    if ( zipEntry == NULL || zipEntry == nullptr ){
        /** File Not Found */
        return (jstring) NULL;
    }

    /* long long crc = zipEntry.getCrc();*/
    jmethodID getCrc  = env->GetMethodID(ZipEntry,OBFUSCATE("getCrc"), OBFUSCATE("()J"));
    long long crc     = env->CallLongMethod(zipEntry, getCrc);

    {
        env->DeleteLocalRef(Activity);
        env->DeleteLocalRef(ZipFile);
        env->DeleteLocalRef(zipFile);
        env->DeleteLocalRef(ZipEntry);
        env->DeleteLocalRef(zipEntry);
    }

    /* Converting long long to jstring.
     * Total digit in long long is about 20, so array size is chosen over 20.
     */
    char crcInChar[32];
    sprintf( crcInChar, "%lld", crc );
    jstring crcInJstring = env->NewStringUTF(crcInChar);
    return  crcInJstring;


}


/**@brief Read Text form a Package File
 *
 * Open the caller's own Application Package (APK) file as Zip file using ZipFile class.
 * Using the ZipEntry class, it attempts to grabs the requested package file from the APK file.
 * If the package file is not found, then it returns NULL.
 * Otherwise, it read and returns text form it.
 *
 * Example:
 *          jstring fileContent = DRM::readTextFromPackageFile(env, context, env->NewStringUTF("assets/agent.js"));
 *
 * @param env
 * @param context
 * @param packageFilePath The full path with name of requested package file in APK file.
 * @return jstring The text read from package file | NULL if unavailable.
 */
__attribute__ ((visibility ("hidden")))
jstring DRM::readTextFromPackageFile(JNIEnv *env, jobject context, jstring packageFilePath) {

    jstring apkPath;

    /* String apkPath = getPackageCodePath( ); */
    jclass     Activity            = env->GetObjectClass(context);
    jmethodID  getPackageCodePath  = env->GetMethodID(Activity,OBFUSCATE("getPackageCodePath"),OBFUSCATE("()Ljava/lang/String;"));
    apkPath                        = (jstring)  env->CallObjectMethod(context, getPackageCodePath);

    /* ZipFile zipFile = new ZipFile(apkPath); */
    jclass ZipFile              = env->FindClass(OBFUSCATE("java/util/zip/ZipFile"));
    jmethodID constructZipFile  = env->GetMethodID(ZipFile, OBFUSCATE("<init>"), OBFUSCATE("(Ljava/lang/String;)V"));
    jobject zipFile             = env->NewObject(ZipFile,constructZipFile, apkPath);

    /* ZipEntry zipEntry = zipFile.getEntry( filePath ); */
    jmethodID getEntry    = env->GetMethodID(ZipFile,OBFUSCATE("getEntry"),OBFUSCATE("(Ljava/lang/String;)Ljava/util/zip/ZipEntry;"));
    jclass ZipEntry       = env->FindClass(OBFUSCATE("java/util/zip/ZipEntry"));
    jobject zipEntry      = env->CallObjectMethod(zipFile, getEntry, packageFilePath );
    if ( zipEntry == NULL || zipEntry == nullptr ){
        /** File Not Found */
        return (jstring) NULL;
    }

    /* InputStream inputStream = zipFile.getInputStream(zipEntry); */
    jclass      InputStream     = env->FindClass(OBFUSCATE("java/io/InputStream"));
    jmethodID   getInputStream  = env->GetMethodID(ZipFile, OBFUSCATE("getInputStream"), OBFUSCATE("(Ljava/util/zip/ZipEntry;)Ljava/io/InputStream;"));
    jobject     inputStream     = env->CallObjectMethod(zipFile,getInputStream, zipEntry);

    /* int     size    = inputStream.available(); */
    jmethodID   available   = env->GetMethodID(InputStream, OBFUSCATE("available"), OBFUSCATE("()I") );
    jint        size        = env->CallIntMethod(inputStream, available);

    /* byte[]  buffer  = new byte[size]; */
    jbyteArray  buffer      = env->NewByteArray(size);
    jmethodID   read        = env->GetMethodID(InputStream, OBFUSCATE("read"), OBFUSCATE("([B)I"));

    /* inputStream.read(buffer); */
    env->CallIntMethod(inputStream, read, buffer);

    /* inputStream.close(); */
    jmethodID   close   = env->GetMethodID(InputStream, OBFUSCATE("close"), OBFUSCATE("()V"));
    env->CallVoidMethod(inputStream, close );

    /* String fileText = new String(buffer); */
    jclass      String              = env->FindClass(OBFUSCATE("java/lang/String"));
    jmethodID   StringConstructor   = env->GetMethodID(String, OBFUSCATE("<init>"), OBFUSCATE("([B)V"));
    jstring     fileText            = static_cast<jstring>(env->NewObject(String, StringConstructor, buffer));

    {
        env->DeleteLocalRef(Activity);
        env->DeleteLocalRef(ZipFile);
        env->DeleteLocalRef(zipFile);
        env->DeleteLocalRef(ZipEntry);
        env->DeleteLocalRef(zipEntry);
        env->DeleteLocalRef(InputStream);
        env->DeleteLocalRef(inputStream);
        env->DeleteLocalRef(buffer);
        env->DeleteLocalRef(String);
    }

    return fileText;
}

/** Decryp a Package File Encrypted By XOR Key (1)
 *
 * Open the caller's own Application Package (APK) file as Zip file using ZipFile class.
 * Using the ZipEntry class, it attempts to grabs the requested package file from the APK file.
 * If the package file is not found, then it returns NULL.
 * Otherwise, it read, decryp and write the file.
 *
 *
 * @param env
 * @param context
 * @param packageFilePath
 * @return
 */
__attribute__ ((visibility ("hidden")))
bool DRM::extractXorOperatedPackageFile(JNIEnv *env, jobject context, jint xorKey, jstring packageFilePath, jstring outputFileName) {


    jstring apkPath;

    /* String apkPath = getPackageCodePath( ); */
    jclass     Activity            = env->GetObjectClass(context);
    jmethodID  getPackageCodePath  = env->GetMethodID(Activity,OBFUSCATE("getPackageCodePath"),OBFUSCATE("()Ljava/lang/String;"));
    apkPath                        = (jstring)  env->CallObjectMethod(context, getPackageCodePath);

    /* ZipFile zipFile = new ZipFile(apkPath); */
    jclass ZipFile              = env->FindClass(OBFUSCATE("java/util/zip/ZipFile"));
    jmethodID constructZipFile  = env->GetMethodID(ZipFile, OBFUSCATE("<init>"), OBFUSCATE("(Ljava/lang/String;)V"));
    jobject zipFile             = env->NewObject(ZipFile,constructZipFile, apkPath);

    /* ZipEntry zipEntry = zipFile.getEntry( filePath ); */
    jmethodID getEntry    = env->GetMethodID(ZipFile,OBFUSCATE("getEntry"),OBFUSCATE("(Ljava/lang/String;)Ljava/util/zip/ZipEntry;"));
    jclass ZipEntry       = env->FindClass(OBFUSCATE("java/util/zip/ZipEntry"));
    jobject zipEntry      = env->CallObjectMethod(zipFile, getEntry, packageFilePath );
    if ( zipEntry == NULL || zipEntry == nullptr ){
        /** File Not Found */
        return false;
    }

    /* InputStream inputStream = zipFile.getInputStream(zipEntry); */
    jclass      InputStream     = env->FindClass(OBFUSCATE("java/io/InputStream"));
    jmethodID   getInputStream  = env->GetMethodID(ZipFile, OBFUSCATE("getInputStream"), OBFUSCATE("(Ljava/util/zip/ZipEntry;)Ljava/io/InputStream;"));
    jobject     inputStream     = env->CallObjectMethod(zipFile,getInputStream, zipEntry);


    /* String outputFilePath    = "/data/data/" + context.getPackageName() + "/cache/" + extractedFileName; */
    std::string outputFilePath  = DRM::getPackageCachePath(env, context);
    outputFilePath.append(env->GetStringUTFChars(outputFileName, NULL));

    /*   OutputStream outputStream   = new FileOutputStream(newFileName); outputStream   = new FileOutputStream(newFileName); */
    jclass      FileOutputStream            = env->FindClass(OBFUSCATE("java/io/FileOutputStream"));
    jmethodID   FileOutputStreamConstructor = env->GetMethodID(FileOutputStream, OBFUSCATE("<init>"), OBFUSCATE("(Ljava/lang/String;)V"));
    jobject     outputStream                = env->NewObject(FileOutputStream, FileOutputStreamConstructor, env->NewStringUTF(outputFilePath.c_str()));
    if ( outputStream == NULL || outputStream == nullptr ){
        /** File can't be created */
        return false;
    }

    /* byte[] buffer    = new byte[1024]; */
    jbyteArray  buffer  = env->NewByteArray(1024);

    /* int length  = inputStream.read(buffer); */
    jmethodID   read    = env->GetMethodID(InputStream, "read", "([B)I");
    int         length  = env->CallIntMethod(inputStream, read, buffer);

    jclass      OutputStream    = env->FindClass(OBFUSCATE("java/io/OutputStream"));
    jmethodID   write           = env->GetMethodID(OutputStream, OBFUSCATE("write"), OBFUSCATE("([BII)V"));
    while ( length > 0 )
    {

        jbyte* bytes = env->GetByteArrayElements(buffer, 0);
        size_t size  = env->GetArrayLength(buffer);
        for ( int i = 0; i < size  ; i++) {
            bytes[i] = (jbyte) (bytes[i] ^ xorKey);
        }
        env->SetByteArrayRegion( buffer, 0, env->GetArrayLength(buffer), bytes);

        /* outputStream.write(buffer, 0, length); */
        env->CallVoidMethod(outputStream, write, buffer, 0, length);

        /* length = inputStream.read(buffer); */
        length = env->CallIntMethod(inputStream, read, buffer);
    }


    /* outputStream.flush(); */
    jmethodID   flush           = env->GetMethodID(OutputStream, OBFUSCATE("flush"), OBFUSCATE("()V"));
    env->CallVoidMethod(outputStream, flush);

    /* inputStream.close(); */
    jmethodID InputStreamClose = env->GetMethodID(InputStream, OBFUSCATE("close"), OBFUSCATE("()V"));
    env->CallVoidMethod(inputStream, InputStreamClose);


    /* outputStream.close(); */
    jmethodID OutputStreamClose = env->GetMethodID(OutputStream, OBFUSCATE("close"), OBFUSCATE("()V"));
    env->CallVoidMethod(outputStream, OutputStreamClose);

    return true;

}

__attribute__ ((visibility ("hidden")))
const char* DRM::getPackageName(JNIEnv *env, jobject context) {
    jclass      Context         = env->FindClass(OBFUSCATE("android/content/Context"));
    jmethodID   getPackageName  = env->GetMethodID(Context, OBFUSCATE("getPackageName"), OBFUSCATE("()Ljava/lang/String;"));
    jstring     packageName     = static_cast<jstring>(env->CallObjectMethod(context, getPackageName));
    return      env->GetStringUTFChars(packageName, NULL);
}

__attribute__ ((visibility ("hidden")))
const char *DRM::getPackageCachePath(JNIEnv *env, jobject context) {
    char packageCachePath[1024];
    sprintf(packageCachePath, OBFUSCATE("/data/data/%s/cache/"), DRM::getPackageName(env, context) );
    return packageCachePath;
}
