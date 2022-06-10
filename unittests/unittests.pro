QT += testlib

# enable 'make check'
CONFIG += testcase

CONFIG += no_testcase_installs

CONFIG += qt warn_on depend_includepath
CONFIG -= app_bundle

TEMPLATE = app

HEADERS = \
    main-unittest.h \
    testwidget.h \
    widgetadapterforpersitencetest.h

SOURCES = \
    main-unittest.cpp \
    testwidget.cpp \
    widgetadapterforpersitencetest.cpp

INCLUDEPATH += \
    ../wm-common \
    ../wm-common/client-access \
    ../wm-common/dsp \
    ../wm-common/gui \
    ../wm-common/scales-and-units \
    ../wm-common/scpi/definitions \
    ../wm-common/scpi \
    ../wm-common/settings \
    ../wm-common/service-interfaces

LIBS += -L../wm-common -lwm-common
