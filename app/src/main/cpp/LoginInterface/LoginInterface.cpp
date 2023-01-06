// Created by Hujifa on 4/2/2022.

#include "LoginInterface.h"
#include "../Layout/Layout.h"

__attribute__ ((visibility ("hidden")))
LoginInterface::LoginInterface(JNIEnv *env, jobject context) {

    /* Hide Action Bar */
    Layout::hideActionBar(env, context);

    /* Getting Current Activity */
    jclass CurrentActivity = env->GetObjectClass(context);

    /*--------------------------------------------------------------------------------------------*/
    /*-----------------------------------mainLayout-----------------------------------------------*/

    /*  LinearLayout mainLayout = new LinearLayout(this); */
    jclass      LinearLayout            = env->FindClass("android/widget/LinearLayout");
    jmethodID   constructLinearLayout   = env->GetMethodID(LinearLayout,"<init>", "(Landroid/content/Context;)V");
    jobject     mainLayout              = env->NewObject(LinearLayout, constructLinearLayout, context);

    /*  LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams( LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT ); */
    jclass      LinearLayout$LayoutParams           = env->FindClass("android/widget/LinearLayout$LayoutParams");
    jmethodID   constructLinearLayout$LayoutParams  = env->GetMethodID(LinearLayout$LayoutParams, "<init>", "(II)V");
    jobject     layoutParams                        = env->NewObject(LinearLayout$LayoutParams, constructLinearLayout$LayoutParams, MATCH_PARENT, MATCH_PARENT);

    /*  mainLayout.setLayoutParams(layoutParams); */
    jmethodID   setLayoutParams = env->GetMethodID(LinearLayout,"setLayoutParams", "(Landroid/view/ViewGroup$LayoutParams;)V");
    env->CallVoidMethod(mainLayout,setLayoutParams,layoutParams);
    layoutParams = NULL;

    /* mainLayout.setGravity(Gravity.CENTER); */
    jmethodID   setGravity = env->GetMethodID(LinearLayout,"setGravity", "(I)V");
    env->CallVoidMethod(mainLayout,setGravity,GRAVITY_CENTER);
    setGravity = NULL;

    /* mainLayout.setOrientation(LinearLayout.VERTICAL); */
    jmethodID   setOrientation = env->GetMethodID(LinearLayout,"setOrientation", "(I)V");
    env->CallVoidMethod(mainLayout, setOrientation, ORIENTATION_VERTICAL);

    /*  mainLayout.setBackgroundColor(Color.parseColor("#ff1f2b3f")); */
    jclass      Color               = env->FindClass("android/graphics/Color");
    jmethodID   parseColor          = env->GetStaticMethodID(Color,"parseColor", "(Ljava/lang/String;)I");
    jint        color               = env->CallStaticIntMethod(Color,parseColor,env->NewStringUTF("#ff1f2b3f"));
    jmethodID   setBackgroundColor  = env->GetMethodID(LinearLayout,"setBackgroundColor", "(I)V");
    env->CallVoidMethod(mainLayout,setBackgroundColor,color);

    /* mainLayout.setPadding(0,0,0,200); */
    jmethodID   setPadding = env->GetMethodID(LinearLayout,"setPadding", "(IIII)V");
    env->CallVoidMethod(mainLayout,setPadding,0,0,0,200);
    setPadding = NULL;

    /*------------------------------------heading-------------------------------------------------*/

    /* TextView heading = new TextView(this); */
    jclass      TextView            = env->FindClass("android/widget/TextView");
    jmethodID   TextViewConstructor = env->GetMethodID(TextView, "<init>", "(Landroid/content/Context;)V");
    jobject     heading             = env->NewObject(TextView, TextViewConstructor, context);

    /* layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT,LinearLayout.LayoutParams.WRAP_CONTENT); */
    layoutParams = env->NewObject(LinearLayout$LayoutParams, constructLinearLayout$LayoutParams, MATCH_PARENT, WRAP_CONTENT);

    /* layoutParams.setMargins(convertDipToPixels(10.0f), convertDipToPixels(0.0f), convertDipToPixels(10.0f), getPixelFromDp(40.0f)); */
    jmethodID   setMargins = env->GetMethodID(LinearLayout$LayoutParams,"setMargins", "(IIII)V");
    env->CallVoidMethod(layoutParams,setMargins,
                            Layout::getPixelFromDp(env, context, 10.0f),
                            Layout::getPixelFromDp(env, context, 0.0f),
                            Layout::getPixelFromDp(env, context, 10.0f),
                            Layout::getPixelFromDp(env, context, 40.0f)
                        );

    /* heading.setLayoutParams(layoutParams); */
    env->CallVoidMethod(heading,setLayoutParams,layoutParams);

    /* heading.setGravity(Gravity.CENTER); */
    setGravity = env->GetMethodID(TextView,"setGravity","(I)V");
    env->CallVoidMethod(heading,setGravity,GRAVITY_CENTER);

    /* heading.setText(Html.fromHtml("<font face='monospace'><b><font color='#ff0000'>Loa</font><font color='#ffffff'>der</font></b></font>")); */
    jclass      Html        = env->FindClass("android/text/Html");
    jmethodID   fromHTMl    = env->GetStaticMethodID(Html,"fromHtml", "(Ljava/lang/String;)Landroid/text/Spanned;");
    jobject     text        = env->CallStaticObjectMethod(Html,fromHTMl,env->NewStringUTF("<font face='monospace'><b><font color='#ff0000'>Loa</font><font color='#12a56b'>der</font></b></font>"));
    jmethodID   setText     = env->GetMethodID(TextView,"setText", "(Ljava/lang/CharSequence;)V");
    env->CallVoidMethod(heading,setText,text);

    /* heading.setTextSize(40.0f); */
    jmethodID   setTextSize = env->GetMethodID(TextView,"setTextSize", "(F)V");
    jfloat      textSize = 40.0;
    env->CallVoidMethod(heading,setTextSize, textSize);

    /*-----------------------------------usernameText---------------------------------------------*/

    /* TextView usernameText = new TextView(this); */
    jobject usernameText = env->NewObject(TextView, TextViewConstructor, context);

    /* layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT,LinearLayout.LayoutParams.WRAP_CONTENT); */
    layoutParams = env->NewObject(LinearLayout$LayoutParams, constructLinearLayout$LayoutParams, MATCH_PARENT, WRAP_CONTENT);

    /* layoutParams.setMargins(convertDipToPixels(10.0f), convertDipToPixels(0.0f), convertDipToPixels(10.0f), getPixelFromDp(40.0f)); */
    env->CallVoidMethod(layoutParams, setMargins, Layout::getPixelFromDp(env, context, 10.0f),
                        Layout::getPixelFromDp(env, context, 0.0f),
                        Layout::getPixelFromDp(env, context, 10.0f),
                        Layout::getPixelFromDp(env, context, 0.0f));

    /* usernameText.setLayoutParams(layoutParams); */
    env->CallVoidMethod(usernameText,setLayoutParams,layoutParams);

    /* usernameText.setText("Username")); */
    env->CallVoidMethod(usernameText,setText,env->NewStringUTF("Username"));

    /* usernameText.setTextColor(Color.parseColor("#ddddd1")); */
    jmethodID setTextColor  = env->GetMethodID(TextView,"setTextColor", "(I)V");
    color                   = env->CallStaticIntMethod(Color,parseColor,env->NewStringUTF("#ddddd1"));
    env->CallVoidMethod(usernameText,setTextColor,color);

    /* usernameText.setTextSize(12.0f); */
    textSize = 12.0f;
    env->CallVoidMethod(usernameText,setTextSize, textSize);

    /*----------------------------------usernameEditText------------------------------------------*/

    /* EditText usernameEditText = new EditText(this); */
    jclass      EditText            = env->FindClass("android/widget/EditText");
    jmethodID   EditTextConstructor = env->GetMethodID(EditText, "<init>", "(Landroid/content/Context;)V");
    jobject     usernameEditText    = env->NewObject(EditText, EditTextConstructor, context);

    /* layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT,LinearLayout.LayoutParams.WRAP_CONTENT); */
    layoutParams = env->NewObject(LinearLayout$LayoutParams, constructLinearLayout$LayoutParams, MATCH_PARENT, WRAP_CONTENT);

    /* layoutParams.setMargins(getPixelFromDp(10.0f), convertDipToPixels(0.0f), convertDipToPixels(10.0f), convertDipToPixels(40.0f)); */
    env->CallVoidMethod(layoutParams, setMargins,
                            Layout::getPixelFromDp(env, context, 12.0f),
                            Layout::getPixelFromDp(env, context, 0.0f),
                            Layout::getPixelFromDp(env, context, 12.0f),
                            Layout::getPixelFromDp(env, context, 0.0f)
                        );

    /* usernameEditText.setLayoutParams(layoutParams); */
    env->CallVoidMethod(usernameEditText,setLayoutParams,layoutParams);

    /* usernameEditText.setSingleLine(true); */
    jmethodID setSingleLine = env->GetMethodID(TextView,"setSingleLine","(Z)V");
    env->CallVoidMethod(usernameEditText,setSingleLine,true);

    /* usernameEditText.setTextColor(Color.parseColor("#ffffff")); */
    color = env->CallStaticIntMethod(Color,parseColor,env->NewStringUTF("#ffffff"));
    env->CallVoidMethod(usernameEditText,setTextColor,color);


    /* usernameEditText.setHint("Username"); */
    jmethodID setHint = env->GetMethodID(TextView,"setHint","(Ljava/lang/CharSequence;)V");
    env->CallVoidMethod(usernameEditText,setHint, env->NewStringUTF("Username"));

    /* usernameEditText.setHintTextColor(Color.parseColor("#6d7582")); */
    jmethodID setHintTextColor  = env->GetMethodID(TextView,"setHintTextColor", "(I)V");
    color                       = env->CallStaticIntMethod(Color,parseColor,env->NewStringUTF("#6d7582"));
    env->CallVoidMethod(usernameEditText,setHintTextColor, color);


    /*-----------------------------------passwordText---------------------------------------------*/

    /* TextView passwordText = new TextView(this); */
    jobject passwordText = env->NewObject(TextView, TextViewConstructor, context);

    /* layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT,LinearLayout.LayoutParams.WRAP_CONTENT); */
    layoutParams = env->NewObject(LinearLayout$LayoutParams, constructLinearLayout$LayoutParams, MATCH_PARENT, WRAP_CONTENT);

    /* layoutParams.setMargins(getPixelFromDp(10.0f), convertDipToPixels(0.0f), convertDipToPixels(10.0f), convertDipToPixels(40.0f)); */
    env->CallVoidMethod(layoutParams, setMargins,
                            Layout::getPixelFromDp(env, context, 10.0f),
                            Layout::getPixelFromDp(env, context, 0.0f),
                            Layout::getPixelFromDp(env, context, 10.0f),
                            Layout::getPixelFromDp(env, context, 0.0f)
                        );

    /* passwordText.setLayoutParams(layoutParams); */
    env->CallVoidMethod(passwordText,setLayoutParams,layoutParams);

    /* passwordText.setText("Username")); */
    env->CallVoidMethod(passwordText,setText,env->NewStringUTF("Password"));

    /* passwordText.setTextColor(Color.parseColor("#ddddd1")); */
    color = env->CallStaticIntMethod(Color,parseColor,env->NewStringUTF("#ddddd1"));
    env->CallVoidMethod(passwordText,setTextColor,color);

    /* passwordText.setTextSize(12.0f); */
    textSize = 12.0f;
    env->CallVoidMethod(passwordText,setTextSize, textSize);

    /*----------------------------------passwordEditText------------------------------------------*/

    /* EditText passwordEditText = new EditText(this); */
    jobject passwordEditText = env->NewObject(EditText, EditTextConstructor, context);

    /* layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT,LinearLayout.LayoutParams.WRAP_CONTENT); */
    layoutParams = env->NewObject(LinearLayout$LayoutParams, constructLinearLayout$LayoutParams, MATCH_PARENT, WRAP_CONTENT);

    /* layoutParams.setMargins(getPixelFromDp(10.0f), convertDipToPixels(0.0f), convertDipToPixels(10.0f), convertDipToPixels(40.0f)); */
    env->CallVoidMethod(layoutParams, setMargins,
                            Layout::getPixelFromDp(env, context, 12.0f),
                            Layout::getPixelFromDp(env, context, 0.0f),
                            Layout::getPixelFromDp(env, context, 12.0f),
                            Layout::getPixelFromDp(env, context, 0.0f)
                        );

    /* passwordEditText.setLayoutParams(layoutParams); */
    env->CallVoidMethod(passwordEditText,setLayoutParams,layoutParams);

    /* passwordEditText.setSingleLine(true); */
    env->CallVoidMethod(passwordEditText,setSingleLine,true);

    /* passwordEditText.setTextColor(Color.parseColor("#ffffff")); */
    color = env->CallStaticIntMethod(Color,parseColor,env->NewStringUTF("#ffffff"));
    env->CallVoidMethod(passwordEditText,setTextColor,color);

    /* passwordEditText.setHint("Username"); */
    env->CallVoidMethod(passwordEditText,setHint, env->NewStringUTF("Password"));

    /* passwordEditText.setHintTextColor(Color.parseColor("#6d7582")); */
    color = env->CallStaticIntMethod(Color,parseColor,env->NewStringUTF("#6d7582"));
    env->CallVoidMethod(passwordEditText,setHintTextColor, color);

    /* passwordEditText.setInputType(129); */
    jmethodID  setInputType = env->GetMethodID(TextView,"setInputType","(I)V");
    env->CallVoidMethod(passwordEditText,setInputType, PASSWORD_TYPE );

    /* if ( passwordPrefsLocal != null ) passwordEditText.setText(passwordPrefsLocal); */


    /**------------------------------------loginButton---------------------------------------------*/

    /* Button loginButton = new Button(this);*/
    jclass      Button              = env->FindClass("android/widget/Button");
    jmethodID   ButtonConstructor   = env->GetMethodID(Button, "<init>", "(Landroid/content/Context;)V");
    jobject     loginButton         = env->NewObject(Button, ButtonConstructor, context);

    /* layoutParams = new LinearLayout.LayoutParams( LinearLayout.LayoutParams.MATCH_PARENT, getPixelFromDp(45.0f) ); */
    layoutParams = env->NewObject(LinearLayout$LayoutParams,constructLinearLayout$LayoutParams,
                                      MATCH_PARENT,
                                      Layout::getPixelFromDp(env, context, 45.0f)
                                  );

    /* layoutParams.setMargins( convertDipToPixels(12.0f), getPixelFromDp(10.0f), convertDipToPixels(12.0f), convertDipToPixels(0.0f) ); */
    env->CallVoidMethod(layoutParams, setMargins,
                            Layout::getPixelFromDp(env, context, 12.0f),
                            Layout::getPixelFromDp(env, context, 10.0f),
                            Layout::getPixelFromDp(env, context, 12.0f),
                            Layout::getPixelFromDp(env, context, 0.0f)
                        );

    /* loginButton.setLayoutParams(layoutParams); */
    env->CallVoidMethod(loginButton,setLayoutParams,layoutParams);

    /* loginButton.setText("LOGIN"); */
    env->CallVoidMethod(loginButton,setText, env->NewStringUTF("LOGIN") );

    /* loginButton.setTextSize(16.0);*/
    env->CallVoidMethod(loginButton,setTextSize, 16.0f);

    /* loginButton.setTypeface(Typeface.DEFAULT_BOLD); */
    jclass      TypeFace                = env->FindClass("android/graphics/Typeface");
    jfieldID    DEFAULT_BOLD_FieldId    = env->GetStaticFieldID(TypeFace, "DEFAULT_BOLD", "Landroid/graphics/Typeface;");
    jobject     DEFAULT_BOLD            = env->GetStaticObjectField(TypeFace, DEFAULT_BOLD_FieldId);
    jmethodID   setTypeface             = env->GetMethodID(TextView,"setTypeface", "(Landroid/graphics/Typeface;)V");
    env->CallVoidMethod(loginButton,setTypeface,DEFAULT_BOLD);

    /* loginButton.setBackgroundColor(Color.parseColor("#12a56b")); */
    jclass      View                    = env->FindClass("android/view/View");
    jmethodID   ViewSetBackgroundColor  = env->GetMethodID(View,"setBackgroundColor", "(I)V");
    color                               = env->CallStaticIntMethod(Color,parseColor,env->NewStringUTF("#12a56b"));
    env->CallVoidMethod(loginButton,ViewSetBackgroundColor, color );


    /*------------------------------------progressBar---------------------------------------------*/

    /* ProgressBar progressBar = new ProgressBar(this); */
    jclass      ProgressBar             = env->FindClass("android/widget/ProgressBar");
    jmethodID   ProgressBarConstructor  = env->GetMethodID(ProgressBar, "<init>", "(Landroid/content/Context;)V");
    jobject     progressBar             = env->NewObject(ProgressBar, ProgressBarConstructor, context);

    /* layoutParams = new LinearLayout.LayoutParams( LinearLayout.LayoutParams.MATCH_PARENT, getPixelFromDp(45.0f) ); */
    layoutParams = env->NewObject(LinearLayout$LayoutParams,constructLinearLayout$LayoutParams,
                                      MATCH_PARENT,
                                      Layout::getPixelFromDp(env, context, 45.0f)
                                  );

    /* progressBar.setLayoutParams(layoutParams); */
    env->CallVoidMethod(progressBar,setLayoutParams,layoutParams);

    /* progressBar.setVisibility(View.GONE); */
    jmethodID setVisibility = env->GetMethodID(View, "setVisibility", "(I)V");
    env->CallVoidMethod(progressBar, setVisibility, GONE);


    /*--------------------------------------------------------------------------------------------*/
    /*------------------------------------bottomLayout--------------------------------------------*/
    /*--------------------------------------------------------------------------------------------*/

    /* LinearLayout bottomLayout = new LinearLayout(this); */
    jobject bottomLayout = env->NewObject(LinearLayout, constructLinearLayout, context);

    /* layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT,LinearLayout.LayoutParams.WRAP_CONTENT); */
    layoutParams = env->NewObject(LinearLayout$LayoutParams, constructLinearLayout$LayoutParams, MATCH_PARENT, WRAP_CONTENT );

    /* layoutParams.setMargins(convertDipToPixels(14.0f), convertDipToPixels(10.0f), convertDipToPixels(16.0f), getPixelFromDp(0.0f)); */
    env->CallVoidMethod(layoutParams, setMargins,
                            Layout::getPixelFromDp(env, context, 14.0f),
                            Layout::getPixelFromDp(env, context, 10.0f),
                            Layout::getPixelFromDp(env, context, 16.0f),
                            Layout::getPixelFromDp(env, context, 0.0f)
                        );

    /* bottomLayout.setLayoutParams(layoutParams);*/
    env->CallVoidMethod( bottomLayout, setLayoutParams, layoutParams );

    /* bottomLayout.setWeightSum(2.0f); */
    jmethodID setWeightSum = env->GetMethodID(LinearLayout, "setWeightSum", "(F)V");
    env->CallVoidMethod(bottomLayout, setWeightSum , 2.0f );

    /*----------------------------------rememberCheckBox------------------------------------------*/

    /* CheckBox rememberCheckBox = new CheckBox(this); */
    jclass      CheckBox            = env->FindClass("android/widget/CheckBox");
    jmethodID   CheckBoxConstructor = env->GetMethodID(CheckBox, "<init>", "(Landroid/content/Context;)V");
    jobject     rememberCheckBox    = env->NewObject(CheckBox, CheckBoxConstructor, context );

    /* layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT,LinearLayout.LayoutParams.WRAP_CONTENT, 1f ); */
    /** Layout With Weight **/
    jmethodID   constructLinearLayout$LayoutParamsWithWeight = env->GetMethodID(LinearLayout$LayoutParams, "<init>", "(IIF)V");
    jobject     layoutParamsWithWeight = env->NewObject(LinearLayout$LayoutParams, constructLinearLayout$LayoutParamsWithWeight, WRAP_CONTENT, WRAP_CONTENT, 1.0f);

    /* rememberCheckBox.setLayoutParams(layoutParams); */
    env->CallVoidMethod(rememberCheckBox, setLayoutParams, layoutParamsWithWeight);

    /* rememberCheckBox.setTextColor(Color.parseColor("#ffffff")); */
    color = env->CallStaticIntMethod(Color,parseColor,env->NewStringUTF("#ffffff"));
    env->CallVoidMethod(rememberCheckBox,setTextColor, color);

    /* rememberCheckBox.setTextSize(16); */
    env->CallVoidMethod( rememberCheckBox,setTextSize, 16.0f);

    /* rememberCheckBox.setText("Remember Me"); */
    env->CallVoidMethod(rememberCheckBox,setText, env->NewStringUTF("Remember Me") );

    /* rememberCheckBox.setTypeface(Typeface.DEFAULT_BOLD); */
    env->CallVoidMethod(rememberCheckBox,setTypeface,DEFAULT_BOLD);

    /* rememberCheckBox.setChecked(rememberMe); */
    jclass      CompoundButton  = env->FindClass("android/widget/CompoundButton");
    jmethodID   setChecked      = env->GetMethodID(CompoundButton, "setChecked", "(Z)V");
    env->CallVoidMethod(rememberCheckBox, setChecked, true);


    /*------------------------------------registerText--------------------------------------------*/

    /* TextView registerText = new TextView(this); */
    jobject registerText = env->NewObject(TextView, TextViewConstructor, context);

    /* layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT,LinearLayout.LayoutParams.WRAP_CONTENT,1f); */
    layoutParamsWithWeight = env->NewObject(LinearLayout$LayoutParams, constructLinearLayout$LayoutParamsWithWeight, WRAP_CONTENT, WRAP_CONTENT, 1.0f);

    /* registerText.setLayoutParams(layoutParams); */
    env->CallVoidMethod( registerText, setLayoutParams, layoutParamsWithWeight);

    /** registerText.setText("Not Registered Yet?"); */
    //env->CallVoidMethod(registerText,setText, env->NewStringUTF("Not Registered Yet?") );

    /* registerText.setText(Html.fromHtml("<font>Not Registered Yet <font color='#ff0000'>?</font></font>")); */
    text    = env->CallStaticObjectMethod(Html,fromHTMl,env->NewStringUTF("<font>Not Registered Yet <font color='#ff0000'>?</font></font>"));
    env->CallVoidMethod( registerText, setText, text );

    /* registerText.setTextSize(16); */
    env->CallVoidMethod( registerText,setTextSize, 16.0f);

    /* registerText.setTextColor(Color.parseColor("#ffffff")); */
    color = env->CallStaticIntMethod(Color,parseColor,env->NewStringUTF("#ffffff"));
    env->CallVoidMethod(registerText,setTextColor, color);

    /* registerText.setGravity(Gravity.RIGHT); */
    env->CallVoidMethod(registerText,setGravity,GRAVITY_RIGHT);

    /* registerText.setTypeface(Typeface.DEFAULT_BOLD); */
    env->CallVoidMethod(registerText,setTypeface, DEFAULT_BOLD );

    /*------------------------------------Adding Views--------------------------------------------*/

    jmethodID addView = env->GetMethodID(LinearLayout,"addView", "(Landroid/view/View;)V");
    /*  bottomLayout.addView(rememberCheckBox); */
    env->CallVoidMethod(bottomLayout,addView,rememberCheckBox);
    /*  registerText.addView(registerText); */
    env->CallVoidMethod(bottomLayout,addView,registerText);

    /* mainLayout.addView(heading); */
    env->CallVoidMethod(mainLayout,addView,heading);
    /* mainLayout.addView(usernameText); */
    env->CallVoidMethod(mainLayout,addView,usernameText);
    /* mainLayout.addView(usernameEditText); */
    env->CallVoidMethod(mainLayout,addView,usernameEditText);
    /* mainLayout.addView(passwordText); */
    env->CallVoidMethod(mainLayout,addView,passwordText);
    /* mainLayout.addView(passwordEditText); */
    env->CallVoidMethod(mainLayout,addView,passwordEditText);
    /* mainLayout.addView(loginButton); */
    env->CallVoidMethod(mainLayout,addView,loginButton);
    /* mainLayout.addView(progressBar); */
    env->CallVoidMethod(mainLayout,addView,progressBar);
    /* mainLayout.addView(bottomLayout); */
    env->CallVoidMethod(mainLayout,addView,bottomLayout);

    /*------------------------------------Adding Global Reference--------------------------------------*/
    /* Adding Global Reference to GUI components so that we can access from anywhere in the program */
    usernameEditTextGlobalRefs = env->NewGlobalRef(usernameEditText);
    passwordEditTextGlobalRefs = env->NewGlobalRef(passwordEditText);
    rememberCheckBoxGlobalRefs = env->NewGlobalRef(rememberCheckBox);
    loginButtonGlobalRefs      = env->NewGlobalRef(loginButton);
    progressBarGlobalRefs      = env->NewGlobalRef(progressBar);

    /*------------------------------------Adding Click Event--------------------------------------*/
    /* Since I can't implement interface via JNI, I created a method "getButtonClickListener" in Java layer that return the interface "OnClickListener" */
    jmethodID getButtonClickListener = env->GetMethodID(CurrentActivity,"getButtonClickListener", "()Landroid/view/View$OnClickListener;");
    jobject   buttonClickListener    = env->CallObjectMethod(context, getButtonClickListener);

    /* button.setOnClickListener(buttonClickListener); */
    /*** Here, "buttonClickListener" is an interface or callback function, which is implemented in Java layer*/
    jmethodID setOnClickListener     = env->GetMethodID(Button,"setOnClickListener", "(Landroid/view/View$OnClickListener;)V");
    env->CallVoidMethod( loginButton, setOnClickListener, buttonClickListener);

    /*-------------------------------------Showing Layout-----------------------------------------*/

    /* setContentView(mainLayout); */
    jmethodID setContentView = env->GetMethodID(CurrentActivity,"setContentView", "(Landroid/view/View;)V");
    env->CallVoidMethod(context, setContentView, mainLayout);


    {
        env->DeleteLocalRef(CurrentActivity);
        env->DeleteLocalRef(LinearLayout);
        env->DeleteLocalRef(LinearLayout$LayoutParams);
        env->DeleteLocalRef(Color);
        env->DeleteLocalRef(TextView);
        env->DeleteLocalRef(Html);
        env->DeleteLocalRef(EditText);
        env->DeleteLocalRef(Button);
        env->DeleteLocalRef(TypeFace);
        env->DeleteLocalRef(View);
        env->DeleteLocalRef(ProgressBar);
        env->DeleteLocalRef(CheckBox);
        env->DeleteLocalRef(CompoundButton);

        env->DeleteLocalRef(mainLayout);
        env->DeleteLocalRef(layoutParams);
        env->DeleteLocalRef(heading);
        env->DeleteLocalRef(text);
        env->DeleteLocalRef(usernameText);
        env->DeleteLocalRef(usernameEditText);
        env->DeleteLocalRef(passwordText);
        env->DeleteLocalRef(passwordEditText);
        env->DeleteLocalRef(loginButton);
        env->DeleteLocalRef(DEFAULT_BOLD);
        env->DeleteLocalRef(progressBar);
        env->DeleteLocalRef(bottomLayout);
        env->DeleteLocalRef(rememberCheckBox);
        env->DeleteLocalRef(layoutParamsWithWeight);
        env->DeleteLocalRef(registerText);
        env->DeleteLocalRef(buttonClickListener);
    }


}
