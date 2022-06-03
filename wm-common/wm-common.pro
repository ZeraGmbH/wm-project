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
    en61850monitor.h \
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
    bigletter.cpp \
    complex.cpp \
    cmdinterpret.cpp \
    en61850monitor.cpp \
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
    en61850monitor.ui
