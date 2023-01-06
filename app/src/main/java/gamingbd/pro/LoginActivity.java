package gamingbd.pro;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Process;
import android.provider.Settings;
import android.view.View;
import android.view.Window;
import static gamingbd.pro.Native.*;

public class LoginActivity extends AppCompatActivity {

    static {
        System.loadLibrary("loader");
    }

    public native void              setUserInterfaceLoginNative();
    public static native void       onClickLoginNative(Context context, View view);                 // Return an interface to C/C++ as, I can't create interface in C/C++ layer
    public static native String     doInBackgroundLoginNative();                                    // Making HTTP request in C/C++ layer
    public static native void       onPostExecuteLoginNative(Context context, String string);       // Task after receiving response in C/C++ layer


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);                                              // Hide actionbar
        setUserInterfaceLoginNative();

        GetSuperUserPermission(this);
        DisableSeLinux(this);
        getOverlayPermission();
    }

    @SuppressLint("WrongConstant")                                                                  // Method to launch ListApplication
    public void enter() throws Exception {
        Intent intent = new Intent(LoginActivity.this, ListApplication.class);
        startActivity(intent);
        finish();
    }

    /**@brief Get Necessary Permission
     * */
    public void getOverlayPermission(){
        if (  !Settings.canDrawOverlays(getApplicationContext())  ) {
            startActivity(new Intent("android.settings.action.MANAGE_OVERLAY_PERMISSION", Uri.parse("package:" + getPackageName())));
            Process.killProcess(Process.myPid());
        }
    }

    public static void getTerminalPermission(Context context){
        String[] perms = {"jackpal.androidterm.permission.RUN_SCRIPT"};
        ActivityCompat.requestPermissions((Activity) context, perms, 200);
    }

    public static boolean isTerminalPermissionGranted(Context context){
        int result = ContextCompat.checkSelfPermission( context, "jackpal.androidterm.permission.RUN_SCRIPT" );
        if ( result == PackageManager.PERMISSION_GRANTED ){
            return  true;
        }
        return false;
    }

    /**@brief Create an Interface to Listen to Click Event
     *
     * @return View.OnClickListener The interface to handle click event.
     */
    public View.OnClickListener getButtonClickListener(){
        return new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                onClickLoginNative(getApplicationContext(), view);
            }
        };
    }


    public void stopFloatingService(){
        if ( isServiceRunning(FloatingViewService.class) ){
            stopService(new Intent(LoginActivity.this, FloatingViewService.class));
        }
    }

    private boolean isServiceRunning( Class<?> serviceClass ) {
        ActivityManager manager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        for (ActivityManager.RunningServiceInfo service : manager.getRunningServices(Integer.MAX_VALUE)) {
            if ( serviceClass.getName().equals(service.service.getClassName()) ) {
                return true;
            }
        }
        return false;
    }

}