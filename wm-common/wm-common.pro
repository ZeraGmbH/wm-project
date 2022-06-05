TEMPLATE = lib
CONFIG += staticlib

QT += qt3support 

HEADERS = \
    client-interfaces/clientiodevice.h \
    dsp/dspactvalues.h \
    dsp/dspvar.h \
    gui/bigletter.h \
    gui/en61850monitor.h \
    gui/lbrowser.h \
    gui/logfileview.h \
    gui/maxrecentfiles.h \
    gui/versionviewbase.h \
    gui/wmeditor.h \
    gui/wmoeviewbase.h \
    gui/wmrawactualconfigbase.h \
    settings/confdata.h \
    settings/ethadress.h \
    settings/ownerrorviewdata.h \
    settings/sessionhelper.h \
    service-interfaces/dspiface.h \
    service-interfaces/zhclientsocket.h \
    service-interfaces/zhserveriface.h \
    calcinfo.h \
    common-modes.h \
    complex.h \
    cbiface.h \
    cmdinterpret.h \
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
    gui/wmrawactualconfigbase.cpp \
    settings/confdata.cpp \
    settings/ethadress.cpp \
    settings/sessionhelper.cpp \
    service-interfaces/dspiface.cpp \
    service-interfaces/zhclientsocket.cpp \
    service-interfaces/zhserveriface.cpp \
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
    gui/wmoeviewbase.ui \
    gui/wmrawactualconfigbase.ui

INCLUDEPATH += \
    dsp \
    settings \
    service-interfaces
