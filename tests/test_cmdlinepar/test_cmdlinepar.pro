QT += testlib qt3support

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS = \
    test_cmdlinepar.h

SOURCES = \
    test_cmdlinepar.cpp

INCLUDEPATH += \
    ../../wm-common/settings \

LIBS += -L../../wm-common -lwm-common
