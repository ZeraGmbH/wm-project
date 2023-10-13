QT += testlib qt3support

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS = \
    test_anglecmpoverfrequency.h

SOURCES = \
    test_anglecmpoverfrequency.cpp

INCLUDEPATH += \
    ../../wm-common/settings \
    ../../wm-common/dsp \
    ../../wm-common

LIBS += -L../../wm-common -lwm-common
