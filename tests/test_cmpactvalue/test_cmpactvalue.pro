QT += testlib qt3support

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS = \
    test_cmpactvalue.h

SOURCES = \
    test_cmpactvalue.cpp

INCLUDEPATH += \
    ../../wm-common/dsp \
    ../../wm-common/just \
    ../../wm-common/settings \
    ../../wm-common/scales-and-units \
    ../../wm-common

LIBS += -L../../wm-common -lwm-common
