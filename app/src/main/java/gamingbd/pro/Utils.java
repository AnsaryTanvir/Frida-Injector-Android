package gamingbd.pro;

import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import eu.chainfire.libsuperuser.Shell;

public class Utils {



    /**@brief Count The Number of User Application
     *
     * Get the number of user application except counting own self.
     * @return
     */
    public static int getUserApplicationCount(Context context){
        int counter = 0;
        List<ApplicationInfo> installedApplications = context.getPackageManager().getInstalledApplications(PackageManager.GET_META_DATA);
        for (ApplicationInfo applicationInfo : installedApplications) {

            if ( applicationInfo.packageName.equals(context.getPackageName()) )
                continue;

            if ( (applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) == 0 )
                counter++;
        }
        return counter;
    }



    /**@brief This Method Is Used to Get The Process ID of a Running Application
     *
     * @param pkg The package name of target application whose process id is to be acquired
     * @return int The process id of target package or -1 if process doesn't exists
     * @exception Shell.ShellDiedException
     */
    public static int getProcessID(String pkg) {

        try {

            ArrayList arrayList             = new ArrayList();
            Shell.PoolWrapper poolWrapper   = Shell.Pool.SU;
            poolWrapper.run("(toolbox ps; toolbox ps -A; toybox ps; toybox ps -A) | grep \" " + pkg + "$\"", arrayList, null, false);
            Iterator iterator               = arrayList.iterator();

            while (iterator.hasNext()) {

                String Trim = ((String) iterator.next()).trim();
                while (Trim.contains("  "))
                    Trim = Trim.replace("  ", " ");

                String[] Split = Trim.split(" ");
                if (Split.length >= 2)
                    return Integer.parseInt(Split[1]);
            }
            return -1;

        } catch (Shell.ShellDiedException e) {
            e.printStackTrace();
            return -1;
        }
    }


    /**@brief Get The Name of The Launcher Activity of Application.
     *
     * @param context   Caller's context.
     * @param targetPackage String The package whose Activity is to be searched.
     * @return String The launcher activity name of targetPackage.
     */
    public static String getActivity(Context context, String targetPackage){

        String activityName = "";
        Intent intent = context.getPackageManager().getLaunchIntentForPackage(targetPackage);
        List<ResolveInfo> activityList = context.getPackageManager().queryIntentActivities(intent,0);
        if(  activityList != null){
            activityName = activityList.get(0).activityInfo.name;
        }
        return activityName;
    }


    {


//
//
//
//    /**
//     * This method is used to temporarily disable SeLinux globally.
//     *
//     * SeLinux or Security-Enhanced Linux is a Linux Security Module aimed to provide
//     * Mandatory Access Control (MAC).
//     * With SELinux enabled, root access can't do everything anymore because it imposes some
//     * restriction such as restricting calls to ptrace (a system call).
//     *
//     * @param context This is the context.
//     * @exception IOException
//     * @exception Shell.ShellDiedException
//     **/
//    public static void disableSeLinux(Context context){
//        try {
//            Shell.Pool.SU.run("su");
//            Shell.Pool.SU.run("setenforce 0");
//        } catch ( Shell.ShellDiedException e) {
//            e.printStackTrace();
//        }
//    }
//
//
//
//    /** @brief This Method Extracts Requested Package File From APK To The Caller's Cache Directory.
//     *
//     *  First open the caller's own Application Package (APK) file as Zip file using ZipFile class.
//     *  Using the ZipEntry class, it attempts to grabs the requested package file from the APK file.
//     *  The requested package file will be extracted to the /data/data/package-name/cache directory.
//     *
//     *  Example:
//     *  bool extractedIcon  = extractPackageFileInCache(context, "assets/icon.png"  , "icon.png"  );
//     *  bool extractedFrida = extractPackageFileInCache(context, "lib/armeabi-v7a/frida", "frida" );
//     *
//     * @param context   Self explanatory
//     * @param requestedPackageFilePath The full path with name of requested package file in APK file.
//     * @param extractedFileName The name of the extracted package file (Should be the same).
//     */
//    public static boolean extractPackageFileInCache(Context context, String requestedPackageFilePath, String extractedFileName ) throws IOException {
//
//        /* Getting Requested Package File */
//        String      applicationPackageFilePath  = context.getPackageCodePath();
//        ZipFile     applicationPackageAsZipFile = new ZipFile(applicationPackageFilePath);
//        ZipEntry    requestedPackageFile        = applicationPackageAsZipFile.getEntry(requestedPackageFilePath);
//        if ( requestedPackageFile == null )
//          return false;
//
//        /* Writing File */
//        InputStream inputStream     = applicationPackageAsZipFile.getInputStream(requestedPackageFile);
//        String newFileName          = "/data/data/" + context.getPackageName() + "/cache/" + extractedFileName;
//        OutputStream outputStream   = new FileOutputStream(newFileName);
//
//        byte[] buffer   = new byte[1024];
//        int length      = inputStream.read(buffer);
//        while ( length > 0) {
//            outputStream.write(buffer, 0, length);
//            length      = inputStream.read(buffer);
//        }
//
//        outputStream.flush();
//        inputStream.close();
//        outputStream.close();
//
//        return true;
//    }
//
//
//
//    /**
//     * @brief This Method Writes String In A File In Cache Directory.
//     *
//     * It first creates a text file in the caller's /data/data/package_name/cache directory.
//     * Then it writes the given string in the file.
//     * The file is expected to be found in /data/data/package_name/cache directory.
//     *
//     * @param context       Self explanatory
//     * @param fileName      Self explanatory
//     * @param fileContent   Self explanatory
//     */
//    public static void writeTextFileInCache(Context context, String fileName, String fileContent ) throws IOException  {
//
//        File file = new File(context.getCacheDir(), fileName);
//        FileOutputStream fileOutputStream = new FileOutputStream(file);
//        try {
//            byte[] x = fileContent.getBytes();
//            fileOutputStream.write(fileContent.getBytes());
//        } finally {
//            fileOutputStream.close();
//        }
//    }
//
//
//
//    /**@brief Write Log in Text File to The Caller's Cache Directory
//     *
//     *  Log is written in /data/data/package_name/cache directory.
//     *
//     * @param context
//     * @param log
//     */
//    public static void writeLog(Context context, String log){
//
//        File file = new File(context.getCacheDir(), "log.txt");
//        FileWriter fileWriter = null;
//        try {
//            fileWriter = new FileWriter(file.getAbsoluteFile());
//            BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);
//            bufferedWriter.append("Debug: "+ log);
//            bufferedWriter.newLine();
//            bufferedWriter.close();
//        } catch (IOException e) {e.printStackTrace();}
//    }
//
    }






}
