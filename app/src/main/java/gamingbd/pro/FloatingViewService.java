package gamingbd.pro;

import static android.view.ViewGroup.LayoutParams.MATCH_PARENT;
import static android.view.ViewGroup.LayoutParams.WRAP_CONTENT;

import android.app.Service;
import android.content.Intent;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.os.Build;
import android.os.IBinder;
import android.util.Base64;
import android.util.Log;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import static gamingbd.pro.Native.*;
import static gamingbd.pro.Loader.*;

public class FloatingViewService extends Service{

    /* Window is the root of everything that floats on screen */
    public WindowManager    Window;

    /* We have a Frame in the Window that will float */
    public FrameLayout      MainFrame;

    /*We have a Main Layout in the Frame */
    public RelativeLayout   MainLayout;

    /* Main Layout contains Collapsed View (ICON) & Expanded View (Title+Button) */
    public RelativeLayout   collapsedView;
    public LinearLayout     expandedView;

    /* Collapsed View contains only the floating icon */
    public ImageView        Icon;

    /* Expanded View contains Title + Buttons */
    public TextView         Title;
    public RelativeLayout   Buttons;


    /* Buttons contain Start Button & Stop Button */
    public Button StartButton, StopButton;


    /* LayoutParameters is used to change the position of the Window (Whole Floating Stuffs) */
    public WindowManager.LayoutParams LayoutParameters;

    /* Floating icon inside the Collapsed View. The icon image is stored in Base64 encoded format */
    public String iconString = IconNative();

    /* Get the Hack name in order to determine floating state */
    public static native String getName();

    /* Deauthenticate using token and finish floating */
    public native void stopFloatingNative();

    /** This method is called when the service is created */
    @Override
    public void onCreate() {
        super.onCreate();
        StartFloat();
    }



    /**@brief Create Floating Layout
     *
     */
    private void StartFloat() {

        /** View Hierarchy */
        /* Layout Scheme :
            Window [Window] {
                Main Frame [FrameLayout] {
                    Main Layout [RelativeLayout] {
                        Collapsed View [RelativeLayout]{
                            Icon [ImageView]
                        }
                        Expanded  View [LinearLayout]{
                            Title [TextView]
                            Buttons [RelativeLayout]{
                                StartButton [Button]
                            }
                        }
                    }
                }
            }

         */

        /** Main Frame [FrameLayout] **/
        {
            /* Frame holds the entire floating view */
            MainFrame = new FrameLayout(this);
            MainFrame.setLayoutParams(new FrameLayout.LayoutParams(MATCH_PARENT, MATCH_PARENT));

            /** Main Layout [RelativeLayout] */
            {
                /* Creating the Main Layout inside the Frame */
                /* MainLayout contains the collapsed view (ICON) & the expanded view (MENU) */
                MainLayout = (new RelativeLayout(this));
                MainLayout.setLayoutParams(new FrameLayout.LayoutParams(WRAP_CONTENT, WRAP_CONTENT));

                /** Collapsed View [RelativeLayout] */
                {
                    /*---------------------- (Collapsed View) or (Floating Icon View) */
                    /* When we close/collapse the floating menu this collapsedView will be shown */
                    /* Initially the floating view should only show the icon [collapsed view] */
                    collapsedView = new RelativeLayout(this);
                    collapsedView.setLayoutParams(new RelativeLayout.LayoutParams(WRAP_CONTENT, WRAP_CONTENT));
                    collapsedView.setVisibility(View.VISIBLE);

                    /* Icon [ImageView] */
                    {
                        /* In the collapsed view there will be only an image as icon */
                        Icon = new ImageView(this);
                        Icon.setLayoutParams(new RelativeLayout.LayoutParams(WRAP_CONTENT, WRAP_CONTENT));
                        /* Icon size 50 dp */
                        int applyDimension = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, (float) 50, getResources().getDisplayMetrics());
                        Icon.getLayoutParams().height = applyDimension;
                        Icon.getLayoutParams().width  = applyDimension;
                        Icon.setScaleType(ImageView.ScaleType.FIT_XY);
                        byte[] decode = Base64.decode( iconString, 0);
                        Icon.setImageBitmap(BitmapFactory.decodeByteArray(decode, 0, decode.length));

                        Icon.setOnClickListener(new View.OnClickListener() {
                            public void onClick(View view) {
                                /* Hide icon & show expanded view */
                                collapsedView.setVisibility(View.GONE);
                                expandedView.setVisibility(View.VISIBLE);
                            }
                        });
                    }
                }

                /** Expanded  View [LinearLayout] */
                {
                    /* Initially the expanded view will be hidden */
                    expandedView = new LinearLayout(this);
                    expandedView.setVisibility(View.GONE);
                    expandedView.setOrientation(LinearLayout.VERTICAL);
                    /** Expanded View Width 400 dp **/
                    expandedView.setLayoutParams(new LinearLayout.LayoutParams(pixelToDp(400), WRAP_CONTENT));

                    {
                        /** Title [TextView] */
                        {
                            Title = new TextView(this);
                            Title.setText("Loader Free Fire");
                            Title.setTextColor(Color.WHITE);
                            Title.setTextSize(18);
                            Title.setGravity(Gravity.CENTER);
                            Title.setTypeface(null, Typeface.BOLD);
                            Title.setPadding(0, 15, 0, 15);
                            Title.setBackgroundColor(Color.parseColor("#b03535"));
                        }


                        /** Buttons [RelativeLayout]  */
                        {
                            Buttons = new RelativeLayout(this);
                            Buttons.setPadding(5, 15, 5, 15);
                            Buttons.setLayoutParams(new RelativeLayout.LayoutParams(MATCH_PARENT, WRAP_CONTENT));
                            Buttons.setVerticalGravity(Gravity.CENTER);
                            Buttons.setBackgroundColor(Color.BLACK);

                            /** InjectButton [Button] **/
                            {
                                StartButton = new Button(this);
                                StartButton.setText("Start");
                                StartButton.setAllCaps(false);
                                StartButton.setTextColor(Color.parseColor("#8C8D92"));
                                StartButton.setTypeface(null, Typeface.BOLD);
                                StartButton.setTextSize(18);
                                StartButton.setLetterSpacing(0.2f);
                                /* Button Height 40 dp */
                                StartButton.setLayoutParams(new RelativeLayout.LayoutParams(MATCH_PARENT, pixelToDp(45)));

                                GradientDrawable gradientDrawable = new GradientDrawable();
                                gradientDrawable.setColor(Color.parseColor("#3A833A"));
                                gradientDrawable.setCornerRadius(100);
                                StartButton.setBackground(gradientDrawable);
                                StartButton.setTextColor(Color.WHITE);
                                StartButton.setOnClickListener(getStartButtonClickListener());
                            }

                            /** StopButton [Button] (For Deauth) **/
                            {
                                StopButton = new Button(this);
                                StopButton.setText("Stop");
                                StopButton.setAllCaps(false);
                                StopButton.setTextColor(Color.parseColor("#8C8D92"));
                                StopButton.setTypeface(null, Typeface.BOLD);
                                StopButton.setTextSize(18);
                                StopButton.setLetterSpacing(0.2f);
                                /* Button Height 40 dp */
                                StopButton.setLayoutParams(new RelativeLayout.LayoutParams(MATCH_PARENT, pixelToDp(45)));

                                GradientDrawable gradientDrawable = new GradientDrawable();
                                gradientDrawable.setColor(Color.parseColor("#3A833A"));
                                gradientDrawable.setCornerRadius(100);
                                StopButton.setBackground(gradientDrawable);
                                StopButton.setTextColor(Color.WHITE);
                                StopButton.setOnClickListener(getStopButtonClickListener());
                                StopButton.setVisibility(View.GONE);
                            }

                        }
                    }
                }
            }
        }

        /* Lets add components to the layout */

        /** Main Frame [FrameLayout] */
        {
            /* Adding Main Layout to the Frame */
            MainFrame.addView(MainLayout);

            /** Main Layout [RelativeLayout] */
            {
                /* Adding Collapsed View(ICON), Expanded View ( Title + Menu + Bottom Buttons ) to Main Layout */
                MainLayout.addView(collapsedView);
                MainLayout.addView(expandedView);

                /** Collapsed View [RelativeLayout] */
                {
                    /* Adding ICON in Collapsed View */
                    collapsedView.addView(Icon);
                }

                /** Expanded  View [LinearLayout] */
                {
                    /* Adding Title in Expanded View ( Title + Menu + Bottom Buttons ) */
                    expandedView.addView(Title);
                    expandedView.addView(Buttons);


                    /** Buttons [RelativeLayout]  */
                    {
                        /* Adding Inject Button */
                        Buttons.addView(StartButton);
                        Buttons.addView(StopButton);
                    }

                }
            }
        }


        Window                   =   (WindowManager) getSystemService(WINDOW_SERVICE);
        LayoutParameters         =   new WindowManager.LayoutParams(WRAP_CONTENT, WRAP_CONTENT, Build.VERSION.SDK_INT >= Build.VERSION_CODES.O ? 2038 : 2002, 8, -3);
        LayoutParameters.gravity =   Gravity.TOP | Gravity.LEFT;
        LayoutParameters.x       =   0;
        LayoutParameters.y       =   100;
        Window.addView(MainFrame, LayoutParameters);
        MainFrame.setOnTouchListener(onTouchListener());
        Icon.setOnTouchListener(onTouchListener());
    }



    /**@brief Create an Interface to Listen to Click Event
     *
     * @return View.OnClickListener The interface to handle click event.
     */
    public View.OnClickListener getStartButtonClickListener(){
        return new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                BackgroundTask inject = new Loader.BackgroundTask(getApplicationContext());
                inject.execute();

                /* If PSH4X or XREG then Keep the floating for deauthentication*/
                if ( getName().equals("PSH4X") || getName().equals("XREG")  ){
                    StartButton.setVisibility(View.GONE);
                    StopButton.setVisibility(View.VISIBLE);
                }
                else {
                    stopSelf();
                }
            }
        };
    }

    /**@brief Create an Interface to Listen to Click Event
     *
     * @return View.OnClickListener The interface to handle click event.
     */
    public View.OnClickListener getStopButtonClickListener(){
        return new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                /* Deauth process */
                stopFloatingNative();
            }
        };
    }



    /**@brief Interface for Touch Event Listener of Floating Menu
     *
     * @return View.OnTouchListener.
     */
    public View.OnTouchListener onTouchListener() {

        return new View.OnTouchListener() {

            final View collapsedView    = FloatingViewService.this.collapsedView;
            final View expandedView     = FloatingViewService.this.expandedView;

            float   initialTouchX;
            float   initialTouchY;
            int     initialX;
            int     initialY;

            public boolean onTouch(View view, MotionEvent motionEvent) {
                switch (motionEvent.getAction()) {

                    case MotionEvent.ACTION_DOWN:
                        initialX        = LayoutParameters.x;
                        initialY        = LayoutParameters.y;
                        initialTouchX   = motionEvent.getRawX();
                        initialTouchY   = motionEvent.getRawY();
                        return true;

                    case MotionEvent.ACTION_UP:

                        int rawX = (int) (motionEvent.getRawX() - initialTouchX);
                        int rawY = (int) (motionEvent.getRawY() - initialTouchY);

                        if ( rawX < 10 && rawY < 10 && isViewCollapsed() ){
                            try {
                                collapsedView.setVisibility(View.GONE);
                                expandedView.setVisibility(View.VISIBLE);
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }

                        /* Make Layout Less Transparent If Dragging Stops */
                    {
                        GradientDrawable gradientDrawable = new GradientDrawable();
                        gradientDrawable.setColor(Color.BLACK);
                        gradientDrawable.setAlpha(200);         // Alpha [0 -> 255] = More Dark
                    }

                    return true;


                    case MotionEvent.ACTION_MOVE:

                        LayoutParameters.x = initialX + ((int) (motionEvent.getRawX() - initialTouchX));
                        LayoutParameters.y = initialY + ((int) (motionEvent.getRawY() - initialTouchY));
                        Window.updateViewLayout(MainFrame, LayoutParameters);

                        /** Make Layout More Transparent While Dragging **/
                    {
                        GradientDrawable gradientDrawable   = new GradientDrawable();
                        gradientDrawable.setColor(Color.BLACK);
                        gradientDrawable.setAlpha(100);         // Alpha [0 -> 255] = More Dark
                    }
                    return true;

                    default:
                        return false;
                }
            }
        };
    }



    /**@brief Check to See if View Is Collapsed
     *
     * @return boolean The status
     */
    public boolean isViewCollapsed() {
        return MainFrame == null || collapsedView.getVisibility() == View.VISIBLE;
    }



    /**@brief Convert pixel Unit to dp Unit
     *
     * @param pixel
     * @return
     */
    public int pixelToDp(int pixel) {
        return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, (float) pixel, getResources().getDisplayMetrics());
    }



    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }



    @Override
    public void onDestroy() {
        if (MainFrame != null)
            Window.removeView(MainFrame);
        stopSelf();
    }



    /**@brief Prevent Floating View from Crashing in Background
     *
     * @param intent
     */
    @Override
    public void onTaskRemoved(Intent intent) {
        super.onTaskRemoved(intent);
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        stopSelf();
    }



    /**@brief Override onStartCommand() to Prevent Service from Recreating Itself
     *
     * @param intent
     * @param i
     * @param i2
     * @return
     */
    @Override
    public int onStartCommand(Intent intent, int i, int i2) {
        return Service.START_NOT_STICKY;
    }
}



