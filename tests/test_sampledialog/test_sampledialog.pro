QT += testlib

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS = \
    test_sampledialog.h

SOURCES = \
    test_sampledialog.cpp

INCLUDEPATH += \
    ../../wm-common/gui

LIBS += -L../../wm-common -lwm-common
