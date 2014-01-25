#-------------------------------------------------
#
# Project created by QtCreator 2014-01-24T16:04:12
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

LIBS += /usr/lib/libopencv_highgui.so \
		/usr/lib/libopencv_imgproc.so \
		/usr/lib/libopencv_core.so

FORMS    += mainwindow.ui
