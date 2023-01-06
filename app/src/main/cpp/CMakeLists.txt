# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wl,--exclude-libs,ALL")

# Declares and names the project.
#------------------- BEGIN Loader ------------------------#
project("loader")
add_library(
        loader
        SHARED
        Loader.cpp
        RootManager/RootManager.cpp

        Activity/Activity.cpp
        Base64/Base64.cpp
        Layout/Layout.cpp
        LoginInterface/LoginInterface.cpp
        DRM/DRM.cpp
        RSA/RSA.cpp
        Utils/obfuscate.h
        HttpURLConnection/HttpURLConnection.cpp
        JSONObject/JSONObject.cpp
        SharedPreferences/SharedPreferences.cpp

        Deauther/Deauther.cpp

)
find_library(
        log-lib
        log
)
target_link_libraries(
        loader
        ${log-lib}
)
#------------------- END Loader ------------------------#