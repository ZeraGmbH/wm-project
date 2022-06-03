TEMPLATE = lib
CONFIG += staticlib

QT += qt3support 

HEADERS = \
    bigletter.h \
    complex.h \
    cbiface.h \
    cmdinterpret.h \
    dspactvalues.h \
    en61850.h \
    ethadress.h \
    lbrowser.h \
    movingwindowfilter.h \
    parse.h \
    range.h \
    releaseinfo.h \
    scpi.h \
    tcpconfig.h \
    tools.h \
    versserial.h \
    widgeom.h \
    wmeditor.h \
    zeraglobal.h

SOURCES = \
    bigletter.cpp \
    complex.cpp \
    cmdinterpret.cpp \
    dspactvalues.cpp \
    ethadress.cpp \
    lbrowser.cpp \
    movingwindowfilter.cpp \
    range.cpp \
    releaseinfo.cpp \
    parse.cpp \
    scpi.cpp \
    tools.cpp \
    wmeditor.cpp \
    widgeom.cpp

