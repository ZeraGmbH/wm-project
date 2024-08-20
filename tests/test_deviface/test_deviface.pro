QT += testlib qt3support

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS = \
    test_deviface.h

SOURCES = \
    test_deviface.cpp

INCLUDEPATH += \
    ../../wm-common/scpi \
    ../../wm-common/scpi/definitions \
    ../../wm-common

LIBS += -L../../wm-common -lwm-common
