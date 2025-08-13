QT += testlib qt3support

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS = \
    test_scpiRatio.h

SOURCES = \
    test_scpiRatio.cpp

INCLUDEPATH += \
    ../../wm-common \
    ../../wm-common/scpi \

LIBS += -L../../wm-common -lwm-common
