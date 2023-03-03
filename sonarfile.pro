QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    mainwindow.cpp \
    mythread.cpp

HEADERS += mainwindow.h \
    mythread.h

FORMS += mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += C:\cv2\opencv\build-qt\install\include\
               C:\cv2\opencv\build-qt\install\include\opencv\
               C:\cv2\opencv\build-qt\install\include\opencv2
INCLUDEPATH += C:/cv2/opencv/build-qt/include
INCLUDEPATH += C:/cv2/opencv/build-qt/install/x64/mingw

LIBS += -LC:\cv2\opencv\build-qt\lib\libopencv_*.a
LIBS += -LC:\cv2\opencv\build-qt\install\x64\mingw\bin
LIBS += C:/cv2/opencv/build-qt/install/x64/mingw/lib/libopencv_core440.dll.a\
        C:/cv2/opencv/build-qt/install/x64/mingw/lib/libopencv_highgui440.dll.a\
        C:/cv2/opencv/build-qt/install/x64/mingw/lib/libopencv_ml440.dll.a\
        C:/cv2/opencv/build-qt/install/x64/mingw/lib/libopencv_imgproc440.dll.a\
        C:/cv2/opencv/build-qt/install/x64/mingw/lib/libopencv_imgcodecs440.dll.a\
        C:/cv2/opencv/build-qt/install/x64/mingw/lib/libopencv_stitching440.dll.a\
        C:/cv2/opencv/build-qt/install/x64/mingw/lib/libopencv_calib3d440.dll.a\
        C:/cv2/opencv/build-qt/install/x64/mingw/lib/libopencv_features2d440.dll.a\
        C:/cv2/opencv/build-qt/install/x64/mingw/lib/libopencv_flann440.dll.a\
        C:/cv2/opencv/build-qt/install/x64/mingw/lib/libopencv_video440.dll.a\
        C:/cv2/opencv/build-qt/install/x64/mingw/lib/libopencv_objdetect440.dll.a\
        C:/cv2/opencv/build-qt/install/x64/mingw/lib/libopencv_videoio440.dll.a\
        C:/cv2/opencv/build-qt/install/x64/mingw/lib/libopencv_photo440.dll.a\

RESOURCES += \
    img.qrc
