package gamingbd.pro;

import android.content.Context;
import android.os.AsyncTask;

public class Loader {

    public static native void doInBackgroundLoader(Context context);
    public static native void onPostExecuteLoader(Context context);

    /**@brief Inject Frida Inside Process
     *
     * First, extract Frida, Agent, Script in cache directory of caller's package
     * Then move the extracted files to /dev
     *
     * Case: Root
     *      Simply run shell command to execute frida & do some cleaning.
     *
     * Case: Virtual
     *       Due to tcgetattr error, I had to use Android Terminal Emulator to execute a command
     *       via intent that tells the Android Terminal Emulator to executes the extracted
     *       shell script.
     *
     *       That shell script contains the necessary command to execute frida and clean-up.
     */
    static class BackgroundTask extends AsyncTask<Void,Void, Void> {

        Context context;

        /**@brief Constructor
         *
         * @param context
         */
        public BackgroundTask(Context context) {
            this.context = context;
        }

        /**@brief Set-up Frida, Agent, Script
         *
         * @param voids
         * @return
         */
        @Override
        protected Void doInBackground(Void... voids) {
            doInBackgroundLoader(context);
            {
                {
                    //            try {
//
//                /** Enable Android Terminal Emulator */
//                Shell.Pool.SU.run("pm enable jackpal.androidterm");
//
//                /** Extract Frida in cache & Move to /dev */
//                {
//                    Shell.Pool.SU.run("cp "+    "/data/data/"+context.getPackageName()+"/cache/"+ fridaFileName + " " +  "/dev" );
//                    Shell.Pool.SU.run("rm "+    "/data/data/"+context.getPackageName()+"/cache/"+ fridaFileName );
//                    Shell.Pool.SU.run("chmod 777 /dev/" + fridaFileName );
//                    Shell.Pool.SU.run("chown root:root /dev/" + fridaFileName );
//                }
//
//                /** Write Script in cache & Move to /dev */
//                {
//                    Utils.writeTextFileInCache(context, "bash", scriptString);
//                    Shell.Pool.SU.run("cp "+    "/data/data/"+context.getPackageName()+"/cache/"+ scriptFileName + " " +  "/dev" );
//                    Shell.Pool.SU.run("rm "+    "/data/data/"+context.getPackageName()+"/cache/"+ scriptFileName);
//                    Shell.Pool.SU.run("chmod 777 /dev/" + scriptFileName );
//                    Shell.Pool.SU.run("chown root:root /dev/" + scriptFileName );
//                }
//
//                /** Write Agent in cache & Move to /dev */
//                {
//                    Utils.writeTextFileInCache(context, agentFileName, agentString);
//                    Shell.Pool.SU.run("cp "+    "/data/data/"+context.getPackageName()+"/cache/"+ agentFileName + " " +  "/dev" );
//                    Shell.Pool.SU.run("rm "+    "/data/data/"+context.getPackageName()+"/cache/"+ agentFileName);
//                    Shell.Pool.SU.run("chmod 777 /dev/" + agentFileName );
//                    Shell.Pool.SU.run("chown root:root /dev/" + agentFileName );
//                }
//
//            } catch (Exception e) {Log.d("Debug", "Error: "+ e);}
                }
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void unused) {
            super.onPostExecute(unused);
            onPostExecuteLoader(context);
            {
                {
                    //            String host = RootManager.getInstance().runCommand("uname -a").getMessage();
//
//            if ( host.contains("-titan") ){
//
//                /** Virtual Injection */
//                try{
//
//                    /** Start Injection Intent */
//                    String initialCommand = String.format("su\nsh /dev/%s", scriptFileName);
//                    Intent intent = new Intent("jackpal.androidterm.RUN_SCRIPT");
//                    intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//                    intent.addCategory(Intent.CATEGORY_DEFAULT);
//                    intent.putExtra("jackpal.androidterm.iInitialCommand", initialCommand);
//                    context.startActivity(intent);
//                }catch (Exception e) {Log.d("Debug", "Error: "+ e);}
//
//            } else {
//
//                /** Root Injection */
//                int PID = Utils.getProcessID(target);
//                String command = String.format("./dev/loader -p %d -s /dev/%s -e", PID, agentFileName);
//                RootManager.getInstance().runCommand(command);
//
//                /** Cleaning & Opening */
//                RootManager.getInstance().runCommand("rm -rf /dev/"+ agentFileName);
//                RootManager.getInstance().runCommand("rm -rf /dev/"+ fridaFileName);
//                RootManager.getInstance().runCommand("rm -rf /dev/"+ scriptFileName);
//            }
                }

            }
        }
    }

}
