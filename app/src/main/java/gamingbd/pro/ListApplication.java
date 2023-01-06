package gamingbd.pro;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Process;
import android.provider.Settings;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;


import java.util.List;
import static gamingbd.pro.Loader.*;
import static gamingbd.pro.Utils.*;


public class ListApplication extends AppCompatActivity {

    public static ListView listView;

    public static Drawable icons[]    = null;
    public static String   names[]    = null;
    public static String   packages[] = null;
    public static int      applicationCount;


    public static native void updateInformation( String string1,  String string2 );

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_list_application);


        /** Count application & change the arrays size dynamically */
        {
            applicationCount = getUserApplicationCount(getApplicationContext());

            icons    = new Drawable[applicationCount];
            names    = new String[applicationCount];
            packages = new String[applicationCount];
        }

        /** Get the installed application's icon, name and package in the arrays */
        {
            try {
                getApplicationInformation(icons, names, packages);
            } catch (PackageManager.NameNotFoundException e) {Log.d("Debug", "Error getApplicationInformation: "+e);}
        }

        /** Create custom adapter from template & show application list */{
            CustomAdapter customAdapter = new CustomAdapter(this, icons, names, packages);
            listView = findViewById(R.id.listViewID);
            listView.setAdapter(customAdapter);
            listView.setOnItemLongClickListener(getOnItemLongClickListener());
        }

    }


    /**@brief Create & Return OnItemLongClickListener Interface
     *
     * @return OnItemLongClickListener The interface to handle OnItemLongClickListener event
     */
    public AdapterView.OnItemLongClickListener getOnItemLongClickListener(){

        return new AdapterView.OnItemLongClickListener() {
            @Override
            public boolean onItemLongClick(AdapterView<?> parent, View view, int position, long id) {

                /** Update information about the target package name, activity name */
                updateInformation( packages[position], Utils.getActivity(getApplicationContext(), packages[position]) );

                /** Start Floating Service & Kill Current Activity */
                StartFloating(getApplicationContext());

                /** Launch target application & finish current activity */
                Intent launchIntent = getPackageManager().getLaunchIntentForPackage(packages[position]);
                if (launchIntent != null)
                    startActivity(launchIntent);

                finish();
                return false;
            }
        };
    }




    /** @brief Start Floating Window
     *
     * @param context
     */
    public static void StartFloating(Context context) {
        if ( Build.VERSION.SDK_INT <  23 || Settings.canDrawOverlays(context)) {
            context.startService(new Intent(context, FloatingViewService.class));
        } else {
            context.startActivity(new Intent("android.settings.action.MANAGE_OVERLAY_PERMISSION", Uri.parse("package:" + context.getPackageName())));
            Process.killProcess(Process.myPid());
        }
    }




    /**@brief Get Installed User Application's Information
     *
     * Iterate over installed packages and grab the user application's icon, name, package name
     * except own information.
     *
     *
     * @param icons     Drawable[] This will hold the application icons.
     * @param names     String[] This will hold the application name.
     * @param packages  String[] This will hold the application package name.
     * @throws PackageManager.NameNotFoundException
     */
    public void getApplicationInformation(Drawable[] icons, String[] names, String[] packages) throws PackageManager.NameNotFoundException {

        int iterator = 0;
        List<ApplicationInfo> installedApplications = getPackageManager().getInstalledApplications(PackageManager.GET_META_DATA);
        for (ApplicationInfo applicationInfo : installedApplications) {

            if ( applicationInfo.packageName.equals(getPackageName()) )
                continue;

            if ( (applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) == 0 ){

                icons[iterator]     = getPackageManager().getApplicationIcon(applicationInfo.packageName);
                names[iterator]     = (String) applicationInfo.loadLabel(getPackageManager());
                packages[iterator]  = applicationInfo.packageName;

                iterator++;
            }
        }

    }

}