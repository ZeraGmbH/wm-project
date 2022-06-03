TEMPLATE = lib
CONFIG += staticlib

QT += qt3support 

HEADERS = \
    gui/en61850monitor.h \
    gui/bigletter.h \
    gui/lbrowser.h \
    gui/versionviewbase.h \
    gui/wmeditor.h \
    complex.h \
    cbiface.h \
    cmdinterpret.h \
    dspactvalues.h \
    en61850.h \
    ethadress.h \
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
    zeraglobal.h

SOURCES = \
    gui/en61850monitor.cpp \
    gui/bigletter.cpp \
    gui/lbrowser.cpp \
    gui/versionviewbase.cpp \
    gui/wmeditor.cpp \
    complex.cpp \
    cmdinterpret.cpp \
    ethadress.cpp \
    logfile.cpp \
    movingwindowfilter.cpp \
    range.cpp \
    releaseinfo.cpp \
    parse.cpp \
    scpi.cpp \
    sessionhelper.cpp \
    tools.cpp \
    widgeom.cpp

FORMS = \
    gui/en61850monitor.ui \
    gui/versionviewbase.ui
