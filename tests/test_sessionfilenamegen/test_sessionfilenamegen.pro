QT += testlib
QT -= gui

# enable make check
CONFIG += testcase

CONFIG += no_testcase_installs

CONFIG += qt console warn_on depend_includepath
CONFIG -= app_bundle

TEMPLATE = app

HEADERS = \
    test_sessionfilenamegen.h

SOURCES = \
    test_sessionfilenamegen.cpp

INCLUDEPATH += \
    ../../wm-common \
    ../../wm-common/settings

LIBS += -L../../wm-common -lwm-common
