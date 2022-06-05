TEMPLATE = lib
CONFIG += staticlib

QT += qt3support 

HEADERS = \
    gui/bigletter.h \
    gui/en61850monitor.h \
    gui/lbrowser.h \
    gui/logfileview.h \
    gui/maxrecentfiles.h \
    gui/versionviewbase.h \
    gui/wmeditor.h \
    gui/wmoeviewbase.h \
    settings/confdata.h \
    settings/ethadress.h \
    settings/ownerrorviewdata.h \
    settings/sessionhelper.h \
    calcinfo.h \
    common-modes.h \
    complex.h \
    cbiface.h \
    cmdinterpret.h \
    dspactvalues.h \
    dspvar.h \
    en61850.h \
    logfile.h \
    maxperiods.h \
    movingwindowfilter.h \
    parse.h \
    range.h \
    releaseinfo.h \
    scpi.h \
    tcpconfig.h \
    tools.h \
    versserial.h \
    widgeom.h \
    wmactvalues.h \
    zeraglobal.h

SOURCES = \
    gui/en61850monitor.cpp \
    gui/bigletter.cpp \
    gui/lbrowser.cpp \
    gui/logfileview.cpp \
    gui/versionviewbase.cpp \
    gui/wmeditor.cpp \
    gui/wmoeviewbase.cpp \
    settings/confdata.cpp \
    settings/ethadress.cpp \
    settings/sessionhelper.cpp \
    complex.cpp \
    cmdinterpret.cpp \
    logfile.cpp \
    movingwindowfilter.cpp \
    range.cpp \
    releaseinfo.cpp \
    parse.cpp \
    scpi.cpp \
    tools.cpp \
    widgeom.cpp

FORMS = \
    gui/en61850monitor.ui \
    gui/versionviewbase.ui \
    gui/wmoeviewbase.ui

INCLUDEPATH += settings
