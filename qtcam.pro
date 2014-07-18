#-------------------------------------------------
#
# Project created by QtCreator 2014-07-16T16:54:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtcam
TEMPLATE = app

QMAKE_CXXFLAGS_RELEASE = -DNDEBUG

SOURCES += main.cpp\
		mainwindow.cpp \
	previewdisplay.cpp

HEADERS  += mainwindow.h \
	previewdisplay.h \
	logutil.h

LIBS += /usr/lib/x86_64-linux-gnu/libopencv_highgui.so \
		/usr/lib/x86_64-linux-gnu/libopencv_imgproc.so \
		/usr/lib/x86_64-linux-gnu/libopencv_core.so

FORMS    += mainwindow.ui

OTHER_FILES += \
	qtcam.pro.user \
	README
