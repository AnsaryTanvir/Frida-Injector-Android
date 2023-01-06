package gamingbd.pro;

import android.content.Context;


public class Native {

    public static native String     IconNative();
    public static native boolean    GetSuperUserPermission(Context context);
    public static native void       DisableSeLinux(Context context);


}

