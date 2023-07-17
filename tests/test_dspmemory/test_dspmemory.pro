QT += testlib qt3support
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  test_despmemory.cpp

INCLUDEPATH += \
    ../../wm-common/gui \
    ../../wm-common/dsp

LIBS += -L../../wm-common -lwm-common
