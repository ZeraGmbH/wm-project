TEMPLATE = lib
CONFIG += staticlib

QT += qt3support 

HEADERS = \
    gui/en61850monitor.h \
    gui/bigletter.h \
    complex.h \
    cbiface.h \
    cmdinterpret.h \
    dspactvalues.h \
    en61850.h \
    ethadress.h \
    lbrowser.h \
    logfile.h \
    movingwindowfilter.h \
    parse.h \
    range.h \
    releaseinfo.h \
    scpi.h \
    sessionhelper.h \
    tcpconfig.h \
    tools.h \
    versserial.h \
    widgeom.h \
    wmeditor.h \
    zeraglobal.h

SOURCES = \
    gui/en61850monitor.cpp \
    gui/bigletter.cpp \
    complex.cpp \
    cmdinterpret.cpp \
    ethadress.cpp \
    lbrowser.cpp \
    logfile.cpp \
    movingwindowfilter.cpp \
    range.cpp \
    releaseinfo.cpp \
    parse.cpp \
    scpi.cpp \
    sessionhelper.cpp \
    tools.cpp \
    wmeditor.cpp \
    widgeom.cpp

FORMS = \
    gui/en61850monitor.ui
