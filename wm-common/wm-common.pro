TEMPLATE = lib
CONFIG += staticlib

QT += qt3support 

HEADERS = \
    client-access/clientiodevice.h \
    client-access/clientsocketdevice.h \
    client-access/deviceserver.h \
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
    scales-and-units/angleunits.h \
    scales-and-units/currentunits.h \
    scales-and-units/errorunits.h \
    scales-and-units/eunit.h \
    scales-and-units/loadpointunits.h \
    scales-and-units/rcfunits.h \
    scales-and-units/scalefactors.h \
    scales-and-units/voltageunits.h \
    scpi/definitions/scpicommoncmdtype.h \
    scpi/definitions/scpiopcstates.h \
    scpi/definitions/scpiaffectatatuscode.h \
    scpi/definitions/scpierrorindicator.h \
    scpi/definitions/scpiexecutecommandstates.h \
    scpi/definitions/scpioperationstates.h \
    scpi/definitions/scpiquestionstates.h \
    scpi/definitions/scpisesrbits.h \
    scpi/definitions/scpistatebits.h \
    scpi/cbiface.h \
    scpi/cmdinterpret.h \
    scpi/cmdtimer.h \
    scpi/parse.h \
    scpi/scpi.h \
    scpi/scpierrortypes.h \
    scpi/scpiface.h \
    scpi/scpistatsyst.h \
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
    en61850.h \
    logfile.h \
    maxperiods.h \
    movingwindowfilter.h \
    range.h \
    releaseinfo.h \
    tcpconfig.h \
    tools.h \
    versserial.h \
    widgeom.h \
    wmactvalues.h \
    wmparameter.h \
    zeraglobal.h

SOURCES = \
    client-access/clientsocketdevice.cpp \
    client-access/deviceserver.cpp \
    gui/en61850monitor.cpp \
    gui/bigletter.cpp \
    gui/lbrowser.cpp \
    gui/logfileview.cpp \
    gui/versionviewbase.cpp \
    gui/wmeditor.cpp \
    gui/wmoeviewbase.cpp \
    gui/wmrawactualconfigbase.cpp \
    scales-and-units/angleunits.cpp \
    scales-and-units/currentunits.cpp \
    scales-and-units/errorunits.cpp \
    scales-and-units/loadpointunits.cpp \
    scales-and-units/rcfunits.cpp \
    scales-and-units/scalefactors.cpp \
    scales-and-units/voltageunits.cpp \
    scpi/cmdinterpret.cpp \
    scpi/cmdtimer.cpp \
    scpi/parse.cpp \
    scpi/scpi.cpp \
    scpi/scpierrortypes.cpp \
    scpi/scpiface.cpp \
    scpi/scpistatsyst.cpp \
    settings/confdata.cpp \
    settings/ethadress.cpp \
    settings/sessionhelper.cpp \
    service-interfaces/dspiface.cpp \
    service-interfaces/zhclientsocket.cpp \
    service-interfaces/zhserveriface.cpp \
    complex.cpp \
    logfile.cpp \
    movingwindowfilter.cpp \
    range.cpp \
    releaseinfo.cpp \
    tools.cpp \
    widgeom.cpp \
    wmparameter.cpp

FORMS = \
    gui/en61850monitor.ui \
    gui/versionviewbase.ui \
    gui/wmoeviewbase.ui \
    gui/wmrawactualconfigbase.ui

INCLUDEPATH += \
    client-access \
    dsp \
    scales-and-units \
    scpi \
    scpi/definitions \
    settings \
    service-interfaces
