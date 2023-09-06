QT += testlib

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS = \
    test_phasejusthelper.h

SOURCES = \
    test_phasejusthelper.cpp

INCLUDEPATH += \
    ../../wm-common/just

LIBS += -L../../wm-common -lwm-common
