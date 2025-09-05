TEMPLATE = lib
CONFIG += staticlib

TRANSLATIONS    = wm-common_de.ts \
                  wm-common_gb.ts \
                  wm-common_pl.ts

QT += widgets network qt3support \

HEADERS = \
    client-access/clientiodevice.h \
    client-access/clientsocketdevice.h \
    client-access/deviceserver.h \
    confguihelper.h \
    dsp/dspactvalues.h \
    dsp/dspmeasdata.h \
    dsp/dspsetup.h \
    dsp/dspvar.h \
    gui/cwmessagebox.h \
    gui/cwmlineedit.h \
    gui/vectordiagramm.h \
    gui/wmconfdialogbase.h \
    gui/wmkeyboardform.h \
    gui/wmmessagebox.h \
    gui/wmrangedialogbase.h \
    gui/wmscreenshoterguibase.h \
    gui/wmviewbase.h \
    gui/bigletter.h \
    gui/en61850monitor.h \
    gui/logfileview.h \
    gui/maxrecentfiles.h \
    gui/statuslabelcontainer.h \
    gui/versionviewbase.h \
    gui/wmeditor.h \
    gui/wmmeasconfigbase.h \
    gui/wmmeasvaluesbase.h \
    gui/wmoeviewbase.h \
    gui/wmoffsetcustomlabels.h \
    gui/wmoffsetvalbase.h \
    gui/wmprogressdialog.h \
    gui/wmrawactualconfigbase.h \
    gui/wmsampledialog.h \
    gui/wmscopedialog.h \
    just/cmpactvalues.h \
    just/justValues.h \
    just/phastjusthelpers.h \
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
    scpi/scpideviface.h \
    scpi/scpierrortypes.h \
    scpi/scpiface.h \
    scpi/scpihelper.h \
    scpi/scpistatsyst.h \
    scpi/wm3000scpiface.h \
    scpi/wm3kscpispecialbase.h \
    screenshooter.h \
    settings/anglecmpoverfrequency.h \
    settings/commandlineparameter.h \
    settings/confdata.h \
    settings/ethadress.h \
    settings/settingschangetimer.h \
    settings/geometrytowidget.h \
    settings/ownerrorviewdata.h \
    settings/sessionfilenamegen.h \
    settings/sessionstreamer.h \
    settings/widgetgeometry.h \
    service-interfaces/dspiface.h \
    service-interfaces/zhclientsocket.h \
    service-interfaces/zhserveriface.h \
    calcinfo.h \
    common-modes.h \
    complex.h \
    en61850.h \
    formatinfo.h \
    logfile.h \
    maxperiods.h \
    movingwindowfilter.h \
    ownerror.h \
    range.h \
    releaseinfo.h \
    tcpconfig.h \
    tools.h \
    versserial.h \
    wmactvalues.h \
    wmglobalcommon.h \
    wmparameter.h \
    zeraglobal.h \
    zerainfo.h

SOURCES = \
    client-access/clientsocketdevice.cpp \
    client-access/deviceserver.cpp \
    confguihelper.cpp \
    dsp/dspmeasdata.cpp \
    dsp/dspsetup.cpp \
    gui/cwmessagebox.cpp \
    gui/cwmlineedit.cpp \
    gui/vectordiagramm.cpp \
    gui/wmconfdialogbase.cpp \
    gui/wmkeyboardform.cpp \
    gui/wmmessagebox.cpp \
    gui/wmrangedialogbase.cpp \
    gui/wmscreenshoterguibase.cpp \
    gui/wmviewbase.cpp \
    gui/en61850monitor.cpp \
    gui/bigletter.cpp \
    gui/logfileview.cpp \
    gui/statuslabelcontainer.cpp \
    gui/versionviewbase.cpp \
    gui/wmeditor.cpp \
    gui/wmmeasconfigbase.cpp \
    gui/wmmeasvaluesbase.cpp \
    gui/wmoffsetvalbase.cpp \
    gui/wmoeviewbase.cpp \
    gui/wmprogressdialog.cpp \
    gui/wmrawactualconfigbase.cpp \
    gui/wmsampledialog.cpp \
    gui/wmscopedialog.cpp \
    just/cmpactvalues.cpp \
    just/phastjusthelpers.cpp \
    scales-and-units/angleunits.cpp \
    scales-and-units/currentunits.cpp \
    scales-and-units/errorunits.cpp \
    scales-and-units/eunit.cpp \
    scales-and-units/loadpointunits.cpp \
    scales-and-units/rcfunits.cpp \
    scales-and-units/scalefactors.cpp \
    scales-and-units/voltageunits.cpp \
    scpi/cmdinterpret.cpp \
    scpi/cmdtimer.cpp \
    scpi/parse.cpp \
    scpi/scpi.cpp \
    scpi/scpideviface.cpp \
    scpi/scpierrortypes.cpp \
    scpi/scpiface.cpp \
    scpi/scpihelper.cpp \
    scpi/scpistatsyst.cpp \
    scpi/wm3000scpiface.cpp \
    screenshooter.cpp \
    settings/anglecmpoverfrequency.cpp \
    settings/commandlineparameter.cpp \
    settings/confdata.cpp \
    settings/ethadress.cpp \
    settings/settingschangetimer.cpp \
    settings/geometrytowidget.cpp \
    settings/sessionfilenamegen.cpp \
    settings/widgetgeometry.cpp \
    service-interfaces/dspiface.cpp \
    service-interfaces/zhclientsocket.cpp \
    service-interfaces/zhserveriface.cpp \
    complex.cpp \
    formatinfo.cpp \
    logfile.cpp \
    movingwindowfilter.cpp \
    ownerror.cpp \
    range.cpp \
    releaseinfo.cpp \
    settings/sessionstreamer.cpp \
    tcpconfig.cpp \
    tools.cpp \
    versserial.cpp \
    wmparameter.cpp \
    zerainfo.cpp

FORMS = \
    gui/wmkeyboardform.ui \
    gui/wmscreenshoterguibase.ui \
    gui/wmviewbase.ui \
    gui/en61850monitor.ui \
    gui/versionviewbase.ui \
    gui/wmmeasconfigbase.ui \
    gui/wmmeasvaluesbase.ui \
    gui/wmoeviewbase.ui \
    gui/wmoffsetvalbase.ui \
    gui/wmrawactualconfigbase.ui

INCLUDEPATH += \
    client-access \
    dsp \
    gui \
    just \
    scales-and-units \
    scpi \
    scpi/definitions \
    settings \
    service-interfaces

# Qt4.8's QString  on modern env causes loads of warnings
QMAKE_CXXFLAGS += -Wno-deprecated-copy
