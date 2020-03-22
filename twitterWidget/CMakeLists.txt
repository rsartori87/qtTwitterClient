cmake_minimum_required(VERSION 3.5)

project(twitterWidget LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets Network NetworkAuth WebKitWidgets REQUIRED)

if(ANDROID)
  add_library(twitterWidget SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    twittertimelinemodel.cpp
    twittertimelinemodel.h
    resources.qrc
    authdialog.ui
    authdialog.cpp
    authdialog.h
  )
else()
  add_executable(twitterWidget
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    twittertimelinemodel.cpp
    twittertimelinemodel.h
    resources.qrc
    authdialog.ui
    authdialog.cpp
    authdialog.h
  )
endif()

target_link_libraries(twitterWidget PRIVATE Qt5::Widgets Qt5::Network Qt5::NetworkAuth Qt5::WebKitWidgets twitterLib)
